#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <mqueue.h>

#include "header.h"

// Restituisce un float casuale nell'intervallo [i_sx, i_dx]
float generaFloat(int i_sx, int i_dx) {
	// (float)rand() / RAND_MAX e' compreso tra 0 e 1
	float x = i_sx + (float)rand() / RAND_MAX * (i_dx - i_sx);
	return x;
}

mqd_t apri_coda(void) {
	struct mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = MAX_MESSAGGI;
	attr.mq_msgsize = sizeof(struct msg_calc);
	attr.mq_curmsgs = 0;

	/* O_CREAT senza O_EXCL: il primo processo crea la coda,
	 * gli altri la riutilizzano (stesso nome). */
	mqd_t q = mq_open(QUEUE_NAME, O_RDWR | O_CREAT, 0644, &attr);
	if (q == (mqd_t)-1) {
		perror("mq_open fallita");
		exit(1);
	}
	return q;
}
