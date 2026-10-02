all: main

main: main.o processi-mutua-esclusione.o
	gcc -o main main.o processi-mutua-esclusione.o -lrt -lpthread

main.o: main.c processi.h
	gcc -c main.c

processi-mutua-esclusione.o: processi-mutua-esclusione.c processi.h
	gcc -c processi-mutua-esclusione.c

clean:
	rm -f *.o
	rm -f main
