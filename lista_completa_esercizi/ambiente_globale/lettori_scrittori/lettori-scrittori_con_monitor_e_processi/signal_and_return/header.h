#include "monitor_signal_return.h"

#define NUM_UTENTI 10

typedef struct {
	int temperatura;
	unsigned int umidita;
	unsigned short pioggia;
} meteo;

typedef struct {
	meteo meteo;

	int numlettori;
	int numscrittori;

	Monitor m;

} MonitorMeteo;

#define NUM_CONDITIONS 2
#define CV_LETT 0
#define CV_SCRITT 1

void Servizio(MonitorMeteo* p);
void Utente(MonitorMeteo* p);
