all: master writer reader

master: master.o
	gcc -o master master.o

writer: writer.o
	gcc -o writer writer.o

reader: reader.o
	gcc -o reader reader.o

master.o: master.c shared_data.h
	gcc -c master.c

writer.o: writer.c shared_data.h
	gcc -c writer.c

reader.o: reader.c shared_data.h
	gcc -c reader.c

clean:
	rm -f *.o
	rm -f master
	rm -f writer
	rm -f reader
