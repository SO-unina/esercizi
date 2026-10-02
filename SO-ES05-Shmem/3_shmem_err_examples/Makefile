all: no_ftruncate stale_object wrong_mapping_size

no_ftruncate: no_ftruncate.o
	gcc -o no_ftruncate no_ftruncate.o -lrt

stale_object: stale_object.o
	gcc -o stale_object stale_object.o -lrt

wrong_mapping_size: wrong_mapping_size.o
	gcc -o wrong_mapping_size wrong_mapping_size.o -lrt

no_ftruncate.o: no_ftruncate.c
	gcc -c no_ftruncate.c

stale_object.o: stale_object.c
	gcc -c stale_object.c

wrong_mapping_size.o: wrong_mapping_size.c
	gcc -c wrong_mapping_size.c

clean:
	rm -f *.o
	rm -f no_ftruncate stale_object wrong_mapping_size
