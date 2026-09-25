// Chiama n volte (default: un miliardo) una funzione della libreria libconta
// Uso: ./ciclo_statico [n]    oppure    ./ciclo_dinamico [n]
#include <stdio.h>
#include <stdlib.h>
#include "conta.h"

int main(int argc, char** argv){
	long n = (argc > 1) ? atol(argv[1]) : 1000000000L;
	long tot = 0;
	long i;

	for(i = 0; i < n; i++)
		tot = incrementa(tot);

	printf("%ld chiamate a incrementa()\n", tot);
	return 0;
}
