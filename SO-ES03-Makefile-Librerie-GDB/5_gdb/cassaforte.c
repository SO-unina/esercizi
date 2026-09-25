// La cassaforte del professore.
// Per aprirla serve un PIN... che non conoscete.
// Riuscite ad aprirla usando solo gdb?
#include <stdio.h>

// Il PIN viene calcolato a runtime: non compare "in chiaro" nell'eseguibile
int genera_pin(void){
	int pin = 0;
	int i;

	for(i = 1; i <= 7; i++)
		pin = (pin * 31 + i * i) % 10000;
	return pin;
}

int verifica(int tentativo){
	int pin = genera_pin();

	return tentativo == pin;
}

void apri_cassaforte(void){
	printf("\n*** CASSAFORTE APERTA! ***\n");
	printf("Dentro c'e' un biglietto: \"Complimenti, ora sai usare gdb!\"\n\n");
}

int main(void){
	int tentativo;

	printf("Inserisci il PIN della cassaforte: ");
	if(scanf("%d", &tentativo) != 1)
		return 1;

	if(verifica(tentativo))
		apri_cassaforte();
	else
		printf("PIN errato. La cassaforte resta chiusa.\n");

	return 0;
}
