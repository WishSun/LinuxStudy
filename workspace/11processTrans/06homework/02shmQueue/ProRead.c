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
    if((shmid = (shmget(SHM_KEY, SHM_SIZE, IPC_CREAT))) == -1)
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

    QueueHead qHead;
    qHead.head = 0;
    qHead.tail = 0;
    qHead.size = sizeof(QueueHead);
   
    //读到的结点
    QueueNode readNode;

    //将要读取的位置
    char *readAddress = NULL;

    //当前循环队列的头和尾位置
    int curHead = 0;
    int curTail = 0;

    while(1)
    {

        //从共享内存头结构中获取当前队列尾位置和头位置
        memcpy((char *)&qHead, (char *)shm_start, sizeof(QueueHead) );
        curTail = qHead.tail;
        curHead = qHead.head;

        //判断队列是否为空，即判断尾和头是否相等
        if(curTail == curHead)
        {
            //队列为空，不可读取结点
            printf("队列为空!\n");
            sleep(3);
            continue;
        }

        //获取队列第一个结点地址(在此处读取)
        readAddress = shm_start + sizeof(QueueHead) + curHead * sizeof(QueueNode);

        //读取结点
        memcpy((char *)&readNode, (char *)readAddress, sizeof(QueueNode));
         
        //打印读出的内容
        printf("%d\n", readNode.val);

        //更新队列头结构
        qHead.head = (curHead + 1) % NODE_NUM;

        //将队列头结构信息刷入共享内存
        memcpy((char *)shm_start, (char *)&qHead, sizeof(QueueHead));

        //1秒读一次
        sleep(3);
    }
    return 0;
}
                                  
