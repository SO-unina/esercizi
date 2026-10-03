/************PROBLEMA DEI LETTORI SCRITTORI**********/
/****soluzione con attesa indefinita degli scrittori****/

/*Il programma sincronizza nell'accesso ad una zona di memoria condivisa.
  Parte dei processi esegue operazioni di esclusiva lettura,i restanti
  di esclusiva scrittura.

  Header file: header.h
  Programma chiamante: lett_scrit_starv_scrittori.c
  Modulo delle procedure: procedure.c
  Direttive per la compilazione dei moduli:Makefile
  Nome del file eseguibile:lettore_scrittore_exe
*/


#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/wait.h>

#include "header.h"


int main(){

	/************DICHIARAZIONE DELLE VARIABILI***************/

	int id_shared, k, numlettori, numscrittori;
	int status;
	pid_t pid;
	Buffer *buf;

	numlettori = 6;
	numscrittori = 6;
	int num_processi = numscrittori + numlettori;

	/************RICHIESTA DEL SEGMENTO DI MEMORIA CONDIVISA***********/

	/* Elimina un eventuale oggetto rimasto da una precedente esecuzione. */
	shm_unlink(SHM_NAME);

	id_shared = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0600);

	if(id_shared<0) { perror("SHM errore"); exit(1); }

	printf("id_shared = %d\n", id_shared);

	if(ftruncate(id_shared, sizeof(Buffer))<0) { perror("FTRUNCATE errore"); exit(1); }

	buf = (Buffer*) mmap(NULL, sizeof(Buffer), PROT_READ | PROT_WRITE, MAP_SHARED, id_shared, 0);

	if(buf==MAP_FAILED) { perror("MMAP errore"); exit(1); }

	close(id_shared);

	//   Inizializzazione struttura dati
	buf->numlettori = 0;
	buf->messaggio = 0;

	//   inizializzazione dei semafori
	sem_init(&buf->mutex_numlettori, 1, 1);
	sem_init(&buf->mutex_lettori_scrittori, 1, 1);

	//generazione di scrittori e lettori
	for (k=0; k<num_processi; k++) {

		pid=fork();

		if (pid == 0)  {                //processo figlio
			if ( (k%2) == 0) {
				printf("sono il figlio scrittore. Il mio pid %d \n", getpid());
				Scrittore(buf);

			} else {
				printf("sono il figlio lettore. Il mio pid %d\n", getpid());
				Lettore(buf);
			}
			exit(0);
		}


	}

	for (k=0; k<num_processi; k++){
		pid=wait(&status);
		if (pid==-1)
			perror("errore");
		else
			printf("Figlio n.ro %d e\' morto con status= %d\n", pid, status);
	}

	/********DEALLOCAZIONE SEMAFORI E MEMORIA CONDIVISA**********/

	sem_destroy(&buf->mutex_numlettori);
	sem_destroy(&buf->mutex_lettori_scrittori);

	munmap(buf, sizeof(Buffer));
	shm_unlink(SHM_NAME);

	return 0;
}
