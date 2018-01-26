SRC=$(wildcard ./*.c)
OBJ=$(patsubst %.c, %.o, $(SRC))
LIB=./libdynamic_test.so

$(LIB):$(OBJ)
	gcc --share -o $@ $^

./%.o:./%.c
	gcc -c $< -o $@ -fPIC

clean:
	rm $(OBJ) $(LIB)
