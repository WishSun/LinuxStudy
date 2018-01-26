#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>


//共享内存的唯一键值
#define IPC_KEY 0x1234567

//共享内存大小
#define MEM_MAX 1024

//接收共享内存标识符，我们用该标识符来对这块共享内存进行操作
int shmid = -1;

int main(void)
{
    //创建一块共享内存，如果共享内存已存在，则打开该共享内存
    if((shmid = shmget(IPC_KEY, MEM_MAX, IPC_CREAT)) == -1)
    {
        perror("shmget");
        exit(1);
    }

    //共享内存映射到进程空间的首地址
    void *shm_start = NULL;

    //将共享内存映射到进程空间，并返回映射后的首地址
    //第三个参数0: 表示对这块共享内存可读可写
    if((shm_start = shmat(shmid, NULL, 0)) == NULL)
    {
        perror("shmat");
        exit(1);
    }

    int i = 0;
    while(1)
    {
        //从共享内存中读取数据
        printf("shar mem:[%s]", (char *)shm_start);

        sleep(1);
    }

    //解除进程空间与该共享内存的映射
    shmdt(shm_start);

    //删除共享内存连接(当到最后个连接数时，就会删除该共享内存)
    shmctl(shmid, IPC_RMID, NULL);


    return 0;
}
