#pragma once

//����
struct Data
{
	ULONG Hint;
	DWORD Address;
};

//�ڵ�
struct Node
{
	Data* data; //����
	struct Node* pNext;//��һ���ڵ��ָ��
	struct Node* pPrevious;//��һ���ڵ��ָ��
};


//���������
struct List
{
	Node* pFront; //��ʼ�ڵ��ָ��
	Node* pRear; //�����ڵ��ָ��
	int Count; //�ڵ�����
};


BOOL ListInit(List** ppList);
VOID TraverList(List* pList, VOID(*Traver)(Node* pNode));
int IsEmpty(List* pList);
VOID InserList(List* ppList, Data* pData);
BOOL DeleteNode(List* pList, BOOL ForWardOrBackWard, int DeleteOfQuantity);
BOOL DeleteAllNode(List* pList);
BOOL FindList(List* pList, DWORD data, LPVOID* lpVoid);
