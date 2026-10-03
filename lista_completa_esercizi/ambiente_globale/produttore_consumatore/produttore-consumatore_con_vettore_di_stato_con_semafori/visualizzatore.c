#include "header.h"

#include <semaphore.h>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

teatro_t * teatro;


int main() {

	/* Apre l'oggetto di shared memory gia' creato dal programma
	 * clienti: senza O_CREAT, se l'oggetto non esiste si ha errore. */
	int shm_fd = shm_open(SHM_NAME, O_RDWR, 0);

	if(shm_fd < 0) {
		perror("Errore shm_open (avviare prima ./clienti)");
		exit(1);
	}

	teatro = mmap(NULL, sizeof(teatro_t), PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0);

	if(teatro == MAP_FAILED) {
		perror("Errore mmap");
		exit(1);
	}

	close(shm_fd);


	while(1) {

		sem_wait(&teatro->mutex);

		int i;
		for(i=0; i<POSTI; i++) {

			char stato='\0';

			if(teatro->posti[i].stato == LIBERO) {
				stato = 'L';
			} else if(teatro->posti[i].stato == INAGGIORNAMENTO) {
				stato = 'A';
			} else if(teatro->posti[i].stato == OCCUPATO) {
				stato = 'O';
			}

			printf("%d:%c ", i, stato);
		}

		printf("\n\n");

		sem_post(&teatro->mutex);

		sleep(1);
	}

	return 0;
}
