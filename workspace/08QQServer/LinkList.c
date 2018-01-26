#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "LinkList.h"

/**
  *链表模块实现文件
  *
 **/


//初始化单链表
void InitList(LinkList *L)
{
	(*L) = (LinkList)malloc(sizeof(LinkNode));
	(*L)->next = NULL;
}

//单链表头插元素
void HeadInsertList(LinkList L, ElemType *elem)
{
	if(L == NULL || elem == NULL)
	{
		return;
	}

	LinkList TL = L;
	LinkList p_NewNode = (LinkList)malloc(sizeof(LinkNode));
	
	p_NewNode->data = *elem;

	p_NewNode->next = TL->next;
	TL->next = p_NewNode;
}

//查找单链表中值为key的结点，返回其指针
LinkList GetNodeByKey(LinkList L, ElemType *key)
{
	LinkList TL;
	
	if(L == NULL)
	{
		return NULL;
	}

	TL =  L->next;	
	
	while(TL)
	{
		if( strcmp(key->m_ClientID, TL->data.m_ClientID) == 0)
		{
			break;
		}
		TL = TL->next;
	}
	if(TL == NULL)
	{
		return NULL;
	}
	return TL;
}

//删除单链表中值为key的结点
void DeleteNodeByKey(LinkList L, ElemType *key)
{
	LinkList TL, Pre;
	
	if(L == NULL)
	{
		return;
	}

    Pre = L;   //删除结点的前一个结点
	TL =  Pre->next;	
	
	while(TL)
	{
		if( strcmp(key->m_ClientID, TL->data.m_ClientID) == 0)
		{
			break;
		}
        Pre = TL;
		TL = TL->next;
	}
	if(TL != NULL)
	{
        Pre->next = TL->next;
        free(TL);
        TL = NULL;
	}
}

//获取单链表的长度
int GetListLength(LinkList L)
{
	LinkList TL;
	int len = 0;
	if(L == NULL)
	{
		return -1;
	}
	
	TL =  L->next;	
	while(TL)
	{
		len++;
		TL = TL->next;
	}
	
	return len;
}

//清空单链表
void ClearList(LinkList L)
{
	LinkList TL = L->next;
	LinkList temp;

	while(TL)
	{
		temp = TL->next;
		free(TL);
		TL = temp;
	}
	L->next = NULL;
}

//销毁单链表
void DestroyList(LinkList *L)
{
	ClearList(*L);

	free(*L);
	(*L) = NULL;
}

