#include <stdio.h>
#include "child.h"
#include <dynamic.h>
#include <static.h>

int main()
{
    print_dynamic();
    print_static();
    print_child();
    printf("this is main!\n");
    return 0;
}
