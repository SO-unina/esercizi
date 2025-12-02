all: chat

chat: chat.o procedure.o
	gcc -g -o chat chat.o procedure.o

chat.o: chat.c header.h
	gcc -g -c chat.c

procedure.o: procedure.c header.h
	gcc -g -c procedure.c

clean:
	rm -f *.o
	rm -f ./chat
	rm -f *~
