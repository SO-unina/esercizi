all: prodcons_vettore_stato

prodcons_vettore_stato: procedure.o prodcons_vettore_stato.o
	gcc -o prodcons_vettore_stato procedure.o prodcons_vettore_stato.o -lrt -lpthread

prodcons_vettore_stato.o: prodcons_vettore_stato.c
	gcc -c prodcons_vettore_stato.c

procedure.o: procedure.h procedure.c
	gcc -c procedure.c



clean:
	rm -f *.o
	rm -f prodcons_vettore_stato
