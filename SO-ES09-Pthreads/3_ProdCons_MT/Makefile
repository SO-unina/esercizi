LFLAGS=-pthread

# Nota: versioni precedenti di PThreads utilizzavano
# un flag differente ("-lpthread").
#LFLAGS=-lpthread

all:  p_c_MT

p_c_MT: p_c_MT.o procedure.o
	gcc  $(LFLAGS) -o p_c_MT p_c_MT.o procedure.o

p_c_MT.o: p_c_MT.c header.h 
	gcc -c  p_c_MT.c

procedure.o: procedure.c header.h
	gcc -c  procedure.c 

clean:
	 rm -f *.o 
	 rm -f ./p_c_MT

