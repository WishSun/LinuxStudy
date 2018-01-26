all:semget sem_child
semget:semget.c
	gcc $^ -o $@
sem_child:sem_child.c
	gcc $^ -o $@
.PHONY:clean
clean:
	rm semget sem_child
