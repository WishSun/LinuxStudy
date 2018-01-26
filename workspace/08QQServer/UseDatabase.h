#include "c_mysql.h"
#include "ProtocolStruct.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

/*
 *使用数据库模块头文件
 */

/*
 *函数功能: 产生ID号
 *参数说明: @count: 当前QQ账户个数
 *          @NewID: 存储新产生的ID号的数组，数组长度为11
 *返回值: 无
 */
void ProductID(int count, char *NewID);


/*
 *函数功能: 注册用户, 注册完后将申请到的ID号填入参数结构体中的m_SrcID成员中
 *参数说明: @NewUser: 用户的注册信息
 *返回值 : 无
 */
void registerUser(ChatInfo *NewUser);

/*
 *显示所有用户信息
 */
void ShowAllUser();

/*
 *函数功能; 判断是否登录成功
 *参数说明: @LoginInfo: 客户端提供的登录信息
 *返回值:  成功返回0
 *         失败返回-1
 */
int IsLoginSuccess(ChatInfo *LoginInfo);

/*
 *函数功能: 根据ID获取到用户昵称，并写入m_Msg成员中
 *参数说明: findName: ChatInfo类型的指针变量用来回送查找到的昵称
 *返回值: 无
 */
void GetClientNameByID(ChatInfo *findName);
