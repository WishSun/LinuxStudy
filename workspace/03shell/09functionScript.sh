#!/bin/bash

function sum_func()
{
	((sum=$1 + $2))    #这里的$1是函数的第一个参数, 出现在函数外面的$1是程序的参数
   #echo $sum

	return $sum
}

sum_func 11 22           #函数调用 : 函数名 + 参数列表

res=$?    #获得函数的返回值，$?即获取上一条语句的返回值
echo $res
