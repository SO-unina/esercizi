			/*-----IMPLEMENTAZIONE DELLE PROCEDURE----*/


#include <stdio.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "header.h"
/* Metodi pubblici del monitor */

void Produci(struct ProdCons * pc, msg m) {

		InizioProduzione(pc);

	        /*********Produzione********/
                pc->mess = m;

		// NOTA: la chiamata di sistema "gettid()" è disponibile solo
		// sul sistema operativo Linux (non è portabile su altre piattaforme)
		
                //int my_id = syscall(SYS_gettid);
                
                pthread_t my_id = pthread_self();

	        printf("Thread #%llu Valore PRODOTTO = [%ld] \n", my_id, pc->mess);


		FineProduzione(pc);


}

msg Consuma(struct ProdCons * pc) {

		msg m;

		InizioConsumo(pc);

		/*********Consumo********/

		// NOTA: la chiamata di sistema "gettid()" è disponibile solo
		// sul sistema operativo Linux (non è portabile su altre piattaforme)
		//int my_id = syscall(SYS_gettid);
                
		pthread_t my_id = pthread_self();

	        printf("Thread #%llu, valore CONSUMATO = [%ld] \n", my_id, pc->mess);

		m = pc->mess;

		FineConsumo(pc);


		return m;
}



/* Metodi privati del monitor */

void InizioConsumo(struct ProdCons * pc){

	pthread_mutex_lock(&pc->mutex);

	if(pc->ok_consumo==0)
		pthread_cond_wait(&pc->ok_cons_cv, &pc->mutex);
}

void FineConsumo(struct ProdCons * pc){

        pc->ok_produzione = 1;
	pc->ok_consumo = 0;

	pthread_cond_signal(&pc->ok_prod_cv);

	pthread_mutex_unlock(&pc->mutex);
}


void InizioProduzione(struct ProdCons * pc){

	pthread_mutex_lock(&pc->mutex);

	if (pc->ok_produzione==0)
		pthread_cond_wait(&pc->ok_prod_cv, &pc->mutex);

}

void FineProduzione (struct ProdCons * pc){

	pc->ok_consumo = 1;
	pc->ok_produzione = 0;

	pthread_cond_signal(&pc->ok_cons_cv);

	pthread_mutex_unlock(&pc->mutex);
}


/* Entry-point dei thread */

void *Produttore(void* p) {

		struct ProdCons * pc = (struct ProdCons *)p;

		int i;
		for(i=0; i<3; i++) {
			struct timeval t1;
			struct timezone t2;

			gettimeofday(&t1,&t2);//valore diverso ad ogni produzione

			msg m = t1.tv_usec;

			Produci(pc, m);
		}

}

void *Consumatore (void * p) {

		struct ProdCons * pc = (struct ProdCons *)p;

		int i;
		for(i=0; i<3; i++) {

			msg m = Consuma(pc);
		}

}


