#!/usr/bin/env bash
# Yorumlayıcıyı bellek denetimiyle (ASan + UBSan) ve çöp toplayıcı her bellek
# ayırmada çalışacak biçimde derleyip test paketini çalıştırır.
#
# Linux ya da macOS'ta doğrudan çalışır. Windows'ta bir Linux kabı içinde
# çalıştırılabilir, örneğin:
#   docker run --rm -v "$PWD":/jus -w /jus <gcc içeren imaj> bash tests/bellek-denetimi.sh
set -e

KOK="$(cd "$(dirname "$0")/.." && pwd)"
CIKTI="$(mktemp -d)"
trap 'rm -rf "$CIKTI"' EXIT

"${CC:-gcc}" -std=c99 -Wall -Wextra -pedantic -g \
    -fsanitize=address,undefined -fno-sanitize-recover=all \
    -DJUS_DEBUG_STRESS_GC \
    "$KOK"/src/*.c -o "$CIKTI/jus" -lm $(if [ "$(uname)" = Darwin ]; then echo "-framework Security -framework CoreFoundation"; else echo "-ldl"; fi)
cd "$KOK"

bash "$KOK/tests/calistir.sh" "$CIKTI/jus"
bash "$KOK/tests/araclar.sh" "$CIKTI/jus"
