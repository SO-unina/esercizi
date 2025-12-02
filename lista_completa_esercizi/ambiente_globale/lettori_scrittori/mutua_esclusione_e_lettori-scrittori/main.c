#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdlib.h>
#include "header.h"
#include <string.h>

#define NUM_DOCENTI 1
#define NUM_STUDENTI 10

int main(int argc,char*argv[]){
	pid_t pid;
	key_t key_shm,key_sem;
	esame_t* esame;
	int i,idsem,idshm;
	int status;
	int num_processi = NUM_DOCENTI + NUM_STUDENTI;
	key_shm = ftok(PATH,ID);
	key_sem = ftok(PATH,ID_SEM);
	idshm = shmget(key_shm,sizeof(esame_t),IPC_CREAT|0664);
	idsem = semget(key_sem,3,IPC_CREAT|0664);
		
	//INIZIALIZZAZIONE SHARED MEMORY
	esame = (esame_t *) shmat(idshm,0,0);
	esame->prossimo_appello[0]='\0'; 
	esame->numero_prenotati = 0;
	esame->numero_lettori = 0;

	//INIZIALIZZAZIONE SEMAFORI
	semctl(idsem,MUTEX,SETVAL,1);
	semctl(idsem,APPELLO,SETVAL,1);
	semctl(idsem,PRENOTATI,SETVAL,1);

	for(i=0; i < num_processi;i++){
		//pid = fork();		
		if((pid=fork()) < 0){
			printf("Errore...\n");
			exit(-1);
		}
		if(pid == 0){
			if(i == 0){
			printf("Generazione processo figlio %d docente con pid %d\n",i,getpid());
			execve("./docente",NULL,NULL);
			}
			else{
			printf("Generazione processo figlio %d studente con pid %d\n",i,getpid());
			execve("./studente",NULL,NULL);
			}
			printf("Qualcosa e' andato storto...\n");
			//sleep(5);
			//exit(0);
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
				printf("Esecuzione terminata di processo %s tramite segnali...\n",(i == 0) ? "docente" : "studente");
				}	
			//printf("Esecuzione terminata incorrettamente processo %s...\n",(i == 0) ? "docente" : "studente");
	}
	shmctl(idshm,IPC_RMID,0);
	semctl(idsem,0,IPC_RMID);
	return 0;
}
