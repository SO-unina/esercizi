// Dal sorgente all'eseguibile: le fasi della "catena" di compilazione
//   preprocessore -> compilatore -> assemblatore -> linker
#include <stdio.h>

#define ANNO 2026
#define QUADRATO(x) ((x) * (x))
#define QUADRATO_SBAGLIATO(x) x * x      // senza parentesi

#ifndef SALUTO
#define SALUTO "Ciao"
#endif

// __x86_64__ e __aarch64__ sono macro predefinite da gcc:
// dipendono dalla macchina per cui si sta compilando
#if defined(__x86_64__)
#define ARCHITETTURA "x86-64"
#elif defined(__aarch64__)
#define ARCHITETTURA "ARM a 64 bit"
#else
#define ARCHITETTURA "sconosciuta"
#endif

#if !defined(__linux__)
#error "Questo esempio e' pensato per Linux!"
#endif

int main(void)
{
#ifdef MACCHINA
	printf("Ciao, macchina! Sono un programma per CPU %s, compilato con gcc %d.%d il %s\n",
	       ARCHITETTURA, __GNUC__, __GNUC_MINOR__, __DATE__);
#else
	printf("%s! Benvenuti al corso di Sistemi Operativi %d\n", SALUTO, ANNO);
#endif

	printf("QUADRATO(1+2)           = %d\n", QUADRATO(1+2));
	printf("QUADRATO_SBAGLIATO(1+2) = %d\n", QUADRATO_SBAGLIATO(1+2));

	return 0;
}
