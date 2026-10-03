#!/bin/sh
set -eu
find . -name Makefile -type f ! -path './git/*' -print | while IFS= read -r mf; do
    make -C "$(dirname "$mf")" clean >/dev/null 2>&1 || true
done
echo "Pulizia completata."
