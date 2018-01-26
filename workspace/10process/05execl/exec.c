#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int main(void)
{
    int pid;
    pid = fork();

    if ( pid < 0 )
    {
        exit(-1);
    }
    else if(pid == 0)
    {
        execl("/bin/ls", "ls", "-l", NULL);
    }

    while(1)
    {
        printf("this is parent : [%d]\n", getpid());
        sleep(1);
    }
    return 0;
}
