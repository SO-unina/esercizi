#ifndef PRODCONS_MSG
#define PRODCONS_MSG

/* Nome della coda POSIX delle richieste, condivisa tra client e server:
 * sostituisce la coppia (percorso, chiave) delle code System V. */
#define CODA_RICHIESTE "/rpc_prodcons_richieste"

/* Ogni processo client crea una propria coda di risposta, il cui nome
 * contiene il PID: sostituisce la ricezione selettiva con mtype=PID. */
#define CODA_RISPOSTA_FMT "/rpc_prodcons_risp_%d"
#define NOME_CODA_MAX 64

#define MAX_MESSAGGI 10

/* Con le code POSIX non esiste il campo "mtype": la funzione da
 * eseguire e' indicata da un normale campo della struttura. */
typedef struct {
    int tipo_funzione;
    int pid_client;
    int parametro1;
    int parametro2;
    int parametro3;
} richiesta_rpc;

typedef struct {
    int errore;
    int risultato;
} risposta_rpc;

void produci_con_somma(int val1, int val2, int val3);
void produci(int val);
int consuma();

#define TYPE_PRODUCI_CON_SOMMA 1
#define TYPE_PRODUCI 2
#define TYPE_CONSUMA 3

/* Nota: la risposta viene inviata sulla coda dedicata al client,
 * individuata a partire dal campo "pid_client" della richiesta */

#endif
