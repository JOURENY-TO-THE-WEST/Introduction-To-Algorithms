#pragma once
#include "..//base.h"

#define T 5
int cur_name = 0;
/*
	��ϰ 18.1-1 t=1ʱ���ڵ��Ͽ���û�йؼ��֡�

	18.1-2   t=2   

	18.1-4  1+(t-1)(��(i=1--h) (t^i))
	
*/

/*
	����B���ļ���д��΢��Щ�鷳�����Ծ��Ȳ�д��,ֻ��ʵ��һ���򵥵Ķ�д������
	�ҵ�˼·�� ÿ���ڵ�����һ���ļ��������B���еĸ����ڵ����ʱҲ������
	�ڵ�ĺ���ָ���ӽڵ��ļ����֡�
*/
typedef struct BTREE
{
	int n;
	int key[2 * T - 1];
	int is_leaf;
	struct BTREE *children[2 * T];
}BTree;


void DiskWrite(BTree *bt,char fname[]) //�����btree�ļ����¡�
{
	char file_name[2 * SIZE];
	sprintf_s(file_name, 2 * SIZE, "btree\\%s.txt", fname);
	FILE *fp;
	fopen_s(&fp, file_name, "w");
	fwrite(bt, sizeof(BTree), 1, fp);
	fclose(fp);
}


BTree *DiskRead(char fname[])
{
	BTree *bt = (BTree *)malloc(sizeof(BTree));
	char file_name[2 * SIZE];
	sprintf_s(file_name, 2 * SIZE, "btree\\%s.txt", fname);
	FILE *fp;
	fopen_s(&fp, file_name, "r");
	fread(bt, sizeof(BTree), 1, fp);
	fclose(fp);
	return bt;
}

void BTreeTraverse(BTree *tree)
{
	if (tree->is_leaf)
	{
		for (int i = 0; i < tree->n; i++)
		{
			printf("%d ", tree->key[i]);
		}
	}
	else
	{
		for (int i = 0; i < tree->n; i++)
		{
			BTreeTraverse(tree->children[i]);
			printf("%d ", tree->key[i]);
		}
		BTreeTraverse(tree->children[tree->n]);
	}
}


BTree *BTreeSearch(BTree *x, int k,int *pos)
{
	int i = 0;
	while (i < x->n&&k > x->key[i])
	{
		i++;
	}
	if (i < x->n&&k == x->key[i])
	{
		*pos = i;
		return x;
	}
	else if (!x->is_leaf)
	{
		return BTreeSearch(x->children[i], k, pos);
	}
	return NULL;
}

BTree *BTreeCreate()
{
	BTree *bt = (BTree *)malloc(sizeof(BTree));
	bt->is_leaf = 1;
	bt->n = 0;
	return bt;
}

void BTreeSplitChild(BTree *x, int i)
{
	BTree *z = (BTree *)malloc(sizeof(BTree));
	BTree *y = x->children[i];
	z->is_leaf = y->is_leaf;
	z->n = T - 1;
	for (int j = 0; j < T - 1; j++)
	{
		z->key[j] = y->key[j + T];
	}
	if (!y->is_leaf)
	{
		for (int j = 0; j < T; j++)
		{
			z->children[j] = y->children[j + T];
		}
	}
	y->n = T - 1;
	for (int j = x->n; j >= i + 1; j--)
	{
		x->children[j + 1] = x->children[j];
	}
	x->children[i+1] = z;
	for (int j = x->n - 1; j >= i; j--)
	{
		x->key[j + 1] = x->key[j];
	}
	x->key[i] = y->key[T-1];
	x->n++;
}


void BTreeInsertNotFull(BTree *x, int k)
{
	int i = x->n;
	if (x->is_leaf)
	{
		while (i>=1&&k<x->key[i-1])
		{
			x->key[i] = x->key[i - 1];
			i--;
		}
		x->key[i] = k;
		x->n++;
	}
	else
	{
		while (i >= 1 && k < x->key[i - 1])
		{
			i--;
		}
		if (x->children[i]->n == 2 * T - 1)
		{
			BTreeSplitChild(x, i);
			if (k > x->key[i])
			{
				i++;
			}
		}

		BTreeInsertNotFull(x->children[i], k);
	}
}
void BTreeInsert(BTree **t, int k)
{
	BTree *r = *t;
	if (r->n == 2 * T - 1)
	{
		BTree *s = (BTree *)malloc(sizeof(BTree));
		*t = s;
		s->is_leaf = 0;
		s->n = 0;
		s->children[0] = r;
		BTreeSplitChild(s, 0);
		BTreeInsertNotFull(s, k);
	}
	else
	{
		BTreeInsertNotFull(r, k);
	}
}

