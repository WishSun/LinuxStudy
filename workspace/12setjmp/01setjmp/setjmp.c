#include <stdio.h>
#include <setjmp.h>
#include <signal.h>


//保存待跳转位置的栈信息
jmp_buf env;

void PrintHello(int signo)
{
    printf("Hello, 你好！\n");
    longjmp(env, 1);
    printf("我不好!\n");
}

void PrintBye(int signo)
{
    printf("Bye, 明天见！\n");
    longjmp(env, 2);
    printf("后天再见吧！");
}

int main(void)
{
    int ret;
    int status = 0;

    //设置返回点
    switch(ret = setjmp(env))
    {
        case 1:  
            printf("PrintHello返回！返回值为%d\n", ret);
            break;
        case 2:
            printf("PrintBye返回！返回值为%d\n", ret);
            break;
        default:
            printf("第一次返回值为%d\n", ret);
            break;
    }


    status += 100;
    printf("状态值为%d\n", status);
    

    //安装信号处理函数
    signal(SIGRTMIN+15, PrintHello);
    signal(SIGRTMIN+13, PrintBye);

    while(1)
    {
        sleep(1);
    }
    return 0;
}
