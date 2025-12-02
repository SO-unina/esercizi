all: main_padre main_lettori main_scrittore

main_padre: main_padre.o buffer.o semafori.o
	gcc -o main_padre  main_padre.o buffer.o semafori.o

main_padre.o: main_padre.c buffer.h
	gcc -c -o main_padre.o  main_padre.c

buffer.o: buffer.c buffer.h
	gcc -c -o buffer.o  buffer.c

semafori.o: semafori.c semafori.h
	gcc -c -o semafori.o  semafori.c


main_lettori: main_lettori.o buffer.o semafori.o
	gcc -o main_lettori  main_lettori.o buffer.o semafori.o

main_scrittore: main_scrittore.o buffer.o semafori.o
	gcc -o main_scrittore  main_scrittore.o buffer.o semafori.o

main_lettori.o: main_lettori.c buffer.h
	gcc -c -o main_lettori.o  main_lettori.c

main_scrittore.o: main_scrittore.c buffer.h
	gcc -c -o main_scrittore.o  main_scrittore.c

clean:
	rm -f *.o
	rm -f main_padre
	rm -f main_lettori
	rm -f main_scrittore
