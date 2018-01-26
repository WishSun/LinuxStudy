#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

//处理子进程退出信号
void sigcb(int signo)
{
    int status;
    if( signo == SIGCHLD )
    {
        printf("child exit!\n");

        //接收子进程退出信息(为子进程收尸)
        wait(&status);
    }
}

int main(void)
{
    int pid = -1;

    //绑定SIGCHLD信号和该信号的处理函数sigcb
    signal(SIGCHLD, sigcb);

    pid = fork();

    if(pid < 0)
    {
        return -1;
    }
    else if(pid == 0)
    {
        //子进程退出
        exit(0);
    }
    while(1)
    {   //父进程死循环，不退出
        sleep(1);
    }
    return 0;
}

/*
int main(void)
{
    int pid = -1;
    int status;
    int ret;

    signal(SIGCHLD, sigcb);

    pid = fork();

    if(pid < 0)
    {
        return -1;
    }else if(pid == 0)
    {
        usleep(3);
        exit(0);
    }

    //-1表示任意子进程, status获取退出状态, 0表示阻塞等待
    while((ret = waitpid(pid, &status, WNOHANG)) == 0)
        printf("have no child exited!\n");

    if( ret > 0 )
    {
        printf("child pid : [%d-%d] exited!\n", ret, pid);
    }
    else
    {
        printf("waitpid error!\n");
    }
    while(1)
    {
        printf("wait--------\n");
        sleep(1);
    }
    return 0;
}

*/
