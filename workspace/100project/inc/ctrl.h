
#ifndef __CTRL_H_
#define __CTRL_H_

typedef void(*HandleType)(char *argList);

//命令结点结构
typedef struct Node
{
    char cmd[256];
    HandleType handle;

    struct Node *next;
}CmdNode;


//处理管道命令入口函数
void run_ctrl();

#endif
