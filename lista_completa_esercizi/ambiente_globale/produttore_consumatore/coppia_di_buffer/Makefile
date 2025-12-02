all: main-padre main-produttore main-consumatore

main-padre: main-padre.o
	gcc -o main-padre  main-padre.o

main-padre.o: main-padre.c buffer.h
	gcc -c -o main-padre.o  main-padre.c

main-produttore: main-produttore.o buffer.o semafori.o
	gcc -o main-produttore   main-produttore.o buffer.o semafori.o

main-consumatore: main-consumatore.o buffer.o semafori.o
	gcc -o main-consumatore  main-consumatore.o buffer.o semafori.o

main-produttore.o: main-produttore.c
	gcc -c -o main-produttore.o  main-produttore.c

main-consumatore.o: main-consumatore.c
	gcc -c -o main-consumatore.o  main-consumatore.c

buffer.o: buffer.c buffer.h
	gcc -c -o buffer.o  buffer.c

semafori.o: semafori.c semafori.h
	gcc -c -o semafori.o  semafori.c

clean:
	rm -f *.o
	rm -f main-padre
	rm -f main-consumatore
	rm -f main-produttore