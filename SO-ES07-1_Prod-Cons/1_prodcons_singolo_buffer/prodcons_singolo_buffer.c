#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>

#include "procedure.h"

int main() {


	srand(time(NULL));

	/* Elimina un eventuale oggetto rimasto da una precedente esecuzione. */
	shm_unlink(SHM_NAME);

	int fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0600);

	if(fd<0) { perror("SHM errore"); exit(1); }

	if(ftruncate(fd, sizeof(struct prodcons))<0) { perror("FTRUNCATE errore"); exit(1); }

	struct prodcons * p;

	p = (struct prodcons *) mmap(NULL, sizeof(struct prodcons), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if(p==MAP_FAILED) { perror("MMAP errore"); exit(1); }

	close(fd);


	p->valore = 0;

	//SEMAFORI COOPERAZIONE tra 1 prod e 1 cons: NO COMPETIZIONE
	sem_init(&p->spazio_disponibile, 1, 1);
	sem_init(&p->messaggio_disponibile, 1, 0);



	int pid = fork();

	if(pid==0) {
		//figlio consumatore

		printf("Inizio figlio consumatore\n");
		consumatore(p);
		exit(0);
	}


	pid = fork();

	if(pid==0) {
		//figlio produttore

		printf("Inizio figlio produttore\n");
		produttore(p);
		exit(0);
	}



	wait(NULL);
	printf("primo figlio terminato\n");

	wait(NULL);
	printf("secondo figlio terminato\n");

	sem_destroy(&p->spazio_disponibile);
	sem_destroy(&p->messaggio_disponibile);

	munmap(p, sizeof(struct prodcons));
	shm_unlink(SHM_NAME);

	return 0;
}
