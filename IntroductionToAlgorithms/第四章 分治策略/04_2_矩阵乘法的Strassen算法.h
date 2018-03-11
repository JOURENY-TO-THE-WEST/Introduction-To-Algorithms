#pragma once
#include "..\bash.h"

typedef struct MATRIX
{
	int row;	//��
	int col;	//��
	int *body;
}matrix;

void FreeMatrix(matrix *m)
{
	free(m->body);
	free(m);
}

//���Դ����м򵥵ľ���˷�
matrix *SquareMatrixMultiply(matrix *a,matrix *b)
{
	matrix *sum = (matrix *)malloc(sizeof(matrix));
	sum->row = a->row;
	sum->col = b->col;
	sum->body = (int *)malloc(sizeof(int)*sum->row*sum->col);

	for (int i = 0; i < a->row; i++)
	{
		for (int j = 0; j < b->col; j++)
		{
			*(sum->body + i * a->col + j);
			for (int k = 0; k < a->col; k++)
			{
				*(sum->body + i * a->col + j) += (*(a->body + i * a->col + k))* (*(b->body + j * b->row + k));
			}
		}
	}
	return sum;
}

void MutiTest()
{
	int abody[2][4] = { 1,1,1,1,2,2,2,2 };
	int bdody[4][2] = { 3,3,3,3,4,4,4,4 };
	matrix a = { 1,4,(int *)abody };
	matrix b = { 4,1,(int *)bdody };
	matrix *sum = SquareMatrixMultiply(&a, &b);
	printf("%d", *(sum->body+1));
}

//�����㷨