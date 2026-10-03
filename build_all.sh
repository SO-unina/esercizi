#!/bin/sh
# Compila tutte le esercitazioni (esclude le varianti *_TODO, volutamente incomplete).
# Prosegue anche in caso di errore e riporta un riepilogo finale.
fail=0
failed=""
for mf in $(find . -name Makefile -type f ! -path '*_TODO/*' ! -path './git/*' | sort); do
    dir=$(dirname "$mf")
    echo "==> make -C $dir"
    if ! make -C "$dir" all 2>&1 && ! make -C "$dir" 2>&1; then
        fail=$((fail+1))
        failed="$failed\n  $dir"
    fi
done
if [ "$fail" -gt 0 ]; then
    printf "\nCartelle con errori di compilazione (%d):%b\n" "$fail" "$failed"
    exit 1
fi
echo "Compilazione completata senza errori."
