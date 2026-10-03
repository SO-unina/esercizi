#!/bin/bash
# Rimuove le risorse IPC POSIX dell'utente corrente rimaste nel sistema
# (l'equivalente POSIX del vecchio ipcrm per le risorse System V).
#
# Le risorse POSIX sono visibili come file:
#   - shared memory:      /dev/shm/<nome>
#   - semafori nominati:  /dev/shm/sem.<nome>
#   - code di messaggi:   /dev/mqueue/<nome>

for f in /dev/shm/*; do
    [ -e "$f" ] || continue
    [ -O "$f" ] && rm -f -- "$f" && echo "Rimosso: $f"
done
echo "Removed POSIX shared memories and named semaphores"

if [ -d /dev/mqueue ]; then
    for f in /dev/mqueue/*; do
        [ -e "$f" ] || continue
        [ -O "$f" ] && rm -f -- "$f" && echo "Rimosso: $f"
    done
    echo "Removed POSIX message queues"
fi
