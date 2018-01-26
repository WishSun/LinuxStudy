#!/bin/bash

a=1               #定义变量a，a默认是字符串(变量与等号之间不能有空格)


#if 和 elif 之后都必须有then，但else之后没有then
if [ $a -gt 1 ]       #if语句，表达式用方括号[]括起来(表达式两边和中括号之间必须有空格), $a代表使用变量a
then

       echo "a > 1";

elif [ $a -eq 1 ]     #elif相当于else if 
then 
 
       echo "a == 1";

else

	   echo "a < 1";

fi     #if语句的结尾



 #-gt(大于)  -lt(小于)  -eq(等于)  -le(小于等于)  -ge(大于等于)  判断数字大小
 # >  <  >=  <= ==  = 判断字符串大小
