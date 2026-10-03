all: start client server

start: start.c prodcons_msg.h
	gcc -o start start.c -lrt

client: client.c prodcons_client.c prodcons_client.h prodcons_msg.h
	gcc -o client client.c prodcons_client.c -lrt

server: server.c prodcons_server.c prodcons_server.h prodcons_msg.h
	gcc -o server server.c prodcons_server.c -lrt -lpthread

clean:
	rm -f *.o
	rm -f start
	rm -f client
	rm -f server
