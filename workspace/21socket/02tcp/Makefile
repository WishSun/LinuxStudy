all: server client
server: server.c 
	gcc $^ -o $@ -pthread
client: client.c 
	gcc $^ -o $@
.PHONY:clean
clean:
	rm server client
