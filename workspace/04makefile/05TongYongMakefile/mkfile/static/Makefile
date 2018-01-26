SRC=$(wildcard ./*.c)
OBJ=$(patsubst %.c, %.o, $(SRC))
LIB=./libstatic_test.a
all:$(LIB)
$(LIB):$(OBJ)
	ar cr $@ $^
%.o:%.c
	gcc -o $@ -c $<

.PHONY:clean
clean:
	rm -r $(LIB) $(OBJ)
