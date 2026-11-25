#ifndef _HEADER_H_
#define _HEADER_H_

#include <mqueue.h>

#define P1 1
#define P2 2

/* Nome della coda POSIX condivisa tra gli eseguibili: tutti i
 * programmi che aprono lo stesso nome accedono alla stessa coda. */
#define QUEUE_NAME "/so_es08_calc"

#define MAX_MESSAGGI 10

/* Con POSIX l'identificativo del processo mittente non e' un campo
 * "tipo" speciale, ma un normale campo del messaggio. */
struct msg_calc {
	long processo;
	float numero;
};

//Genera un valore float nell'intervallo [i_sx,i_dx]
float generaFloat(int i_sx, int i_dx);

//Apre (creandola se necessario) la coda condivisa
mqd_t apri_coda(void);

#endif // _HEADER_H_
