#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <string.h>



void prenota(int semid,esame_t* esame){
	accedi_prenotati(semid);
	esame->numero_prenotati += 1;
	lascia_prenotati(semid);
}

void stampa_appello(int semid,esame_t* esame){
	inizio_lettura(semid,esame);
	printf("Processo studente %d - Data di appello: %s\n",getpid(),esame->prossimo_appello);
	fine_lettura(semid,esame);
}

void esegui(int semid,esame_t* esame){
	int i;
	int t;
	t = 1 + (rand() % 8);
	printf("Sono studente e aspetto tempo %d secondi\n",t);
	sleep(t);
	stampa_appello(semid,esame);
	prenota(semid,esame);
}

int main(int argc,char* argv[]){
	key_t key_shm,key_sem;
	int shmid,semid;
	key_shm = ftok(PATH,ID);
	key_sem = ftok(PATH,ID_SEM);
	esame_t* esame = NULL;
	struct timeval t1;
	gettimeofday(&t1, NULL);
	srand(t1.tv_usec * t1.tv_sec);
	shmid = shmget(key_shm,sizeof(esame_t),IPC_CREAT|0664);
	/*if(shmid == -1){
		printf("Il segmento esiste gia' accedo come client\n");
	}else{
		printf("Creo un nuovo segmento...\n");
	}*/
	
	esame = (esame_t*) shmat(shmid,0,0);
	semid = semget(key_sem,0,0);
	printf("Debug [PS] - idshm %d idsem %d\n",shmid,semid);
	//printf("Esame Prossimo appello %s\n",esame->prossimo_appello);
	esegui(semid,esame);		
	return 0;
}
