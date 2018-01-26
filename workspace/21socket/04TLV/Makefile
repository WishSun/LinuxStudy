all:server client
server:server.c
	gcc $^ -o $@ -g -pthread
client:client.c
	gcc $^ -o $@ 
.PHONY:clean
clean:
	rm server client
