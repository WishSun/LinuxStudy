#include "LinkList.h"
#include "ProtocolStruct.h"

/*
 *使用链表模块头文件
 */


/*
 *函数功能: 向在线链表中添加新客户端的UDP地址信息
 *参数说明: @_clientID: 客户端ID号
 *          @_UdpAddress: 客户端的UDP地址信息
 *返回值: 无
 */
void AddNewUDPClient(LinkList _L, UserAddressInfo *_client);


/*
 *函数功能: 完善链表中客户端结点中的TCP地址信息
 *参数说明: @_L: 链表头指针
 *          @_client: 客户端的地址结构信息(只包括ID和TCP描述符)
 */
void CompleteTCPClient(LinkList _L, UserAddressInfo *_client);

/*
 *函数功能: 通过目的ID号来查询到对应的UDP地址
 *参数说明: @_L : 在线链表头指针
 *          @_DestID: 目的ID号
 */
struct sockaddr_in GetUDPAddressByID( LinkList _L, char *_DestID );

/*
 *函数功能: 根据ID号删除结点
 *参数说明: @_L: 链表头指针
 *          @_DeleteID: 删除结点的ID号
 *返回值 : 无
 */
void DeleteNodeByID( LinkList _L, char *_DeleteID);
