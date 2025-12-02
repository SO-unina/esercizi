#ifndef __HEADER__
#define __HEADER__

#include <pthread.h>

#define NUMERO_BUFFER 3

#define LIBERO 0
#define IN_USO 1
#define OCCUPATO 2


typedef struct {
	int identificativo;
	int quota;
} Volo;

typedef struct {

	Volo vettore_voli[NUMERO_BUFFER];

	int vettore_stato[NUMERO_BUFFER];	// LIBERO, OCCUPATO, I N USO

	int num_liberi;

	pthread_mutex_t mutex;
	pthread_cond_t produttori;

} GestioneVoli;



// Nel caso di passaggio di parametri tramite variabile globale,
// è necessario definire in un header la variabile con la parola chiave "extern",
// e definire la variabile *in uno solo* dei file oggetto
 
//extern GestioneVoli * g;



void InserisciVolo(GestioneVoli * g, int identificativo);
void RimuoviVolo(GestioneVoli * g, int identificativo);
void AggiornaVolo(GestioneVoli * g, int identificativo, int quota);



#endif /* __HEADER__ */




  

