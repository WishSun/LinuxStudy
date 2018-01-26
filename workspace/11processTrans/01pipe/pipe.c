#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>

int main(void)
{
    int pipefd[2] = {0};

    //
    if(pipe(pipefd) == -1)
    {
        perror("pipefd");
        exit(1);
    }

    int pid ;
    if((pid = fork()) == -1)
    {
        perror("fork");
        exit(1);
    }

    if(pid == 0)
    {//子进程

        //关闭读端
        close(pipefd[0]);

        //pipefd[1]用于向管道写入数据
        write(pipefd[1], "hello, 你好！", strlen("hello, 你好！"));
        exit(0);
    }

    char buff[1024] = {0};

    //关闭写端
    close(pipefd[1]);

    //pipefd[0]用于向管道读入数据
    read(pipefd[0], buff, 1024);
    
    printf("buff:[%s]\n", buff);

    return 0;
}
