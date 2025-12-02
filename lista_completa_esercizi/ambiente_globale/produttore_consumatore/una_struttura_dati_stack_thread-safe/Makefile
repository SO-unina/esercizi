stack: stack.o stack_utente.o
	gcc -lpthread -o stack stack.o stack_utente.o

stack.o: stack.c stack.h
	gcc -c -o stack.o stack.c

stack_utente.o: stack_utente.c stack.h
	gcc -c -o stack_utente.o stack_utente.c

clean:
	rm -f stack
	rm -f *.o
