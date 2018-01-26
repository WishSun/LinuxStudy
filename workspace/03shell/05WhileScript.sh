#!/bin/bash

i=0
while [ $i -lt 10 ]    #当i<10时循环
do
		echo "i=$i"
		((i++))        #双括号(())即数学计算表达式i++
done
