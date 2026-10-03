all: stack

stack: stack.o stack_utente.o
	gcc -o stack stack.o stack_utente.o -lpthread

stack.o: stack.c stack.h
	gcc -c stack.c

stack_utente.o: stack_utente.c stack.h
	gcc -c stack_utente.c

clean:
	rm -f *.o
	rm -f stack
