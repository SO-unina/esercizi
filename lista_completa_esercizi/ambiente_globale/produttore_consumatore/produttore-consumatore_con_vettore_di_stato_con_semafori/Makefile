all: clienti visualizzatore

clienti: semafori.o clienti.o
	gcc -o clienti clienti.o semafori.o

clienti.o: clienti.c header.h 
	gcc -c clienti.c -o clienti.o

visualizzatore: semafori.o visualizzatore.o
	gcc -o visualizzatore visualizzatore.o semafori.o

visualizzatore.o: visualizzatore.c header.h 
	gcc -c visualizzatore.c -o visualizzatore.o

semafori.o: semafori.c semafori.h
	gcc -c semafori.c -o semafori.o
      
clean:
	rm clienti
	rm visualizzatore
	rm -f *.o 
	rm -f *~

