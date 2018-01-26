#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_BUFFLEN  1024

int main(void)
{
    if(mkfifo("test.fifo", 0777) == -1)
    {
        //如果创建命名管道失败是不是因为管道文件已存在,则退出
        if(errno != EEXIST)
        {
            perror("mkfifo");
            exit(1);
        }
    }

    //以只读方式打开管道文件
    int fd = open("test.fifo", O_RDONLY);
    if(fd == -1)
    {
        perror("open");
        exit(1);
    }

    char buff[MAX_BUFFLEN] = {0};
    while(1)
    {
        memset(buff, 0x00, MAX_BUFFLEN);
        if(read(fd, buff, MAX_BUFFLEN) > 0)
        {
            printf("recv msg:[%s]\n", buff);

            //忽略大小写比较buff的前5个字符是否为param
            if(!strncasecmp(buff, "param", 5))
            {
                printf("buyaoxiazhihui\n");
            }
        }
    }
    close(fd);

    return 0;
}
