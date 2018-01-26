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
int main(void)
{
    UserAddressInfo NewClient;
    LinkList L;
    InitList(&L); //初始化链表

    memset(&NewClient, 0, sizeof(UserAddressInfo));

    struct sockaddr_in  addr;

    addr.sin_family = AF_INET;  
    addr.sin_port = 5000;  //指定端口为5000
    inet_aton("192.168.132.136", &addr.sin_addr);  //指定服务器IP地址

    strcpy(NewClient.m_ClientID, "1653239732");
    NewClient.m_UdpAddress = addr;


    AddNewUDPClient(L, &NewClient);

    printf("链表长度为: %d\n", GetListLength(L));
    printf("ID = %s \n", L->next->data.m_ClientID);
    printf("port = %d \n", L->next->data.m_UdpAddress.sin_port);

    return 0;
}
*/
