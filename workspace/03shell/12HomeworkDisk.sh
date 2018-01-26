#!/bin/bash


#功能需求: 监控系统磁盘使用比例，当使用率达到指定比例，则删除指定目录下的文件
#df -h  : 查看磁盘使用比例情况


#下面是两个字符串分割命令

#awk命令
#awk -F ' ' '{print $5}'  : 将字符串用空格分割，并取出第5个字符串打印
#-F  指出分隔符(这里是空格), 不加-F的话，使用默认分隔符:空格和tab键
#print 打印    $5取第5列,  意思就是打印出第5列

#cut命令
#cut -d '%d' -f 1   :
#-d  指出分隔符(这里是%)   -f  指出取第几列


#df -h | grep sda | awk -F ' ' '{print $5}' | cut -d '%' -f 1   : 取出系统磁盘使用比例值, sda指的是硬盘分区


#先在另一个终端运行 11ExampleProductFile.sh ./tmp 10   #启动不断在./tmp目录下产生大小为20M文件
#然后运行 12HomeworkDisk.sh 30 ./tmp  30     #启动不断检测磁盘使用比例，如果大于等于30%，则删除./tmp下的所有文件

if [ $# -lt 2 ]
then
	echo "参数错误！第一个参数是指定目录，第二各参数是磁盘比例上限"
	exit
fi

max=$2   #大小参数
path=$1   #目录参数

while [ true ]
do 
	#得到磁盘使用的百分值
	value=$(df -h | grep sda | awk -F ' '  '{print $5}' | cut -d '%' -f 1 )

	if [ $value -lt ${size} ]
	then
		echo "检测磁盘中...  磁盘使用比例为${value}%, 小于${max}%"
		
	else
		echo "磁盘占用过高！删除指定目录 ${path} 下的所有文件！"
		rm -r ${path}/*
	fi


	sleep 0.3    #每0.3秒检测一次

done
