#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "header.h"


void InizializzaMonitor(MonitorSchedulatore * s) {

	s->testa = 0;
	s->coda = 0;

	init_monitor(&(s->m), 2);
}


void RimuoviMonitor(MonitorSchedulatore * s) {

	remove_monitor(&(s->m));
}


void InserisciRichiesta(MonitorSchedulatore * s, richiesta * r) {

	enter_monitor(&(s->m));


	/*
	  Verifica se la coda di richieste sia piena.
	  In caso affermativo, il processo è posto in attesa.
	*/

	if( ((s->testa + 1) % DIMENSIONE_CODA) == s->coda ) {

		printf("Utente %d in attesa di spazio...\n", getpid());
		wait_condition(&(s->m), COND_VAR_PROD);
	}


	printf("[%d] Produzione in testa: %d\n", getpid(), s->testa);

	s->coda_richieste[s->testa].posizione = r->posizione;
	s->coda_richieste[s->testa].processo = r->processo;

	s->testa = (s->testa + 1) % DIMENSIONE_CODA;

	signal_condition(&(s->m), COND_VAR_CONS);


	leave_monitor(&(s->m));
}


void PrelevaRichiesta(MonitorSchedulatore * s, richiesta * r) {

	enter_monitor(&(s->m));


	/*
	  Verifica se la coda di richieste sia vuota.
	  In caso affermativo, il processo è posto in attesa.
	*/

	if(s->testa == s->coda) {

		printf("Schedulatore in attesa di richieste...\n");
		wait_condition(&(s->m), COND_VAR_CONS);
	}


	printf("[%d] Consumazione in coda: %d\n", getpid(), s->coda);

	r->posizione = s->coda_richieste[s->coda].posizione;
	r->processo = s->coda_richieste[s->coda].processo;

	s->coda = (s->coda + 1) % DIMENSIONE_CODA;

	signal_condition(&(s->m), COND_VAR_PROD);


	leave_monitor(&(s->m));
}


