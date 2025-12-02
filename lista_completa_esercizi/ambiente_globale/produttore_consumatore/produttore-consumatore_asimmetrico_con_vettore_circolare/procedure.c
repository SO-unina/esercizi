#include "header.h"

void Inizializza(GestioneIO * g){
	pthread_mutex_init(&(g->mutex),NULL);
	pthread_cond_init(&(g->ok_produci),NULL);
	g->testa=0;
	g->coda=0;
	g->num_elem=0;
}

void Produci(GestioneIO * g ,Buffer * b ){+
	pthread_mutex_lock(&(g->mutex));
		while(g->num_elem==DIM){
			pthread_cond_wait(&(g->ok_produci),&(g->mutex));
		}
		g->vettore[g->testa].indirizzo=b->indirizzo;
		g->vettore[g->testa].dato=b->dato;
printf("[PRODUTTORE] Ho prodotto indirizzo '%d' dato '%d' in posizione'%d' \n",b->indirizzo,b->dato,g->testa);
		g->testa=(g->testa + 1 )%DIM;
		g->num_elem=g->num_elem + 1;
	pthread_mutex_unlock(&(g->mutex));
}
int Consuma(GestioneIO * g , Buffer * b ){
	pthread_mutex_lock(&(g->mutex));
		if(g->num_elem==0){
			pthread_mutex_unlock(&(g->mutex));
			return 1;
		}
		else{
			b->indirizzo=g->vettore[g->coda].indirizzo;
			b->dato=g->vettore[g->coda].dato;
			g->coda=(g->coda + 1 )%DIM;
			g->num_elem=g->num_elem - 1;
			pthread_cond_signal(&(g->ok_produci));
			pthread_mutex_unlock(&(g->mutex));
			return 0;
		}	
}

void Rimuovi(GestioneIO * g){
	pthread_mutex_destroy(&(g->mutex));
	pthread_cond_destroy(&(g->ok_produci));
}

void * Consumatore ( void * arg){
	GestioneIO * g = ( GestioneIO *) arg;
	int i;
	Buffer b;
	for(i=0;i<ITERAZIONI;i++){
		while(!Consuma(g,&b)){
			printf("[CONSUMATORE] Ho prelevato indirizzo '%d' e dato '%d' \n",b.indirizzo,b.dato);
		}
		printf("[CONSUMATORE] BUFFER VUOTO \n");		
		sleep(3);	

	}
	pthread_exit(NULL);
}

void * Produttore ( void * arg){
	GestioneIO * g = ( GestioneIO *) arg;
	int i;
	Buffer b;
	b.indirizzo=rand()%11;
	b.dato=rand()%11;
	for(i=0;i<PRODUZIONI;i++){
		Produci(g,&b);
		b.indirizzo=b.indirizzo+1;
		b.dato=b.dato+1;
		sleep(1);
	}
	pthread_exit(NULL);
}

