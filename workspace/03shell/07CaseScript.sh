#!/bin/bash

#switch()
#{
# case 0: 
# case 1:
# default:
#}

case $1 in 
        0)  #case 0
			echo "param0=0";;     #注意有两个分号, 代表本开关的结束(每个开关最后必须要有双分号)
        1)  #case 1
			echo "param0=1";; 
		*)  #default
			echo "error";; 
esac    #case语句结束

#从键盘获取一个任意输入赋值给num, 也相当于定义了一个变量num
read -p "please input a num: " num
echo "num = $num"
