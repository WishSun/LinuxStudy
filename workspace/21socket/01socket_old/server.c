#include <stdio.h> 
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

//每次接收TCP包的请求大小是1M
#define BUFFER_SIZE  (1024 * 1024)

/**函数说明: 自定义错误处理函数
  *参数说明: 错误发生函数名
  *返回值: 无
 **/
void my_error(char *strerr)
{
    perror(strerr);
    exit(1);
}


/**函数说明: 与单个客户端通信的线程函数
  *功能 :    传输文件
  *参数说明: 与客户端通信的套接字描述符
 **/
void *route_tcp(void *arg)
{
    int cfd = *(int*)arg;   //获取到与客户端通信的套接字
    free(arg);      //释放其所占内存

    printf("进入线程环境...\n");

    //每次接收1M
    char buf[BUFFER_SIZE] = {};
    int len = 0;
    int sum = 0;
    while( 1 )
    {
        memset(buf, 0, sizeof(buf));
        if((len = read(cfd, buf, BUFFER_SIZE)) == 0 )
        {
            printf("client close\n");
            break;
        }
        sum += len;
        printf("当前接收到包大小: %d, 已接收%d\n", len, sum);
       // printf("buf[%s]\n", buf);
        printf("实际写入大小:%d\n", write(cfd, buf, len));
    }
    close(cfd);
}

/**函数说明: 初始化套接字描述符(创建、绑定)
  *参数说明: @SocketType: 套接字类型，TCP为 SOCK_STREAM; UDP为 SOCK_DGRAM;
  *返回值:   执行成功返回对应类型的套接字描述符
  *          执行失败退出程序
 **/
int initSocket(int SocketType)
{
    int sock_fd = socket(AF_INET, SocketType, 0);
    if( sock_fd == -1 )
    {
        my_error("socket");
    }

     //为了使其他程序也可绑定该端口
    int option_value = 1;
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(option_value));


    struct sockaddr_in  addr;

    addr.sin_family = AF_INET;  
    addr.sin_port = htons(5000);  //指定端口为5000
    inet_aton("192.168.132.133", &addr.sin_addr);  //指定服务器IP地址

    //绑定服务器地址和端口到套接字
    if( bind(sock_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) < 0 )
    {
        my_error("bind");
    }

    return sock_fd;
}


/**函数说明: 与多个客户端进行udp通信的线程函数
  *功能 :  聊天
  *参数说明: 不用
 **/
void *route_udp(void *arg)
{
    //创建服务端监听套接字(无连接的UDP)
    int udp_fd = initSocket(SOCK_DGRAM);

    printf("udp 通信线程绑定完毕，正在等待接收消息...\n");


    char buf[1024] = {};
    //接收客户端地址结构
    struct sockaddr_in cli_addr;
    int len = sizeof(cli_addr);

    while( 1 )
    {
        memset(buf, 0, sizeof(buf));

        //从客户端接收UDP消息，并获取客户端地址
        recvfrom(udp_fd, buf, 1024, 0, (struct sockaddr*)&cli_addr, &len);

        printf("udp client say: %s\n", buf);

        //通过客户端地址将消息返回给客户端
        sendto(udp_fd, buf, strlen(buf), 0, (struct sockaddr*)&cli_addr, len);
    }

}


//监听队列长度(最大接收TCP客户端请求个数)
#define LISTEN_NUM  800

int main(void)
{
    pthread_t  udp_tid;
    //创建udp通信线程
    pthread_create(&udp_tid, NULL,route_udp, NULL);
    //分离线程
    pthread_detach(udp_tid);


    //创建服务端监听套接字(面向连接的TCP)
    int listen_fd = initSocket(SOCK_STREAM);
    //监听客户端请求(将listen_fd转换为监听套接字)
    if( listen(listen_fd, LISTEN_NUM) < 0 )
    {
        my_error("listen");
    }
    printf("绑定成功, 正在监听客户端\n");

    //客户端地址和端口信息
    struct sockaddr_in client_addr;
    int cli_len = sizeof(struct sockaddr_in);

    while ( 1 )
    {
        //获取到与客户端通信的套接字cli_fd
        int cli_fd = accept(listen_fd, NULL, NULL); 
        if( cli_fd == -1 )
        {
            my_error("accept");
        }

        printf("有新客户端连接...\n");

        pthread_t  tid;
        int *pcli_fd = malloc(sizeof(int));
        *pcli_fd = cli_fd;

        //创建通信线程，并将通信套接字作为参数传递给线程函数
        pthread_create(&tid, NULL, route_tcp, pcli_fd);
        //分离线程
        pthread_detach(tid);
    }
    return 0;
}
