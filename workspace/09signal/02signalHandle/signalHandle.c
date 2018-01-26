#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void sig_handle(int signum)
{
    printf("recv signal: [%d]\n", signum);
}

int main(void)
{
    //使用自定义信号处理方式
    //SIG_IGN : 忽略
    signal(SIGINT, sig_handle);
    signal(SIGQUIT, sig_handle);
    signal(SIGTSTP, sig_handle);

    while(1)
    {
        sleep(1);
    }

    return 0;
}
