/*************************************************************************
	> File Name: udpClient.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月23日 星期二 16时31分28秒
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

    char buff[1024] = {0};
    while(1)
    {
        sprintf(buff, "今天的风沙好大--搞得我都迟到了---！\n");
        sendto(sock_fd, buff, strlen(buff), 0, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));

        sleep(1);
    }

    return 0;
}
