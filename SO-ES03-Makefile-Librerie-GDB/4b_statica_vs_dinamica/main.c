#include <stdio.h>
#include "mat.h"

int main(int argc, char** argv){
	int v[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int n = sizeof(v) / sizeof(v[0]);

	printf("[%s] libmat %s: somma(1..10) = %d (atteso 55)\n",
	       argv[0], mat_versione(), somma(v, n));
	return 0;
}
