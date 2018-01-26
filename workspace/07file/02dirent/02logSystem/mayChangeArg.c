#include <stdio.h>

//这里的__VA_ARGS__指的就是...(可变参数说明)
#define PRINT(test, ...)\
    printf("%s\n", test);\
    printf(__VA_ARGS__)


int main(void)
{
    PRINT("xiwang", "hello%s | %d \n", "你好", 100);
    return 0;
}

