all: start

start: main.o procedure.o
	gcc -o start main.o procedure.o -lpthread

main.o: main.c header.h
	gcc -c main.c

procedure.o: procedure.c header.h
	gcc -c procedure.c

clean:
	rm -f *.o
	rm -f start
