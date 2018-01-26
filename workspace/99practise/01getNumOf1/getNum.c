#include <stdio.h>

int getNum1(int n)
{
    int count = 0;

    while(n)
    {
        count++;
        n = n & (n-1);
    }
    return count;
}

int getNum2(int n)
{
    int count = 0;
    unsigned int flag = 1;

    while(flag)
    {
        if(flag & n)
        {
            count++;
        }
        flag = flag << 1;
    }
    return count;
}


int main(void)
{
    int num;

    scanf("%d", &num);
    printf("%d 的二进制表示中有 %d 个 1\n", num, getNum1(num)); 
    printf("%d 的二进制表示中有 %d 个 1\n", num, getNum2(num)); 
    return 0;
}
