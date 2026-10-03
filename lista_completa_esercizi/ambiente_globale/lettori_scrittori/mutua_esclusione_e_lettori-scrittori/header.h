#ifndef __HEADER
#define __HEADER

#include <semaphore.h>

/* Nome dell'oggetto di shared memory POSIX condiviso tra i tre
 * eseguibili (main, docente, studente). I nomi POSIX iniziano
 * con '/' e sono visibili in /dev/shm. */
#define SHM_NAME "/esami_appello"

#define DATA_1 "14/02/2013"
#define DATA_2 "05/03/2013"
#define DATA_3 "02/04/2013"

/* Tutto lo stato condiviso (dati + semafori) vive nella shared memory:
 * i semafori sono anonimi e process-shared (sem_init con pshared=1). */
typedef struct tipo_esame{
	char prossimo_appello[20];
	int numero_prenotati;
	//Variabili di sincronizzazione
	int numero_lettori;
	sem_t mutex;      /* protegge numero_lettori */
	sem_t appello;    /* lettori/scrittori su prossimo_appello */
	sem_t prenotati;  /* mutua esclusione su numero_prenotati */
}esame_t;

void inizio_lettura(esame_t* esame);
void fine_lettura(esame_t* esame);
void inizio_scrittura(esame_t* esame);
void fine_scrittura(esame_t* esame);
void accedi_prenotati(esame_t* esame);
void lascia_prenotati(esame_t* esame);

#endif
