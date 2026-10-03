CC=gcc
LFLAGS=-pthread

# Nota: versioni precedenti di PThreads utilizzavano
# un flag differente ("-lpthread").
#LFLAGS=-lpthread

all: counter_thread

counter_thread: counter_thread.c
	$(CC) $(LFLAGS) -o counter_thread counter_thread.c

clean:
	rm -f counter_thread

