#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdlib.h>
#include "header.h"

void wait_sem(int idsem,int numsem){
	struct sembuf sem_buf;
	sem_buf.sem_num = numsem;
	sem_buf.sem_flg=0;
	sem_buf.sem_op=-1;
	semop(idsem,&sem_buf,1);
}

void signal_sem(int idsem,int numsem){
	struct sembuf sem_buf;
	sem_buf.sem_num = numsem;
	sem_buf.sem_flg=0;
	sem_buf.sem_op=1;
	semop(idsem,&sem_buf,1);
}

void inizio_lettura(int sem,esame_t* esame){
	wait_sem(sem,MUTEX);
	esame->numero_lettori = esame->numero_lettori+1;
	printf("DEBUG: Numero lettori %d\n",esame->numero_lettori);
	if(esame->numero_lettori == 1)
		wait_sem(sem,APPELLO);
	signal_sem(sem,MUTEX);
}

void fine_lettura(int sem,esame_t* esame){
	wait_sem(sem,MUTEX);
	esame->numero_lettori = esame->numero_lettori - 1;
	printf("DEBUG: Numero lettori %d\n",esame->numero_lettori);
	if(esame->numero_lettori == 0)
		signal_sem(sem,APPELLO);
	signal_sem(sem,MUTEX);
}

void inizio_scrittura(int sem){
	wait_sem(sem,APPELLO);
	printf("DEBUG: Inizio scrittura\n");
}

void fine_scrittura(int sem){
	signal_sem(sem,APPELLO);
	printf("DEBUG: Fine scrittura\n");
}

void accedi_prenotati(int sem){
	wait_sem(sem,PRENOTATI);
	printf("DEBUG: Inizio scrittura prenotati\n");
}

void lascia_prenotati(int sem){
	signal_sem(sem,PRENOTATI);
	printf("DEBUG: Fine scrittura prenotati\n");
}


