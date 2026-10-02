all: single_executable writer reader

single_executable: single_executable.o
	gcc -o single_executable single_executable.o -lrt

writer: writer.o
	gcc -o writer writer.o -lrt

reader: reader.o
	gcc -o reader reader.o -lrt

single_executable.o: single_executable.c shared_data.h
	gcc -c single_executable.c

writer.o: writer.c shared_data.h
	gcc -c writer.c

reader.o: reader.c shared_data.h
	gcc -c reader.c

clean:
	rm -f *.o
	rm -f single_executable writer reader
