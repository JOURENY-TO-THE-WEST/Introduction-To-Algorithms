#pragma once

/* 
��������˼�룺
		�������±� i ��ʾ��ǰ�����Ԫ�أ�
		��ô0��i-1Ϊ�Ѿ�����õ�Ԫ�أ�
		�������ҵ�i��λ�ò�����
*/

void InsertSort(int *array, int length)
{
	int key,j;
	for (int i = 1; i < length; i++)
	{
		key = array[i];
		j = i - 1;
		while ((j >= 0)&&(array[j]>key))
		{
			array[j + 1] = array[j--];
		}
		array[j + 1] = key;
	}
}




