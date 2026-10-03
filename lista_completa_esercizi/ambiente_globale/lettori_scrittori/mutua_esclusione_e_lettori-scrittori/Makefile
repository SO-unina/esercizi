all: main docente studente

main: main.c header.h
	gcc -o main main.c -lrt -lpthread

docente: docente.c procedure.o
	gcc -o docente docente.c procedure.o -lrt -lpthread

studente: studente.c procedure.o
	gcc -o studente studente.c procedure.o -lrt -lpthread

procedure.o: procedure.c header.h
	gcc -c procedure.c

clean:
	rm -f *.o
	rm -f main docente studente
