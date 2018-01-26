test:
	make -C ./dynamic
	#make -C:进入指定的目录下执行make命令
	make -C ./static
	#将静态、动态库文件拷贝到lib目录下
	cp ./static/libstatic_test.a  ./lib
	cp ./dynamic/libdynamic_test.so  ./lib
	
	make -C ./src

install:
	#为lib目录下的动态链接库文件在/usr/lib目录下创建软连接，这里使用的必须是绝对路径
	sudo ln -s /home/sunxiwang/03AgainStudyLinux/workspace/04makefile/03makefile/lib/libdynamic_test.so  /usr/lib/libdynamic_test.so 
	#将最后生成的可执行文件main拷贝到./bin目录下
	mv ./src/main ./bin

clean:
	make clean -C ./dynamic
	#make clean -C: 进入指定目录下执行make clean命令
	make clean -C ./static
	make clean -C ./src
