target:
	#生成动态库
	make -C ./dynamic/01ctrl/
	make -C ./dynamic/02scan/
	#将动态库文件拷贝到lib目录
	cp ./dynamic/01ctrl/libdynamic_ctrl.so ./lib/
	cp ./dynamic/02scan/libdynamic_scan.so ./lib/
	#为lib目录下的动态库文件创建软链接
	sudo ln -s /home/sunxiwang/03AgainStudyLinux/workspace/100project/lib/libdynamic_ctrl.so  /usr/lib/libdynamic_ctrl.so
	sudo ln -s /home/sunxiwang/03AgainStudyLinux/workspace/100project/lib/libdynamic_scan.so  /usr/lib/libdynamic_scan.so
	#生成可执行程序
	make -C ./src

install:
	#将可执行文件拷贝到/bin目录下
	cp ./src/main ./src/InputCmd ./bin
uninstall:
	#卸载
	rm -rf ./bin/main ./bin/InputCmd

.PHONY:clean
clean:
	make clean -C ./src
	make clean -C ./dynamic/01ctrl/
	make clean -C ./dynamic/02scan/
	rm -rf ./lib/*
	sudo rm -rf usr/lib/libdynamic_scan.so  usr/lib/libdynamic_ctrl.so
	sudo rm  /usr/lib/libdynamic_scan.so  /usr/lib/libdynamic_ctrl.so
