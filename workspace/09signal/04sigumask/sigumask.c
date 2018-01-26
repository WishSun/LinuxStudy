#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>


int main(void)
{
    //定义一个信号集合
    sigset_t n_set;
    sigset_t o_set;

    //清空信号集合
    sigemptyset(&n_set);

    //将SIGINT信号添加到信号集n_set中
    sigaddset(&n_set, SIGINT);

    //将n_set集合中的信号添加到信号屏蔽集合中
    //将原先屏蔽集合中信号保存到o_set中
    sigprocmask(SIG_BLOCK, &n_set, &o_set);

    getchar();
    //从信号屏蔽集合中将n_set中的信号删除
    sigprocmask(SIG_UNBLOCK, &n_set, NULL);

    //将信号屏蔽集合设置为o_set集合中的信号
    //sigprocmask(SIG_SETMASK, &n_set, NULL);

    while(1)
    {
        printf("test--------\n");
        sleep(1);
    }
    return 0;
}

