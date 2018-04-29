#pragma once
#include "..//base.h"


enum Color { white,gray,black };

// �ڽӱ��б��Ӧ������Ķ���
typedef struct _ENode
{
	int ivex;                   // �ñ���ָ��Ķ����λ��
	struct _ENode *next_edge;   // ָ����һ������ָ��
}ENode, *PENode;

// �ڽӱ��б�Ķ���
typedef struct _VNode
{
	char data;              // ������Ϣ
	enum Color color;		//��ɫ
	struct _VNode *pre;		//ǰ�����

	int d;					//Դ�ڵ㵽��ǰ�ڵ�ľ���
	int f;
	//  dfs�� d ��ʼʱ�䣬f ����ʱ�䡣
	ENode *first_edge;      // ָ���һ�������ö���Ļ�
}VNode;

// �ڽӱ�
typedef struct _LGraph
{
	int vexnum;             // ͼ�Ķ������Ŀ
	int edgnum;             // ͼ�ıߵ���Ŀ
	VNode vexs[SIZE];
}LGraph;

