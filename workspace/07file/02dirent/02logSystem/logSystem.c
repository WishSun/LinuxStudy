#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>

int log_fd;
long log_maxsize = 2<<10;

//打开日志文件
int open_log(char *path)
{
    log_fd = open(path, O_WRONLY | O_CREAT | O_APPEND, 0777);
    if(log_fd < 0)
    {
        printf("open log file :[%s] error", path);
        
    }
}

//写日志
int write_log(char *msg)
{
    struct stat st;
    time_t t;
    struct tm lt;

    fstat(log_fd, &st);
    if(st.st_size > log_maxsize)
    {
        ftruncate(log_fd, 0);
    }

    t = time(NULL);  //获取系统当前时间到1970.0.0经过的秒数
    localtime_r(&t, &lt);  //转化为时间结构体struct tm
    char buff[1024];

    sprintf(buff, "%d-%d-%d %d:%d:%d", 
            lt.tm_year+1900, lt.tm_mon+1, lt.tm_mday,
            lt.tm_hour, lt.tm_min, lt.tm_sec);

    write(log_fd, buff, strlen(buff));
    write(log_fd, msg, strlen(msg));

    return 0;
}

//关闭日志文件
int close_log()
{
    close(log_fd);
    return 0;
}

int main(void)
{
    if (open_log("./test.log") < 0)
    {
        return -1;
    }

    while(1)
    {
        write_log("xiwang : hello world\n");
        sleep(1);
    }

    close_log();
    return 0;
}
