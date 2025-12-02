programma: main.o coda_richieste.o semafori.o
	gcc -o programma  main.o coda_richieste.o semafori.o

main.o: main.c coda_richieste.h
	gcc -c -o main.o   main.c

coda_richieste.o: coda_richieste.c coda_richieste.h
	gcc -c -o coda_richieste.o coda_richieste.c

semafori.o: semafori.c semafori.h
	gcc -c -o semafori.o  semafori.c

clean:
	rm -f *.o
	rm -f programma