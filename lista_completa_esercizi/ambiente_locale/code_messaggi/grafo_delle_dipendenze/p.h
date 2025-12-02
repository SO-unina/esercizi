#ifndef _P_H_
#define _P_H_

#define P1 1
#define P2 2
#define P3 3
#define P4 4
#define P5 5
#define P6 6

#define FTOK_PATH_Q_OP_1  "."
#define FTOK_PATH_Q_RIS_1 "."
#define FTOK_PATH_Q_OP_2  "."
#define FTOK_PATH_Q_RIS_2 "."

#define FTOK_CHAR_Q_OP_1  'a'
#define FTOK_CHAR_Q_RIS_1 'b'
#define FTOK_CHAR_Q_OP_2  'c'
#define FTOK_CHAR_Q_RIS_2 'd'

struct msg_operandi {
	long processo;
	int operandi[4];
};

struct msg_risposta {
	long processo;
	int risposta;
};

#endif // _P_H_
