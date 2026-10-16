			/*-----IMPLEMENTAZIONE DELLE PROCEDURE-----*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <mqueue.h>
#include "header.h"

static mqd_t queue_rts; //statiche-->non e' necessaria la loro visibilita' fuori dal modulo
static mqd_t queue_ots;

static mqd_t apri_coda(const char *name) {
	struct mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = MAX_MESSAGGI;
	attr.mq_msgsize = sizeof(Messaggio);
	attr.mq_curmsgs = 0;

	mqd_t q = mq_open(name, O_RDWR | O_CREAT, 0664, &attr);
	if (q == (mqd_t)-1) {
		perror("mq_open");
		exit(1);
	}
	return q;
}

// inizializzazione code di servizio
void initServiceQueues(void) {
	queue_rts = apri_coda(QUEUE_RTS);
	queue_ots = apri_coda(QUEUE_OTS);
}

// rimozione code di servizio
void removeServiceQueues(void) {
	mq_close(queue_rts);
	mq_close(queue_ots);
	mq_unlink(QUEUE_RTS);
	mq_unlink(QUEUE_OTS);
}

// Send Sincrona: la mq_send POSIX e' asincrona, quindi la
// sincronia si ottiene con il protocollo di rendezvous
// (RTS -> OTS -> messaggio) su due code di servizio
void SendSincr(Messaggio *m, mqd_t queue) {
	Messaggio m1, m2;
	// costruzione messaggio RTS
	strcpy(m1.mess, "Richiesta di invio");
	// invio messaggio RTS
	mq_send(queue_rts, (const char *)&m1, sizeof(Messaggio), 0);
	// ricezione OTS (bloccante: attende che il ricevente sia pronto)
	mq_receive(queue_ots, (char *)&m2, sizeof(Messaggio), NULL);
	// invio messaggio
	mq_send(queue, (const char *)m, sizeof(Messaggio), 0);
}

// Receive Bloccante
void ReceiveBloc(Messaggio *m, mqd_t queue) {
	Messaggio m1, m2;
	// ricezione messaggio RTS
	mq_receive(queue_rts, (char *)&m1, sizeof(Messaggio), NULL);
	// costruzione messaggio OTS
	strcpy(m2.mess, "Ready to send");
	// invio messaggio OTS
	mq_send(queue_ots, (const char *)&m2, sizeof(Messaggio), 0);
	// ricezione messaggio
	mq_receive(queue, (char *)m, sizeof(Messaggio), NULL);
}

void Produttore(mqd_t queue, char *text) {
	Messaggio m;
	// costruzione del messaggio da trasmettere
	strcpy(m.mess, text);
	// invio messaggio
	SendSincr(&m, queue);
	printf("MESSAGGIO INVIATO: <%s>\n", m.mess);
}

void Consumatore(mqd_t queue) {
	Messaggio m;
	// ricezione messaggio
	ReceiveBloc(&m, queue);
	printf("MESSAGGIO RICEVUTO: <%s>\n", m.mess);
}
