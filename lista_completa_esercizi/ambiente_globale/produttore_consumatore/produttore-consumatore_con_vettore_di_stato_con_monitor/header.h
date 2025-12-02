#define NUMF 10
#define NUMC 10
#define NUMPROC 20
#define NELEM 100
#define NUMVAR 2
#define SPAZIO_D 0
#define MERCE_D 1
#define LIBERO 0
#define OCCUPATO 1
#define IN_USO 2


typedef struct
{
	unsigned int id_fornitore;
	unsigned int stato;
}Scaffale;


typedef Scaffale Magazzino[NELEM];

void Fornitore(Monitor*, int*, int*, Magazzino*);
void Cliente(Monitor*, int*, int*, Magazzino*);
