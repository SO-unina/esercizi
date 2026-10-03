#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>

#include "header.h"


int main() {

	printf("Processo P3 avviato\n");
	struct msg_calc m_r;
	float media_cum[2] = {0.0f, 0.0f};

	mqd_t id_queue = apri_coda();

	printf("ID QUEUE: %d\n", (int)id_queue);


	int i;
	for (i = 0; i < 22; i++) {
		/* Ricezione bloccante: il buffer ha esattamente la
		 * dimensione mq_msgsize fissata alla creazione. */
		mq_receive(id_queue, (char *)&m_r, sizeof(struct msg_calc), NULL);
		printf("Ricevuto messaggio dal processo <%lu> ,con valore <%f>\n", m_r.processo, m_r.numero);
		if (m_r.processo == P1) {
			media_cum[P1 - 1] += m_r.numero / 11;
		} else if (m_r.processo == P2) {
			media_cum[P2 - 1] += m_r.numero / 11;
		} else {
			printf("Processo non riconosciuto\n");
		}
	}

	for (i = 0; i < 2; i++) {
		printf("<Media %d = %f>\n", i + 1, media_cum[i]);
	}

	mq_close(id_queue);
	return 0;
}
