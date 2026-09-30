/*
 * "Soluzione 1" delle slide: fork() in un ciclo, senza distinguere padre e figlio.
 * Anche i figli continuano il ciclo e creano altri processi.
 *
 *   ./fork_sbagliato      N = 3
 *   ./fork_sbagliato 5    N = 5
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	int i, N = 3;

	if (argc > 1)
		N = atoi(argv[1]);
	if (N > 10) {	/* protezione: con N grande diventa una fork bomb */
		fprintf(stderr, "N troppo grande (massimo 10)\n");
		exit(1);
	}

	for (i = 0; i < N; i++)
		fork();

	printf("Sono %d, mio padre è %d\n", getpid(), getppid());

	while (wait(NULL) > 0)	/* aspetto i miei figli, se ne ho */
		;

	return 0;
}
