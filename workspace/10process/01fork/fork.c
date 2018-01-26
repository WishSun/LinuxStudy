#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

int main(void)
{
    int pid;

    printf("parent pid:[%d], gid:[%d], sid[%d]\n", getpid(), getpgid(getpid()), getsid(getpid()));

    if ((pid = fork()) == -1)
    {
        perror("fork");
        exit(-1);
    }

    if(pid == 0)
    {
        printf("this is child1! pid:[%d], ppid:[%d], pgid:[%d], sid[%d]\n",
                getpid(), getppid(), getpgid(getpid()), getsid(getpid()));
    }
    else
    {   
        if((pid = fork()) == -1)
        {
            perror("fork");
            exit(-1);
        }
        if(pid == 0)
        {
            printf("this is child2! pid:[%d], ppid:[%d], pgid:[%d], sid[%d]\n",
                getpid(), getppid(), getpgid(getpid()), getsid(getpid()));
        }

        else
        {
            sleep(1);
        }
    }
   // printf("淘宝嘉年华!\n");
    return 0;
}
