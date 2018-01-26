/*************************************************************************
	> File Name: cond.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月22日 星期一 15时19分09秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>

//定义互斥锁变量
pthread_mutex_t mutex;

//定义条件变量
pthread_cond_t cond;

int global = 1;

//线程函数
void *thr_func(void *args)
{
    pthread_detach(pthread_self());
    int *i = (int *)args;

    while(1)
    {
        //加锁
        pthread_mutex_lock(&mutex);

        while(global == 1)
        {
            //1-解mutex锁， 2-睡眠， 3-被唤醒后加mutex锁
            pthread_cond_wait(&cond, &mutex);
        }
        global = 1;

        if(*i < 100)
        {
            usleep(1);
            printf("this is child----[%d]\n", (*i)++);
        }
        //解锁
        pthread_mutex_unlock(&mutex);

        //信号唤醒其他线程条件变量的等待
        pthread_cond_signal(&cond);
    }
    return NULL;
}

int main(void)
{
    pthread_t tid;
    int ret, i = 0;

    //初始化锁
    pthread_mutex_init(&mutex, NULL);

    //初始化条件变量
    pthread_cond_init(&cond, NULL);


    ret = pthread_create(&tid, NULL, thr_func, (void *)&i);
    if(ret != 0)
    {
        perror("pthread_create");
        exit(1);
    }

    while(1)
    {
        //加锁
        pthread_mutex_lock(&mutex);

        while(global == 0)
        {
            //1-解mutex锁， 2-睡眠， 3-被唤醒后加mutex锁
            pthread_cond_wait(&cond, &mutex);
        }
        global = 0;

        if(i < 100)
        {
            usleep(1);
            printf("this is main!----[%d]\n", i++);
        }
        //解锁
        pthread_mutex_unlock(&mutex);

        //信号唤醒其他线程条件变量的等待
        pthread_cond_signal(&cond);
    }
    return 0;
}
