#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define DIM 10
#define ITERAZIONI 4
#define PRODUZIONI 3


typedef struct{
	int indirizzo;
	int dato;
} Buffer;

typedef struct{
	Buffer vettore[DIM];
	int num_elem;
	int coda;
	int testa;
	pthread_mutex_t mutex;
	pthread_cond_t ok_produci;

} GestioneIO;

void Inizializza(GestioneIO * g);
void Produci(GestioneIO * g ,Buffer * b );
int Consuma(GestioneIO * g , Buffer * b );
void Rimuovi(GestioneIO * g);
void * Consumatore ( void * );
void * Produttore ( void * );

