#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <string.h>

void azzera_prenotati(int semid,esame_t* esame){	
	inizio_lettura(semid,esame);
	accedi_prenotati(semid);
	printf("\n");
	printf("-------Stampa stato esame------\n");
	printf("Data di esame: %s\n",esame->prossimo_appello);
	printf("Numero prenotati: %d\n",esame->numero_prenotati);	
	printf("-------------------------------\n");
	printf("\n");
	esame->numero_prenotati = 0;
	lascia_prenotati(semid);
	fine_lettura(semid,esame);
}

void aggiorna_data(int semid,esame_t* esame,char* data){
	printf("DEBUG: Aggiorna data %s\n",data);
	inizio_scrittura(semid);
	/*if(esame->prossimo_appello != NULL){	
		printf("Debug\n");
		esame->prossimo_appello = (char*) malloc(strlen(data)+1);
	}else{
		esame->prossimo_appello = (char*) malloc(strlen(data)+1);
	}*/
	//Aggiorna la data di esame
	strcpy(&esame->prossimo_appello,data);
	printf("DEBUG: Prossimo appello %s\n",esame->prossimo_appello);	
	fine_scrittura(semid);
}

void esegui(int semid,esame_t* esame){
	printf("Docente in esecuzione\n");
	int i;
	char* data;
	for(i=0; i < 3;i++){
		switch(i){
			case 0:
			data = DATA_1;
			break;
			case 1:
			data = DATA_2;
			break;
			case 2:
			data = DATA_3;
			break;
			default:
			data = "default";
			break;
		}
 		aggiorna_data(semid,esame,data);
		sleep(3);
		azzera_prenotati(semid,esame);
	}	
}

int main(int argc,char* argv[]){
	key_t key_shm,key_sem;
	int shmid,semid;
	key_shm = ftok(PATH,ID);
	key_sem = ftok(PATH,ID_SEM);
	esame_t* esame = NULL;

	//shmid = shmget(key_shm,sizeof(esame_t),IPC_CREAT|0664);
	shmid = shmget(key_shm,0,0);
	/*if(shmid == -1){
		printf("Il segmento esiste gia' accedo come client\n");
	}else{
		printf("Creo un nuovo segmento...\n");
	}*/
	
	esame = (esame_t*) (shmat(shmid,0,0));
	semid = semget(key_sem,0,0);
	printf("Debug [PD] : idshm %d idsem %d\n",shmid,semid);
	esegui(semid,esame);		
	return 0;
}
