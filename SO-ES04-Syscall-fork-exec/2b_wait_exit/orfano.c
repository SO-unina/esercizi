/*
 * Il padre termina prima del figlio: il figlio diventa orfano
 * e viene "adottato" da un altro processo (init o un subreaper).
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	pid_t pid = fork();

	if (pid == -1) {
		perror("fork fallita");
		exit(1);
	}

	if (pid == 0) {
		printf("figlio %d: mio padre è %d\n", getpid(), getppid());
		sleep(2);	/* nel frattempo il padre termina */
		printf("figlio %d: ora mio padre è %d\n", getpid(), getppid());
		exit(0);
	}

	sleep(1);
	printf("padre %d: termino senza aspettare il figlio\n", getpid());
	return 0;
}
