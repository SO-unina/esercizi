all: prodcons_coda_circolare

prodcons_coda_circolare: procedure.o prodcons_coda_circolare.o
	gcc -o prodcons_coda_circolare procedure.o prodcons_coda_circolare.o -lrt -lpthread

prodcons_coda_circolare.o: prodcons_coda_circolare.c
	gcc -c prodcons_coda_circolare.c

procedure.o: procedure.h procedure.c
	gcc -c procedure.c



clean:
	rm -f *.o
	rm -f prodcons_coda_circolare
