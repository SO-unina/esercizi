all: magazzino

magazzino: main.o procedure.o monitor.o
	gcc -o magazzino main.o procedure.o monitor.o -lpthread

main.o: main.c header.h monitor.h
	gcc -c main.c

procedure.o: procedure.c header.h monitor.h
	gcc -c procedure.c

monitor.o: monitor.c monitor.h
	gcc -c monitor.c

clean:
	rm -f *.o
	rm -f magazzino
