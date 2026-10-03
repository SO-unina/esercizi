all: start

start: main.o monitor.o
	gcc -o start main.o monitor.o -lpthread

main.o: main.c header.h
	gcc -c main.c

monitor.o: monitor.c header.h
	gcc -c monitor.c

clean:
	rm -f *.o
	rm -f start
