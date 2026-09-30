all: shell shell-background

shell: shell.c
	gcc -o shell shell.c

shell-background: shell-background.c
	gcc -o shell-background shell-background.c

clean:
	rm -rf shell shell-background
	rm -rf *.o
