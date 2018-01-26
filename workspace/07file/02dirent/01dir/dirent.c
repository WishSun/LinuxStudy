#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>

int main(void)
{   
        DIR *p_dir = NULL;
        struct dirent *p_dirent = NULL;

        mkdir(".", 0777);

        p_dir = opendir("./tmp");
        if(p_dir == NULL)
        {
            perror("opendir error");
            return -1;
        }
        
        while((p_dirent = readdir(p_dir)) != NULL)
        {
            if(p_dirent != NULL)
            {
                printf("file type: [%d]-[%s]\n", p_dirent->d_type, p_dirent->d_name);
            }
        }

        closedir(p_dir);
        return 0;
}
