SRC=$(wildcard ./*.c)
OBJ=$(patsubst %.c, %.o, $(SRC))
LIB=./libstatic_test.a

$(LIB):$(OBJ)
	ar -cr $@ $^

./%.o:./%.c
	gcc -c $< -o $@

.PHONY:clean
clean:
	rm $(OBJ) $(LIB)
