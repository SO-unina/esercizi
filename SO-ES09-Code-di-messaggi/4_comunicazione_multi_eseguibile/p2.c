#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <mqueue.h>

#include "header.h"


int main() {

	printf("Processo P2 avviato\n");
	struct msg_calc m_p2;
	m_p2.processo = P2;

	mqd_t id_queue = apri_coda();

	srand(time(NULL));


	int i;
	for (i = 0; i < 11; i++) {
		m_p2.numero = generaFloat(2, 20);
		printf("Invio messaggio: <%lu,%f>\n", m_p2.processo, m_p2.numero);
		mq_send(id_queue, (const char *)&m_p2, sizeof(struct msg_calc), 0);
		sleep(2);
	}

	mq_close(id_queue);
	return 0;
}
