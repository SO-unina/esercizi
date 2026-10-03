all: app

app: main.o client.o printer.o server.o
	gcc -o app main.o client.o printer.o server.o -lrt

main.o: main.c common.h
	gcc -c main.c

client.o: client.c common.h
	gcc -c client.c

printer.o: printer.c common.h
	gcc -c printer.c

server.o: server.c common.h
	gcc -c server.c

clean:
	rm -f *.o
	rm -f app
