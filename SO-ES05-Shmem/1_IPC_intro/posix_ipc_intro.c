#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define SHM_NAME "/so_es05_intro"

int main(void) {
    const size_t size = 4096;

    /* O_EXCL evita di riutilizzare per errore un oggetto rimasto nel sistema. */
    int fd = shm_open(SHM_NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd == -1) {
        if (errno == EEXIST) {
            printf("La shared memory %s esiste gia'. " "Rimuoverla con shm_unlink.\n", SHM_NAME);
        }
        perror("shm_open");
        exit(1);
    }

    /* Un nuovo oggetto POSIX shared memory ha inizialmente dimensione zero. */
    if (ftruncate(fd, size) == -1) {
        perror("ftruncate");
        exit(1);
    }

    /* MAP_SHARED rende le modifiche visibili agli altri processi che mappano l'oggetto. */
    char *area = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (area == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }

    sprintf(area, "Messaggio scritto nella shared memory POSIX dal PID %d", getpid());
    printf("%s\n", area);

    if (munmap(area, size) == -1) {
        perror("munmap");
        exit(1);
    }
    if (close(fd) == -1) {
        perror("close");
        exit(1);
    }

    /* shm_unlink rimuove il nome dell'oggetto quando non serve piu'. */
    if (shm_unlink(SHM_NAME) == -1) {
        perror("shm_unlink");
        exit(1);
    }

    return 0;
}
