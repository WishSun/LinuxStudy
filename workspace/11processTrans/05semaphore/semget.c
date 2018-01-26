#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>


//信号量的唯一键值
#define IPC_KEY 0x1234567

//信号量标识符
int semid = -1;

union semun
{
    int             val;
    unsigned short  *array;  
};

//初始化信号量
// @init_flag  : 1 : 需要初始化信号量
//               0 : 不初始化信号量
int sem_init(int init_flag)
{
    //用于获取或设置信号量状态信息
    //其中成员val用于设置单个信号量的值
    //成员array用于获取或者设置多个信号量的值
    union semun   sem_val;

    //创建信号量，若信号量存在，则打开信号量
    if((semid = semget(IPC_KEY, 1, IPC_CREAT | 0777)) == -1)
    {
        perror("semget");
        exit(1);
    }
    
    //如果init_flag为1, 才要进行初始化。因为信号只需初始化一次
    if(init_flag)
    {
        sem_val.val = 1;

        //当操作单个信号量值的时候，表示操作的是第n个信号量，从0开始计数,则第一个信号量semnum就是0
        //用sem_val.val设置第0个信号量，即单个信号量的时候
        semctl(semid, 0, SETVAL, sem_val);
    }
    return 0;
}

//销毁信号量
int sem_destroy()
{
    semctl(semid, 0, IPC_RMID, NULL);
}

//P操作
int sem_p()
{
    struct  sembuf op;
    op.sem_num = 0;
    op.sem_op  = -1;
    op.sem_flg = SEM_UNDO;

    if(semop(semid, &op, 1) == -1)
    {
        perror("semop");
        exit(1);
    }
}

//V操作
int sem_v()
{
    struct  sembuf op;
    op.sem_num = 0;
    op.sem_op  = 1;
    op.sem_flg = SEM_UNDO;

    if(semop(semid, &op, 1) == -1)
    {
        perror("semop");
        exit(1);
    }
}

int main(void)
{
    sem_init(1);

    while(1)
    {
        sem_p();
        printf("this is parent!\n");
        sleep(1);
        sem_v();
    }
    return 0;

}
