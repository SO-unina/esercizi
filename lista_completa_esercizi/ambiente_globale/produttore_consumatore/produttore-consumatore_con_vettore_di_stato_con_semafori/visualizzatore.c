#include "header.h"
#include "semafori.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/sem.h>

int id_mem, id_sem;


int main() {

	int i;

	key_t key_mem = ftok(".", 'm');
	id_mem = shmget(key_mem, POSTI*sizeof(posto) + sizeof(int), 0644);

	if(id_mem < 0) {
		perror("Errore shmget");
		exit(1);
	}

	posti = shmat(id_mem, 0, 0);

	if(posti == ((void*)-1)) {
		perror("Errore shmat");
		exit(1);
	}

	disponibilita = (int*)(posti + POSTI);




	key_t key_sem = ftok(".", 's');
	id_sem = semget(key_sem, 1, 0644);

	if(id_sem < 0) {
		perror("Errore semget");
		exit(1);
	}


	while(1) {

		Wait_Sem(id_sem, MUTEX);

		int i;
		for(i=0; i<POSTI; i++) {

			char stato='\0';

			if(posti[i].stato == LIBERO) {
				stato = 'L';
			} else if(posti[i].stato == INAGGIORNAMENTO) {
				stato = 'A';
			} else if(posti[i].stato == OCCUPATO) {
				stato = 'O';
			}

			printf("%d:%c ", i, stato);
		}

		printf("\n\n");

		Signal_Sem(id_sem, MUTEX);

		sleep(1);
	}

	semctl(id_sem, 0, IPC_RMID);
	shmctl(id_mem, IPC_RMID, 0);

	return 0;
}


