all: main.o monitor.o
	gcc -pthread -o start main.o monitor.o

main.o: main.c header.h
	gcc -c -o main.o main.c

monitor.o: monitor.c header.h
	gcc -c -o monitor.o monitor.c

clean:
	rm -f *.o
	rm -f start
