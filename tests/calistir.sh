#!/usr/bin/env bash
# JUS test çalıştırıcısı.
#
# Kullanım: tests/calistir.sh [jus-yolu]
#
# Her test bir .jus dosyasıdır; beklentiler dosyanın içinde yorum olarak yazılır:
#   # bekle: <metin>   standart çıktıda beklenen satır (sırayla)
#   # hata: <metin>    hata çıktısında geçmesi gereken metin
#   # kod: <sayı>      beklenen çıkış kodu (yazılmazsa 0)
#   # girdi: <metin>   programa standart girdiden verilecek satır
#
# Adı '_' ile başlayan klasörlerdeki dosyalar test değil, testlerin kullandığı
# yardımcı modüllerdir. tests/ag altındaki dosyalar gerçek ağa çıkar ve
# tests/ag_testi.sh ile ayrıca çalıştırılır.

JUS="${1:-./jus}"
KOK="$(cd "$(dirname "$0")" && pwd)"

gecen=0
kalan=0
cikti="$(mktemp)"
hata="$(mktemp)"
trap 'rm -f "$cikti" "$hata"' EXIT

while IFS= read -r dosya; do
    beklenen="$(sed -n 's/^.*# bekle: \{0,1\}//p' "$dosya" | tr -d '\r')"
    beklenen_kod="$(sed -n 's/^.*# kod: *//p' "$dosya" | tr -d '\r' | head -n 1)"
    beklenen_kod="${beklenen_kod:-0}"
    girdi="$(sed -n 's/^.*# girdi: //p' "$dosya" | tr -d '\r')"

    if [ -n "$girdi" ]; then
        printf '%s\n' "$girdi" | "$JUS" "$dosya" > "$cikti" 2> "$hata"
    else
        "$JUS" "$dosya" < /dev/null > "$cikti" 2> "$hata"
    fi
    gercek_kod=$?
    gercek="$(tr -d '\r' < "$cikti")"

    sorun=""
    if [ "$gercek" != "$beklenen" ]; then
        sorun="çıktı beklenenden farklı"
    elif [ "$gercek_kod" != "$beklenen_kod" ]; then
        sorun="çıkış kodu $gercek_kod, beklenen $beklenen_kod"
    else
        while IFS= read -r ileti; do
            [ -z "$ileti" ] && continue
            if ! grep -qF -- "$ileti" "$hata"; then
                sorun="hata çıktısında bulunamadı: $ileti"
                break
            fi
        done <<< "$(sed -n 's/^.*# hata: //p' "$dosya" | tr -d '\r')"
    fi

    ad="${dosya#"$KOK"/}"
    if [ -z "$sorun" ]; then
        gecen=$((gecen + 1))
    else
        kalan=$((kalan + 1))
        echo "BAŞARISIZ  $ad: $sorun"
        if [ "$gercek" != "$beklenen" ]; then
            echo "  --- beklenen"
            printf '%s\n' "$beklenen" | sed 's/^/  /'
            echo "  --- gerçek"
            printf '%s\n' "$gercek" | sed 's/^/  /'
        fi
        if [ -s "$hata" ]; then
            echo "  --- hata çıktısı"
            sed 's/^/  /' "$hata"
        fi
    fi
done < <(find "$KOK" -name '*.jus' -not -path '*/_*' -not -path '*/araclar/*' -not -path '*/ag/*' | sort)

echo "$gecen test geçti, $kalan test başarısız."
[ "$kalan" -eq 0 ]
