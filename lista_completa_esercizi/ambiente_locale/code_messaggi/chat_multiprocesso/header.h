#ifndef __HEADER__
#define __HEADER__

// Path per la FTOK
#define FTOK_PATH "."

// Tipo per il messaggio
#define TYPE 1

// Struct raltiva ai messaggi
struct mesg {
	long msgType;		// tipo messaggio
	char message[20];	// messaggio effettivo
};

void Sender(int, int);
void Receiver(int);

#endif // __HEADER__
