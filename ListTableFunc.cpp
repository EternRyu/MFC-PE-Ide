#include "pch.h"
#include "ListTableFunc.h"

//初始化链表管理器
BOOL ListInit(List** ppList)
{
	*ppList = (List*)malloc(sizeof(List));
	if (!*ppList)
	{
		return FALSE;
	}
	(*ppList)->pFront = NULL;
	(*ppList)->pRear = NULL;
	(*ppList)->Count = 0;
	//free (*ppList);//释放指针
	return TRUE;
}

//遍历链表管理器
VOID TraverList(List* pList, VOID(*Traver)(Node* pNode))
{
	Node* pTemp = pList->pFront;//指向第一个节点
	int ListSize = pList->Count;//几个节点
	while (ListSize)//节点不为零
	{
		Traver(pTemp);//当前节点位置传回*Traver所指向的函数
		pTemp = pTemp->pNext;//从当前节点取得下一节点位置
		ListSize--;//循环-1
	}

}

//判断链表是否为空
int IsEmpty(List* pList)
{
	if (!pList->Count)
	{
		return FALSE;
	}
	return TRUE;
}

//链表插入节点时 头插法
VOID InserList(List* pList, Data* pData)
{
	Node* pNode = (Node*)malloc(sizeof(Node));
	pNode->data = pData;
	Node* OldNode = pList->pRear;//从节点管理器获取尾端节点的位置

	if (!IsEmpty(pList))//如果没有节点
	{
		//链表管理器修改起始地址
		pList->pRear = pNode;
		pList->pFront = pNode;
		//形成环形
		pNode->pNext = pNode;
		pNode->pPrevious = pNode;
	}
	else//如果有节点
	{
		pList->pRear = pNode;//链表管理器修改结束地址
		//修改节点指针指向
		OldNode->pNext = pNode;//旧节点的前向指针指向新插入的节点
		pNode->pPrevious = OldNode;//新插入的节点的后向指针指向旧节点
	}
	pList->Count++;//节点数量+1
}

//删除节点
BOOL DeleteNode(List* pList, BOOL ForWardOrBackWard, int DeleteOfQuantity)
{

	if (!DeleteOfQuantity)
	{
		DeleteOfQuantity = 1;//默认删除1个
	}
	for (int i = DeleteOfQuantity; i > 0; i--)
	{
		if (IsEmpty(pList))//判断是否为空
		{
			Node* TempNode = NULL;
			if (!ForWardOrBackWard)//等于FALSE向后 TRUE向前
			{
				TempNode = pList->pRear;//取得末端节点指针
				pList->pRear = TempNode->pPrevious;//指向后一个节点
			}
			else
			{
				Node* TempNode = pList->pFront;//取得起始端节点指针
				TempNode = TempNode->pNext;//指向前一个节点
			}
			free(TempNode);//释放指针
			pList->Count--;
			return TRUE;
		}
		return FALSE;
	}
}

//查找链表
BOOL FindList(List* pList, DWORD data, LPVOID* lpVoid)
{
	if (pList->Count = 0)
	{
		return FALSE;
	}
	BOOL PositiveOrReverse;
	Node* TempNode=NULL;
	if ((pList->Count) / 2 > data)
	{
		TempNode = pList->pFront; //获取链表起始节点指针
		while (data != TempNode->data->Hint)
		{
			TempNode = TempNode->pNext;
		}
		if (data == TempNode->data->Hint)
		{
			*lpVoid=(LPVOID)TempNode->data->Address;
			return TRUE;
		}
	}
	else
	{
		TempNode = pList->pRear; //获取链表起始节点指针
		while (data != TempNode->data->Hint)
		{
			TempNode = TempNode->pPrevious;
		}
		if (data == TempNode->data->Hint)
		{
			*lpVoid = (LPVOID)TempNode->data->Address;
			return TRUE;
		}
	}

	return FALSE;
}

//删除所有节点
BOOL DeleteAllNode(List* pList)
{

	if (pList->Count!=0)
	{
		if (IsEmpty(pList))//判断是否为空
		{
			Node* TempNode = pList->pFront;//取得起始端节点指针
			TempNode = TempNode->pNext;//指向前一个节点
			free(LPVOID(TempNode->data->Address));
			free(TempNode->data);
			free(TempNode);//释放指针
			pList->Count--;
			return TRUE;
		}
	}

	return FALSE;
}

