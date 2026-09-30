/*
 * Copy-on-write: la fork() di un processo che occupa 256 MB è quasi istantanea,
 * perché padre e figlio condividono le stesse pagine fisiche (in sola lettura).
 * La copia vera avviene solo quando il figlio scrive: una pagina alla volta,
 * con un page fault per ogni pagina.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/wait.h>

#define DIM (256L * 1024 * 1024)	/* 256 MB */

/* millisecondi trascorsi da t0 */
static double ms_da(struct timespec t0)
{
	struct timespec t;

	clock_gettime(CLOCK_MONOTONIC, &t);
	return (t.tv_sec - t0.tv_sec) * 1e3 + (t.tv_nsec - t0.tv_nsec) / 1e6;
}

/* page fault "minori" (senza accesso al disco) del processo fino a ora */
static long page_fault(void)
{
	struct rusage r;

	getrusage(RUSAGE_SELF, &r);
	return r.ru_minflt;
}

/* scrive valore in tutti i 256 MB e stampa tempo e page fault */
static void scrivi(char *buf, int valore, const char *chi)
{
	struct timespec t0;
	long pf0 = page_fault();

	clock_gettime(CLOCK_MONOTONIC, &t0);
	memset(buf, valore, DIM);
	printf("%-38s %7.1f ms %7ld page fault\n", chi, ms_da(t0), page_fault() - pf0);
}

int main(void)
{
	struct timespec t0;
	char *buf = malloc(DIM);

	if (buf == NULL) {
		perror("malloc");
		exit(1);
	}
	printf("256 MB = %ld pagine da %ld byte\n\n", DIM / sysconf(_SC_PAGESIZE), sysconf(_SC_PAGESIZE));

	scrivi(buf, 1, "padre: prima scrittura");	/* ora le pagine sono davvero in RAM */
	fflush(stdout);				/* svuoto il buffer di printf prima della fork */

	clock_gettime(CLOCK_MONOTONIC, &t0);
	pid_t pid = fork();

	if (pid == -1) {
		perror("fork fallita");
		exit(1);
	}

	if (pid == 0) {
		printf("%-38s %7.1f ms\n", "figlio: fork", ms_da(t0));
		scrivi(buf, 2, "figlio: prima scrittura");	/* il kernel copia ogni pagina */
		scrivi(buf, 3, "figlio: seconda scrittura");	/* le pagine ora sono sue */
		exit(buf[DIM - 1] == 3 ? 0 : 1);
	}

	wait(NULL);
	free(buf);
	return 0;
}
