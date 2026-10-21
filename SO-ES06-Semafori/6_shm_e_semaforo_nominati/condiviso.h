/* Nomi delle risorse: la shm contiene SOLO i dati, il semaforo per la
 * cooperazione e' un oggetto nominato a se', fuori dalla struct. */
#define SHM_NAME "/so_es06_caso4"
#define SEM_PRONTO "/so_es06_caso4_pronto"

typedef struct {
    int valore;
} condiviso_t;
