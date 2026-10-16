all: prodottoScalareMP prodottoScalareMT

prodottoScalareMP: prodottoScalareMP.c
	gcc -o prodottoScalareMP prodottoScalareMP.c -lrt -lpthread

prodottoScalareMT: prodottoScalareMT.c
	gcc -o prodottoScalareMT prodottoScalareMT.c -lpthread

clean:
	rm -rf *.o
	rm -rf prodottoScalareMP
	rm -rf prodottoScalareMT
