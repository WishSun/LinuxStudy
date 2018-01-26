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
    //创建一块键值为IPC_KEY, 大小为MEM_MAX共享内存, 如果已存在则失败,错误代码为EEXIST
    //if((shmid = shmget(IPC_KEY, MEM_MAX, IPC_CREAT | IPC_EXCL)) == -1)

    //创建一块共享内存，如果共享内存已存在，则打开该共享内存
    if((shmid = shmget(IPC_KEY, MEM_MAX, IPC_CREAT | 0777)) == -1)
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
        //清空共享内存
        memset(shm_start, 0x00, MEM_MAX);

        //向共享内存中写内容
        sprintf((char *)shm_start, "this is share memory, %d\n", i++);
        sleep(1);
    }

    //解除进程空间与该共享内存的映射
    shmdt(shm_start);

    //删除共享内存连接(当到最后个连接数时，就会删除该共享内存)
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
