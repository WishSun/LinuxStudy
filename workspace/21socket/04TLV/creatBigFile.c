/*************************************************************************
	> File Name: creatBigFile.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月25日 星期四 16时57分37秒
 ************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    int fd ;
    if((fd = open("bigfile", O_CREAT|O_WRONLY, 0644)) == -1)
    {
        perror("open");
        exit(1);
    }

    lseek(fd, 1024 * 1024 * 100, SEEK_SET);
    write(fd, "aa", strlen("aa"));
    close(fd);
    return 0;
}
