#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <mqueue.h>

#include "header.h"


int main() {

	printf("Processo P1 avviato\n");
	struct msg_calc m_p1;
	m_p1.processo = P1;

	mqd_t id_queue = apri_coda();

	srand(time(NULL));


	int i;
	for (i = 0; i < 11; i++) {
		m_p1.numero = generaFloat(0, 10);
		printf("Invio messaggio: <%lu,%f>\n", m_p1.processo, m_p1.numero);
		mq_send(id_queue, (const char *)&m_p1, sizeof(struct msg_calc), 0);
		sleep(1);
	}

	mq_close(id_queue);
	return 0;
}
