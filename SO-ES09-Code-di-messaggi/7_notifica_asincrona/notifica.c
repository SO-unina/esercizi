/*********NOTIFICA ASINCRONA CON mq_notify (SIGEV_THREAD)*********/
/* Il processo padre crea la coda, si registra con mq_notify e poi
   "lavora" senza mai chiamare una mq_receive bloccante.
   Il processo figlio (mittente) invia qualche messaggio a intervalli.
   Ogni volta che un messaggio arriva nella coda VUOTA, la libreria C
   crea un nuovo thread che esegue la funzione gestore_notifica().

   Riferimenti: man 3 mq_notify, man 7 mq_overview, man sigevent
*/

#define _GNU_SOURCE		// per gettid() (identificativo del thread)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>		// struct sigevent, SIGEV_THREAD
#include <mqueue.h>
#include <sys/wait.h>

#define QUEUE_NAME "/so_es08_notify"
#define MAX_MESSAGGI 10

typedef struct {
	char testo[40];
} Messaggio;

static void registra_notifica(mqd_t *queue);


// Eseguita in un thread NUOVO a ogni notifica. L'argomento e' il valore
// messo in sigev_value al momento della registrazione (qui: &queue).
static void gestore_notifica(union sigval sv) {
	mqd_t *queue = sv.sival_ptr;
	Messaggio m;

	printf("   [notifica, thread %d] la coda non e' piu' vuota\n", gettid());

	// 1) la registrazione vale per UNA sola notifica: si rinnova subito,
	//    PRIMA di svuotare la coda (vedi README)
	registra_notifica(queue);

	// 2) si svuota la coda: e' aperta con O_NONBLOCK, quindi mq_receive
	//    non si blocca e, quando la coda e' vuota, restituisce -1 con
	//    errno == EAGAIN
	while (1) {
		if (mq_receive(*queue, (char *)&m, sizeof(Messaggio), NULL) == -1) {
			if (errno != EAGAIN) {		// EAGAIN = coda vuota, non e' un errore
				perror("mq_receive");
			}
			break;
		}
		printf("   [notifica, thread %d] ricevuto <%s>\n", gettid(), m.testo);
	}
}


// Registra il processo per la prossima notifica sulla coda
static void registra_notifica(mqd_t *queue) {
	struct sigevent sev;
	memset(&sev, 0, sizeof(sev));

	sev.sigev_notify = SIGEV_THREAD;			// notifica = funzione eseguita in un nuovo thread
	sev.sigev_notify_function = gestore_notifica;	// la funzione da eseguire
	sev.sigev_notify_attributes = NULL;		// attributi del thread: quelli predefiniti
	sev.sigev_value.sival_ptr = queue;		// argomento passato alla funzione

	if (mq_notify(*queue, &sev) == -1) {
		perror("mq_notify");
		exit(1);
	}
}


// Mittente: attende 'attesa' secondi e poi invia il testo
static void invia(mqd_t queue, int attesa, char *testo) {
	Messaggio m;
	sleep(attesa);
	strcpy(m.testo, testo);
	printf("[mittente] invio <%s>\n", m.testo);
	mq_send(queue, (const char *)&m, sizeof(Messaggio), 0);
}


int main() {

	pid_t pid;
	int i;
	struct mq_attr attr;

	attr.mq_flags = 0;
	attr.mq_maxmsg = MAX_MESSAGGI;
	attr.mq_msgsize = sizeof(Messaggio);
	attr.mq_curmsgs = 0;

	// rimuove un'eventuale coda rimasta da esecuzioni precedenti
	mq_unlink(QUEUE_NAME);

	// O_NONBLOCK: le mq_receive del gestore non si bloccano mai
	mqd_t queue = mq_open(QUEUE_NAME, O_RDONLY | O_CREAT | O_EXCL | O_NONBLOCK, 0600, &attr);
	if (queue == (mqd_t)-1) {
		perror("errore mq_open");
		exit(1);
	}

	pid = fork();
	if (pid == 0) {		// figlio: mittente
		// apre la coda per nome, in sola scrittura e in modalita' bloccante
		mqd_t q = mq_open(QUEUE_NAME, O_WRONLY);
		if (q == (mqd_t)-1) {
			perror("errore mq_open (mittente)");
			exit(1);
		}
		invia(q, 2, "primo");
		invia(q, 2, "secondo");
		invia(q, 0, "terzo");		// subito dopo il secondo
		invia(q, 2, "quarto");
		mq_close(q);
		exit(0);
	}

	// padre: si registra e poi fa altro, senza mai bloccarsi sulla coda
	registra_notifica(&queue);
	printf("[main, thread %d] registrato con mq_notify, ora lavoro\n", gettid());

	for (i = 1; i <= 8; i++) {
		printf("[main] lavoro... (%d/8)\n", i);
		sleep(1);
	}

	wait(NULL);

	// cancella la registrazione (sevp == NULL)
	mq_notify(queue, NULL);

	mq_getattr(queue, &attr);
	printf("[main] messaggi rimasti in coda: %ld\n", attr.mq_curmsgs);

	mq_close(queue);
	mq_unlink(QUEUE_NAME);
	return 0;
}
