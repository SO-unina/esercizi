#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <string.h>



void prenota(esame_t* esame){
	accedi_prenotati(esame);
	esame->numero_prenotati += 1;
	lascia_prenotati(esame);
}

void stampa_appello(esame_t* esame){
	inizio_lettura(esame);
	printf("Processo studente %d - Data di appello: %s\n",getpid(),esame->prossimo_appello);
	fine_lettura(esame);
}

void esegui(esame_t* esame){
	int t;
	t = 1 + (rand() % 8);
	printf("Sono studente e aspetto tempo %d secondi\n",t);
	sleep(t);
	stampa_appello(esame);
	prenota(esame);
}

int main(){
	int shm_fd;
	esame_t* esame = NULL;
	struct timeval t1;
	gettimeofday(&t1, NULL);
	srand(t1.tv_usec * t1.tv_sec);

	/* Apre l'oggetto di shared memory gia' creato dal main:
	 * senza O_CREAT, se l'oggetto non esiste si ha errore. */
	shm_fd = shm_open(SHM_NAME, O_RDWR, 0);
	if(shm_fd < 0){
		perror("Errore shm_open studente");
		exit(-1);
	}

	esame = mmap(NULL, sizeof(esame_t), PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if(esame == MAP_FAILED){
		perror("Errore mmap studente");
		exit(-1);
	}
	close(shm_fd);

	printf("Debug [PS] - shared memory %s mappata\n",SHM_NAME);
	esegui(esame);
	return 0;
}
