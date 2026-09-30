/*
 * Il figlio termina subito, ma il padre per 30 secondi non chiama wait():
 * nel frattempo il figlio è uno zombie (stato Z, <defunct> in ps).
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
	pid_t pid = fork();

	if (pid == -1) {
		perror("fork fallita");
		exit(1);
	}

	if (pid == 0) {
		printf("figlio %d: termino subito\n", getpid());
		exit(0);
	}

	printf("padre %d: dormo 30 secondi senza chiamare wait\n", getpid());
	sleep(30);

	wait(NULL);	/* raccolgo lo stato del figlio: solo ora lo zombie sparisce */
	printf("padre: ho chiamato wait, lo zombie %d non c'è più\n", pid);

	return 0;
}
