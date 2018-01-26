#include "UseLinkList.h"
#include <string.h>
#include <stdio.h>


/*
 *使用链表模块实现文件
 */


/*
 *函数功能: 向在线链表中添加新客户端的UDP地址信息
 *参数说明: @_L: 链表头指针
 *          @_client: 客户端的地址结构信息(只包括ID和UDP地址)
 *返回值: 无
 */
void AddNewUDPClient(LinkList _L, UserAddressInfo *_client)
{
    HeadInsertList(_L, _client);
}

/*
 *函数功能: 完善链表中客户端结点中的TCP地址信息
 *参数说明: @_L: 链表头指针
 *          @_client: 客户端的地址结构信息(只包括ID和TCP描述符)
 */
void CompleteTCPClient(LinkList _L, UserAddressInfo *_client)
{
    //找到目标客户端结点
    LinkList DestClient = GetNodeByKey(_L, _client); 

    //修改其TCP描述符的值
    DestClient->data.m_TcpSocketFd = _client->m_TcpSocketFd;
}

/*
 *函数功能: 通过目的ID号来查询到对应的UDP地址
 *参数说明: @_L : 在线链表头指针
 *          @_DestID: 目的ID号
 */
struct sockaddr_in GetUDPAddressByID( LinkList _L, char *_DestID )
{
    UserAddressInfo Dest;
    memset(&Dest, 0, sizeof(UserAddressInfo));
    strcpy(Dest.m_ClientID, _DestID);

    //获取到目的客户端的UDP地址
    LinkList DestClient = GetNodeByKey(_L, &Dest);
    return DestClient->data.m_UdpAddress;
}

/*
 *函数功能: 根据ID号删除结点
 *参数说明: @_L: 链表头指针
 *          @_DeleteID: 删除结点的ID号
 *返回值 : 无
 */
void DeleteNodeByID( LinkList _L, char *_DeleteID)
{
    //构造删除参数
    UserAddressInfo deleteKey;
    memset(&deleteKey, 0, sizeof(UserAddressInfo));
    strcpy(deleteKey.m_ClientID, _DeleteID);

    //删除结点
    DeleteNodeByKey(_L, &deleteKey);
}
