all: main.o procedure.o monitor_signal_continue.o
	gcc -o start main.o procedure.o monitor_signal_continue.o

main.o: main.c procedure.o monitor_signal_continue.o
	gcc -c main.c

procedure.o: procedure.c procedure.h monitor_signal_continue.o
	gcc -c procedure.c

monitor_signal_continue.o: monitor_signal_continue.c monitor_signal_continue.h
	gcc -c monitor_signal_continue.c
      
clean:
	rm -f *.o 
	rm -f start

