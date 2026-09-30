/*
 * La exec sostituisce il programma, ma il processo resta lo stesso (stesso PID).
 * Nessuna fork: è questo stesso processo a "diventare" il comando richiesto.
 *
 *   ./sostituzione ps -o pid,ppid,comm
 *   ./sostituzione comando_inesistente
 */
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "Uso: %s comando [argomenti...]\n", argv[0]);
		return 1;
	}

	printf("Sono il processo %d, mio padre è %d: ora eseguo %s\n", getpid(), getppid(), argv[1]);
	fflush(stdout);	/* svuoto il buffer di printf: dopo la exec andrebbe perso */

	/* argv[1] è il comando da cercare nel PATH, &argv[1] il suo vettore di argomenti
	 * (anche questo termina con NULL, perché argv[argc] vale NULL) */
	execvp(argv[1], &argv[1]);

	perror("exec fallita");	/* si arriva qui solo se la exec fallisce */
	return 1;
}
