all: prodcons_singolo_buffer

prodcons_singolo_buffer: procedure.o prodcons_singolo_buffer.o
	gcc -o prodcons_singolo_buffer procedure.o prodcons_singolo_buffer.o -lrt -lpthread

prodcons_singolo_buffer.o: prodcons_singolo_buffer.c
	gcc -c prodcons_singolo_buffer.c

procedure.o: procedure.h procedure.c
	gcc -c procedure.c



clean:
	rm -f *.o
	rm -f prodcons_singolo_buffer
