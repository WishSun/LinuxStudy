#!/bin/bash

path=$(pwd)         #$()中放命令, 返回命令执行的结果

echo $path          #输出当前工作路径



num1=11
num2=22


sum=$num1+$num2    #将num1和num2两个字符串连接起来赋值给sum
echo "sum=$sum"    #输出  11+22



#数学运算:  

#方式1
sum=`expr $num1 + $num2`  #将num1和num2对应的数字相加得出的值赋值给sum, 加号两边必须要有空格
echo "sum=$sum"    #输出  33


#方式2, let命令: 可以将后面的操作变为数值运算(不加空格)
let sum=$num1+$num2  #将num1和num2对应的数字相加得出的值赋值给sum, 加号两边不能有空格
echo "sum=$sum"    #输出  33


#方式3, expr命令: 可以将后面的操作变为数值运算(要加空格)
sum=$(expr $num1 + $num2)
echo "sum=$sum"    #输出  33


#方式4, 双括号括起来的是一个表达式
sum=$((num1+num2))   #如果计算要返回其值，则要加上$。如果不返回值的话，不可加$
echo "sum=$sum"
