start: main.o procedure.o monitor_hoare.o
	gcc -o start main.o procedure.o monitor_hoare.o

main.o: main.c header.h
	gcc -o main.o -c main.c

procedure.o: procedure.c header.h
	gcc -o procedure.o -c procedure.c

monitor_hoare.o: monitor_hoare.c monitor_hoare.h
	gcc -o monitor_hoare.o -c monitor_hoare.c

clean:
	rm -f *.o
	rm -f start
