/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月25日 星期四 10时04分01秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include "fileStruct.h"

//对tcp发送数据函数进行封装
int send_data(int fd, char *buff, int len)
{
    int s_len = 0;
    int ret;

    while(s_len < len)
    {
        //将接受的数据放到已有数据后边: buff + r_len
        //接收长度为总长度len - 已经接收的长度r_len
        ret = send(fd, buff + s_len, len - s_len, 0);
        if(ret <= 0)
        {
            if(errno == EAGAIN || errno == EINTR)
            {
                continue;
            }
            return -1;
        }
        s_len += ret;
    }
}

//对tcp接收数据函数进行封装
int recv_data(int fd, char *buff, int len)
{
    int r_len = 0;
    int ret;

    while(r_len < len)
    {
        //将接受的数据放到已有数据后边: buff + r_len
        //接收长度为总长度len - 已经接收的长度r_len
        ret = recv(fd, buff + r_len, len - r_len, 0);
        if(ret <= 0)
        {
            if(ret == 0)
            {
                printf("client closed!");
                return -1;
            }
            else if(errno == EAGAIN || errno == EINTR)
            {
                continue;
            }
        }
        r_len += ret;
    }
}

//接收并处理客户端发送过来的请求
int recv_handle(int cli_fd)
{
    seq_head_t  seq_head;

    //先接收请求头结构
    if(recv_data(cli_fd, (char *)&seq_head, sizeof(seq_head_t)) == -1)
    {
        printf("recv seq head error!\n");
        return -1;
    }

    //判断请求类型
    switch(seq_head.type)
    {
        case SEQ_LIST://请求文件列表
        { 
            file_head_t *fileList = NULL;
            break;
        }
        case SEQ_FILE://请求文件数据 
        {
            char fileName[PATH_MAX];
            memset(fileName, 0x00, PATH_MAX);

            //获取文件名
            if(recv_data(cli_fd, fileName, seq_head.length) == -1)
            {
                printf("recv file name error!\n");
                return -1;
            }

            printf("请求的文件是: %s\n", fileName);

            //判断文件是否存在
            if(access(fileName, F_OK) != 0)
            {
                printf("file[%s] is not exist!\n", fileName);
                return -1;
            }

            //获取文件属性
            struct stat st;
            if(stat(fileName, &st) < 0)
            {
                perror("stat");
                return -1;
            }

            //填充文件头信息(包含文件名，文件长度)
            file_head_t file_head;
            memset(&file_head, 0x00, sizeof(file_head_t));
            memcpy(file_head.fileName, fileName, strlen(fileName));
            file_head.fileLen = st.st_size;

            //定义请求回复的公共头
            seq_head_t rsp_head;
            rsp_head.type = RSP_FILE;
            rsp_head.length = sizeof(file_head_t);  //数据大小就是文件头结构的大小

            //发送回复头信息
            if(send_data(cli_fd, (char *)&rsp_head, sizeof(seq_head_t)) == -1)
            {
                return -1;
            }
            
            //发送回复数据(文件头信息)
            if(send_data(cli_fd, (char *)&file_head, rsp_head.length) == -1)
            {
                return -1;
            }

            //发送回复数据(文件数据信息)
            int fd = open(fileName, O_RDONLY);
            if(fd == -1)
            {
                perror("open file");
                return -1;
            }
            int read_len;
            char buff[BUFF_MAX] = {0};
            while((read_len = read(fd, buff, BUFF_MAX)) > 0)
            {
                if(send_data(cli_fd, buff, read_len) == -1)
                {
                    return -1;
                }
                memset(buff, 0x00, BUFF_MAX);
            }
            //关闭文件
            close(fd);

            break;   
        }
        default:      //错误请求 
        {
            break;
        }
    }

    return 0;
}

//处理与客户端通信线程函数
void *DealClient(void *arg)
{
    //分离线程
    pthread_detach(pthread_self());

    //获取与客户端通信的套接字描述符
    int cli_fd = *(int *)arg;

    //处理数据通信
    while(1)
    {
        //接收并处理客户端发送过来的请求
        if( recv_handle(cli_fd) == -1)
        {
            close(cli_fd);
            pthread_exit(NULL);
        }  
    }        
}

int main(void)
{
    int lst_fd;    //监听套接字描述符
    int cli_fd;    //新连接的客户端套接字描述符
    struct sockaddr_in  lst_addr;    //服务器端地址信息
    struct sockaddr_in  cli_addr;    //新连接的客户端地址信息
    int len = sizeof(struct sockaddr_in);

    //创建监听套接字
    lst_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(lst_fd == -1)
    {
        perror("socket");
        exit(1);
    }

    //配置监听地址和端口
    lst_addr.sin_family = AF_INET;
    lst_addr.sin_port = htons(SERVER_PORT);
    lst_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    //将监听套接字绑定到指定地址和端口
    if(bind(lst_fd, (struct sockaddr *)&lst_addr, len) == -1)
    {
        perror("bind");
        exit(1);
    }

    //开始监听客户端,MAX_LISTEN是同时监听最大连接数(创建了一个监听队列)
    if(listen(lst_fd, MAX_LISTEN) == -1)
    {
        perror("listen");
        exit(1);
    }

    while(1)
    {
        //接受新连接的客户端连接
        cli_fd = accept(lst_fd, (struct sockaddr *)&cli_addr, &len);
        if(cli_fd == -1)
        {
            perror("accept");
            continue;
        }

        //为新到来的客户端创建处理线程
        pthread_t cli_tid;
        if(pthread_create(&cli_tid, NULL, DealClient, (void *)&cli_fd) == -1)
        {
            perror("pthread_create");
            continue;
        }
    }

    return 0;
}
