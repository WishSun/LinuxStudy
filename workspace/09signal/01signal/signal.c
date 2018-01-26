#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main(void)
{
    int pid;
    union sigval val;

    //getpid获取当前进程自身的进程ID
    pid = getpid();

    //向自身进程发送SIGKILL信号, 并打印killed
    //kill(pid, SIGKILL);

    val.sival_int = 999;
    //向自身进程发送SIGKILL信号, 并传送一个union sigval类型参数val, 打印killed
    sigqueue(pid, SIGKILL, val);


    sleep(10);
    return 0;
}
