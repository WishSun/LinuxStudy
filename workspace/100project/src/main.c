#include <stdio.h>
#include <stdlib.h>
#include "../inc/ctrl.h"

int main(void)
{
    //run_scan(".");
    run_ctrl();
    while(1)
    {
        sleep(10);
    }
    return 0;
}
