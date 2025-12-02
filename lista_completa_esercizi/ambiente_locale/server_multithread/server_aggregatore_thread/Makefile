programma: main.o collettore.o sensore.o aggregatore.o
	gcc -o programma  main.o collettore.o sensore.o aggregatore.o

main.o: main.c sensore.h
	gcc -c main.c -o main.o

collettore.o: collettore.c sensore.h
	gcc -c collettore.c -o collettore.o

aggregatore.o: aggregatore.c sensore.h
	gcc -c aggregatore.c -o aggregatore.o

sensore.o: sensore.c sensore.h
	gcc -c sensore.c -o sensore.o

clean:
	rm -f *.o
	rm -f programma
