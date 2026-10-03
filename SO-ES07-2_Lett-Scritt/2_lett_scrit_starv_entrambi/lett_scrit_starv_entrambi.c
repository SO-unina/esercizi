/****PROBLEMA DEI LETTORI-SCRITTORI CON ATTESA INDEFINITA DI ENTRAMBI****/
/*Il programma sincronizza processi lettori e processi scrittori nell'accesso ad
  una zona di memoria condivisa. La soluzione adottata può implicare starvation
  per entrambe le categorie di processi: lettori e scrittori assumono un
  comportamento perfettamente simmetrico

  Header file:header.h
  Programma chiamante:lett_scrit_starv_entrambi.c
  Modulo delle procedure:procedure.c
  Direttive per la compilazione dei moduli:Makefile
  Nome del file eseguibile:lettore_scrittore_exe
*/


#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/wait.h>
#include "header.h"

int main(){

	/************DICHIARAZIONE DELLE VARIABILI***************/

	int id_shared, k;
	int status;
	Buffer *ptr_sh;

	pid_t pid;
	int num_processi = 10;

	/************RICHIESTA DEL SEGMENTO DI MEMORIA CONDIVISA***********/


	/* Elimina un eventuale oggetto rimasto da una precedente esecuzione. */
	shm_unlink(SHM_NAME);

	id_shared = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0600);

	if(id_shared<0) { perror("SHM errore"); exit(1); }

	printf("id_shared=%d \n", id_shared);

	if(ftruncate(id_shared, sizeof(Buffer))<0) { perror("FTRUNCATE errore"); exit(1); }

	ptr_sh = (Buffer*) mmap(NULL, sizeof(Buffer), PROT_READ | PROT_WRITE, MAP_SHARED, id_shared, 0);

	if(ptr_sh==MAP_FAILED) { perror("MMAP errore"); exit(1); }

	close(id_shared);

	//   Inzializzazione struttura dati
	ptr_sh->numlettori = 0;
	ptr_sh->numscrittori = 0;
	ptr_sh->messaggio = 0;


	//   inizializzazione dei semafori
	sem_init(&ptr_sh->mutex_lettori_scrittori, 1, 1);
	sem_init(&ptr_sh->mutex_numlettori, 1, 1);
	sem_init(&ptr_sh->mutex_numscrittori, 1, 1);
	sem_init(&ptr_sh->mutex_scrittori, 1, 1);



	/****GENERAZIONE DEI PROCESSI E OPERAZIONI DI R/W****/


	//generazione di scrittori e lettori
	for (k=0; k<num_processi; k++) {

		pid=fork();

		if (pid == 0)  {                //processo figlio
			if ((k%2) == 0) {

				printf("sono il figlio scrittore. Il mio pid %d \n", getpid());
				Scrittore(ptr_sh);
			}else {

				printf("sono il figlio lettore. Il mio pid %d \n", getpid());
				Lettore(ptr_sh);
			}
			exit(0);
		}
	}


	for (k=0; k<num_processi; k++){
		pid = wait(&status);
		if (pid == -1)
			perror("errore");
		else
			printf ("Figlio n.ro %d e\' morto con status= %d \n ", pid, status);
	}

	sem_destroy(&ptr_sh->mutex_lettori_scrittori);
	sem_destroy(&ptr_sh->mutex_numlettori);
	sem_destroy(&ptr_sh->mutex_numscrittori);
	sem_destroy(&ptr_sh->mutex_scrittori);

	munmap(ptr_sh, sizeof(Buffer));
	shm_unlink(SHM_NAME);

	return 0;

}
