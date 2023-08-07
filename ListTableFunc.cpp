#include "pch.h"
#include "ListTableFunc.h"

//��ʼ�����������
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
	//free (*ppList);//�ͷ�ָ��
	return TRUE;
}

//�������������
VOID TraverList(List* pList, VOID(*Traver)(Node* pNode))
{
	Node* pTemp = pList->pFront;//ָ���һ���ڵ�
	int ListSize = pList->Count;//�����ڵ�
	while (ListSize)//�ڵ㲻Ϊ��
	{
		Traver(pTemp);//��ǰ�ڵ�λ�ô���*Traver��ָ��ĺ���
		pTemp = pTemp->pNext;//�ӵ�ǰ�ڵ�ȡ����һ�ڵ�λ��
		ListSize--;//ѭ��-1
	}

}

//�ж������Ƿ�Ϊ��
int IsEmpty(List* pList)
{
	if (!pList->Count)
	{
		return FALSE;
	}
	return TRUE;
}

//�������ڵ�ʱ ͷ�巨
VOID InserList(List* pList, Data* pData)
{
	Node* pNode = (Node*)malloc(sizeof(Node));
	pNode->data = pData;
	Node* OldNode = pList->pRear;//�ӽڵ��������ȡβ�˽ڵ��λ��

	if (!IsEmpty(pList))//���û�нڵ�
	{
		//����������޸���ʼ��ַ
		pList->pRear = pNode;
		pList->pFront = pNode;
		//�γɻ���
		pNode->pNext = pNode;
		pNode->pPrevious = pNode;
	}
	else//����нڵ�
	{
		pList->pRear = pNode;//����������޸Ľ�����ַ
		//�޸Ľڵ�ָ��ָ��
		OldNode->pNext = pNode;//�ɽڵ��ǰ��ָ��ָ���²���Ľڵ�
		pNode->pPrevious = OldNode;//�²���Ľڵ�ĺ���ָ��ָ��ɽڵ�
	}
	pList->Count++;//�ڵ�����+1
}

//ɾ���ڵ�
BOOL DeleteNode(List* pList, BOOL ForWardOrBackWard, int DeleteOfQuantity)
{

	if (!DeleteOfQuantity)
	{
		DeleteOfQuantity = 1;//Ĭ��ɾ��1��
	}
	for (int i = DeleteOfQuantity; i > 0; i--)
	{
		if (IsEmpty(pList))//�ж��Ƿ�Ϊ��
		{
			Node* TempNode = NULL;
			if (!ForWardOrBackWard)//����FALSE��� TRUE��ǰ
			{
				TempNode = pList->pRear;//ȡ��ĩ�˽ڵ�ָ��
				pList->pRear = TempNode->pPrevious;//ָ���һ���ڵ�
			}
			else
			{
				Node* TempNode = pList->pFront;//ȡ����ʼ�˽ڵ�ָ��
				TempNode = TempNode->pNext;//ָ��ǰһ���ڵ�
			}
			free(TempNode);//�ͷ�ָ��
			pList->Count--;
			return TRUE;
		}
		return FALSE;
	}
}

//��������
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
		TempNode = pList->pFront; //��ȡ������ʼ�ڵ�ָ��
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
		TempNode = pList->pRear; //��ȡ������ʼ�ڵ�ָ��
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

//ɾ�����нڵ�
BOOL DeleteAllNode(List* pList)
{

	if (pList->Count!=0)
	{
		if (IsEmpty(pList))//�ж��Ƿ�Ϊ��
		{
			Node* TempNode = pList->pFront;//ȡ����ʼ�˽ڵ�ָ��
			TempNode = TempNode->pNext;//ָ��ǰһ���ڵ�
			free(LPVOID(TempNode->data->Address));
			free(TempNode->data);
			free(TempNode);//�ͷ�ָ��
			pList->Count--;
			return TRUE;
		}
	}

	return FALSE;
}

