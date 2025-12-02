
#include "monitor_signal_continue.h"
#include "procedure.h"
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define NUM_PRODUTTORI_1  1
#define NUM_PRODUTTORI_2  3
#define NUM_CONSUMATORI  1

#define NUM_PRODUZIONI_1  3
#define NUM_PRODUZIONI_2  3
#define NUM_CONSUMAZIONI  12

int main(int argc,char* argv[])
{
	
	int status,indice;
	PriorityProdCons* p = NULL;
	int id_prod_cons;
	pid_t pid;


	
    
    //richiesta del buffer

	id_prod_cons = shmget(IPC_PRIVATE, sizeof(PriorityProdCons),IPC_CREAT|0664);
	printf("[DEBUG] - id_monitor=%d \n",id_prod_cons);
    
	p = (PriorityProdCons *) (shmat(id_prod_cons,0,0));
	
    inizializza_prod_cons(p);

    
	srand(time(NULL));
	
	
    
	// avvio produttori e consumatori

	for (int k=0; k<NUM_PRODUTTORI_1; k++) {

		pid=fork();

		if (pid==0)  {

			//processo figlio
			printf("Sono un produttore di tipo 1. Il mio pid è %d \n",getpid());

			for(int i=0; i < NUM_PRODUZIONI_1; i++) {

				int value = rand() % 12;

				produci_alta_prio(p, value);

				sleep(1);
			}
			
			exit(0);
		}
	}

	for (int k=0; k<NUM_PRODUTTORI_2; k++) {

		pid=fork();

		if (pid==0)  {

	    	//processo figlio
			printf("Sono un produttore di tipo 2. Il mio pid è %d \n",getpid());

			for(int i = 0; i < NUM_PRODUZIONI_2; i++) {

				int value = 13 + (rand() % 12);
				
				produci_bassa_prio(p, value);
				
				sleep(1);
			}
			
			exit(0);
		}
	}

	for (int k=0;k<NUM_CONSUMATORI;k++) {

		pid=fork();
		if (pid==0)  {
			
			//processo figlio
			printf("Sono un consumatore. Il mio pid è %d \n",getpid());

			for(int i=0; i < NUM_CONSUMAZIONI; i++) {

				sleep(2);

				int value = consuma(p);
			}

			exit(0);
		}
	}


	int num_processi = NUM_PRODUTTORI_1 + NUM_PRODUTTORI_2 + NUM_CONSUMATORI;

	for (int k=0; k<num_processi; k++) {

		pid=wait(&status);

		if(pid<0) {
			perror("Errore wait");
		} else {
			printf("Figlio n.ro %d è terminato con status=%d\n", pid, WEXITSTATUS(status));
		}
	}

	printf("Processo padre terminato\n");

	// rimozione memoria condivisa e semafori
	rimuovi_prod_cons(p);
	shmctl(id_prod_cons,IPC_RMID,0);

	return 0;
}
