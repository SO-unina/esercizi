programma: main.o prodcons.o semafori.o
	gcc -o programma  main.o prodcons.o semafori.o

main.o: main.c prodcons.h
	gcc -c -o main.o  main.c

prodcons.o: prodcons.c prodcons.h
	gcc -c -o prodcons.o  prodcons.c

semafori.o: semafori.c semafori.h
	gcc -c -o semafori.o  semafori.c

clean:
	rm -f *.o
	rm -f programma