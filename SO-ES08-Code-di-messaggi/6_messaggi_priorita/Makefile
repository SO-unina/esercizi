all: sender receiver

sender: sender.o
	gcc -o sender sender.o -lrt

receiver: receiver.o
	gcc -o receiver receiver.o -lrt

sender.o: sender.c messages.h
	gcc -c sender.c

receiver.o: receiver.c messages.h
	gcc -c receiver.c

clean:
	rm -f *.o
	rm -f sender receiver
