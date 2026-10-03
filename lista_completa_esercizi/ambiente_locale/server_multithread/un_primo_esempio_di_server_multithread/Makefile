all: Start

Start: Main.o Server.o Client.o
	gcc -o Start Main.o Server.o Client.o -lrt -lpthread

Main.o: Main.c Header.h
	gcc -c Main.c

Server.o: Server.c Header.h
	gcc -c Server.c

Client.o: Client.c Header.h
	gcc -c Client.c

clean:
	rm -f *.o
	rm -f Start
