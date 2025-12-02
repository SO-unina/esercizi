main : main.o procedure.o monitor_hoare.o
	gcc -o magazzino main.o procedure.o monitor_hoare.o

main.o : main.c header.h monitor_hoare.h
	gcc -c main.c

procedure.o : procedure.c header.h monitor_hoare.h
	gcc -c procedure.c

monitor_hoare.o : monitor_hoare.c monitor_hoare.h
	gcc -c monitor_hoare.c

clean :
	rm -f *.o
	rm -f magazzino 
