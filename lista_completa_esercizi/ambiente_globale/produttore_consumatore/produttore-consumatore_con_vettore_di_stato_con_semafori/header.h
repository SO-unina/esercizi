#ifndef __TEATRO__
#define __TEATRO__

typedef struct {
	unsigned int id_cliente;
	unsigned int stato;
} posto;

#define POSTI 80

#define CLIENTI 50

#define LIBERO 0
#define OCCUPATO 1
#define INAGGIORNAMENTO 2

#define MUTEX 0

posto * posti;
int * disponibilita;

#endif // __TEATRO__
