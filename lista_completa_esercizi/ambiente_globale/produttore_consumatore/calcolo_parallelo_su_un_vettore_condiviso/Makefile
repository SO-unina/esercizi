all: minimo-mutua-esclusione minimo-prod-cons

minimo-mutua-esclusione: main.o processi-mutua-esclusione.o semafori.o
	gcc -o minimo-mutua-esclusione  main.o processi-mutua-esclusione.o semafori.o

minimo-prod-cons: main.o processi-prod-cons.o semafori.o
	gcc -o minimo-prod-cons  main.o processi-prod-cons.o semafori.o

main.o: main.c
	gcc -c -g -o main.o main.c

processi-mutua-esclusione.o: processi-mutua-esclusione.c
	gcc -c -g -o processi-mutua-esclusione.o processi-mutua-esclusione.c

processi-prod-cons.o: processi-prod-cons.c
	gcc -c -g -o processi-prod-cons.o processi-prod-cons.c

semafori.o: semafori.c
	gcc -c -g -o semafori.o semafori.c

clean:
	rm -f *.o
	rm -f minimo-mutua-esclusione
	rm -f minimo-prod-cons
