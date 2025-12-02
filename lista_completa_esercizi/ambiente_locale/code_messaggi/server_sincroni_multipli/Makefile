all: main client server

main: main.c
	gcc -o main  main.c

client: client.o serversync.o
	gcc -o client  client.o serversync.o

server: server.o serversync.o
	gcc -o server  server.o serversync.o

client.o: client.c serversync.h
	gcc -c -o client.o  client.c

server.o: server.c serversync.h
	gcc -c -o server.o  server.c

serversync.o: serversync.c serversync.h
	gcc -c -o serversync.o  serversync.c

clean:
	rm -f *.o
	rm -f client
	rm -f server
	rm -f main
	ipcs -q | grep "0x" | awk '{ system("ipcrm -q" $$2) }'
