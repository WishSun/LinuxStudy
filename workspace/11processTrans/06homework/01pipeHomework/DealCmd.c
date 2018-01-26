#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>


//---功能函数区开始位置-----------------------------------------------
void PrintHello()
{
    printf("你好呀！小哥哥\n");
}

void PrintBye()
{
    printf("再见！小哥哥\n");
}

//---功能函数区结束位置-----------------------------------------------


//命令结点
typedef struct Node
{
    char cmd[256];
    void(*handle)();

    struct Node *next;
}CmdNode;

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
void HeadInsert(char *cmd, void(*handle)())
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
void FindHandle(char *cmd)
{
    CmdNode *pTemp = pHead;
    pTemp = pTemp->next;

    while(pTemp)
    {
        if(!strncasecmp(cmd, pTemp->cmd, strlen(cmd)))
        {
            printf("执行命令: %s\n", pTemp->cmd);
            //执行命令处理函数
            pTemp->handle();
            return;
        }
        pTemp = pTemp->next;
    }
    printf("命令 %s 无效!\n", cmd);
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
}

//去掉命令末尾的空字符
void deleteSpaceOfCmd(char *cmdBuff)
{
    while(!isspace(*cmdBuff))
    {
        cmdBuff++;
    }
    *cmdBuff = '\0';
}


#define MAX_BUFFLEN 256

//读取管道命令
void ReadPipe()
{
    char cmdBuff[MAX_BUFFLEN] = {0};
    int ret;
    while(1)
    {
        memset(cmdBuff, 0x00, MAX_BUFFLEN);
        if( read(fd, cmdBuff, MAX_BUFFLEN) <= 0 )
        {
            //向自己发送一个终止信号
            kill(getpid(), SIGINT);
        }
        //去掉命令末尾的空字符
        deleteSpaceOfCmd(cmdBuff);

        //在链表中找到该命令，并执行
        FindHandle(cmdBuff);
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


int main(void)
{
    signal(SIGINT, SigQuitExe);
    signal(SIGQUIT, SigQuitExe);

    //创建或打开管道文件
    if(mkfifo("cmd.fifo", 0644) == -1)
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

    //读取管道命令
    ReadPipe();

    return 0;
}


