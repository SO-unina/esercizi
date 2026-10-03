all: clienti visualizzatore

clienti: clienti.o
	gcc -o clienti clienti.o -lrt -lpthread

clienti.o: clienti.c header.h
	gcc -c clienti.c

visualizzatore: visualizzatore.o
	gcc -o visualizzatore visualizzatore.o -lrt -lpthread

visualizzatore.o: visualizzatore.c header.h
	gcc -c visualizzatore.c

clean:
	rm -f *.o
	rm -f clienti
	rm -f visualizzatore
