programma: main.o client.o registro.o server.o
	gcc -o programma main.o client.o registro.o server.o

main.o: main.c registro.h
	gcc -c main.c

client.o: client.c registro.h
	gcc -c client.c

registro.o: registro.c registro.h
	gcc -c registro.c

server.o: server.c registro.h
	gcc -c server.c

clean:
	rm -f *.o
	rm -f programma