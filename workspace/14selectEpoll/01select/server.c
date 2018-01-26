/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月26日 星期五 09时51分41秒
 ************************************************************************/

#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>

/*
//设置文件为非阻塞
void setnoneblock(int fd)
{
    int flag;
    flag = fcntl(fd, F_GETFD, 0);
    fcntl(fd, F_SETFD, flag | O_NONBLOCK);
}
*/

int main(void)
{
    int lis_fd, len, cli_fd;
    struct sockaddr_in serv_addr;
    struct sockaddr_in cli_addr;

    //定义select监测描述符集合
    fd_set  allSet;

    //定义监测描述符集合的备份
    fd_set  resetSet;

    //创建监听套接字
    if((lis_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    //配置服务器地址和端口
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9000);
    serv_addr.sin_addr.s_addr = inet_addr("192.168.132.149");

    len = sizeof(struct sockaddr);

    //将监听套接字绑定到指定地址和端口进行监听
    if(bind(lis_fd, (struct sockaddr *)&serv_addr, len) == -1)
    {
        perror("bind");
        exit(1);
    }

    //开始监听
    if(listen(lis_fd, 1024) == -1)
    {
        perror("listen");
        exit(1);
    }

    //最大描述符
    int max_fd = lis_fd;

    FD_ZERO(&allSet);          //清空描述符集合
    FD_SET(lis_fd, &allSet);   //将监听套接字描述符添加进描述符集合

    struct timeval tv;
    
    //接受新到来的TCP连接
    while(1)
    {
        //设置select超时时间
        tv.tv_sec = 3;
        tv.tv_usec = 0;   

        //备份描述符集合
        memcpy(&resetSet, &allSet, sizeof(allSet));

        //小于0: 出错; 等于0: 超时未有就绪描述符; 大于0: 有返回值那么多个描述符就绪了
        int nfds = select(max_fd + 1, &resetSet, NULL, NULL, &tv);
        if (nfds < 0)
        {
            perror("select error!\n");
            exit(1);
        }
        else if(nfds == 0)
        {
            printf("time out!\n");
            continue;
        }
        int i = 0;
        for(i = 0; i < max_fd + 1; i++)
        {
            //判断描述符i是否就绪
            if(FD_ISSET(i, &resetSet))
            {
                if(i == lis_fd)
                {//如果是监听套接字就绪，说明有新客户端连接到来
                    if((cli_fd = accept(lis_fd, (struct sockaddr *)&cli_addr, &len)) == -1)
                    {
                       perror("accept");
                       continue;           
                    }

                    //将新的描述符添加到探测集合
                    FD_SET(cli_fd, &allSet);

                    //设置最大描述符
                    max_fd = max_fd > cli_fd ? max_fd : cli_fd;
                }
                else
                {//如果不是监听套接字，则说明是客户端描述符有数据到来
                    char buff[1024] = {0};
                    int ret = recv(i, buff, 1024, 0);;
                    if(ret == 0)
                    {
                        FD_CLR(i, &allSet);
                        close(i);
                        continue;
                    }
                    printf("client say: [%s]", buff);
                }
            }
        }

    }

    return 0;
}
