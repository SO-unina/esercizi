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


	/* Elimina un eventuale oggetto rimasto da una precedente esecuzione. */
	shm_unlink(SHM_NAME);

	int fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0600);

	if(fd<0) { perror("SHM errore"); exit(1); }

	if(ftruncate(fd, sizeof(struct prodcons))<0) { perror("FTRUNCATE errore"); exit(1); }

	struct prodcons * p;

	p = (struct prodcons *) mmap(NULL, sizeof(struct prodcons), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if(p==MAP_FAILED) { perror("MMAP errore"); exit(1); }

	close(fd);


	for(int i=0; i<DIM_BUFFER; i++) {
		p->stato[i] = BUFFER_VUOTO;
	}


	//SEMAFORI COOPERAZIONE tra i prod e i cons
	sem_init(&p->spazio_disponibile, 1, DIM_BUFFER);
	sem_init(&p->messaggio_disponibile, 1, 0);

	//SEMAFORI COMPETIZIONE tra i prod e i cons
	sem_init(&p->mutex_c, 1, 1);
	sem_init(&p->mutex_p, 1, 1);



	for(int i=0; i<NUM_CONSUMATORI; i++) {

		int pid = fork();

		if(pid==0) {

			//figlio consumatore

			printf("Inizio figlio consumatore\n");

			// NOTA: il generatore di numeri pseudo-casuali
			// viene inizializzato in modo diverso per ogni
			// processo (usando il valore del PID e il tempo)
			srand(getpid()*time(NULL));

			consumatore(p);

			exit(0);
		}
	}




	for(int i=0; i<NUM_PRODUTTORI; i++) {

		int pid = fork();

		if(pid==0) {

			//figlio produttore

			printf("Inizio figlio produttore\n");

			// NOTA: il generatore di numeri pseudo-casuali
			// viene inizializzato in modo diverso per ogni
			// processo (usando il valore del PID e il tempo)
			srand(getpid()*time(NULL));

			produttore(p);

			exit(0);
		}
	}



	for(int i=0; i<NUM_PRODUTTORI; i++) {
		wait(NULL);
		printf("Figlio produttore terminato\n");
	}

	for(int i=0; i<NUM_CONSUMATORI; i++) {
		wait(NULL);
		printf("Figlio consumatore terminato\n");
	}

	sem_destroy(&p->spazio_disponibile);
	sem_destroy(&p->messaggio_disponibile);
	sem_destroy(&p->mutex_c);
	sem_destroy(&p->mutex_p);

	munmap(p, sizeof(struct prodcons));
	shm_unlink(SHM_NAME);

	return 0;

}
