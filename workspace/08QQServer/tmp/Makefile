SRC=$(wildcard ./*.c)
OBJ=$(patsubst %.c, %.o, $(SRC))
BIN=./main

$(BIN):$(OBJ)
	gcc -o $@ $^ -lmysqlclient 
#-o后面跟可执行程序名字或编译后的名字

./%.o:./%.c
	gcc -c $< -o $@
#-o指定编译后的名字
#-c只编译不连接，后面跟.c文件

#显式声明伪对象clean
.PHONY:clean
clean:
	rm $(OBJ) $(BIN)

