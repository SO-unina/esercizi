#ifndef PRODCONS_CLIENT
#define PRODCONS_CLIENT

/* Da chiamare nel processo che effettua le chiamate RPC, dopo la fork:
 * apre la coda delle richieste e crea la coda di risposta del processo
 * (il cui nome contiene il PID). */
void init_client(void);

/* Chiude la coda delle richieste e chiude/rimuove la coda di risposta */
void fini_client(void);

#endif
