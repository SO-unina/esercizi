#ifndef SHARED_DATA_H
#define SHARED_DATA_H

#define SHM_NAME "/so_es05_shared_data"
#define TEXT_SIZE 256

typedef struct {
    int value;
    char text[TEXT_SIZE];
} shared_data_t;

#endif
