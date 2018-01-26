all:shmget shm_child 
shmget:shmget.c
	gcc $^ -o $@
shm_child:shm_child.c
	gcc $^ -o $@
.PHONY:clean
clean:
	rm shmget shm_child
