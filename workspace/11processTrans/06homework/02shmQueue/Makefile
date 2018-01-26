all: writeQueue readQueue
writeQueue:ProWrite.c
	gcc $^ -o $@ 
readQueue:ProRead.c
	gcc $^ -o $@ 
.PHONY:clean
clean:
	rm writeQueue readQueue
