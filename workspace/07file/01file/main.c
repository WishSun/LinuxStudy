#include <stdio.h>
#include <unistd.h> //包含一些宏定义
#include <fcntl.h>  //包含一些文件控制函数
#include <errno.h>
#include <string.h>

int main(void)
{
    int fd;
    printf("%d\n", umask(0)); //设置调用进程的创建掩码为0
    fd = open("test.txt", O_RDWR | O_CREAT, 0777);
    if ( fd < 0 )
    {
        printf("open errno: %s\n", strerror(errno));
        perror("open 2 errno ::");
        return -1;
    }

    lseek(fd, 0, SEEK_END);
    write(fd, "hellohaoxuesheng", strlen("hellohaoxuesheng"));

    lseek(fd, 0, SEEK_SET);
    struct stat st;
    if ( stat("./test.txt", &st) < 0 )
    {
        perror("get file stat error!");
        return -1;
    }

    ftruncate(fd, 5);
    
    printf("file size : %d \n", (int)st.st_size);

    close(fd);
    return 0;
}
