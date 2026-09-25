//Programma di esempio: somma di due vettori
// vers. 2.0
// File contenente il programma principale
#include <stdio.h>
#include "lib.h"

int main (){
	//vettori
	int a[NMAX],b[NMAX],c[NMAX];
	//riempimento degli array
	int n;

	do {
		//Inserimento riempimento 
		printf("\nInserire il numero degli elementi : ");
		scanf("%d", &n);
	} while (n<1 || n>NMAX); //il riempimento deve essere compreso tra 1 e la cardinalita'
	
	leggi_vettore(a,n);
	leggi_vettore(b,n);

	somma_vettori(a,b,c,n);

	stampa_vettore(c,n);
	
	return 0;
}
