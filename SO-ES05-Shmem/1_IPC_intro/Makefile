all: posix_ipc_intro

posix_ipc_intro: posix_ipc_intro.o
	gcc -o posix_ipc_intro posix_ipc_intro.o -lrt

posix_ipc_intro.o: posix_ipc_intro.c
	gcc -c posix_ipc_intro.c

clean:
	rm -f *.o
	rm -f posix_ipc_intro
