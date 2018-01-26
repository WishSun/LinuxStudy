#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void)
{
    int pid;

    pid = fork();

    if(pid < 0)
    {
        exit(-1);
    }
    //退出父进程
    else if(pid > 0)
    {
        exit(0);
    }

    //改变创建文件的权限掩码
    umask(0);

    //创建一个新会话
    setsid();

    //改变进程工作目录
    chdir("/");

    //关闭所有文件描述符, getdtablesize函数获取进程可打开最大文件数
    int i = 0;
    for(i = 0; i < getdtablesize(); i++)
    {
        close(1);
    }

    while(1)
    {
        sleep(1);
    }

}
