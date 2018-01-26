#include "UseDatabase.h"

/*
 *使用数据库模块实现文件
 */

/*
 *函数功能: 产生ID号
 *参数说明: @count: 当前QQ账户个数
 *          @NewID: 存储新产生的ID号的数组，数组长度为11
 *返回值: 无
 */
void ProductID(int count, char *NewID)
{
    int  i = 9;

    count += 1;
    while(count > 0)
    {
        NewID[i] = count%10 + '0';
        count /= 10;
        i--;
    }
}


/*
 *函数功能: 注册用户, 注册完后将申请到的ID号填入参数结构体中的m_SrcID成员中
 *参数说明: @NewUser: 用户的注册信息
 *返回值 : 无
 */
void registerUser(ChatInfo *NewUser)
{
    char NewID[11] = "1000000000";
    int count;

    //定义一个数据库句柄
    MYSQL *msq = NULL;

    //初始化数据库句柄
    msq = InitMysql();   

    //连接数据库
    if(-1 == ConnectMysql(msq, "localhost", "root", "123456", "QQDatabase"))
    {
        exit(1);
    }

    //获取账户个数
    MysqlQuery(msq, "select * from QQ_user");
    MYSQL_RES *res = NULL;
    res = GetSQLExecuteResult(msq);
    count = GetResultRowsNum(res);

    //产生新账户ID
    ProductID(count, NewID);
    strcpy(NewUser->m_SrcID, NewID); //将注册的ID号存入m_SrcID成员给客户端返回回去

    //构建插入SQL语句
    char query[100];
    memset(query, 0, sizeof(query));
    sprintf(query, "insert into QQ_user(userID, userPwd, userName, userState) values('%s', '%s', '%s', %d)", NewID, NewUser->m_DestID, NewUser->m_Msg, 0);

    //执行插入账户语句
    MysqlQuery(msq, query);

    //收尾工作
    FreeResult(res);
    MysqlClose(msq);
}


//显示所有用户信息
void ShowAllUser()
{
    //定义一个数据库句柄
    MYSQL *msq = NULL;

    //初始化数据库句柄
    msq = InitMysql();   

    //连接数据库
    if(-1 == ConnectMysql(msq, "localhost", "root", "123456", "QQDatabase"))
    {
        exit(1);
    }

    MysqlQuery(msq, "select * from QQ_user");

    //获取查询结果集
    MYSQL_RES *res = NULL;
    res = GetSQLExecuteResult(msq);

    //获取结果集中的列数
    int col = GetResultFieldsNum(res);

    //获取结果集中的元组数
    int row = GetResultRowsNum(res);

    printf("表行数为%d, 表列数为%d\n", row, col);

    //遍历结果集中的表头
    MYSQL_FIELD *field = NULL;
    while( (field = GetFieldInfoInResult(res)) != NULL )
    {
        printf("%s \t", field->name);
    }
    printf("\n");

    MYSQL_ROW rows;
    int i;
    while( (rows = mysql_fetch_row(res)) != NULL )
    {
        for ( i = 0; i<col; i++ )
        {
            printf("%s\t", rows[i]);
        }
        printf("\n");
    }

    FreeResult(res);
    MysqlClose(msq);


}


/*
 *函数功能; 判断是否登录成功
 *参数说明: @LoginInfo: 客户端提供的登录信息
 *返回值:  成功返回0
 *         失败返回-1
 */
int IsLoginSuccess(ChatInfo *LoginInfo)
{
    //定义一个数据库句柄
    MYSQL *msq = NULL;

    //初始化数据库句柄
    msq = InitMysql();   

    //连接数据库
    if(-1 == ConnectMysql(msq, "localhost", "root", "123456", "QQDatabase"))
    {
        exit(1);
    }

    char query[100];
    memset(query, 0, sizeof(query));
    sprintf(query, "select * from QQ_user where userID = '%s' and userPwd = '%s'", LoginInfo->m_SrcID, LoginInfo->m_DestID);


    //执行查询语句
    MysqlQuery(msq, query);

    //获取查询结果集
    MYSQL_RES *res = NULL;
    res = GetSQLExecuteResult(msq);

    //获取结果集中的元组数
    int row = GetResultRowsNum(res);

    //登录失败
    if(row == 0)
    {
        return -1;
    }
    else
    {//登录成功
        return 0;
    }

    FreeResult(res);
    MysqlClose(msq);
}


/*
 *函数功能: 根据ID获取到用户昵称，并写入m_Msg成员中
 *参数说明: findName: ChatInfo类型的指针变量用来回送查找到的昵称
 *返回值: 无
 */
void GetClientNameByID(ChatInfo *findName)
{
    //定义一个数据库句柄
    MYSQL *msq = NULL;

    //初始化数据库句柄
    msq = InitMysql();   

    //连接数据库
    if(-1 == ConnectMysql(msq, "localhost", "root", "123456", "QQDatabase"))
    {
        exit(1);
    }

    char query[100];
    memset(query, 0, sizeof(query));
    sprintf(query, "select userName from QQ_user where userID = '%s'", findName->m_SrcID);


    //执行查询语句
    MysqlQuery(msq, query);

    //获取查询结果集
    MYSQL_RES *res = NULL;
    res = GetSQLExecuteResult(msq);

    MYSQL_ROW rows;
    rows = mysql_fetch_row(res);
    sprintf(findName->m_Msg, "%s", rows[0]);

    FreeResult(res);
    MysqlClose(msq);

}

