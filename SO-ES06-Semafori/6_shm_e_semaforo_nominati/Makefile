all: master scrittore lettore

master: master.o
	gcc -o master master.o -lrt -lpthread

scrittore: scrittore.o
	gcc -o scrittore scrittore.o -lrt -lpthread

lettore: lettore.o
	gcc -o lettore lettore.o -lrt -lpthread

master.o: master.c condiviso.h
	gcc -c master.c

scrittore.o: scrittore.c condiviso.h
	gcc -c scrittore.c

lettore.o: lettore.c condiviso.h
	gcc -c lettore.c

clean:
	rm -f *.o
	rm -f master
	rm -f scrittore
	rm -f lettore
