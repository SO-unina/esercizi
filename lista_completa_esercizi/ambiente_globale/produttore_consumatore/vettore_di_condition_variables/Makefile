all: start

start: main.o dvd.o
	gcc -o start main.o dvd.o -lpthread

main.o: main.c dvd.h
	gcc -c main.c

dvd.o: dvd.c dvd.h
	gcc -c dvd.c

clean:
	rm -f *.o
	rm -f start
