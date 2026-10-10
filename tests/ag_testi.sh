#!/usr/bin/env bash
# Gerçek ağa çıkan sınamalar: şifreli bağlantı (https) ve sertifika doğrulaması.
#
# Kullanım: tests/ag_testi.sh [jus-yolu]
# JUS_AG_TESTI=0 ortam değişkeni verilirse hiçbir şey yapmadan başarılı çıkar
# (ağ erişimi olmayan ortamlar için).
#
# Kullanılan dış sunucular: example.com (IANA) ve badssl.com (hatalı sertifika
# örnekleri). Bunlara erişilemiyorsa sınama başarısız olur.

if [ "${JUS_AG_TESTI:-1}" = "0" ]; then
    echo "Ağ sınaması atlandı (JUS_AG_TESTI=0)."
    exit 0
fi

JUS="${1:-./jus}"
KOK="$(cd "$(dirname "$0")" && pwd)"
case "$JUS" in
    /*|?:*) ;;
    *) JUS="$PWD/$JUS" ;;
esac

gecen=0
kalan=0
cikti="$(mktemp)"
trap 'rm -f "$cikti"' EXIT

# denetle <açıklama> <beklenen metin> <program dosyası>
denetle() {
    local aciklama="$1" beklenen="$2" dosya="$3"
    "$JUS" "$KOK/ag/$dosya" > "$cikti" 2>&1 < /dev/null
    if grep -qF -- "$beklenen" "$cikti"; then
        gecen=$((gecen + 1))
    else
        kalan=$((kalan + 1))
        echo "BAŞARISIZ  $aciklama: çıktıda bulunamadı: $beklenen"
        sed 's/^/  /' "$cikti"
    fi
}

denetle "https ile GET" "200 doğru" https_getir.jus
denetle "ham şifreli bağlantı" "HTTP/1.1 200 OK" sifreli_baglan.jus
denetle "süresi dolmuş sertifika" "süresi dolmuş" sertifika_hatalari.jus
denetle "yanlış sunucu adı" "verilen sunucu adına ait değil" sertifika_hatalari.jus
denetle "kendinden imzalı sertifika" "güvenilir bir kök sertifikaya dayanmıyor" sertifika_hatalari.jus

echo "$gecen ağ denetimi geçti, $kalan başarısız."
[ "$kalan" -eq 0 ]
