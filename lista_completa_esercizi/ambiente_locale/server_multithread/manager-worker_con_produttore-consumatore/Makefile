all: main client server

main: main.c header.h
	gcc -o main main.c -lrt -lpthread

client: client.c header.h
	gcc -o client client.c -lrt -lpthread

server: server.c header.h
	gcc -o server server.c -lrt -lpthread

clean:
	rm -f *.o
	rm -f main client server
