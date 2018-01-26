LIB = -ldynamic_ctrl -ldynamic_scan

all:./main ./InputCmd
./main:main.c
	gcc $^ -o $@ -pthread  -L../lib $(LIB)
./InputCmd:InputCmd.c
	gcc $^ -o $@
.PHONY:clean
clean:
	rm ./main ./InputCmd

