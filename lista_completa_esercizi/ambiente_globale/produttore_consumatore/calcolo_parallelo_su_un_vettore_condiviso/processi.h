#ifndef _PROCESSI_H_
#define _PROCESSI_H_

int inizializza_semafori();

void figlio(int *vettore,
            int *buffer,
            int sem_id,
            int elemento_iniziale,
            int qta_elementi);

void padre(int *buffer,
           int sem_id);

#define NUM_PROCESSI 10
#define NUM_ELEMENTI 10000

#endif /* _PROCESSI_H_ */