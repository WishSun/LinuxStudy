/*************************************************************************
	> File Name: mutex.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月22日 星期一 14时03分43秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

//定义互斥锁变量
pthread_mutex_t  mutex;

void *thr_func(void *args)
{
    //分离本线程
    pthread_detach(pthread_self());

    int *i = (int *)args;
    while(1)
    {
        //加锁
        pthread_mutex_lock(&mutex);
        if(*i < 100)
        {
            usleep(100);
            printf("this is child---[%d]\n", (*i)++);
        }
        //解锁
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(void)
{
    pthread_t tid;
    int ret;
    int i = 0;

    //初始化锁变量
    pthread_mutex_init(&mutex, NULL);


    //创建线程
    ret = pthread_create(&tid, NULL, thr_func, (void *)&i);
    if(ret != 0)
    {
        perror("pthread create");
        exit(1);
    }

    while(1)
    {
        //加锁
        pthread_mutex_lock(&mutex);
        if(i < 100)
        {
            usleep(100);
            printf("this is main!---[%d]\n", i++);
        }
        //解锁
        pthread_mutex_unlock(&mutex);
    }

    return 0;
}
