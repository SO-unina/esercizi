#include <sys/types.h>

#define RICHIESTE 5
#define CLIENT 3

typedef struct{
	long pid;
	int v1;
	int v2;
} msg_richiesta;

typedef struct{
	long pid;
	int v3;
} msg_risposta;

void client(int id_c, int id_s);
void server(int id_c, int id_s);
void* Prodotto(void*);
