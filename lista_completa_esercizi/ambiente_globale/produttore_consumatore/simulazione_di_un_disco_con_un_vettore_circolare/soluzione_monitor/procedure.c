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


void InserisciRichiesta(MonitorSchedulatore * s, int id, richiesta * r) {

	enter_monitor(&(s->m));


	/*
	  Verifica se la coda di richieste sia piena.
	  In caso affermativo, il thread è posto in attesa.

	  NOTA: semantica signal-and-continue: la condizione logica va
	  ricontrollata in un ciclo while (con il monitor di Hoare bastava
	  un semplice if).
	*/

	while( ((s->testa + 1) % DIMENSIONE_CODA) == s->coda ) {

		printf("Utente %d in attesa di spazio...\n", id);
		wait_condition(&(s->m), COND_VAR_PROD);
	}


	printf("[Utente %d] Produzione in testa: %d\n", id, s->testa);

	s->coda_richieste[s->testa].posizione = r->posizione;
	s->coda_richieste[s->testa].id_utente = r->id_utente;

	s->testa = (s->testa + 1) % DIMENSIONE_CODA;

	signal_condition(&(s->m), COND_VAR_CONS);


	leave_monitor(&(s->m));
}


void PrelevaRichiesta(MonitorSchedulatore * s, richiesta * r) {

	enter_monitor(&(s->m));


	/*
	  Verifica se la coda di richieste sia vuota.
	  In caso affermativo, il thread è posto in attesa.

	  NOTA: semantica signal-and-continue: la condizione logica va
	  ricontrollata in un ciclo while (con il monitor di Hoare bastava
	  un semplice if).
	*/

	while(s->testa == s->coda) {

		printf("Schedulatore in attesa di richieste...\n");
		wait_condition(&(s->m), COND_VAR_CONS);
	}


	printf("[Schedulatore] Consumazione in coda: %d\n", s->coda);

	r->posizione = s->coda_richieste[s->coda].posizione;
	r->id_utente = s->coda_richieste[s->coda].id_utente;

	s->coda = (s->coda + 1) % DIMENSIONE_CODA;

	signal_condition(&(s->m), COND_VAR_PROD);


	leave_monitor(&(s->m));
}
