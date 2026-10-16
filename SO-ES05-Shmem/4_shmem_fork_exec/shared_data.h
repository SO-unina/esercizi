#ifndef SHARED_DATA_H
#define SHARED_DATA_H

/* Nome dell'oggetto di shared memory condiviso tra gli eseguibili */
#define SHM_NAME "/so_es05_fork_exec"
#define TEXT_SIZE 256

typedef struct {
    int value;
    char text[TEXT_SIZE];
} shared_data_t;

#endif
