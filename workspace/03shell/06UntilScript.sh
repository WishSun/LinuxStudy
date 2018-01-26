#!/bin/bash

i=0

until [ $i -gt 10 ]   #判断表达式为假时进入循环
do 
		echo "i=$i"
        ((i++))
done
