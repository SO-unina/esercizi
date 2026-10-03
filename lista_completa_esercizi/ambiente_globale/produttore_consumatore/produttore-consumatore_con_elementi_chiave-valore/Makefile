all: programma

programma: main.o prodcons.o
	gcc -o programma main.o prodcons.o -lrt -lpthread

main.o: main.c prodcons.h
	gcc -c main.c

prodcons.o: prodcons.c prodcons.h
	gcc -c prodcons.c

clean:
	rm -f *.o
	rm -f programma
