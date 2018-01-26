#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define TRUE  1
#define FALSE 0


//Increment函数对num进行加1操作，并判断是否超过最大位数N_Digits所对应的数
//超过则返回TRUE，没超过返回FALSE
int Increment(char *number)
{
    //超过最大位数值标记
    int isOverflow = FALSE;

    //进位值
    int nTakeOver = 0;

    int nLen = strlen(number), i = 0;

    for(i = nLen - 1; i >= 0; i--)
    {
        //计算每一位上相加后的值
        int nSum = number[i] - '0' + nTakeOver;

        //最低位直接加1
        if(i == nLen - 1)
        {
            nSum++;
        }

        if(nSum >= 10)
        {//处理进位------------------

            if(i == 0)
            {
                //如果最高位要向上进位，说明数已经加到最大了
                //设置超过最大位数值标记为TRUE
                isOverflow = TRUE;
            }
            else
            {
                //进位
                nSum -= 10;
                nTakeOver = 1;
                number[i] = '0' + nSum;
            }

        }
        else
        {
            number[i] = '0' + nSum;
            break;
        }
    }

    return isOverflow;
}

//打印numStr所对应的数字
void PrintNum(char *numStr)
{
    while(*numStr == '0')
    {
        numStr++;
    }
    printf("%s\t", numStr);
}

int main(void)
{
    int N_Digits;
    char *numStr = NULL;

    printf("请输入最大数的位数: \n");
    scanf("%d", &N_Digits);
    
    //为存储数字的字符串指针申请内存
    numStr = (char *)malloc(N_Digits + 1);

    //初始化该内存
    memset(numStr, '0', N_Digits);
    numStr[N_Digits] = '\0';

    //Increment函数对num进行加1操作，并判断是否超过最大位数N_Digits所对应的数
    //超过则返回TRUE，没超过返回FALSE
    if(!Increment(numStr))
    {
        //将num打印出来
        PrintNum(numStr);
    }

    free(numStr);
    numStr = NULL;
    
    return 0;
}
