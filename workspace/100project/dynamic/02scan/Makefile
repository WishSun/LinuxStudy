SRC=$(wildcard ./*.c)
OBJ=$(patsubst %.c, %.o, $(SRC))
LIB=./libdynamic_scan.so

$(LIB):$(OBJ)
	gcc $^ -o $@ --share -pthread

./%.o:./%.c
	gcc -c $< -o $@ -fPIC

clean:
	rm $(OBJ) $(LIB)
