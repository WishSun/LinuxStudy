#include <stdio.h>
#include <unistd.h>
#include <signal.h>
/*
//自定义信号处理函数
void sig_handle(int sig_num)
{
    printf("[%d]\n", sig_num);
}

int main(void)
{
    struct sigaction new_act;
    struct sigaction old_act;

    //清空信号集
    sigemptyset(&(new_act.sa_mask));
    new_act.sa_flags = 0;
    new_act.sa_handler = sig_handle;

    //为SIGINT添加新的处理动作new_act,
    //将原先的操作保存到old_act中。如果不想保存，置为NULL即可。
    sigaction(SIGINT, &new_act, &old_act);

    while(1)
    {
        printf("test------\n");
        sleep(10);
    }
    return 0;
}
*/

//自定义信号处理函数
void sig_action(int sig_num, siginfo_t *siginfo, void *context)
{
    printf("recv signo:[%d]-[%d]\n", sig_num, siginfo->si_int);
}

int main(void)
{
    struct sigaction new_act;
    struct sigaction old_act;

    //发送信号所携带的参数
    union sigval val;

    //清空信号集
    sigemptyset(&(new_act.sa_mask));

    //指定调用三个参数的信号处理函数
    new_act.sa_flags = SA_SIGINFO;

    //指定信号处理函数
    new_act.sa_sigaction = sig_action;

    //安装信号处理函数
    sigaction(SIGINT, &new_act, &old_act);
    int i = 0;
    while(1)
    {
        val.sival_int = i++;
        sigqueue(getpid(), SIGINT, val);
        sleep(1);
    }
}
