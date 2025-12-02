#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "header.h"

#define NUM_WORKER 3

static int id_send;
static int id_recv;
static pthread_t threads[NUM_WORKER];

void init_buffer(Buffer* b){
	b->count = 0;
	b->testa = 0;
	b->coda = 0;

	pthread_cond_init(&(b->ok_cons),NULL);
	pthread_cond_init(&(b->ok_prod),NULL);
	pthread_mutex_init(&(b->mutex),NULL);

	printf("[SERVER] - Buffer inizializzato...\n");
}

void remove_buffer(Buffer* b){
	pthread_cond_destroy(&(b->ok_cons));
	pthread_cond_destroy(&(b->ok_prod));
	pthread_mutex_destroy(&(b->mutex));

	printf("[SERVER] - Buffer distrutto...\n");
}



void copia_messaggio(Messaggio* dst, Messaggio* src){
	dst->pid = src->pid;
	dst->op1 = src->op1;
	dst->op2 = src->op2;
}

void insert_element(Buffer* b,Messaggio* m){
	pthread_mutex_lock(&(b->mutex));

	while(b->count == MAX){
		pthread_cond_wait(&(b->ok_prod),&(b->mutex));
	}

	//produzione
	copia_messaggio(&(b->elems[b->testa]),m);
	b->testa = (b->testa + 1)%MAX;
	b->count++;

	printf("[DEBUG] - Produzione stato della struttura: conto %d, testa %d, coda %d\n",b->count,b->testa,b->coda);

 	pthread_cond_signal(&(b->ok_cons));
	pthread_mutex_unlock(&(b->mutex));
}

Messaggio get_element(Buffer* b){
	Messaggio res;

	pthread_mutex_lock(&(b->mutex));
	while(b->count == 0){
		pthread_cond_wait(&(b->ok_cons),&(b->mutex));
	}

	//consumo
	copia_messaggio(&res,&(b->elems[b->coda]));
	b->coda = (b->coda + 1) % MAX;
	b->count--;

	printf("[DEBUG] - Consumo stato della struttura: conto %d, testa %d, coda %d\n",b->count,b->testa,b->coda);

	pthread_cond_signal(&(b->ok_prod));		
	pthread_mutex_unlock(&(b->mutex));

	return res;
}

void* manager(void* arg){
	int i;
	int rc;

	Buffer* b = (Buffer*) arg;

	Messaggio temp;	


	printf("[MANAGER] - start del thread...\n");


	while(1){


		printf("[MANAGER] - accesso alla coda di messaggi...\n");

		rc = msgrcv(id_send,&temp,sizeof(Messaggio)-sizeof(long),0,IPC_NOWAIT);

		if(rc > 0){

			// E' arrivato un messaggio, il valore di ritorno contiene
			// il numero di bytes trasmessi col messaggio

			if(temp.op1 == -1 && temp.op2 == -1){

				printf("[MANAGER] - Ho ricevuto il messaggio di termine...\n");

				printf("[MANAGER] - Termino gli altri threads...\n");

				for(i = 0; i < NUM_WORKER-1;i++){
					pthread_cancel(threads[i]);
				}

				//Rimuovi strutture
				printf("[MANAGER] - Rimuovo tutte le risorse...\n");

				remove_buffer(b);
				free(b);
				msgctl(id_send,IPC_RMID,0);
				msgctl(id_recv,IPC_RMID,0);

				//Esci
				exit(0);	
			}

			//Se il messaggio non e' di termine

			printf("[MANAGER] - Ho ricevuto il messaggio lo inserisco nel buffer...\n");
			insert_element(b,&temp);

		}
		else if(errno == ENOMSG) {

			// La variabile globale di libreria "errno" è settata con un codice
			// indicativo che la coda è vuota

			printf("[MANAGER] - Nessun messaggio ricevuto, rimango in attesa...\n");
		}
		else {

			perror("[MANAGER] - Errore su msgrcv()");
			exit(1);
		}

		sleep(1);
	}
}

void *worker(void* arg){

	printf("[WORKER] - start del thread...\n");

	Buffer* b = (Buffer *) arg;

	Messaggio c;
	Risposta r;

	while(1){

		c = get_element(b);

		r.type = c.pid;
		r.risultato = c.op1 * c.op2;

		printf("[WORKER] - Ho ricevuto la richiesta {%d,%d} dal processo %d con risultato %d\n",c.op1,c.op2,c.pid,r.risultato);

		msgsnd(id_recv,&r,sizeof(Risposta)-sizeof(long),0);  
	}
}



int main(){

        int i,status;
        pid_t pid;

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

	key_t key_send,key_res;
        key_send = ftok(FTOK_QUEUE_CMD_PATH,FTOK_QUEUE_CMD_TOK);
        key_res = ftok(FTOK_QUEUE_RES_PATH,FTOK_QUEUE_RES_TOK);

        id_send = msgget(key_send, 0);
        id_recv = msgget(key_res, 0);


        printf("[SERVER] - id1 %d, id2 %d\n",id_send,id_recv);


	//Inizializziamo strutture
	
	Buffer* b = (Buffer *) malloc(sizeof(Buffer));

	init_buffer(b);
	

	//Avvio dei thread
	for(i = 0; i < NUM_WORKER;i++){

		printf("[MASTER] - creo thread %d...\n",i);

		if(i == NUM_WORKER-1){
			pthread_create(&threads[i],&attr,manager,(void *) b);
		}else{
			pthread_create(&threads[i],&attr,worker,(void *) b);
		}
	}

	printf("[MASTER] - bye bye...\n");
	pthread_attr_destroy(&attr);
	pthread_exit(NULL);

        return 0;
}

