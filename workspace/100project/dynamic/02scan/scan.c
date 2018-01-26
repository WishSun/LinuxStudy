#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <pthread.h>
#include "../../inc/scan.h"

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

//输出目录结构
void PrintDirentStruct(char *direntName, int level)
{
    //定义一个目录流指针
    DIR *p_dir = NULL;

    //定义一个目录结构体指针
    struct dirent *p_dirent = NULL;

    //打开目录，返回一个目录流指针指向第一个目录项
    p_dir = opendir(direntName);
    if(p_dir == NULL)
    {
        my_error("opendir error");
    }

    //循环读取每个目录项, 当返回NULL时读取完毕
    while((p_dirent = readdir(p_dir)) != NULL)
    {
        //目录流指针(为之后的popen函数使用)
        FILE *fp = NULL;

        //存储命令
        char cmd[256] = {0};

        //存储fuser命令执行结果
        char resultBuff[1024] = {0};

        //备份之前的目录名
        char *backupDirName = NULL;


        //排除掉当前目录和上级目录
        if(!strcmp(p_dirent->d_name, ".") || !strcmp(p_dirent->d_name, ".."))
        {
            continue;
        }

        int i;
        for(i = 0; i < level; i++)
        {
            printf("|");
            printf("     ");
        }
        printf("|--- ");

        sprintf(cmd, "fuser -a %s 2>&1", p_dirent->d_name);
        
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


//浏览目录线程函数
void *thr_scan(void *args)
{
    //线程分离
    pthread_detach(pthread_self());

    printf("%s\n", (char *)args);
    PrintDirentStruct((char *)args, 0);

    pthread_exit(NULL);
}


//浏览目录入口函数
void run_scan(char *path)
{
    pthread_t tid;

    if(pthread_create(&tid, NULL, thr_scan, (void *)path) < 0)
    {
        perror("pthread_create");
        exit(1);
    }
    return ;
}

