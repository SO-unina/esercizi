#ifndef __HEADER__
#define __HEADER__

#include <pthread.h>

#define NUMERO_BUFFER 5

#define LIBERO 0
#define IN_USO 1
#define OCCUPATO 2


typedef struct {
	int operandi[4];	// il buffer contiene un array di operandi (da 2 a 4)
	int totale_operandi;	// il numero di operandi presenti nell'array
} buffer;

typedef struct {

	buffer elaborazioni[NUMERO_BUFFER];
	int stato[NUMERO_BUFFER];	// 0 = LIBERO, 1 = IN USO, 2 = OCCUPATO

	int num_occupati;
	int num_liberi;

	pthread_mutex_t mutex;
	pthread_cond_t produttori;
	pthread_cond_t consumatori;

} MonitorElaborazioni;

void inizializza(MonitorElaborazioni * m);
void distruggi(MonitorElaborazioni * m);
void produci_richiesta(MonitorElaborazioni * m, buffer * b);
int  consuma_richiesta(MonitorElaborazioni * m, buffer * b);

#endif /* __HEADER__ */
