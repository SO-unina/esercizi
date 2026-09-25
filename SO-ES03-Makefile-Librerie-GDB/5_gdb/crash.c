// Registro esami: cerca uno studente per cognome e ne stampa il voto.
// Funziona... finche' non cerchiamo qualcuno che non c'e'.
#include <stdio.h>
#include <string.h>

typedef struct {
	char cognome[32];
	int voto;
} studente_t;

studente_t registro[] = {
	{"Rossi", 28},
	{"Esposito", 30},
	{"Bianchi", 24},
};

#define NUM_STUDENTI (sizeof(registro) / sizeof(registro[0]))

studente_t *cerca(const char *cognome){
	size_t i;

	for(i = 0; i < NUM_STUDENTI; i++){
		if(strcmp(registro[i].cognome, cognome) == 0)
			return &registro[i];
	}
	return NULL;	// non trovato
}

void stampa_voto(const char *cognome){
	studente_t *s = cerca(cognome);
	printf("%s ha preso %d\n", s->cognome, s->voto);
}

int main(void){
	stampa_voto("Esposito");
	stampa_voto("Rossi");
	stampa_voto("Verdi");		// non e' nel registro!
	stampa_voto("Bianchi");
	return 0;
}
