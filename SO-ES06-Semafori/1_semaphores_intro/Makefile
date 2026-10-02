all: named_sem process_shared_sem

named_sem: named_sem.o
	gcc -o named_sem named_sem.o -lrt -lpthread

process_shared_sem: process_shared_sem.o
	gcc -o process_shared_sem process_shared_sem.o -lrt -lpthread

named_sem.o: named_sem.c
	gcc -c named_sem.c

process_shared_sem.o: process_shared_sem.c
	gcc -c process_shared_sem.c

clean:
	rm -f *.o
	rm -f named_sem process_shared_sem
