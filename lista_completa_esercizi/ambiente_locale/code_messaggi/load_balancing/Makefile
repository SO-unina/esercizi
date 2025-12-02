start: main.o procedure.o
	gcc -o start main.o procedure.o

main.o: main.c header.h
	gcc -o main.o -c main.c

procedure.o: procedure.c header.h
	gcc -o procedure.o -c procedure.c

clean:
	rm -f *.o
	rm -f start

 
