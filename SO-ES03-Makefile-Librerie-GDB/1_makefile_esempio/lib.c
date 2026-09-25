//Libreria per la somma di due vettori

#include <stdio.h>
#include "lib.h"

//Definizione funzione leggi_vettore
void leggi_vettore(int a[], const int n) {
	int i;

	printf("LEGGI VETTORE....\n");
	for (i=0; i<n; i++) {
		printf("Inserire elemento n.ro %d: ", i);
		scanf("%d", &a[i]);
	}
}

//Definizione funzione stampa_vettore
void stampa_vettore(const int a[], const int n) {
	int i;
	printf("STAMPA VETTORE....\n");
	for (i=0;i<n;i++) 
		printf("Elemento n.ro %d => %d\n", i, a[i]);
}

//Definizione funzione somma_vettore
void somma_vettori(const int a[], const int b[], int c[], const int n) {
	int i;
	printf("SOMMA VETTORI....\n");
	for (i=0;i<n;i++) 
		c[i] = a[i] + b[i];
}

