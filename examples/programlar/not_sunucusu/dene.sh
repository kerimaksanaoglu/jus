#!/usr/bin/env bash
# Not sunucusunu başlatır, istemci testlerini çalıştırır ve sunucunun düzgün
# kapandığını denetler.
#
# Kullanım: dene.sh [jus-yolu] [port]

JUS="${1:-./jus}"
case "$JUS" in
    /*|?:*) ;;
    *) JUS="$PWD/$JUS" ;;
esac
PORT="${2:-18473}"
cd "$(dirname "$0")" || exit 1

gunluk="$(mktemp)"
trap 'rm -f "$gunluk"' EXIT

"$JUS" sunucu.jus "$PORT" > "$gunluk" 2>&1 &
sunucu=$!

JUS_NOT_PORT="$PORT" "$JUS" test istemci_test.jus
testler=$?

# Testler sunucuyu /kapat ile durdurur; durmadıysa zorla kapat.
for _ in 1 2 3 4 5 6 7 8 9 10; do
    kill -0 "$sunucu" 2>/dev/null || break
    sleep 0.3
done
if kill -0 "$sunucu" 2>/dev/null; then
    kill "$sunucu"
    echo "Sonuç: KALDI (sunucu kendiliğinden kapanmadı)"
    exit 1
fi
wait "$sunucu"
sunucu_kodu=$?

if [ "$testler" -eq 0 ] && [ "$sunucu_kodu" -eq 0 ] && grep -q "Sunucu durdu." "$gunluk"; then
    echo "Sonuç: GEÇTİ"
    exit 0
fi
echo "Sonuç: KALDI (testler: $testler, sunucu çıkış kodu: $sunucu_kodu)"
echo "--- sunucu günlüğü"
cat "$gunluk"
exit 1
