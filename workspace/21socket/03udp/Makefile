all:server client
server:udpServer.c
	gcc $^ -o $@
client:udpClient.c
	gcc $^ -o $@
.PHONY:clean
clean:
	rm server client
