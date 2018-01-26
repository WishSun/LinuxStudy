#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main(void)
{
    int cli_fd;

    //创建客户端通讯套接字
    if((cli_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }

    //指定服务器端IP地址和端口
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(struct sockaddr_in));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9000);
    inet_aton("192.168.132.147", &server_addr.sin_addr);

    //请求连接服务器
    if(connect(cli_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in)) < 0)
    {
        perror("connect");
        exit(1);
    }

    printf("连接服务器成功\n");

    char buf[1024];
    int read_len;
    memset(buf, 0, sizeof(buf));

    //不断从键盘读取输入发送往服务器
    while(fgets(buf, 1024, stdin) != NULL)
    {

        if( write(cli_fd, buf, strlen(buf)) <= 0 )
        {
            perror("write");
            exit(1);
        }
        memset(buf, 0, sizeof(buf));

        //读取服务器发送给客户端的信息
        read_len = read(cli_fd, buf, 1024);
        if(read_len <= 0)
        {
            printf("server close \n");
            exit(1);
        }

        printf("server say: %s\n", buf);
        memset(buf, 0, sizeof(buf));
    }

    close(cli_fd);
    return 0;
}
