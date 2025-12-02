#ifndef PRODCONS_MSG
#define PRODCONS_MSG

typedef struct {
    long type;
    int pid_client;
    int parametro1;
    int parametro2;
    int parametro3;
} richiesta_rpc;

typedef struct {
    long type;
    int errore;
    int risultato;
} risposta_rpc;

void produci_con_somma(int val1, int val2, int val3);
void produci(int val);
int consuma();

#define TYPE_PRODUCI_CON_SOMMA 1
#define TYPE_PRODUCI 2
#define TYPE_CONSUMA 3

/* Nota: nei messaggi di risposta, usare come valore di "type" il PID del client */

#endif