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
    union semun   sem_val;

    if((semid = semget(IPC_KEY, 1, IPC_CREAT | 0777)) == -1)
    {
        perror("semget");
        exit(1);
    }
    
    if(init_flag)
    {
        sem_val.val = 1;

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
    sem_init(0);

    while(1)
    {
        sem_p();
        printf("this is child!\n");
        sleep(3);
        sem_v();
    }
    return 0;

}
