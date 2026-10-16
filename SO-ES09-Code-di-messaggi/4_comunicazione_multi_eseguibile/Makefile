all: start p1 p2 p3

start: start.c header.h procedure.o
	gcc start.c -o start procedure.o -lrt

p1: p1.c header.h procedure.o
	gcc p1.c -o p1 procedure.o -lrt

p2: p2.c header.h procedure.o
	gcc p2.c -o p2 procedure.o -lrt

p3: p3.c header.h procedure.o
	gcc p3.c -o p3 procedure.o -lrt

procedure.o: procedure.c header.h
	gcc -c procedure.c

clean:
	rm -f *.o
	rm -f p1 p2 p3 start
