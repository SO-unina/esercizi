#ifndef _SENSORE_H_
#define _SENSORE_H_

typedef struct {

    long tipo;
    int valore;

} messaggio;

#define TIPO 1

void sensore(int id_coda_sensore);

#endif