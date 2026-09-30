CC = gcc
CFLAGS = -Wall

all: stato zombie orfano

# Nessuna regola per stato, zombie e orfano: make usa la regola implicita
# che produce l'eseguibile X da X.c (gcc -Wall X.c -o X)

clean:
	rm -f stato zombie orfano

.PHONY: all clean
