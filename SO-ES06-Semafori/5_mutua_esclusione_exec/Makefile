all: start lavoratore

start: start.o
	gcc -o start start.o -lrt -lpthread

lavoratore: lavoratore.o
	gcc -o lavoratore lavoratore.o -lrt -lpthread

start.o: start.c header.h
	gcc -c start.c

lavoratore.o: lavoratore.c header.h
	gcc -c lavoratore.c

clean:
	rm -f *.o
	rm -f start
	rm -f lavoratore
