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
