all: main client server

main: main.c serversync.h
	gcc -o main main.c -lrt

client: client.o serversync.o
	gcc -o client client.o serversync.o -lrt

server: server.o serversync.o
	gcc -o server server.o serversync.o -lrt

client.o: client.c serversync.h
	gcc -c client.c

server.o: server.c serversync.h
	gcc -c server.c

serversync.o: serversync.c serversync.h
	gcc -c serversync.c

clean:
	rm -f *.o
	rm -f main
	rm -f client
	rm -f server
