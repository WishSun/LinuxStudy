#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
    FILE *fp = NULL;

    //创建一个子进程，执行ls命令，将执行结果写入一个临时文件中，返回文件流指针
    fp = popen("ls", "r");
    if (fp == NULL)
    {
        return -1;
    }
    char buff[1024] = {0};

    while( fgets(buff, 1024, fp) != NULL )
    {
        printf("buff:[%s]\n", buff);
        memset(buff, 0x00, sizeof(buff));
    }

    //只能用pclose关闭fp
    pclose(fp);
    return 0;
}
