#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

void azzera_prenotati(esame_t* esame){
	inizio_lettura(esame);
	accedi_prenotati(esame);
	printf("\n");
	printf("-------Stampa stato esame------\n");
	printf("Data di esame: %s\n",esame->prossimo_appello);
	printf("Numero prenotati: %d\n",esame->numero_prenotati);
	printf("-------------------------------\n");
	printf("\n");
	esame->numero_prenotati = 0;
	lascia_prenotati(esame);
	fine_lettura(esame);
}

void aggiorna_data(esame_t* esame,const char* data){
	printf("DEBUG: Aggiorna data %s\n",data);
	inizio_scrittura(esame);
	//Aggiorna la data di esame
	strcpy(esame->prossimo_appello,data);
	printf("DEBUG: Prossimo appello %s\n",esame->prossimo_appello);
	fine_scrittura(esame);
}

void esegui(esame_t* esame){
	printf("Docente in esecuzione\n");
	int i;
	const char* data;
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
 		aggiorna_data(esame,data);
		sleep(3);
		azzera_prenotati(esame);
	}
}

int main(){
	int shm_fd;
	esame_t* esame = NULL;

	/* Apre l'oggetto di shared memory gia' creato dal main:
	 * senza O_CREAT, se l'oggetto non esiste si ha errore. */
	shm_fd = shm_open(SHM_NAME, O_RDWR, 0);
	if(shm_fd < 0){
		perror("Errore shm_open docente");
		exit(-1);
	}

	esame = mmap(NULL, sizeof(esame_t), PROT_READ|PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if(esame == MAP_FAILED){
		perror("Errore mmap docente");
		exit(-1);
	}
	close(shm_fd);

	/* I semafori sono campi della struttura in shared memory,
	 * gia' inizializzati dal main. */
	printf("Debug [PD] : shared memory %s mappata\n",SHM_NAME);
	esegui(esame);
	return 0;
}
