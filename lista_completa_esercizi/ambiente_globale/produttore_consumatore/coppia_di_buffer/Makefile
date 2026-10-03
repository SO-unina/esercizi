all: main-padre main-produttore main-consumatore

main-padre: main-padre.o
	gcc -o main-padre main-padre.o -lrt -lpthread

main-produttore: main-produttore.o buffer.o
	gcc -o main-produttore main-produttore.o buffer.o -lrt -lpthread

main-consumatore: main-consumatore.o buffer.o
	gcc -o main-consumatore main-consumatore.o buffer.o -lrt -lpthread

main-padre.o: main-padre.c buffer.h
	gcc -c main-padre.c

main-produttore.o: main-produttore.c buffer.h
	gcc -c main-produttore.c

main-consumatore.o: main-consumatore.c buffer.h
	gcc -c main-consumatore.c

buffer.o: buffer.c buffer.h
	gcc -c buffer.c

clean:
	rm -f *.o
	rm -f main-padre
	rm -f main-produttore
	rm -f main-consumatore
