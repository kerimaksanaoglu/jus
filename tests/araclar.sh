#!/usr/bin/env bash
# Komut satırı araçlarının sınaması: jus --surum, jus test ...
#
# Kullanım: tests/araclar.sh [jus-yolu]

JUS="${1:-./jus}"
case "$JUS" in
    /*|?:*) ;;
    *) JUS="$PWD/$JUS" ;;
esac
KOK="$(cd "$(dirname "$0")" && pwd)"

gecen=0
kalan=0

# denetle <açıklama> <beklenen çıkış kodu> <çıktıda aranacak metin> <komut...>
denetle() {
    local aciklama="$1" beklenen_kod="$2" aranan="$3"
    shift 3
    local cikti kod
    cikti="$("$@" 2>&1 < /dev/null)"
    kod=$?
    if [ "$kod" != "$beklenen_kod" ]; then
        echo "BAŞARISIZ  $aciklama: çıkış kodu $kod, beklenen $beklenen_kod"
        printf '%s\n' "$cikti" | sed 's/^/  /'
        kalan=$((kalan + 1))
    elif ! printf '%s' "$cikti" | grep -qF -- "$aranan"; then
        echo "BAŞARISIZ  $aciklama: çıktıda bulunamadı: $aranan"
        printf '%s\n' "$cikti" | sed 's/^/  /'
        kalan=$((kalan + 1))
    else
        gecen=$((gecen + 1))
    fi
}

cd "$KOK/araclar" || exit 1

denetle "sürüm" 0 "JUS " "$JUS" --surum
denetle "yardım" 0 "Kullanım:" "$JUS" --yardim
denetle "bilinmeyen seçenek" 64 "bilinmeyen seçenek" "$JUS" --yok
denetle "olmayan dosya" 66 "dosya açılamadı" "$JUS" yok.jus

denetle "test: geçen test" 1 "geçti  test_toplama" "$JUS" test ornek
denetle "test: geçen test 2" 1 "geçti  test_liste" "$JUS" test ornek
denetle "test: beklenti hatası" 1 "Beklenen 5, bulunan 4." "$JUS" test ornek
denetle "test: doğrulama iletisi" 1 "Doğrulama başarısız: bir ikiden büyük değil" "$JUS" test ornek
denetle "test: çalışma hatası" 1 "hesap_test.jus:19: Sıfıra bölünemez." "$JUS" test ornek
denetle "test: alt klasör" 1 "geçti  test_her_zaman_geçer" "$JUS" test ornek
denetle "test: özet" 1 "2 dosya, 6 test: 3 geçti, 3 kaldı." "$JUS" test ornek
denetle "test: yalnızca geçenler" 0 "1 dosya, 1 test: 1 geçti, 0 kaldı." "$JUS" test ornek/alt
denetle "test: tek dosya" 0 "1 geçti, 0 kaldı." "$JUS" test ornek/alt/gecen_test.jus
denetle "test: dosya yok" 66 "bulunamadı" "$JUS" test ornek/alt/yok

echo "$gecen araç denetimi geçti, $kalan başarısız."
[ "$kalan" -eq 0 ]
