/*功能: 实现函数- 完成对指定目录的多层级监控，将目录下的所有文件找出来，并打印完整路径的文件名*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>

int scan_dir(char *path)
{
    DIR *p_dir = NULL;
    char buff[1024];
    if (path == NULL)
    {
        return -1;
    }

    //调用opendir函数打开目录，使用DIR*来接收返回的目录流指针
    p_dir = opendir(path);
    if (p_dir == NULL)
    {
        printf("opendir :[%s] error: %s\n", path, strerror(errno));
        return -1;
    }

    //定义一个文件结构体指针
    struct dirent *p_dirent = NULL;

    //循环读取目录中的内容
    while ((p_dirent = readdir(p_dir)) != NULL)
    {
        if (!strcmp(p_dirent->d_name, ".") || !strcmp(p_dirent->d_name, ".."))
        {
            continue;
        }

        memset(buff, 0x00, 1024);
        sprintf(buff, "%s/%s", path, p_dirent->d_name);

        printf("file name: [%s]\n", buff);
        if(p_dirent->d_type == DT_DIR)
        {
            scan_dir(buff);
            continue;
        }
    }

    //关闭目录(参数是目录流指针)
    closedir(p_dir);
    return 0;
}

int main(int argc, char* argu[])
{
    scan_dir(argu[1]);
    return 0;
}
