/*************************************************************************
	> File Name: pthread.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月22日 星期一 09时58分44秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

void *thr_func(void *args)
{ 
    pthread_t tid;

    //分离本线程，线程终止后不需pthread_join函数等待
    pthread_detach(tid = pthread_self());

    printf("this is new pthread : %s---%d\n", (char *)args, (int)tid);
    sleep(3);

    pthread_exit("over!");
}

int main(void)
{
    pthread_t tid;
    int ret;
    char buff[] = "my pthread!!!";

    ret = pthread_create(&tid, NULL, thr_func, (void *)buff);
    if(ret != 0)
    {
        perror("pthread create");
        exit(1);
    }

   // char *ptr;
   // pthread_join(tid, (void **)&ptr);
   // printf("线程退出: [%s]\n", ptr);

    while(1)
    {
        printf("this is main pthread! --%d\n", (int)pthread_self());
        sleep(3);
    }
    return 0;
}
