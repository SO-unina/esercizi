eseguibile: main.o procedure.o
	gcc -lpthread main.o procedure.o  -o eseguibile

main.o: main.c procedure.h
	gcc -c main.c -o main.o

procedure.o: procedure.c procedure.h
	gcc -c procedure.c -o procedure.o

clean:
	rm -f *.o
	rm -f eseguibile
