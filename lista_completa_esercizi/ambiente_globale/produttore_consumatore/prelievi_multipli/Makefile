all: prodcons

prodcons: main.o monitor_hoare.o prodcons.o
	gcc -o prodcons   main.o monitor_hoare.o prodcons.o

main.o: main.c prodcons.h
	gcc -c -o main.o  main.c

monitor_hoare.o: monitor_hoare.c monitor_hoare.h
	gcc -c -o monitor_hoare.o  monitor_hoare.c

prodcons.o: prodcons.c prodcons.h
	gcc -c -o prodcons.o  prodcons.c

clean:
	rm -f *.o
	rm -f prodcons
