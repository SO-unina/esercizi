
LFLAGS=-pthread

# Nota: versioni precedenti di PThreads utilizzavano
# un flag differente ("-lpthread").
#LFLAGS=-lpthread

all:  l_s_MT

l_s_MT: l_s_MT.o procedure.o
	gcc  $(LFLAGS) -o l_s_MT l_s_MT.o procedure.o

l_s_MT.o: l_s_MT.c header.h 
	gcc -c l_s_MT.c

procedure.o: procedure.c header.h
	gcc -c procedure.c 

clean:
	 rm -f *.o 
	 rm -f ./l_s_MT

