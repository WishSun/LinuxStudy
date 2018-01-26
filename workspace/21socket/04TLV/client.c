/*************************************************************************
	> File Name: client.c
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
                printf("server closed!");
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

//发送请求给服务器
int send_handle(int sock_fd)
{
    seq_head_t  seq_head;
    seq_head_t  rsp_head;
    file_head_t file_head;

    char filename[] = "./bigfile";
    seq_head.type = SEQ_FILE;
    seq_head.length = strlen(filename);

    //发送文件请求
    if(send_data(sock_fd, (char *)&seq_head, sizeof(seq_head_t)) == -1)
    {
        printf("send SEQ_FILE error\n");
        return -1;
    }

    //发送请求的文件名
    if(send_data(sock_fd, filename, seq_head.length) < -1)
    {
        printf("send filename error\n");
        return -1;
    }

     //从服务器接收对请求的回应
    if(recv_data(sock_fd, (char *)&rsp_head, sizeof(seq_head_t)) == -1)
    {
        printf("recv file head error\n");
        return -1;
    }

    //检查回应是否正确
    if(rsp_head.type != RSP_FILE)
    {
        printf("rsp type is error!\n");
        return -1;
    }

    //从服务器接收文件头
    if(recv_data(sock_fd, (char *)&file_head, rsp_head.length) == -1)
    {
        printf("recv file head error\n");
        return -1;
    }

    //从服务器接收文件数据信息
    int fd;
    //打开一个新文件来存储接收的文件数据，如果文件存在则清空文件数据
    if((fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0664)) == -1)
    {
        printf("open file: [%s] error\n", filename);
        return -1;
    }
    int write_len = 0;
    char buff[BUFF_MAX];

    printf("\n已接收");
    printf("  0%%");
    while(write_len < file_head.fileLen)
    {
        double flen = write_len;
        printf("\b\b\b\b%3.0f%%", (flen/file_head.fileLen) * 100);

        //计算出还要接收的字节数len
        int len = file_head.fileLen - write_len;
        len = len > BUFF_MAX ? BUFF_MAX : len;

        //从服务器接收len字节数据
        if(recv_data(sock_fd, buff, len) == -1)
        {
            printf("recv file data error\n");
            return -1;
        }

        //将接收的文件数据写入文件
        write(fd, buff, len);
        write_len += len;
    }

    printf("\n接收完毕!\n");
    close(fd);

    return 0;
}

//去掉命令开头和末尾的空字符
void deleteSpaceOfCmd(char *cmdBuff)
{
    char *strHead = cmdBuff;
    char *strTail = cmdBuff + strlen(cmdBuff);

    while(isspace(*strHead))
    {
        strHead++;
    }
    while(isspace(*strTail))
    {
        strTail--;
    }
    int len = strTail - strHead + 1;
    int i = 0;
    for(i = 0; i<len; i++)
    {
        *cmdBuff = *strHead;
        cmdBuff++;
        strHead++;
    }
    *cmdBuff = '\0';
}

int main(void)
{
    int sock_fd;
    struct sockaddr_in serverAddr;
    int len = sizeof(struct sockaddr_in);

    //创建客户端通信套接字
    if((sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
    {
        perror("socket");
        exit(1);
    }

    //配置服务器地址和端口
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    //连接服务器
    if(connect(sock_fd, (struct sockaddr *)&serverAddr, len) == -1)
    {
        perror("connect");
        exit(1);
    }

    printf("请输入命令:\n");
    printf("命令说明:\n\tshow : 显示服务器端文件名和大小\n\tget + 文件名 : 从服务器下载改文件到当前目录\n\thelp : 显示命令说明\n\tquit : 退出程序\n");
    char cmdBuff[256] = {0};
    while(1)
    {
        memset(cmdBuff, 0x00, sizeof(cmdBuff));
        printf(">> ");
        if(fgets(cmdBuff, 256, stdin) != NULL)
        {
            deleteSpaceOfCmd(cmdBuff);
            if(!strncasecmp(cmdBuff, "show", strlen("show")))
            {
                printf("执行show命令\n");
            }
            else if(!strncasecmp(cmdBuff, "get", strlen("get")))
            {
                send_handle(sock_fd);
            }
            else if(!strncasecmp(cmdBuff, "help", strlen("help")))
            {
                printf("命令说明:\n\tshow : 显示服务器端文件名和大小\n\tget + 文件名 : 从服务器下载改文件到当前目录\n\thelp : 显示命令说明\n\tquit : 退出程序\n");
            }
            else if(!strncasecmp(cmdBuff, "quit", strlen("quit")))
            {
                close(sock_fd);
                exit(0);
            }
            else
            {
                printf("命令无效！请重新输入\n");
            }
        }
    }


    return 0;
}
