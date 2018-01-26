#include "c_mysql.h"
#include <stdio.h>
/**
  *数据库模块实现文件
  *
  */


/**
  *函数功能: 初始化数据库
  *参数说明: 无
  *返回值 :  返回一个数据库连接句柄
  */
MYSQL* InitMysql()
{
    MYSQL *msq = mysql_init(NULL);
    if( msq == NULL )
    {
        printf("init %s\n", mysql_error(msq));
    }
    return msq;
}


/**
  *函数功能: 连接指定数据库
  *参数说明: @msq: 一个初始化过的数据库连接句柄
  *          @host:数据库位置主机名
  *          @user:访问数据库的用户名
  *          @passwd: 用户名所对应的密码
  *          @db    : 所要连接的数据库名
  *返回值:   成功返回0
  *          失败返回-1
  */
int ConnectMysql(MYSQL *msq, const char *host, const char *user, const char *passwd, const char *db)
{
    if( !mysql_real_connect(msq, host, user, passwd, db, 3306, NULL, 0) )
    {
        printf("connect %s\n", mysql_error(msq));
        return -1;
    }
    return 0;
}


/**
  *函数功能: 执行指定SQL语句查询
  *参数说明: @msq: 已连接的数据库句柄
  *          @query: 指定的SQL语句
  *返回值  : 无
  */
void MysqlQuery(MYSQL *msq, const char *query)
{
	mysql_query(msq, query);
}


/**
  *函数功能: 在执行指定SQL语句后，必须通过该函数来获取执行结果
  *参数说明: @msq:已连接的数据库句柄
  *返回值  : 返回一个MYSQL_RES结构，用来存储执行结果
  */
MYSQL_RES *GetSQLExecuteResult(MYSQL *msq)
{
	return  mysql_store_result(msq);
}


/**
  *函数功能: 获取查询结果的列数
  *参数说明: @res: MYSQL_RES类型的结果结构
  *返回值 :  列数值
  */
int GetResultFieldsNum(MYSQL_RES *res)
{
	return mysql_num_fields(res); 
}
 

/**
  *函数功能: 获取查询结果的行数
  *参数说明: @res: MYSQL_RES类型的结果结构
  *返回值 :  行数值
  */
int GetResultRowsNum(MYSQL_RES *res)
{
	return mysql_num_rows(res);  
}

 
/**
  *函数功能: 根据结果集参数得到结果集的所有列的信息，重复调用
             该函数，以检索关于结果集中所有列的信息。未剩余字
             段时，返回NULL, 每次执行新的查询时，将复位该函数
             ，以返回关于第一个字段的信息。
  *参数说明: @res: MYSQL_RES类型的结果结构
  *返回值 :  列信息结构指针
  */
MYSQL_FIELD *GetFieldInfoInResult(MYSQL_RES *res)
{
	return mysql_fetch_field(res);
}


/**
  *函数功能: 根据结果集参数得到结果集的所有元组的信息，重复调
             用该函数，以检索关于结果集中所有元组的信息。未剩
             余行时(即到了最后一个元组)，返回NULL, 每次执行新
             的查 询时，将复位该函数 ，以返回关于第一个元组的
             信息。 *参数说明: @res: MYSQL_RES类型的结果结构
  *返回值 :  元组信息结构
  */
MYSQL_ROW  GetRowInfoInResult(MYSQL_RES *res)
{
    return mysql_fetch_row(res);
}


/**
  *函数功能: 释放结果集对象
  *参数类型: MYSQL_RES类型结果集指针
  *返回值 : 无
  */
void FreeResult(MYSQL_RES *res)
{
	mysql_free_result(res); 
}


/**
  *函数功能: 关闭数据库连接
  *参数说明: @msq: 数据库连接句柄
  *返回值 :  无
  */
void MysqlClose(MYSQL *msq)
{
    mysql_close(msq);   
}



