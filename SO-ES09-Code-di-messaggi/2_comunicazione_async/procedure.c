			/*-----IMPLEMENTAZIONE DELLE PROCEDURE-----*/

#include <stdio.h>
#include <string.h>
#include <mqueue.h>
#include "header.h"

void Produttore(mqd_t queue, char *text) {
	Messaggio m;
	// costruzione del messaggio da trasmettere
	strcpy(m.mess, text);
	// invio messaggio (send asincrona: si blocca solo a coda piena;
	// priorita' 0 = ordine FIFO)
	mq_send(queue, (const char *)&m, sizeof(Messaggio), 0);
	printf("MESSAGGIO INVIATO: <%s>\n", m.mess);
}

void Consumatore(mqd_t queue) {
	Messaggio m;
	// ricezione messaggio (bloccante); il buffer deve essere
	// grande almeno mq_msgsize byte
	mq_receive(queue, (char *)&m, sizeof(Messaggio), NULL);
	printf("MESSAGGIO RICEVUTO: <%s>\n", m.mess);
	printMsgInfo(queue);
}

void printMsgInfo(mqd_t queue) {
	struct mq_attr attr;
	// mq_getattr recupera gli attributi correnti della coda,
	// tra cui il numero di messaggi presenti (mq_curmsgs)
	mq_getattr(queue, &attr);
	printf("Messages Number: %ld\n", attr.mq_curmsgs);
}
