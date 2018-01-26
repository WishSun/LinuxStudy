#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

//队列头结构
typedef struct
{
    int head;
    int tail;
    int size;
}QueueHead;

//队列结点
typedef struct
{
    int val;
}QueueNode;


#define  SHM_KEY   0x7777777

#define  NODE_NUM  10

//共享内存能存放1个队列头结构，和10个数据结点
#define  SHM_SIZE  (sizeof(QueueNode)*(NODE_NUM) + sizeof(QueueHead))

int main(void)
{
    int shmid;

    //创建一块共享内存
    if((shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0777)) == -1)
    {
        perror("shmget");
        exit(1);
    }

    void *shm_start = NULL;

    //获取共享内存到该进程空间的映射地址
    if((shm_start = shmat(shmid, NULL, 0)) == NULL)
    {
        perror("shmat");
        exit(1);
    }

    //初始化共享内存循环队列头结构为空队列
    QueueHead qHead;
    qHead.head = 0;
    qHead.tail = 0;
    qHead.size = sizeof(QueueHead);

    //将头结构刷入共享内存
    memcpy((char *)shm_start, (char *)&qHead, sizeof(QueueHead));
    
    //将要写入的新结点
    QueueNode newNode;

    //将要写入的位置
    char *DestAddress = NULL;
    int curHead = 0;
    int curTail = 0;
    int value = 10;

    while(1)
    {

        //从共享内存头结构中获取当前队列尾位置和头位置
        memcpy((char *)&qHead, (char *)shm_start, sizeof(QueueHead) );
        curTail = qHead.tail;
        curHead = qHead.head;

        //判断是否可以向队列中插入新结点
        if(((curTail + 1) % NODE_NUM) == curHead)
        {
            //队列已满，不可插入
            printf("不可插入\n");
            sleep(1);
            continue;
        }

        //构造新结点
        newNode.val = value++;

        //获取新结点写入地址
        DestAddress = shm_start + sizeof(QueueHead) + curTail * sizeof(QueueNode);
         
        //将结点写入队列
        memcpy(DestAddress, (char *)&newNode, sizeof(QueueNode));

        //更新队列头结构
        qHead.tail = (curTail + 1) % NODE_NUM;

        //将头结构刷入共享内存
        memcpy((char *)shm_start, (char *)&qHead, sizeof(QueueHead));

        //1秒写一次
        sleep(1);
    }
    return 0;
}
                                  
