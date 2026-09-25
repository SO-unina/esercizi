// Calcola la media dei voti di uno studente.
// Il programma compila senza warning... ma la media e' sbagliata!
// Usate gdb per scoprire perche'.
#include <stdio.h>

#define NUM_ESAMI 5

double calcola_media(const int voti[], int n){
	int somma = 0;
	int i;

	for(i = 1; i < n; i++){
		somma += voti[i];
	}

	double media = somma / n;
	return media;
}

int main(void){
	int voti[NUM_ESAMI] = {30, 18, 27, 24, 28};

	double media = calcola_media(voti, NUM_ESAMI);

	printf("Media dei voti: %.2f\n", media);	// dovrebbe essere 25.40
	return 0;
}
