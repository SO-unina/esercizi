#ifndef __HEADER_H
#define __HEADER_H

#include "monitor.h"

#define NUMF 10
#define NUMC 10
#define NUMTHREAD 20
#define NELEM 100
#define NUMVAR 2
#define SPAZIO_D 0
#define MERCE_D 1
#define LIBERO 0
#define OCCUPATO 1
#define IN_USO 2


typedef struct
{
	unsigned int id_fornitore;
	unsigned int stato;
}Scaffale;


typedef Scaffale Magazzino[NELEM];


/* argomenti passati ad ogni thread: identificativo del thread
   e puntatori al monitor e alle variabili condivise */
typedef struct
{
	int id;
	Monitor* M;
	int* livello_scorte;
	int* scaffali_liberi;
	Magazzino* magazzino;
}ArgomentiThread;


void* Fornitore(void*);
void* Cliente(void*);

#endif
