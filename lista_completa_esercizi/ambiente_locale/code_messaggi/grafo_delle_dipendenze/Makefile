all: start p1 p2 p3 p4 p5 p6

start: start.c p.h
	gcc -o start start.c -lrt

p1: p1.c p.h
	gcc -o p1 p1.c -lrt

p2: p2.c p.h
	gcc -o p2 p2.c -lrt

p3: p3.c p.h
	gcc -o p3 p3.c -lrt

p4: p4.c p.h
	gcc -o p4 p4.c -lrt

p5: p5.c p.h
	gcc -o p5 p5.c -lrt

p6: p6.c p.h
	gcc -o p6 p6.c -lrt

clean:
	rm -f *.o
	rm -f start p1 p2 p3 p4 p5 p6
