/*************************************************************************
	> File Name: udpServer.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月23日 星期二 16时29分44秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(void)
{
    int sock_fd;
    struct sockaddr_in serv_addr;

    //创建套接字
    if((sock_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        perror("socket");
        exit(1);
    }

    //配置服务器地址信息
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9000);
    serv_addr.sin_addr.s_addr = inet_addr("192.168.132.147");

    //绑定地址和端口
    if(bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }

    char buff[1024] = {0};
    struct sockaddr_in cli_addr;
    int len = sizeof(struct sockaddr_in);
    while(1)
    {
        memset(&cli_addr, 0x00, len);
        memset(buff, 0x00, 1024);
        recvfrom(sock_fd, buff, 1024, 0, (struct sockaddr *)&cli_addr, &len);

        printf("recv buff:[%s] from [%s-%d]\n", buff, inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
    }

    close(sock_fd);

    return 0;
}
