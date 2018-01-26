/*************************************************************************
	> File Name: server.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月26日 星期五 14时56分59秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <sys/epoll.h>
#include <sys/types.h>


int main(void)
{
    int lis_fd, cli_fd;
    struct sockaddr_in sev_addr;
    struct sockaddr_in cli_addr;
    int len = sizeof(struct sockaddr);

    //创建监听套接字
    if((lis_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }

    int option = 1;
    //设置lis_fd可地址重用
    setsockopt(lis_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&option, sizeof(int));

    //配置服务器地址和端口
    sev_addr.sin_family = AF_INET;
    sev_addr.sin_port = htons(9000);
    sev_addr.sin_addr.s_addr = inet_addr("192.168.132.149");

    //为监听套接字绑定地址和端口
    if(bind(lis_fd, (struct sockaddr *)&sev_addr, len))
    {
        perror("bind");
        exit(1);
    }

    //开始监听
    if(listen(lis_fd, 10) == -1)
    {
        perror("listen");
        exit(1);
    }

    //创建一个epoll的监听集合
    int epfd;
    if((epfd = epoll_create(10)) == -1)
    {
        perror("epoll_create");
        exit(1);
    }

    //存放已就绪的描述符的事件数组
    struct epoll_event evs[10];
    struct epoll_event ev;

    //将监听套接字描述符添加到监听列表
    ev.events = EPOLLIN;   //监听可读事件
    ev.data.fd = lis_fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, lis_fd, &ev);

    while(1)
    {
        //等待监听的描述符就绪事件的发生, 如有被监听的描述符就绪，则将该描述符放入evs数组中，返回值为就绪描述符个数
        int nfds = epoll_wait(epfd, evs, 10, 3000);
        if(nfds < 0)
        {
            perror("epoll_wait");
            continue;
        }
        else if(nfds == 0)
        {
            printf("time out!\n");
            continue;
        }
        int i = 0;

        //nfds是就绪描述符个数, evs中前nfds个描述符都是已就绪的描述符
        for(i = 0; i < nfds; i++)
        {
            if(evs[i].data.fd == lis_fd)
            {   //监听套接字描述符就绪，说明有新连接到来
                if((cli_fd = accept(lis_fd, (struct sockaddr *)&cli_addr, &len)) == -1)
                {
                    perror("accept");
                    continue;
                }

                //将新客户端的通信套接字描述符添加到监听列表
                ev.data.fd = cli_fd;
                ev.events = EPOLLIN;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cli_fd, &ev);
            }
            else if(evs[i].events & EPOLLIN)
            {   //判断该描述符是否为可读事件
                int ret;
                char buff[1024] = {0};
                if(recv(evs[i].data.fd, buff, 1024, 0) == -1)
                {
                    perror("recv");

                    //将该描述符从监听列表中移除，并关闭该描述符
                    epoll_ctl(epfd, EPOLL_CTL_DEL, evs[i].data.fd, &ev);
                    close(evs[i].data.fd);
                    continue;
                }
                printf("buff:[%s]\n", buff);
            }
            else
            {
                epoll_ctl(epfd, EPOLL_CTL_DEL, evs[i].data.fd, &ev);
                close(evs[i].data.fd);
            }
        }

    }
    return 0;
}
