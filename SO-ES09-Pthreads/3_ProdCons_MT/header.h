			/*-----HEADER FILE-----*/
#include <pthread.h>

#ifndef HEADER_H
#define HEADER_H

/*messaggio*/
typedef long msg;


struct ProdCons {

	/* mutex del monitor */
	pthread_mutex_t mutex;

	/* condition variables del monitor */
	pthread_cond_t ok_prod_cv;
	pthread_cond_t ok_cons_cv;

	/* variabili di utilità per la sincronizzazione */
	int ok_produzione;
	int ok_consumo;

	/* buffer condiviso */
	msg mess;
};


void *Consumatore(void *);
void *Produttore(void *);

void Produci(struct ProdCons *, msg m);
msg Consuma(struct ProdCons *);

void InizioConsumo(struct ProdCons *);
void InizioProduzione(struct ProdCons *);
void FineConsumo(struct ProdCons *);
void FineProduzione(struct ProdCons *);

#endif
