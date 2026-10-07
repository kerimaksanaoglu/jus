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

# denetle <açıklama> <beklenen çıkış kodu> <çıktıda aranacak metin; boşsa aranmaz> <komut...>
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
    elif [ -n "$aranan" ] && ! printf '%s' "$cikti" | grep -qF -- "$aranan"; then
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

# Biçimlendirici
bicim="$(mktemp -d)"
printf 'eğer  x>1 :\n  yaz( "a" ,x )   #yorum\n\n\n\n\ndeğişken l=[1,2 ,3]\n' > "$bicim/a.jus"
printf 'değişken x = 1\n' > "$bicim/temiz.jus"
printf 'yaz("kapanmamış)\n' > "$bicim/bozuk.jus"
denetle "biçimle: denetle (bozuk biçim)" 1 "biçimlendirilmeli:" "$JUS" bicimle --denetle "$bicim/a.jus"
denetle "biçimle: denetle (temiz)" 0 "" "$JUS" bicimle --denetle "$bicim/temiz.jus"
denetle "biçimle: yaz" 0 "biçimlendirildi:" "$JUS" bicimle "$bicim/a.jus" "$bicim/temiz.jus"
beklenen_bicim="$(printf 'eğer x > 1:\n    yaz("a", x)  # yorum\n\n\ndeğişken l = [1, 2, 3]\n')"
if [ "$(cat "$bicim/a.jus")" = "$beklenen_bicim" ]; then
    gecen=$((gecen + 1))
else
    echo "BAŞARISIZ  biçimle: dosya içeriği beklenenden farklı"
    cat "$bicim/a.jus" | sed 's/^/  /'
    kalan=$((kalan + 1))
fi
denetle "biçimle: ikinci kez değişiklik yok" 0 "" "$JUS" bicimle --denetle "$bicim/a.jus"
denetle "biçimle: sözcük hatası" 65 "biçimlendirilemedi" "$JUS" bicimle "$bicim/bozuk.jus"
denetle "biçimle: dosya yok" 65 "dosya açılamadı" "$JUS" bicimle "$bicim/yok.jus"
denetle "biçimle: kullanım" 64 "Kullanım:" "$JUS" bicimle
rm -rf "$bicim"

# Paket yöneticisi: yerel bir git deposundan paket kur, kullan, kaldır.
if command -v git > /dev/null 2>&1; then
    calisma="$(mktemp -d)"
    mkdir -p "$calisma/kaynak/jus-selam" "$calisma/proje/alt"
    printf 'kullan "selam/yardimci"\nfonksiyon merhaba(ad):\n    dön yardimci.onek() + ad\n' > "$calisma/kaynak/jus-selam/selam.jus"
    printf 'fonksiyon onek():\n    dön "Merhaba, "\n' > "$calisma/kaynak/jus-selam/yardimci.jus"
    (cd "$calisma/kaynak/jus-selam" && git init -q . && git add . &&
        git -c user.name=test -c user.email=test@example.com commit -q -m ilk)
    printf 'kullan selam\nyaz(selam.merhaba("paket"))\n' > "$calisma/proje/ana.jus"
    printf 'kullan selam\nyaz(selam.merhaba("alt"))\n' > "$calisma/proje/alt/modul.jus"
    printf 'kullan "alt/modul"\n' > "$calisma/proje/ana2.jus"

    cd "$calisma/proje" || exit 1
    denetle "paket: boş liste" 0 "Kurulu paket yok." "$JUS" paket listele
    denetle "paket: kurulmadan kullan" 70 "Kurulu paketlerde de yok" "$JUS" ana.jus
    denetle "paket: kur" 0 "Kullanmak için: kullan selam" "$JUS" paket kur "$calisma/kaynak/jus-selam"
    denetle "paket: kullan" 0 "Merhaba, paket" "$JUS" ana.jus
    denetle "paket: alt klasördeki modülden kullan" 0 "Merhaba, alt" "$JUS" ana2.jus
    denetle "paket: listele" 0 "selam" "$JUS" paket listele
    denetle "paket: ikinci kez kur" 1 "zaten kurulu" "$JUS" paket kur "$calisma/kaynak/jus-selam"
    denetle "paket: kaldır" 0 "'selam' paketi kaldırıldı." "$JUS" paket kaldır selam
    denetle "paket: kaldırıldıktan sonra" 70 "modülü yüklenemedi" "$JUS" ana.jus
    denetle "paket: olmayanı kaldır" 1 "kurulu bir paket yok" "$JUS" paket kaldır selam
    denetle "paket: geçersiz adres" 64 "geçerli bir git adresi değil" "$JUS" paket kur 'a;b'
    denetle "paket: kullanım" 64 "Kullanım: jus paket" "$JUS" paket
    cd "$KOK" || exit 1
    rm -rf "$calisma"
else
    echo "git bulunamadı; paket denetimleri atlandı."
fi

echo "$gecen araç denetimi geçti, $kalan başarısız."
[ "$kalan" -eq 0 ]
