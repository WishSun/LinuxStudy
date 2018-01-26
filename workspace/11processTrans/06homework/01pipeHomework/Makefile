all: DealCmd InputCmd
DealCmd:DealCmd.c
	gcc $^ -o $@ -g
InputCmd:InputCmd.c
	gcc $^ -o $@
.PHONY:clean
clean:
	rm DealCmd InputCmd
