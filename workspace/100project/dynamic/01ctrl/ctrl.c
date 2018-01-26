#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>
#include <malloc.h>
#include "../../inc/ctrl.h"
#include "../../inc/scan.h"

//获取参数个数和将各个参数值到挪到argu中
char* GetArgcAndArgu(int *argc, char *argu[], const char *argList)
{
    *argc = 0;
    int count = 0;
    int len = strlen(argList);
    char *argTemp = malloc(len + 1);

    //分割符
    char *delims = " ";

    char *result = NULL;
    memset(argTemp, 0x00, len + 1);
    memcpy(argTemp, argList, len);
    printf("argTemp : %s\n", argTemp);

    result = strtok(argTemp, delims);
    while(result != NULL)
    {
        argu[count] = result;
        count++;
        (*argc)++;
        result = strtok(NULL, delims);
    }

    return argTemp;
}


//---功能函数区开始位置-----------------------------------------------
void PrintHello(char *argList)
{
    printf("你好呀！小哥哥\n");
}

void PrintBye(char *argList)
{
    printf("再见！小哥哥\n");
}

//查看指定目录功能
void ScanDirent(char *argList)
{
    char ch;
    int argc;
    char *argu[10] = {NULL};
    char *freePoint = NULL;

    //获取参数个数和将各个参数值到挪到argu数组中
    freePoint = GetArgcAndArgu(&argc, argu, argList);
    if(argc > 10)
    {
        printf("参数有误！\n");
        return;
    }

    printf("ind:[%d]\n", optind);
    optind = 1;
    while((ch = getopt(argc, argu, "d:")) != -1)
    {
        printf("------------------\n");
        switch(ch)
        {
            case 'd': 
            if(optarg != NULL)
            {   
                 run_scan(optarg);
            }
            break;
            default:
            printf("命令有误！\n");
        }
    }

    free(freePoint);
}

//---功能函数区结束位置-----------------------------------------------


//管道文件描述符
static int fd;

//定义链表头结点指针
static CmdNode *pHead = NULL;

//初始化命令链表
void InitLink()
{
    pHead = (CmdNode*)malloc(sizeof(CmdNode));
    pHead->next = NULL;
}

//头插一个命令结点
void HeadInsert(char *cmd, HandleType handle)
{
    //检查参数
    if(cmd == NULL || handle == NULL)
    {
        printf("argu error!\n");
        return;
    }

    //构造结点
    CmdNode *pNewNode = (CmdNode *)malloc(sizeof(CmdNode));
    sprintf(pNewNode->cmd, "%s", cmd);
    pNewNode->handle = handle;

    pNewNode->next = pHead->next;
    pHead->next = pNewNode;
}

//在链表中找到命令并执行
void FindHandle(char *cmdName, char *argList)
{
    CmdNode *pTemp = pHead;
    pTemp = pTemp->next;

    while(pTemp)
    {
        if(!strncasecmp(cmdName, pTemp->cmd, strlen(cmdName)))
        {
            printf("执行命令: %s\n", pTemp->cmd);
            //执行命令处理函数
            pTemp->handle(argList);
            return;
        }
        pTemp = pTemp->next;
    }
    printf("命令 %s 无效!\n", cmdName);
}

//销毁命令链表
void DestroyLink()
{
    if(pHead == NULL)
    {
        return;
    }

    CmdNode *pTemp = pHead->next, *pDel = NULL;
    while(pTemp)
    {
        pDel = pTemp;
        pTemp = pTemp->next;
        free(pDel);
        pDel = NULL;
    }

    free(pHead);
    pHead = NULL;
}


//添加命令和命令处理函数
void AddCmdAndHandle()
{
    HeadInsert("hello", PrintHello);
    HeadInsert("bye", PrintBye);
    HeadInsert("scan", ScanDirent);
}

//获取命令参数列表中的命令名
void GetCmdName(char *cmdName, char *cmdArguList)
{
    char *arguList = cmdArguList;

    int count = 0;
    while(!isspace(*arguList))
    {
        count++;
        arguList++;
    }
    memcpy(cmdName, cmdArguList, count);
}


#define MAX_BUFFLEN 256

//读取管道命令
void ReadPipe()
{
    //命令名
    char cmdName[MAX_BUFFLEN] = {0};

    //命令参数列表
    char cmdArguBuff[MAX_BUFFLEN] = {0};

    int ret;
    while(1)
    {
        memset(cmdName, 0x00, MAX_BUFFLEN);
        memset(cmdArguBuff, 0x00, MAX_BUFFLEN);

        if( read(fd, cmdArguBuff, MAX_BUFFLEN) <= 0 )
        {
            //向自己发送一个终止信号
            kill(getpid(), SIGINT);
        }

        //去掉最后的回车
        cmdArguBuff[strlen(cmdArguBuff) - 1] = '\0';

        //获取命令名
        GetCmdName(cmdName, cmdArguBuff);

        //在链表中找到该命令，并执行
        FindHandle(cmdName, cmdArguBuff);
    }

}

//处理退出程序信号
void SigQuitExe(int sig_num)
{
    if(sig_num == SIGINT || sig_num == SIGQUIT)
    {
        //关闭描述符，清空链表
        close(fd);
        DestroyLink();
    }

    exit(0);
}

//初始化控制
void ctrl_init()
{
    signal(SIGINT, SigQuitExe);
    signal(SIGQUIT, SigQuitExe);

    //创建或打开管道文件
    if(mkfifo("cmd.fifo", 0777) == -1)
    {
        if(errno != EEXIST)
        {
            perror("mkfifo");
            exit(1);
        }
    }

    //以只读方式打开管道文件
    fd = open("cmd.fifo", O_RDONLY);
    if(fd == -1)
    {
        perror("open");
        exit(1);
    }

    //初始化链表
    InitLink();

    //将所有命令添加到命令链表中
    AddCmdAndHandle();

}

//处理管道命令线程函数
void *thr_ctrl(void *args)
{
    //分离线程
    pthread_detach(pthread_self());

    //读取管道命令并处理
    ReadPipe();
}

//处理管道命令入口函数
void run_ctrl()
{
    pthread_t tid;

    //初始化控制
    ctrl_init();

    //创建处理线程
    if(pthread_create(&tid, NULL, thr_ctrl, NULL) < 0)
    {
        perror("pthread_create");
        exit(1);
    }
}


