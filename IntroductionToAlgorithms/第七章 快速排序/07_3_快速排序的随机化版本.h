#pragma once
#include "..\base.h"
#include "07_1_�������������.h"

//������Ŀ��ţ����Ͻڵ�_Partition��Ӧ

int RandomizedPartition(int a[], int p, int r)
{
	int i = Random(p, r);
	int temp = a[i];
	a[r] = a[i];
	a[i] = a[r];
	return _Partition(a, p, r);
}

void RandomizedQuickSort(int a[], int p, int r)
{
	if (p < r)
	{
		int q = RandomizedPartition(a, p, r);
		RandomizedQuickSort(a, p, q - 1);
		RandomizedQuickSort(a, q + 1, r);
	}
}