#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>

#define MAXSIZE (1024*1024)

//自定义错误处理函数
void my_error(const char *strerr)
{
    perror(strerr);
    exit(1);
}

//解析fuser执行结果信息
//如果有进程占用则返回占用进程的ID
//否则返回0
int AnalyzeFuser(const char *resultBuffer)
{
    if(resultBuffer == NULL)
        return 0;
    while(resultBuffer)
    {
        if(*resultBuffer == ':')
        {
            resultBuffer++;
            break;
        }
        resultBuffer++;
    }

    return atoi(resultBuffer);
}

//输出树枝
void PrintTree(int level)
{    
    int i;

    for(i = 0; i < level; i++)
    {
        printf("|");
        printf("     ");
    }
    printf("|--- ");
}

//输出树状目录结构
void PrintDirentStruct(char direntName[], int level)
{

    //定义一个目录流指针
    DIR *p_dir = NULL;

    //定义一个目录结构体指针
    struct dirent *p_dirent = NULL;

    //定义一个命令缓冲区
    char cmdBuff[MAXSIZE] = {0};

    //打开目录，返回一个目录流指针指向第一个目录项
    p_dir = opendir(direntName);
    if(p_dir == NULL)
    {
        my_error("opendir error");
        exit(1);
    }

    //命令缓冲区大小
    int buffSize = MAXSIZE;


    //循环读取每个目录项, 当返回NULL时读取完毕
    while((p_dirent = readdir(p_dir)) != NULL)
    {
        //目录流指针(为之后的popen函数使用)
        FILE *fp = NULL;

        //存储fuser命令执行结果
        char resultBuff[1024] = {0};

        //备份之前的目录名
        char *backupDirName = NULL;

        //排除掉当前目录和上级目录
        if(!strcmp(p_dirent->d_name, ".") || !strcmp(p_dirent->d_name, ".."))
        {
            continue;
        }

        //构造命令头
        sprintf(cmdBuff, "fuser -a ");

        //如果不是目录的话
        if(p_dirent->d_type != DT_DIR)
        {
            //如果命令缓冲区已满，则去执行。执行完毕后清空缓冲区,继续装载命令
            if(buffSize < strlen(cmd))
            {
                //去执行fuser
                //解析结果和输出
                ExecFuser();

                //清空命令缓冲区
                memset(cmdBuff, 0x00, MAXSIZE);
                buffSize = MAXSIZE;
            }

            //构造fuser命令参数
            sprintf("%s  ", p_dirent->d_name);
            strcat(cmdBuff, p_dirent->d_name, strlen(p_dirent->d_name));
            buffSize -= strlen(p_dirent->d_name);
            continue;
        }

        //如果命令缓冲区已满，则去执行。执行完毕后清空缓冲区,继续装载命令
        if(buffSize < strlen(cmd))
        {
            ExecFuser();

            //清空命令缓冲区
            memset(cmdBuff, 0x00, MAXSIZE);
            buffSize = MAXSIZE;
        }


        //将目录文件也添加进去
        sprintf("%s  ", p_dirent->d_name);
        strcat(cmdBuff, p_dirent->d_name, strlen(p_dirent->d_name));
        buffSize -= strlen(p_dirent->d_name);

        //立即执行fuser并分析结果
        ExecFuser();

        //清空命令缓冲区
        memset(cmdBuff, 0x00, MAXSIZE);
        buffSize = MAXSIZE;
        

        strcat(smdBuff, cmd, strlen(cmd));
        buffSize -= strlen(cmd);

        //执行fuser命令
        fp = popen(cmd, "r");
        if(fp == NULL)
        {
            perror("popen");
            exit(-1);
        }

        //获取fuser执行结果信息
        fread(resultBuff, 1024, 1, fp);


            

        //构造fuser命令
        sprintf(cmd, "fuser -a %s 2>&1  ", p_dirent->d_name);
        strcat(smdBuff, cmd, strlen(cmd));
        
        fp = popen(cmd, "r");
        if(fp == NULL)
        {
            perror("popen");
            exit(-1);
        }

        //获取fuser执行结果信息
        fread(resultBuff, 1024, 1, fp);
        pclose(fp);
       
        int occupyPid = 0; //占用文件的进程ID
        if((occupyPid = AnalyzeFuser(resultBuff)) != 0)
        {
            printf("[%s] : 被%d进程占用\n", p_dirent->d_name, occupyPid);
        }
        else    
        {
            printf("%s\n", p_dirent->d_name);
        }

//------------------------------------------------------------------------------
        //如果目录项仍是一个目录的话，进入目录
        if(p_dirent->d_type == DT_DIR)
        {
            char *pathBuff = NULL;

            //为子目录路径申请内存
            pathBuff = malloc(strlen(direntName) + strlen(p_dirent->d_name) + 2);

            //构建子目录路径
            sprintf(pathBuff, "%s/%s", direntName, p_dirent->d_name);

            //递归进入子目录
            PrintDirentStruct(pathBuff, level + 1);

            //释放内存
            free(pathBuff);
            pathBuff = NULL;
        }

    }

    closedir(p_dir);

}

int main(int argc , char *argu[])
{
    //目录名
    char direntName[1024];
    memset(direntName, 0, sizeof(direntName));

    if(argc == 1)
    {
        sprintf(direntName, "%s", ".");
    }
    else if(argc == 2)
    {
        sprintf(direntName, "%s", argu[1]);
    }
    else
    {
        my_error("argument error");
    }

    //输出目录结构
    printf("%s\n", direntName);

    PrintDirentStruct(direntName, 0);
    return 0;

}
