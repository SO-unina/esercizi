all: main_padre main_lettori main_scrittore

main_padre: main_padre.o buffer.o
	gcc -o main_padre main_padre.o buffer.o -lrt -lpthread

main_lettori: main_lettori.o buffer.o
	gcc -o main_lettori main_lettori.o buffer.o -lrt -lpthread

main_scrittore: main_scrittore.o buffer.o
	gcc -o main_scrittore main_scrittore.o buffer.o -lrt -lpthread

main_padre.o: main_padre.c buffer.h
	gcc -c main_padre.c

main_lettori.o: main_lettori.c buffer.h
	gcc -c main_lettori.c

main_scrittore.o: main_scrittore.c buffer.h
	gcc -c main_scrittore.c

buffer.o: buffer.c buffer.h
	gcc -c buffer.c

clean:
	rm -f *.o
	rm -f main_padre
	rm -f main_lettori
	rm -f main_scrittore
