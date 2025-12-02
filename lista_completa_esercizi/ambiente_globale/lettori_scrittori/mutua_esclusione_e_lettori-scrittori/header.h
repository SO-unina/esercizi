#ifndef __HEADER
#define __HEADER
#define PATH "."
#define ID 'S'
#define ID_SEM 'P'

#define DATA_1 "14/02/2013"
#define DATA_2 "05/03/2013"
#define DATA_3 "02/04/2013"

#define MUTEX 0
#define APPELLO 1
#define PRENOTATI 2

typedef struct tipo_esame{
	char prossimo_appello[20];
	int numero_prenotati;
	//Variabili di sincronizzazione
	int numero_lettori;
}esame_t;

void inizio_lettura(int sem,esame_t* esame);
void fine_lettura(int sem,esame_t* esame);
void inizio_scrittura(int sem);
void fine_scrittura(int sem);
void accedi_prenotati(int sem);
void lascia_prenotati(int sem);

#endif
