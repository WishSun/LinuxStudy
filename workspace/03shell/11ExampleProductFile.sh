#!/bin/bash

# 测试辅助脚本：
# 功能需求i:  向指定目录下不断生成指定大小的文件
#df -h : 查看硬盘使用率

#dd if=/dev/zero  of=./hello.txt bs=1M count=2   :  生成文件(实质上是拷贝命令)
#从/dev/zero这个文件中拷贝内容到./hello.txt中，拷贝过程是：一块一块拷贝，每块大小为1M，拷贝两次。即命令完成后.hello.txt的大小为2M 
#if(input file) : 输入文件
#of(output file) : 输出文件
#bs(block size) : 块大小
#count : 块数


path=$1      #获取指定目录名
size="$2M"   #获取指定大小，单位为M


mkdir -p $path   #创建指定目录
i=0

while [ true ]
do
		#产生2*sizeM大小的文件
		dd if=/dev/zero  of=${path}/hello${i}.txt bs=$size count=2    
		((i++))
		sleep 0.2
done
