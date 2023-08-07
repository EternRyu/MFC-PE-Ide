#pragma once

//数据
struct Data
{
	ULONG Hint;
	DWORD Address;
};

//节点
struct Node
{
	Data* data; //数据
	struct Node* pNext;//下一个节点的指针
	struct Node* pPrevious;//上一个节点的指针
};


//链表管理器
struct List
{
	Node* pFront; //起始节点的指针
	Node* pRear; //结束节点的指针
	int Count; //节点数量
};


BOOL ListInit(List** ppList);
VOID TraverList(List* pList, VOID(*Traver)(Node* pNode));
int IsEmpty(List* pList);
VOID InserList(List* ppList, Data* pData);
BOOL DeleteNode(List* pList, BOOL ForWardOrBackWard, int DeleteOfQuantity);
BOOL DeleteAllNode(List* pList);
BOOL FindList(List* pList, DWORD data, LPVOID* lpVoid);
