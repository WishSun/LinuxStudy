SRC=$(wildcard ./*.c)
OBJ=$(patsubst %.c, %.o, $(SRC))
BIN=./libdynamic_test.so
all:$(BIN)
$(BIN):$(OBJ)
	gcc $^ -o $@ --share
%.o:%.c
	gcc -o $@ -c $< -fPIC

.PHONY:clean
clean:
	rm -r $(BIN) $(OBJ)
