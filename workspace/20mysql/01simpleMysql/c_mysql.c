#include <mysql/mysql.h>
#include <stdlib.h>
#include <stdio.h>

int main( void )
{
	MYSQL *mysql = NULL;
	mysql = mysql_init(NULL);
	if( mysql == NULL )
	{
		printf("init %s\n", mysql_error(mysql));
	}
	printf("初始化成功\n");

    //连接一个叫xiwang的数据库
	if( !mysql_real_connect(mysql, "localhost", "root", "123456", "xiwang", 3306, NULL, 0) )
	{
		printf("init %s\n", mysql_error(mysql));
		exit(0);
	}
	printf("连接数据库成功\n");

	//mysql_query(mysql, "insert into student(id, name) value(4, 'xiaoma')");
	//mysql_query(mysql, "delete from student where id=4");
	mysql_query(mysql, "select sno, sname from student");

	MYSQL_RES * res = NULL;
	res = mysql_store_result(mysql);
	int col = mysql_num_fields(res); //获取查询结果的列数
	int row = mysql_num_rows(res);  //获取查询结果的行数
	MYSQL_FIELD * field = NULL;

	printf("表行数为%d，表列数为%d\n", row, col);	
	while ( (field = mysql_fetch_field(res)) != NULL )
	{
		printf("%s\t", field->name);  //输出表头
	}
	printf("\n");	
	
	
	MYSQL_ROW rows;
	int i;
	while( (rows = mysql_fetch_row(res)) != NULL )
	{// 检索结果集(select结果)的下一行，如果没有要检索的行则返回NULL
		for ( i=0; i<col; i++)
		{
			printf("%s\t", rows[i]);
		}
		printf("\n");
		//将查找的结果的每个元组输出
	}
	
	mysql_free_result(res); //释放res	
	mysql_close(mysql);    //关闭
	printf("数据库已断开\n");
	return 0;
}
