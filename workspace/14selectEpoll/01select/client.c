/*************************************************************************
	> File Name: client.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月26日 星期五 10时15分26秒
 ************************************************************************/

#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(void)
{
    int sock_fd, len;
    struct sockaddr_in sev_addr;

    //配置服务端地址和端口
    sev_addr.sin_family = AF_INET;
    sev_addr.sin_port = htons(9000);
    sev_addr.sin_addr.s_addr = inet_addr("192.168.132.149");

    len = sizeof(struct sockaddr_in);

    //创建客户端套接字
    if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    //连接服务器
    if(connect(sock_fd, (struct sockaddr *)&sev_addr, len) == -1)
    {
        perror("connect");
        exit(1);
    }

    char buff[] = "I am client!\n";
    while(1)
    {
        if(send(sock_fd, buff, strlen(buff), 0) == -1)
        {
            perror("send");
            exit(1);
        }
        sleep(1);
    }

    return 0;
}

