all: main

main: main.o
	gcc -o main main.o -lrt -lpthread

main.o: main.c
	gcc -c main.c

clean:
	rm -f *.o
	rm -f main
