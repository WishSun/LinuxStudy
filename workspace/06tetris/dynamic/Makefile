#SRC存储所有.c文件名
SRC=$(wildcard ./*.c)

#OBJ存储所有由.c文件编译后的.o文件名
OBJ=$(patsubst %.c,  %.o, $(SRC))

#----------------------------配置Makefile区域------------------------------

#%BIN是生成的对象,在make中未赋值的变量视为未定义
#若要编译静态库或动态库，直接将库的名字赋值给对应的变量。但另外两个必须为空
ExeBIN=
StaticBIN=
DynamicBIN=./libkeyboard.so

#最终生成目标
#生成那种类型对象，就把对应的变量值赋给BIN
BIN=${DynamicBIN}


#生成可执行文件对象命令
#若需要连接其他库时更改libFile变量: 连接的库的名字
#                  更改libPath变量: 连接的库的路径
#若不需要连接其他库，则将上面两个变量置为空
ExeCmd=gcc -o $@ $^
#生成可执行文件对象所需的库
LibFile=-lstatic_test -ldynamic_test
#库的路径(路径前加上-L选项)
LibPath=-L../05lib

#-------------------------以下内容无需改变----------------------------


#生成动态链接库对象命令
DynCmd=gcc --share -o $@ $^
#生成动态链接obj文件命令
ObjCmd=-fPIC


#生成静态库对象命令
StaCmd=ar cr  $@ $^


#生成最终目标对象
$(BIN):$(OBJ)
ifdef ExeBIN
	${ExeCmd} ${LibPath} ${LibFile}
endif
ifdef StaticBIN
	${StaCmd}
endif
ifdef DynamicBIN
	${DynCmd}
endif


./%.o:./%.c
#如果是编译动态链接库
ifdef DynamicBIN
	gcc -c $< -o $@ ${ObjCmd}
endif
#如果不是编译动态链接库,即编译静态库或可执行程序
ifndef DynamicBIN
	gcc -c $< -o $@ 
endif


#清除所有无用文件
clean:    
	rm -rf  $(OBJ) $(BIN)

