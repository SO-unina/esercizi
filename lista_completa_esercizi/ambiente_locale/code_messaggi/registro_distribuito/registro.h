#ifndef _REGISTRO_H_
#define _REGISTRO_H_

typedef struct {
    long tipo;
    int id_server;
    int id_coda;
} messaggio_registro;

typedef struct {
    long tipo;
    int valore;
} messaggio_server;

#define BIND 1
#define QUERY 2
#define SERVICE 3
#define EXIT 4


void client(int id_coda_registro_richieste, int id_coda_registro_risposte);
void registro(int id_coda_registro, int id_coda_registro_risposte);
void server(int id_coda_registro, int id_coda_registro_risposte, int id_server);

#endif
