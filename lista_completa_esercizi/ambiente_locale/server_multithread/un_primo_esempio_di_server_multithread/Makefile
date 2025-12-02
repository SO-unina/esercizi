all: Start

Start: Main.o Server.o Client.o
	gcc -g -o Start Main.o Server.o Client.o -pthread

main.o: Main.c Header.h
	gcc -g -c Main.c

Server.o: Server.c Header.h
	gcc -g -c Server.c

Client.o: Client.c Header.h
	gcc -g -c Client.c

clean:
	rm -f *.o
	rm -f ./Start
