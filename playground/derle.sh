#!/usr/bin/env bash
# Yorumlayıcıyı tarayıcıda çalışacak biçimde WebAssembly'ye derler.
# Emscripten (emcc) kurulu olmalıdır: https://emscripten.org
#
# Kullanım: bash playground/derle.sh
# Çıktı: playground/jus.js ve playground/jus.wasm
set -e

KOK="$(cd "$(dirname "$0")/.." && pwd)"

emcc -O2 -std=c99 "$KOK"/src/*.c -o "$KOK/playground/jus.js" \
    -sMODULARIZE=1 \
    -sEXPORT_NAME=createJus \
    -sEXPORTED_RUNTIME_METHODS=FS,callMain \
    -sINVOKE_RUN=0 \
    -sEXIT_RUNTIME=1 \
    -sALLOW_MEMORY_GROWTH=1 \
    -sSTACK_SIZE=1048576

echo "playground/jus.js ve playground/jus.wasm üretildi."
