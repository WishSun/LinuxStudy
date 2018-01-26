#!/bin/bash

#进程监控脚本
#功能需求: 监控程序是否运行，如果程序没有运行，那么启动程序。


while [ true ]
do

	pro=$(ps -ef | grep "$1" | grep -v grep | grep -v "$0")
	#过滤出目标进程信息，如果pro为空串说明目标进程未运行
    #反向过滤掉grep进程 和 本进程信息, 因为这两个进程信息中包含目标进程的名字

	if [ -z "$pro" ]
	then 
   		  echo "程序未启动，重新启动程序"
	      sh $1
	else
   		  echo "程序正在运行..."
	fi
	
	sleep 0.5

done
