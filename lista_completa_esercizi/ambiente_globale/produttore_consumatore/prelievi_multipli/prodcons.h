#ifndef _PRODCONS_H_
#define _PRODCONS_H_

#include "monitor_hoare.h"

#define DIM 10

typedef struct {
    Monitor m;
    int vettore[DIM];
    int testa;
    int coda;
    int totale_elementi;
} ProdCons;

#define SPAZIO_DISP 0
#define MESS_DISP 1

void inizializza(ProdCons * p);
void consuma(ProdCons * p, int* val_1, int* val_2);
void produci(ProdCons * p, int val);
void rimuovi(ProdCons * p);

#endif
