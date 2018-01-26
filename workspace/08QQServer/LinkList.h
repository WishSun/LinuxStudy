#include "ProtocolStruct.h"
/**
  *链表模块头文件
  *
 **/

typedef UserAddressInfo ElemType;

//结点类型
typedef struct Node
{
	ElemType 	data;
	struct Node	*next;
}LinkNode, *LinkList;



//初始化单链表
void InitList(LinkList *L);

//单链表头插元素
void HeadInsertList(LinkList L, ElemType *elem);

//查找单链表中值为key的结点，返回其指针
LinkList GetNodeByKey(LinkList L, ElemType *key);

//删除单链表中值为key的结点
void DeleteNodeByKey(LinkList L, ElemType *key);

//获取单链表的长度
int GetListLength(LinkList L);

//清空单链表
void ClearList(LinkList L);

//销毁单链表
void DestroyList(LinkList *L);



