all: fork_ex copia fork_sbagliato

fork_ex: main.c
	gcc -o fork_ex main.c

copia: copia.c
	gcc -o copia copia.c

fork_sbagliato: fork_sbagliato.c
	gcc -o fork_sbagliato fork_sbagliato.c

clean:
	rm -rf *.o
	rm -rf fork_ex copia fork_sbagliato
