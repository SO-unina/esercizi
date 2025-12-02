#DEBUG = -DDEBUG_ 

all: meteo

meteo: meteo.o procedure.o monitor_hoare.o
	gcc -o meteo meteo.o procedure.o monitor_hoare.o

meteo.o: meteo.c header.h
	gcc -c -o meteo.o meteo.c

procedure.o: procedure.c header.h
	gcc -c -o procedure.o procedure.c

monitor_hoare.o: monitor_hoare.c monitor_hoare.h
	gcc -c -o monitor_hoare.o $(DEBUG) monitor_hoare.c

clean:
	rm -rf *.o
	rm -rf meteo
