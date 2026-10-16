#ifndef _P_H_
#define _P_H_

#include <mqueue.h>

#define P1 1
#define P2 2

#define QUEUE_NAME "/so_es08_calc"

struct msg_calc {
	long processo;
	float numero;
};

#endif // _P_H_
