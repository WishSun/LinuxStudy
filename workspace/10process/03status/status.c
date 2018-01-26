#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


int main(void)
{
    int pid;
    int status;

    if((pid = fork()) < 0)
    {
        perror("fork");
        exit(-1);
    }
    else if(pid == 0)
    {
        sleep(3);
        exit(10);
    }
    wait(&status);

    //通过status判断子进程是否为正常退出
    if (WIFEXITED(status))
    {
        //WEXITSTATUS返回子进程的退出状态
        printf("exited status:[%d]\n", WEXITSTATUS(status));
    }

    return 0;
}
