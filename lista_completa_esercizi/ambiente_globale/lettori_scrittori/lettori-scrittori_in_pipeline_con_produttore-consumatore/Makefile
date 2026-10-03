all: eseguibile

eseguibile: main.o procedure.o
	gcc -o eseguibile main.o procedure.o -lpthread

main.o: main.c procedure.h
	gcc -c main.c

procedure.o: procedure.c procedure.h
	gcc -c procedure.c

clean:
	rm -f *.o
	rm -f eseguibile
