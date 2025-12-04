			/*-----IMPLEMENTAZIONE DELLE PROCEDURE----*/


#include <stdio.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
//#include <pthread.h>

#include "header.h"

/* Entry-point dei thread */

void * Scrittore(void* p) {

		struct LettScritt * ls = (struct LettScritt *)p;

		int i;

		for(i=0; i<3; i++) {

			struct timeval t1;
			struct timezone t2;

			gettimeofday(&t1,&t2);//valore diverso ad ogni scrittura

			msg m = t1.tv_usec;

			Scrivi(ls, m);
		}

		pthread_exit(NULL);
}

void * Lettore (void * p) {

		struct LettScritt * ls = (struct LettScritt *)p;

		int i;

		for(i=0; i<3; i++) {
			msg m = Leggi(ls);
		}

		pthread_exit(NULL);
}



/* Metodi pubblici del monitor */

msg Leggi(struct LettScritt * ls) {

		msg m;

		InizioLettura(ls);

		/*********Lettura********/

		// NOTA: la chiamata di sistema "gettid()" è disponibile solo
		// sul sistema operativo Linux (non è portabile su altre piattaforme)
		//int my_id = syscall(SYS_gettid);

                pthread_t my_id = pthread_self(); // portable way to get thread ID

	        printf("Thread #%llu, valore LETTO = [%ld] \n", my_id, ls->mess);

		m = ls->mess;

		FineLettura(ls);


		return m;
}

void Scrivi(struct LettScritt * ls, msg m) {

		InizioScrittura(ls);

	        /*********Scrittura********/

		// NOTA: la chiamata di sistema "gettid()" è disponibile solo
		// sul sistema operativo Linux (non è portabile su altre piattaforme)
		//int my_id = syscall(SYS_gettid);

                pthread_t my_id = pthread_self(); // portable way to get thread ID

                ls->mess = m;

	        printf("Thread #%llu Valore SCRITTO = [%ld] \n", my_id, ls->mess);

		FineScrittura(ls);
}


/* Metodi privati del monitor */

void InizioLettura(struct LettScritt * ls){
	pthread_mutex_lock(&ls->mutex);

	while (ls->num_scrittori>0) {
		ls->num_lettori_wait++;
		pthread_cond_wait(&ls->ok_lett_cv, &ls->mutex);
		ls->num_lettori_wait--;
	}

	ls->num_lettori++;

	/* NOTA: la procedura IniziaLettura() esce temporaneamente dalla
	 * sezione critica, in modo da consentire a più thread di effettuare
	 * contemporaneamente l'operazione di lettura.
	 */

	pthread_mutex_unlock(&ls->mutex);
}

void FineLettura(struct LettScritt * ls){

	pthread_mutex_lock(&ls->mutex);

	ls->num_lettori--;

	if(ls->num_lettori == 0)
		pthread_cond_signal(&ls->ok_scritt_cv);

	pthread_mutex_unlock(&ls->mutex);
}


void InizioScrittura(struct LettScritt * ls){

	pthread_mutex_lock(&ls->mutex);

	while (ls->num_lettori > 0 || ls->num_scrittori > 0) {
		ls->num_scrittori_wait++;
		pthread_cond_wait(&ls->ok_scritt_cv, &ls->mutex);
		ls->num_scrittori_wait--;
	}

	ls->num_scrittori++;

	/* NOTA: La procedura InizioScrittura() esce temporaneamente dalla
	 * sezione critica, in modo da consentire ad eventuali altri thread
	 * scrittori di porsi in attesa sulla variabile-condition.
	 *
	 * Nella procedura FineScrittura(), il thread uscente risveglierà
	 * uno degli scrittori in attesa (se presente), in modo da bilanciare
	 * la starvation.
	 */

	pthread_mutex_unlock(&ls->mutex);
}

void FineScrittura (struct LettScritt * ls){

	pthread_mutex_lock(&ls->mutex);

	ls->num_scrittori--;

	if(ls->num_scrittori_wait > 0) {

		/* se ci sono scrittori in attesa, si dà loro
		 * la priorità */

		pthread_cond_signal(&ls->ok_scritt_cv);

	} else {

		/* risveglia tutti i lettori in attesa */

		pthread_cond_broadcast(&ls->ok_lett_cv);
	}

	pthread_mutex_unlock(&ls->mutex);
}



