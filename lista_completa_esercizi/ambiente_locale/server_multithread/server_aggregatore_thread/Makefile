all: programma

programma: main.o collettore.o sensore.o aggregatore.o
	gcc -o programma main.o collettore.o sensore.o aggregatore.o -lrt -lpthread

main.o: main.c sensore.h aggregatore.h collettore.h
	gcc -c main.c

collettore.o: collettore.c sensore.h collettore.h
	gcc -c collettore.c

aggregatore.o: aggregatore.c sensore.h aggregatore.h
	gcc -c aggregatore.c

sensore.o: sensore.c sensore.h
	gcc -c sensore.c

clean:
	rm -f *.o
	rm -f programma
