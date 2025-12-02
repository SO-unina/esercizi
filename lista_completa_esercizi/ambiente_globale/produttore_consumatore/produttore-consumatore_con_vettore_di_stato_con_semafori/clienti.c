#include "header.h"
#include "semafori.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>

int id_mem, id_sem;

void Cliente();


int main() {

	int i;

	key_t key_mem = ftok(".", 'm');
	id_mem = shmget(key_mem, POSTI*sizeof(posto) + sizeof(int), IPC_CREAT|0644);

	if(id_mem < 0) {
		perror("Errore shmget");
		exit(1);
	}

	posti = shmat(id_mem, 0, 0);
	disponibilita = (int *)(posti + POSTI);

	if(posti == ((void*)-1)) {
		perror("Errore shmat");
		exit(1);
	}


	for(i=0; i<POSTI; i++) {
		posti[i].stato = LIBERO;
		posti[i].id_cliente = 0;
	}

	(*disponibilita) = POSTI;




	key_t key_sem = ftok(".", 's');
	id_sem = semget(key_sem, 1, IPC_CREAT|0644);

	if(id_sem < 0) {
		perror("Errore semget");
		exit(1);
	}

	semctl(id_sem, MUTEX, SETVAL, 1);


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

	semctl(id_sem, 0, IPC_RMID);
	shmctl(id_mem, IPC_RMID, 0);

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


	Wait_Sem(id_sem, MUTEX);

	printf("<%d> Disponibilita: %d\n", getpid(), *disponibilita);

	if( (*disponibilita)<numposti ) {
		printf("<%d> Disponibilita esaurita (ho tentato di allocare %d posti, ci sono %d posti liberi)\n", getpid(), numposti, *disponibilita);

		Signal_Sem(id_sem, MUTEX);
		return;
	}

	j=0;
	while(j<POSTI && allocati < numposti) {

		if( posti[j].stato == LIBERO ) {
			posti[j].stato = INAGGIORNAMENTO;
			postiscelti[allocati] = j;
			allocati++;

			printf("<%d> Ho messo in aggiornamento il posto %d\n", getpid(), j);
		}

		j++;
	}

	(*disponibilita) = (*disponibilita) - numposti;

	Signal_Sem(id_sem, MUTEX);


	sleep(1);


	for(j=0; j<numposti; j++) {
		p = postiscelti[j];
		posti[p].stato = OCCUPATO;
		posti[p].id_cliente = getpid();
		printf("<%d> Ho occupato il posto %d\n", getpid(), p);
	}

}
