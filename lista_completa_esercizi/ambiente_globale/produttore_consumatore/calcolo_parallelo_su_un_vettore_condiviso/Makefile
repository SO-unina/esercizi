all: minimo-mutua-esclusione minimo-prod-cons

minimo-mutua-esclusione: main.o processi-mutua-esclusione.o
	gcc -o minimo-mutua-esclusione main.o processi-mutua-esclusione.o -lrt -lpthread

minimo-prod-cons: main.o processi-prod-cons.o
	gcc -o minimo-prod-cons main.o processi-prod-cons.o -lrt -lpthread

main.o: main.c processi.h
	gcc -c main.c

processi-mutua-esclusione.o: processi-mutua-esclusione.c processi.h
	gcc -c processi-mutua-esclusione.c

processi-prod-cons.o: processi-prod-cons.c processi.h
	gcc -c processi-prod-cons.c

clean:
	rm -f *.o
	rm -f minimo-mutua-esclusione
	rm -f minimo-prod-cons
