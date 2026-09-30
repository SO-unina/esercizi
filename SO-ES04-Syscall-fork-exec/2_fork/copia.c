/*
 * Dopo la fork() padre e figlio hanno ciascuno la propria copia delle variabili:
 * se il figlio modifica x, il padre non se ne accorge.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int x = 10;	/* variabile globale (area dati) */

int main(void)
{
	printf("[%d] prima della fork: x = %d\n", getpid(), x);

	pid_t pid = fork();

	if (pid == -1) {
		perror("fork fallita");
		exit(1);
	}

	if (pid == 0) {
		x = x + 5;
		printf("[%d] figlio: x = %d, indirizzo di x = %p\n", getpid(), x, (void *)&x);
		exit(0);
	}

	wait(NULL);	/* aspetto che il figlio termini (la vediamo tra poco) */
	printf("[%d] padre:  x = %d, indirizzo di x = %p\n", getpid(), x, (void *)&x);

	return 0;
}
