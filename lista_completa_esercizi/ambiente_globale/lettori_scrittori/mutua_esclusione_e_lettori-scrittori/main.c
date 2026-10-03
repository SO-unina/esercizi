#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>
#include "header.h"

#define NUM_DOCENTI 1
#define NUM_STUDENTI 10

int main(){
	pid_t pid;
	esame_t* esame;
	int i,shm_fd;
	int status;
	int num_processi = NUM_DOCENTI + NUM_STUDENTI;

	/* Elimina un eventuale oggetto rimasto da una precedente
	 * esecuzione, cosi' la shm_open con O_EXCL non fallisce. */
	shm_unlink(SHM_NAME);

	shm_fd = shm_open(SHM_NAME, O_CREAT|O_EXCL|O_RDWR, 0664);
	if(shm_fd < 0){
		perror("Errore shm_open");
		exit(-1);
	}

	/* La dimensione dell'oggetto va impostata con ftruncate PRIMA di mmap. */
	if(ftruncate(shm_fd, sizeof(esame_t)) < 0){
		perror("Errore ftruncate");
		exit(-1);
	}

	//INIZIALIZZAZIONE SHARED MEMORY
	esame = mmap(NULL, sizeof(esame_t), PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if(esame == MAP_FAILED){
		perror("Errore mmap");
		exit(-1);
	}
	close(shm_fd);

	esame->prossimo_appello[0]='\0';
	esame->numero_prenotati = 0;
	esame->numero_lettori = 0;

	//INIZIALIZZAZIONE SEMAFORI
	//Semafori anonimi process-shared (pshared=1), dentro la shared memory
	sem_init(&esame->mutex,1,1);
	sem_init(&esame->appello,1,1);
	sem_init(&esame->prenotati,1,1);

	for(i=0; i < num_processi;i++){
		if((pid=fork()) < 0){
			printf("Errore...\n");
			exit(-1);
		}
		if(pid == 0){
			if(i == 0){
			printf("Generazione processo figlio %d docente con pid %d\n",i,getpid());
			/* exec sostituisce l'immagine del processo: occorre svuotare
			 * il buffer di stdout prima, o la stampa puo' andare persa. */
			execl("./docente","docente",NULL);
			}
			else{
			printf("Generazione processo figlio %d studente con pid %d\n",i,getpid());
			execl("./studente","studente",NULL);
			}
			printf("Qualcosa e' andato storto...\n");
			exit(-1);
		}else{
		//Padre
			//printf("Processo padre attende figli...\n");
		}
	}

	for(i = 0; i < num_processi;i++){
			wait(&status);
			if(WIFEXITED(status)){
				printf("Esecuzione terminata normalmente processo %d...\n",i);
				}
			if(WIFSIGNALED(status)){
				if(i == 0)
					printf("Esecuzione terminata di processo docente tramite segnali...\n");
				else
					printf("Esecuzione terminata di processo studente tramite segnali...\n");
				}
	}

	/* Cleanup: il creatore distrugge i semafori, rimuove la mappatura
	 * e infine rimuove il nome dell'oggetto da /dev/shm. */
	sem_destroy(&esame->mutex);
	sem_destroy(&esame->appello);
	sem_destroy(&esame->prenotati);
	munmap(esame, sizeof(esame_t));
	shm_unlink(SHM_NAME);
	return 0;
}
