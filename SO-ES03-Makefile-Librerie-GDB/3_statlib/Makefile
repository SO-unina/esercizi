CURR=${PWD}

.PHONY: all clean

all: calc

calc: main.c mat.h libmat.a
	gcc -o calc main.c -L$(CURR) -lmat

libmat.a: mat.o
	ar rcs libmat.a mat.o

mat.o: mat.c mat.h
	gcc -c mat.c

clean:
	rm -f ./calc
	rm -f libmat.a
	rm -f *.o
