#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

//使用管道实现 ps -ef  | grep ssh

int main(void)
{
    int pipefd[2];
    int pid;

    if(pipe(pipefd) == -1)
    {
        perror("pipe");
        exit(1);
    }

    
    if((pid = fork()) == -1)
    {
        perror("fork");
        exit(1);
    }

    if(pid == 0)
    {
        //子进程执行ps -ef

        //关闭管道读取端
        close(pipefd[0]);

        //将标准输出重定向到管道输入端
        dup2(pipefd[1], 1);
        execl("/bin/ps", "ps", "-ef", NULL);
        exit(0);
    }

    //父进程执行grep ssh

    //关闭管道写入端
    close(pipefd[1]);

    //将标准输入重定向到管道读取端，原本从标准输入读取内容，现在从管道读取端读取内容
    dup2(pipefd[0], 0);
    execl("/bin/grep", "grep", "ssh", NULL);
    return 0;
}
