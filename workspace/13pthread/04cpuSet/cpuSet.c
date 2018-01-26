/*************************************************************************
	> File Name: cpuSet.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月22日 星期一 16时41分45秒
 ************************************************************************/

//必须先定义这个宏
#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include <sched.h>



//绑定CPU核心
int cpu_bind(int cpuid)
{
    cpu_set_t set;

    CPU_ZERO(&set);
    CPU_SET(cpuid, &set);
    if(pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &set) != 0)
    {
        printf("bind core: [%d] failed!\n", cpuid);
        exit(1);
    }
}

//线程函数
void *thr_func(void *args)
{
    unsigned long i = 0;

    //绑定本线程到第一个cpu核心
    cpu_bind(0);

    while(1)
    {
        i++;
    }

    return NULL;
}

int main(void)
{
    pthread_t tid;
    int ret;

    ret = pthread_create(&tid, NULL, thr_func, NULL);
    if(ret != 0)
    {
        perror("pthread_create");
        exit(1);
    }

    pthread_join(tid, NULL);
    
    return 0;
}
