/*****PROBLEMA DEI PRODUTTORI-CONSUMATORI: soluzione mediante Threads*****/
/*Il programma sincronizza produttori e consumatori nell'accesso ad una variabile condivisa utilizzando i Pthreads. */


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include "header.h"


#define NUM_THREADS     10


int main(){

     pthread_attr_t attr;
     pthread_t threads[NUM_THREADS];


     //creazione di una istanza di struttura monitor
     struct ProdCons * pc = malloc(sizeof(struct ProdCons));

     //inizializzazione mutex e condition
     pthread_mutex_init(&pc->mutex,NULL);
     pthread_cond_init(&pc->ok_prod_cv,NULL);
     pthread_cond_init(&pc->ok_cons_cv,NULL);

     //inizializzazione delle variabili di stato
     pc->ok_produzione = 1;
     pc->ok_consumo = 0;


     //impostazione dei thread come joinable
     pthread_attr_init(&attr);
     pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);	

     //generazione di scrittori e lettori
     int k;
     for (k=0;k<NUM_THREADS; k++) {

	if (k%2)  {
	     printf("Sono il thread Consumatore, id %d \n",k);
	     pthread_create(&threads[k], &attr, Consumatore, (void *) pc);
	   } else {
	     sleep(1);
             printf("Sono il thread Produttore, id %d \n",k);
	     pthread_create(&threads[k], &attr, Produttore, (void *) pc);	
	    }
	}

     for (k=0; k<NUM_THREADS;k++){
       pthread_join(threads[k],NULL);
       printf ("Thread n.ro %d terminato\n ",k);
     }

     /*deallocazione risorse*/	
     pthread_attr_destroy(&attr);
     pthread_mutex_destroy(&pc->mutex);
     pthread_cond_destroy(&pc->ok_prod_cv);
     pthread_cond_destroy(&pc->ok_cons_cv);

     free(pc);

     pthread_exit(0);
}
