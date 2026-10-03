#include "header.h"

#include <semaphore.h>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

teatro_t * teatro;

void Cliente();

int main() {

	int i;

	/* Elimina un eventuale oggetto rimasto da una precedente
	 * esecuzione, cosi' la shm_open con O_EXCL non fallisce. */
	shm_unlink(SHM_NAME);

	int shm_fd = shm_open(SHM_NAME, O_CREAT|O_EXCL|O_RDWR, 0644);

	if(shm_fd < 0) {
		perror("Errore shm_open");
		exit(1);
	}

	/* La dimensione dell'oggetto va impostata con ftruncate PRIMA di mmap. */
	if(ftruncate(shm_fd, sizeof(teatro_t)) < 0) {
		perror("Errore ftruncate");
		exit(1);
	}

	teatro = mmap(NULL, sizeof(teatro_t), PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0);

	if(teatro == MAP_FAILED) {
		perror("Errore mmap");
		exit(1);
	}

	close(shm_fd);

	for(i=0; i<POSTI; i++) {
		teatro->posti[i].stato = LIBERO;
		teatro->posti[i].id_cliente = 0;
	}

	teatro->disponibilita = POSTI;

	/* Semaforo anonimo condiviso tra processi: pshared = 1 e
	 * collocazione nella struttura in shared memory. */
	if(sem_init(&teatro->mutex, 1, 1) < 0) {
		perror("Errore sem_init");
		exit(1);
	}

	for(i=0; i<CLIENTI; i++) {

		pid_t pid = fork();

		if(pid == 0) {
			Cliente();
			exit(0);
		}

		if(pid < 0) {
			perror("Errore fork");
			exit(1);
		}
	}

	int status;
	for(i=0; i<CLIENTI; i++) {
		wait(&status);
	}

	/* Cleanup: il creatore distrugge il semaforo, rimuove la mappatura
	 * e infine rimuove il nome dell'oggetto da /dev/shm (shm_unlink).
	 * Un eventuale visualizzatore ancora in esecuzione conserva la
	 * propria mappatura fino alla terminazione. */
	sem_destroy(&teatro->mutex);
	munmap(teatro, sizeof(teatro_t));
	shm_unlink(SHM_NAME);

	return 0;
}

void Cliente() {

	srand(time(NULL)+getpid());

	int sleeptime = rand() % 6;
	int numposti = (rand() % 4) + 1;
	int allocati = 0;

	int postiscelti[4];
	int j;
	int p;

	sleep(sleeptime);

	sem_wait(&teatro->mutex);

	printf("<%d> Disponibilita: %d\n", getpid(), teatro->disponibilita);

	if( teatro->disponibilita < numposti ) {
		printf("<%d> Disponibilita esaurita (ho tentato di allocare %d posti, ci sono %d posti liberi)\n", getpid(), numposti, teatro->disponibilita);

		sem_post(&teatro->mutex);
		return;
	}

	j=0;
	while(j<POSTI && allocati < numposti) {

		if( teatro->posti[j].stato == LIBERO ) {
			teatro->posti[j].stato = INAGGIORNAMENTO;
			postiscelti[allocati] = j;
			allocati++;

			printf("<%d> Ho messo in aggiornamento il posto %d\n", getpid(), j);
		}

		j++;
	}

	teatro->disponibilita = teatro->disponibilita - numposti;

	sem_post(&teatro->mutex);

	sleep(1);

	for(j=0; j<numposti; j++) {
		p = postiscelti[j];
		teatro->posti[p].stato = OCCUPATO;
		teatro->posti[p].id_cliente = getpid();
		printf("<%d> Ho occupato il posto %d\n", getpid(), p);
	}

}