void BTreeDelete(BTree **t, int k)
{
	BTree *x = *t;
	int i = 0;
	while (i < x->n&&k > x->key[i])
	{
		i++;
	}
	if (k == x->key[i])
	{
		if (x->is_leaf)
		{
			for (int j = i; j < x->n - 1; j++)
			{
				x->key[j] = x->key[j + 1];
			}
			x->n--;
		}
		else
		{
			if (x->children[i]->n >= T)
			{
				x->key[i] = x->children[i]->key[x->children[i]->n - 1];
				BTreeDelete(&(x->children[i]), x->children[i]->key[x->children[i]->n - 1]);
			}
			else if (x->children[i + 1]->n >= T)
			{
				x->key[i + 1] = x->children[i + 1]->key[x->children[i + 1]->n - 1];
				BTreeDelete(&(x->children[i + 1]), x->children[i + 1]->key[x->children[i + 1]->n - 1]);
			}
			else
			{
				BTree *y = x->children[i];
				BTree *z = x->children[i + 1];
				y->key[y->n] = k;
				for (int j = 0; j < x->n - 1; j++)
				{
					x->key[i] = x->key[i + 1];
					x->children[i + 1] = x->children[i + 2];
				}
				x->n--;
				for (int j = 0; j < z->n; j++)
				{
					y->key[y->n + i] = z->key[i];
					y->children[y->n + i + 1] = z->children[i];
				}
				y->n += z->n + 1;
				y->children[y->n] = z->children[z->n];
				free(z);
				BTreeDelete(&(x->children[i]), k);

			}
		}
	}
	else
	{
		if (x->children[i]->n == T - 1)
		{
			if (i == 0)
			{
				if (x->children[i + 1]->n >= T)
				{
					BTree *z = x->children[i + 1];
					BTree *y = x->children[i];
					y->key[y->n] = x->key[i];
					y->children[y->n + 1] = z->children[0];
					y->n++;
					x->key[i] = z->key[0];
					for (int j = 0; j < z->n - 1; j++)
					{
						z->key[j] = z->key[j + 1];
						z->children[j] = z->children[j + 1];
					}
					z->children[z->n - 1] = z->children[z->n];
					z->n--;
				}
				else
				{
					BTree *y = x->children[i];
					BTree *z = x->children[i + 1];
					y->key[y->n] = k;
					for (int j = 0; j < x->n - 1; j++)
					{
						x->key[i] = x->key[i + 1];
						x->children[i + 1] = x->children[i + 2];
					}
					x->n--;
					for (int j = 0; j < z->n; j++)
					{
						y->key[y->n + i] = z->key[i];
						y->children[y->n + i + 1] = z->children[i];
					}
					y->n += z->n + 1;
					y->children[y->n] = z->children[z->n];
					free(z);
				}
			}
			else if (i == x->n)
			{
				if (x->children[i - 1]->n >= T)
				{
					BTree *y = x->children[i - 1];
					BTree *z = x->children[i];
					for (int j = z->n; j > 0; j--)
					{
						z->key[j] = z->key[j - 1];
						z->children[j + 1] = z->children[j];
					}
					z->children[1] = z->children[0];
					z->key[0] = x->key[i - 1];
					z->children[0] = y->children[y->n];
					x->key[i - 1] = y->key[y->n - 1];
					y->n--;
				}
				else
				{
					BTree *y = x->children[i-1];
					BTree *z = x->children[i];
					y->key[y->n] = k;
					x->n--;
					for (int j = 0; j < z->n; j++)
					{
						y->key[y->n + i] = z->key[i];
						y->children[y->n + i + 1] = z->children[i];
					}
					y->n += z->n + 1;
					y->children[y->n] = z->children[z->n];
					free(z);
				}
			}
			else if (x->children[i - 1]->n >= T)
			{
				BTree *y = x->children[i - 1];
				BTree *z = x->children[i];
				for (int j = z->n; j > 0; j--)
				{
					z->key[j] = z->key[j - 1];
					z->children[j + 1] = z->children[j];
				}
				z->children[1] = z->children[0];
				z->key[0] = x->key[i-1];
				z->children[0] = y->children[y->n];
				x->key[i-1] = y->key[y->n - 1];
				y->n--;
			}
			else if (x->children[i + 1]->n >= T)
			{
				BTree *z = x->children[i + 1];
				BTree *y = x->children[i];
				y->key[y->n] = x->key[i];
				y->children[y->n + 1] = z->children[0];
				y->n++;
				x->key[i] = z->key[0];
				for (int j = 0; j < z->n - 1; j++)
				{
					z->key[j] = z->key[j + 1];
					z->children[j] = z->children[j + 1];
				}
				z->children[z->n - 1] = z->children[z->n];
				z->n--;
			}
			else
			{
				BTree *y = x->children[i];
				BTree *z = x->children[i + 1];
				y->key[y->n] = k;
				for (int j = 0; j < x->n - 1; j++)
				{
					x->key[i] = x->key[i + 1];
					x->children[i + 1] = x->children[i + 2];
				}
				x->n--;
				for (int j = 0; j < z->n; j++)
				{
					y->key[y->n + i] = z->key[i];
					y->children[y->n + i + 1] = z->children[i];
				}
				y->n += z->n + 1;
				y->children[y->n] = z->children[z->n];
				free(z);
			}
		}
		BTreeDelete(&x->children[i], k);
	}
	if (x->n == 0)
	{
		*t = x->children[0];
	}

}

void test()
{
	BTree *bt = BTreeCreate();
	for (int i = 1; i <100; i++)
	{
		BTreeInsert(&bt, i);
	}
	for (int i = 1; i < 90; i += 10)
	{
		BTreeDelete(&bt, i);
	}

	BTreeTraverse(bt);
	printf("done\n");
}