#ifndef _HEADER_H_
#define _HEADER_H_

#include <pthread.h>

#define DIMENSIONE 5

typedef struct {
	int vettore[DIMENSIONE];
	int testa;
	int coda;
	int numero_elementi;
	int produttori_in_attesa;

	pthread_mutex_t mutex;
	pthread_cond_t cv_produttori;
	pthread_cond_t cv_consumatori;

} MonitorCoda;

// il valore di ritorno di produzione() indica se il monitor è sovraccarico
int produzione(MonitorCoda *m, int valore);

// il valore di ritorno di consumazione() è il valore prelevato dalla coda
int consumazione(MonitorCoda *m);

void inizializza(MonitorCoda *m);

#endif
