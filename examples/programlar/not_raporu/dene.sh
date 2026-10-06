#!/usr/bin/env bash
# Not raporu betiğinin uçtan uca sınaması.
#
# Kullanım: dene.sh [jus-yolu]      (varsayılan: ./jus)
#
# Test girdileri ve çıktıları sistemin geçici klasöründe üretilir.

JUS="${1:-./jus}"
# Göreli yolu betik klasörüne geçmeden önce mutlak yola çevir.
case "$JUS" in
    /*|?:*) ;;
    *) JUS="$(pwd)/${JUS#./}" ;;
esac

KLASOR="$(cd "$(dirname "$0")" && pwd)"
GECICI="$(mktemp -d)"
trap 'rm -rf "$GECICI"' EXIT
cd "$KLASOR" || exit 1

gecen=0
kalan=0
CIKTI=""
KOD=0

# ham <komut...>: çıktıyı (CR atılmış) CIKTI'ya, çıkış kodunu KOD'a koyar.
ham() {
    "$@" > "$GECICI/cikti.txt" 2>&1
    KOD=$?
    CIKTI="$(tr -d '\r' < "$GECICI/cikti.txt")"
}

basarili() { gecen=$((gecen + 1)); echo "geçti: $1"; }
basarisiz() {
    kalan=$((kalan + 1))
    echo "KALDI: $1"
    [ -n "$2" ] && echo "$2" | sed 's/^/    | /'
}

# denetle <ad> <beklenen kod> <çıktıda geçmesi gereken metin|->
denetle() {
    local ad="$1" kod="$2" metin="$3" tamam=1
    [ "$KOD" = "$kod" ] || tamam=0
    if [ "$metin" != "-" ] && [[ "$CIKTI" != *"$metin"* ]]; then tamam=0; fi
    if [ "$tamam" = 1 ]; then
        basarili "$ad"
    else
        basarisiz "$ad (kod=$KOD, beklenen=$kod, beklenen metin='$metin')" "$CIKTI"
    fi
}

# içerir <ad> <dosya> <metin>: dosya içeriği metni içeriyor mu.
içerir() {
    if grep -qF -- "$3" "$2"; then basarili "$1"; else basarisiz "$1 ('$3' bulunamadı)"; fi
}

# ---------------------------------------------------------------- örnek veri
RAPOR="$GECICI/rapor.json"
ham "$JUS" not_raporu.jus ornek_veri.json "$RAPOR"
denetle "örnek veri: çıkış kodu" 0 "Not raporu: 17 öğrenci, 4 ders"
denetle "örnek veri: rapor yazıldı iletisi" 0 "Rapor yazıldı:"
denetle "öğrenci: en yüksek ortalama" 0 "1008  Gül Arslan              98.00  AA"
denetle "öğrenci: eksik dersli kayıt" 0 "1013  Burak Özkan                  66     70      73      -    69.67  DC"
denetle "öğrenci: ondalıklı not" 0 "1015  Ali Can Tunç               59.5   64.5      67     62    63.25  DC"
denetle "öğrenci: en düşük ortalama" 0 "1009  Ünal Koç                     39     44      51     47    45.25  FF"
denetle "ders istatistiği: Matematik" 0 "Matematik       17     74.56        100        39"
denetle "ders istatistiği: Tarih (16 not)" 0 "Tarih           16     76.06         99        45"

# Ada sırası: Türk alfabesine göre (Ilgaz < Ismail < İpek; Ç, Ö, Ş, Ü yerlerinde).
beklenen_siralama="Ali Can Tunç|Ayşe Yılmaz|Burak Özkan|Çağla Şahin|Ebru Güneş|Elif Doğan|Gül Arslan|Hasan Hüseyin Yıldız|Ilgaz Kaya|Ismail Polat|İpek Demir|Mehmet Çelik|Ömer Faruk Aydın|Sevgi Aksoy|Şule Öztürk|Ünal Koç|Zeynep Kurt"
gercek_siralama="$(sed -n '/^Ada göre/,/^Ortalamaya göre/p' "$GECICI/cikti.txt" | tr -d '\r' \
    | grep -E '^ *[0-9]{4}  ' | sed -E 's/^ *[0-9]{4}  //; s/ +[0-9.-]+ +[0-9.-]+ +[0-9.-]+ +[0-9.-]+ +[0-9.]+  [A-Z]{2}$//' | paste -sd'|')"
if [ "$gercek_siralama" = "$beklenen_siralama" ]; then
    basarili "ada göre Türk alfabesi sırası"
else
    basarisiz "ada göre sıra yanlış" "beklenen: $beklenen_siralama
gerçek:   $gercek_siralama"
fi

# Ortalama sırası: numaralar yüksekten düşüğe.
beklenen_no="1008 1003 1014 1017 1006 1001 1012 1010 1016 1004 1013 1007 1015 1002 1011 1005 1009"
gercek_no="$(sed -n '/^Ortalamaya göre/,/^Ders istatistikleri/p' "$GECICI/cikti.txt" | tr -d '\r' \
    | grep -E '^ *[0-9]+  +[0-9]{4}  ' | awk '{print $2}' | paste -sd' ')"
if [ "$gercek_no" = "$beklenen_no" ]; then
    basarili "ortalamaya göre sıra"
else
    basarisiz "ortalamaya göre sıra yanlış" "beklenen: $beklenen_no
gerçek:   $gercek_no"
fi

# JSON çıktı dosyası
[ -f "$RAPOR" ] && basarili "çıktı dosyası oluştu" || basarisiz "çıktı dosyası oluşmadı"
içerir "JSON: dönem" "$RAPOR" '"dönem": "2024-2025 Güz"'
içerir "JSON: öğrenci sayısı" "$RAPOR" '"öğrenci_sayısı": 17'
içerir "JSON: harf notu" "$RAPOR" '"harf_notu": "AA"'
içerir "JSON: ders istatistiği" "$RAPOR" '"en_düşük": 39'
içerir "JSON: ortalama sırası" "$RAPOR" '"sıralama_ortalamaya_göre"'
içerir "JSON: Türkçe karakter korunur" "$RAPOR" '"ad": "Çağla Şahin"'
içerir "JSON: boş hata listesi" "$RAPOR" '"hatalar": []'

# Çıktı JSON'u yeniden okunabiliyor mu: rapor dosyasını girdi olarak vermek
# 'öğrenciler' alanı olmadığı için anlaşılır bir hatayla reddedilmeli.
ham "$JUS" not_raporu.jus "$RAPOR" "$GECICI/x.json"
denetle "rapor dosyası girdi olarak reddedilir" 1 "'öğrenciler' listesi içeren"

# ---------------------------------------------------------------- doğrulama
cat > "$GECICI/hatali.json" <<'EOF'
{"öğrenciler": [
 {"numara": 1, "ad": "Ayşe Kara", "notlar": {"Matematik": 80, "Fizik": 90}},
 {"numara": 1, "ad": "Tekrar Numara", "notlar": {"Matematik": 50}},
 {"numara": 2, "ad": "Geçersiz Not", "notlar": {"Matematik": 120, "Fizik": "yüksek", "Tarih": true}},
 {"numara": 3, "notlar": {"Matematik": 50}},
 {"ad": "Numarasız", "notlar": {"Matematik": 50}},
 {"numara": 4.5, "ad": "Kesirli", "notlar": {}},
 {"numara": 5, "ad": "Notsuz"},
 "metin kaydı",
 {"numara": 6, "ad": "Veli Çam", "notlar": {"Matematik": 60}}
]}
EOF
ham "$JUS" not_raporu.jus "$GECICI/hatali.json" "$GECICI/hatali_cikti.json"
denetle "hatalı kayıtlar: çıkış kodu 3" 3 "10 sorun"
denetle "tekrar eden numara" 3 "Kayıt 2: numara 1 daha önce 1. kayıtta kullanılmış"
denetle "aralık dışı not" 3 "Kayıt 3: 'Matematik' notu geçersiz: 120"
denetle "metin not" 3 "Kayıt 3: 'Fizik' notu geçersiz: \"yüksek\""
denetle "mantıksal not" 3 "Kayıt 3: 'Tarih' notu geçersiz: true"
denetle "eksik ad" 3 "Kayıt 4: 'ad' alanı eksik"
denetle "eksik numara" 3 "Kayıt 5: 'numara' alanı eksik"
denetle "kesirli numara" 3 "Kayıt 6: 'numara' pozitif tam sayı olmalı, bulunan: 4.5"
denetle "boş notlar" 3 "Kayıt 6: 'notlar' boş"
denetle "eksik notlar" 3 "Kayıt 7: 'notlar' alanı eksik"
denetle "nesne olmayan kayıt" 3 "Kayıt 8: kayıt bir nesne olmalı"
denetle "geçerli kayıtlarla rapor sürer" 3 "Not raporu: 2 öğrenci, 2 ders"
içerir "JSON: hatalar kaydedilir" "$GECICI/hatali_cikti.json" "Kayıt 4: 'ad' alanı eksik"

# ---------------------------------------------------------------- hatalı girdi dosyaları
ham "$JUS" not_raporu.jus "$GECICI/yok.json" "$GECICI/o.json"
denetle "olmayan dosya" 1 "Girdi dosyası bulunamadı"
printf '{bozuk json' > "$GECICI/bozuk.json"
ham "$JUS" not_raporu.jus "$GECICI/bozuk.json" "$GECICI/o.json"
denetle "bozuk JSON" 1 "geçerli JSON değil"
: > "$GECICI/bos.json"
ham "$JUS" not_raporu.jus "$GECICI/bos.json" "$GECICI/o.json"
denetle "boş dosya" 1 "geçerli JSON değil"
printf '[1, 2, 3]' > "$GECICI/liste.json"
ham "$JUS" not_raporu.jus "$GECICI/liste.json" "$GECICI/o.json"
denetle "kök nesne değil" 1 "JSON nesnesi olmalı"
printf '{"öğrenciler": "yok"}' > "$GECICI/tur.json"
ham "$JUS" not_raporu.jus "$GECICI/tur.json" "$GECICI/o.json"
denetle "öğrenciler liste değil" 1 "bir liste olmalı"
printf '{"öğrenciler": []}' > "$GECICI/bosliste.json"
ham "$JUS" not_raporu.jus "$GECICI/bosliste.json" "$GECICI/o.json"
denetle "öğrenci yok" 1 "geçerli öğrenci kaydı yok"
[ ! -f "$GECICI/o.json" ] && basarili "hatalı girdide çıktı dosyası oluşmaz" || basarisiz "hatalı girdide çıktı dosyası oluştu"

# ---------------------------------------------------------------- kullanım ve çıktı hataları
ham "$JUS" not_raporu.jus a b c
denetle "fazla argüman" 2 "Çok fazla argüman"
ham "$JUS" not_raporu.jus ornek_veri.json "$GECICI/yok_klasor/o.json"
denetle "yazılamayan çıktı yolu" 1 "Çıktı dosyası yazılamadı"

# ---------------------------------------------------------------- tek öğrenci, ondalık yuvarlama
printf '{"öğrenciler": [{"numara": 7, "ad": "Tek Kişi", "notlar": {"Ders": 83.125, "Diğer": 90}}]}' > "$GECICI/tek.json"
ham "$JUS" not_raporu.jus "$GECICI/tek.json" "$GECICI/tek_cikti.json"
denetle "tek öğrenci" 0 "Not raporu: 1 öğrenci, 2 ders"
denetle "ortalama yuvarlama (86.5625 -> 86.56)" 0 "86.56  BA"
içerir "JSON: yuvarlanmış ortalama" "$GECICI/tek_cikti.json" '"ortalama": 86.56'

echo
if [ "$kalan" = 0 ]; then
    echo "Sonuç: GEÇTİ ($gecen denetim)"
    exit 0
fi
echo "Sonuç: KALDI ($kalan başarısız, $gecen geçti)"
exit 1
