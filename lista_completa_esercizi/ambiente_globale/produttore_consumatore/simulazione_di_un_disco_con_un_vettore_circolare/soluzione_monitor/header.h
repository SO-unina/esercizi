#ifndef __HEADER__
#define __HEADER__

#include "monitor.h"

#define TOTALE_UTENTI 5
#define TOTALE_RICHIESTE 5
#define TOTALE_POSIZIONI 20
#define DIMENSIONE_CODA 10

#define COND_VAR_CONS 0
#define COND_VAR_PROD 1


/*
  Struttura dati che rappresenta una singola richiesta
  verso il disco
*/

typedef struct {
	unsigned int posizione;
	int id_utente;
} richiesta;



/*
  Struttura dati che rappresenta la coda di richieste,
  in cui gli utenti collocheranno le richieste, e lo
  schedulatore preleverà le richieste.

  Contiene un array di richieste (di dimensione fissa),
  un puntatore testa e un puntatore coda, e una struttura
  dati di appoggio per il monitor.
*/

typedef struct {

	richiesta coda_richieste[DIMENSIONE_CODA];

	int testa;
	int coda;

	Monitor m;

} MonitorSchedulatore;



/*
  Argomenti passati ad ogni thread: identificativo del thread
  e puntatore alla struttura condivisa
*/

typedef struct {
	int id;
	MonitorSchedulatore * s;
} ArgomentiThread;


void InizializzaMonitor(MonitorSchedulatore * s);
void RimuoviMonitor(MonitorSchedulatore * s);
void InserisciRichiesta(MonitorSchedulatore * s, int id, richiesta * r);
void PrelevaRichiesta(MonitorSchedulatore * s, richiesta * r);

void * Utente(void *);
void * Schedulatore(void *);


#endif /* __HEADER__ */
