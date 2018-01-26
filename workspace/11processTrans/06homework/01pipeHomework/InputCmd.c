#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>


#define MAX_BUFFLEN  256

int main(void)
{
    //以只写方式打开管道文件
    int fd = open("cmd.fifo", O_WRONLY);
    if(fd == -1)
    {
        perror("open");
        exit(1);
    }

    char cmdBuff[MAX_BUFFLEN] = {0};

    printf("请输入控制命令: \n");
    printf(">> ");

    while(fgets(cmdBuff, MAX_BUFFLEN, stdin) != NULL)
    {
        if(write(fd, cmdBuff, MAX_BUFFLEN) < 0)
        {
            perror("write");
            exit(1);
        }
        printf(">> ");
        memset(cmdBuff, 0x00, MAX_BUFFLEN);
    }

    //close(fd);

    return 0;
}
