CC=gcc
LFLAGS=-pthread

# Nota: versioni precedenti di PThreads utilizzavano
# un flag differente ("-lpthread").
#LFLAGS=-lpthread

all: helloMT_pass_stack helloMT_pass_stack_no_warn helloMT_pass_heap

helloMT_pass_stack: helloMT_pass_stack.c
	$(CC) $(LFLAGS) -o helloMT_pass_stack helloMT_pass_stack.c

helloMT_pass_stack_no_warn: helloMT_pass_stack_no_warn.c
	$(CC) $(LFLAGS) -o helloMT_pass_stack_no_warn helloMT_pass_stack_no_warn.c

helloMT_pass_heap: helloMT_pass_heap.c
	$(CC) $(LFLAGS) -o helloMT_pass_heap helloMT_pass_heap.c

clean:
	rm -f helloMT_pass_stack helloMT_pass_stack_no_warn helloMT_pass_heap

