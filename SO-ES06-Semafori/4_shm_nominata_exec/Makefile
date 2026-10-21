all: master lavoratore

master: master.o
	gcc -o master master.o -lrt -lpthread

lavoratore: lavoratore.o
	gcc -o lavoratore lavoratore.o -lrt -lpthread

master.o: master.c condiviso.h
	gcc -c master.c

lavoratore.o: lavoratore.c condiviso.h
	gcc -c lavoratore.c

clean:
	rm -f *.o
	rm -f master
	rm -f lavoratore
