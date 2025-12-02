all: start client server

start: start.c
	gcc  start.c  -o start

client: client.c prodcons_client.c prodcons_client.h prodcons_msg.h
	gcc  client.c prodcons_client.c  -o client

server: server.c prodcons_server.c prodcons_server.h prodcons_msg.h
	gcc  server.c prodcons_server.c  -o server -pthread

clean:
	rm -f start
	rm -f client
	rm -f server