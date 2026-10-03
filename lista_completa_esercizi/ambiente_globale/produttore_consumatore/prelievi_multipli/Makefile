all: prodcons

prodcons: main.o monitor.o prodcons.o
	gcc -o prodcons main.o monitor.o prodcons.o -lpthread

main.o: main.c prodcons.h monitor.h
	gcc -c main.c

monitor.o: monitor.c monitor.h
	gcc -c monitor.c

prodcons.o: prodcons.c prodcons.h monitor.h
	gcc -c prodcons.c

clean:
	rm -f *.o
	rm -f prodcons
