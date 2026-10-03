all: programma

programma: main.o coda_richieste.o
	gcc -o programma main.o coda_richieste.o -lrt -lpthread

main.o: main.c coda_richieste.h
	gcc -c main.c

coda_richieste.o: coda_richieste.c coda_richieste.h
	gcc -c coda_richieste.c

clean:
	rm -f *.o
	rm -f programma
