prodcons: prodcons.o main.o
	gcc -lpthread -o prodcons prodcons.o main.o

prodcons.o: prodcons.c prodcons.h
	gcc -c -o prodcons.o prodcons.c

main.o: main.c prodcons.h
	gcc -c -o main.o main.c

clean:
	rm -f prodcons
	rm -f *.o
