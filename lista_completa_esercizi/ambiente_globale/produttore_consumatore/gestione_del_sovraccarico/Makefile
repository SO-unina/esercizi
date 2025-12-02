eseguibile: main.o procedure.o
	gcc -pthread main.o procedure.o -o eseguibile

main.o: main.c header.h
	gcc -c main.c -o main.o

procedure.o: procedure.c header.h
	gcc -c procedure.c -o procedure.o

clean:
	rm -f *.o
	rm -f eseguibile
