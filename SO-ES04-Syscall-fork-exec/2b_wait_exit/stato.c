/*
 * Il padre raccoglie con wait() lo stato di terminazione del figlio.
 *
 *   ./stato 42   il figlio termina con exit(42)
 *   ./stato      il figlio resta in attesa finché non riceve un segnale
 *                (da un altro terminale, o dallo stesso con ./stato &: kill PID_DEL_FIGLIO)
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	pid_t pid = fork();

	if (pid == -1) {
		perror("fork fallita");
		exit(1);
	}

	if (pid == 0) {
		if (argc > 1) {
			printf("figlio %d: termino con exit(%s)\n", getpid(), argv[1]);
			exit(atoi(argv[1]));
		}
		printf("figlio %d: aspetto un segnale...\n", getpid());
		pause();	/* si blocca finché non arriva un segnale */
		exit(0);
	}

	int stato;
	pid_t figlio = wait(&stato);	/* il padre si blocca finché il figlio non termina */

	printf("padre: wait ha restituito %d, stato = 0x%04x\n", figlio, stato);

	if (WIFEXITED(stato))
		printf("padre: il figlio ha chiamato exit, WEXITSTATUS = %d\n", WEXITSTATUS(stato));
	else if (WIFSIGNALED(stato))
		printf("padre: il figlio è stato ucciso da un segnale, WTERMSIG = %d\n", WTERMSIG(stato));

	return 0;
}
