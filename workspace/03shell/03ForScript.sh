#!/bin/bash

echo "循环1输出: "
for loop in 1 2 3   #loop 是一个循环变量，后面的数字相当于一个数组(以空格来分隔)，
                    #每次循环取出一个数字放入loop循环变量中，直到最后一个数字被放入之后退出循环
do        #do : for循环的开始，相当于左大括号{

        echo $loop     #输出loop的值，并换行

done      #done : for循环的开始，相当于右大括号}

echo "循环2输出: "
for loop1 in "1 2 3" "abc"  #后面的数组成员是两个字符串
do        
        echo $loop1
done     


echo "循环3输出: "
#  $(命令语句)  执行"命令语句"并返回其结果

for i in $(seq 1 2 10)      #seq 是一个命令  1表示i的初值  2表示i的步长  10表示i的最大值    
do 
		echo "i=${i}abc"    #${i}取出变量i的值
done


echo "循环4输出: "
for ((i=0;i<10;i++))       #把(())中的内容当成数学运算, 并不返回结果
do 
		echo "i=$i"
done


for i in "$@"    #从这里可以看出$@和$*的区别，$@时，将输出每一个i值。但是$*时，将把所有参数作为一个字符串输出
do
    echo "i=$i"
done
