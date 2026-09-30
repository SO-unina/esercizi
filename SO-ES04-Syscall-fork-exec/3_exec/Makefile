all: main_cp main_ls sostituzione

main_cp: main_cp.c
	gcc -o main_cp main_cp.c

main_ls: main_ls.c
	gcc -o main_ls main_ls.c

sostituzione: sostituzione.c
	gcc -o sostituzione sostituzione.c

clean:
	rm -rf main_cp
	rm -rf main_ls
	rm -rf sostituzione
	rm -rf *.o
