start: main.o dvd.o
	gcc -lpthread -o start main.o dvd.o

main.o: main.c dvd.h
	gcc -c -o main.o main.c

dvd.o: dvd.c dvd.h
	gcc -c -o dvd.o dvd.c

clean:
	rm -f *.o
	rm -f start
