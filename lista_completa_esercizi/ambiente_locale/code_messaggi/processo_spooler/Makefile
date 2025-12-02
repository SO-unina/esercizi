################################################################################
######                  MAKE 'printer' 'server' 'client'                 #######
################################################################################
PROG=app
HEAD=common.h
SRC=main.c client.c printer·c server.c
OBJ=main.o client.o printer.o server.o

CC=gcc
LDFLAGS="-O2"
CCFLAGS="-O2"

all: app

$(PROG): $(OBJ)
	gcc $(LDFLAGS) $(OBJ) -o $(PROG)

##########
main.o: $(HEAD) main.c 
	gcc $(CCFLAGS) -c main.c -o main.o

printer.o: $(HEAD) printer.c
	gcc $(CCFLAGS) -c printer.c -o printer.o

server.o: $(HEAD) server.c
	gcc $(CCFLAGS) -c server.c -o server.o

client.o: $(HEAD) client.c
	gcc $(CCFLAGS) -c client.c -o client.o
##########

.PHONY: clean all

clean:
	rm -Rf app *.o *~

