all: main docente studente

main: main.c
	gcc -g -o main main.c

docente: docente.c procedure.o
	gcc -o docente docente.c procedure.o

studente: studente.c procedure.o
	gcc -o studente studente.c procedure.o

procedure.o: procedure.c header.h
	gcc -c procedure.c

clean:
	rm -f *.o
	rm -f main docente studente
