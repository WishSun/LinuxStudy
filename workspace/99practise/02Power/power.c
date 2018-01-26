#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

//定义一个全局变量来判断函数执行正确还是异常，初值为正确
int Bool_InputError = FALSE;

//判断两个double值相等，精度为小数点后6位
int Equal(double num1, double num2)
{
    if(num1 - num2 > -0.0000001 && num1 - num2 < 0.0000001)
    {
        return TRUE;
    }

    return FALSE;
}

//求base的exponent(无符号数)次方(普通方式)
double GetPowerOfUnsignedExponent1(double base, unsigned int exponent)
{
    int i = 0;
    double result = 1.0;
    for(i = 0; i < exponent; i++)
    {
        result *= base;
    }

    return result;
}

//求base的exponent(无符号数)次方(递归方式)
double GetPowerOfUnsignedExponent2(double base, unsigned int exponent)
{
    if(exponent == 0)
    {
        return 1;
    }
    if(exponent == 1)
    {
        return base;
    }

    //位运算效率更高
    double result = GetPowerOfUnsignedExponent2(base, exponent>>1);
    result *= result;

    //如果指数为奇数，则要多乘一次基数, 因为奇数位运算一直除2，会忽略掉多出来的1
    if(exponent & 0x1 == 1)
    {
        result *= base;
    }
    return result;
}

//求base的exponent次方
double power(double base, int exponent)
{
    
    //如果基数为0，并且指数小于0的话，求次方将会产生除0操作，导致程序崩溃
    if(Equal(base, 0.0) && exponent < 0)
    {
        //此处程序执行时异常的，返回的0.0并非有效值
        Bool_InputError = TRUE;
        return 0.0;
    }

    unsigned int absExponent = exponent; 
    if(exponent < 0)
    {
        //将指数转为绝对值
        absExponent = (unsigned int)(-exponent);
    }

    //结果
    double result;

    result = GetPowerOfUnsignedExponent2(base, absExponent);

    if(exponent < 0)
    {
        result = 1.0/result;
    }

    return result;
}

int main(void)
{
    double result;
    result = power(0, -3);

    if(Bool_InputError != TRUE)
    {
        printf("0的-3次方为: %.6f\n", result);
    }
    else
    {
        printf("参数异常！\n");
    }
    return 0;
}
