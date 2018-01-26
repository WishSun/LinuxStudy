/*************************************************************************
	> File Name: fileStruct.h
	> Author: 
	> Mail: 
	> Created Time: 2018年01月25日 星期四 09时38分35秒
 ************************************************************************/

#ifndef _FILESTRUCT_H
#define _FILESTRUCT_H
#include <stdint.h>
#include <limits.h>

//linux中文件名最大长度为256字节
#ifndef PATH_MAX
#define PATH_MAX 256
#endif

#define BUFF_MAX 1024

//TLV格式中，type字段的类型
enum cmd_t
{
    SEQ_LIST = 0,       //请求文件列表
    RSP_LIST,           //对请求文件列表的回复
    SEQ_FILE,           //请求文件数据
    RSP_FILE,           //对请求文件数据的回复
};

//公共请求头结构
typedef struct __seq_head_t
{
    uint8_t     type;           //此次请求的类型
    uint64_t    length;         //此次请求携带的数据的长度
}seq_head_t;

//文件头结构
typedef struct __file_head_t
{
    char        fileName[PATH_MAX];  //文件名
    uint64_t    fileLen;             //文件长度
}file_head_t;

//服务器IP和端口
#define SERVER_IP "192.168.132.149"
#define SERVER_PORT 9000
#define MAX_LISTEN  1024


#endif
