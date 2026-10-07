#!/usr/bin/env bash
# Yapılacaklar aracının uçtan uca sınaması.
#
# Kullanım: dene.sh [jus-yolu]      (varsayılan: ./jus)
#
# Not: Windows'ta komut satırı argümanlarındaki Türkçe karakterler yorumlayıcıya
# bozuk ulaştığı için komut argümanları ASCII tutulmuştur. Türkçe karakterler
# veri dosyası üzerinden sınanır.

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

DOSYA="$GECICI/liste.json"
gecen=0
kalan=0
CIKTI=""
KOD=0

# calistir <argümanlar...>: çıktıyı CIKTI'ya, çıkış kodunu KOD'a koyar.
calistir() {
    ham "$JUS" yapilacaklar.jus --dosya "$DOSYA" "$@"
}

# ham <komut...>: komutu CALISMA klasöründe (varsayılan: betik klasörü)
# çalıştırır; çıktıyı (CR atılmış) CIKTI'ya, çıkış kodunu KOD'a koyar.
ham() {
    (cd "${CALISMA:-$KLASOR}" && "$@") > "$GECICI/cikti.txt" 2>&1
    KOD=$?
    CIKTI="$(tr -d '\r' < "$GECICI/cikti.txt")"
}

# denetle <ad> <beklenen kod> <çıktıda geçmesi gereken metin|->
denetle() {
    local ad="$1" kod="$2" metin="$3" tamam=1
    [ "$KOD" = "$kod" ] || tamam=0
    if [ "$metin" != "-" ] && [[ "$CIKTI" != *"$metin"* ]]; then tamam=0; fi
    if [ "$tamam" = 1 ]; then
        gecen=$((gecen + 1))
        echo "geçti: $ad"
    else
        kalan=$((kalan + 1))
        echo "KALDI: $ad (kod=$KOD, beklenen=$kod, beklenen metin='$metin')"
        echo "$CIKTI" | sed 's/^/    | /'
    fi
}

# yok_mu <ad> <çıktıda bulunmaması gereken metin>
yok_mu() {
    if [[ "$CIKTI" == *"$2"* ]]; then
        kalan=$((kalan + 1))
        echo "KALDI: $1 ('$2' bulunmamalıydı)"
        echo "$CIKTI" | sed 's/^/    | /'
    else
        gecen=$((gecen + 1))
        echo "geçti: $1"
    fi
}

# --- Boş liste ---
calistir listele
denetle "boş liste" 0 "Gösterilecek görev yok."
[ ! -f "$DOSYA" ] && { gecen=$((gecen + 1)); echo "geçti: listelemek dosya oluşturmaz"; } \
    || { kalan=$((kalan + 1)); echo "KALDI: listelemek dosya oluşturmamalı"; }

# --- Ekleme ---
calistir ekle Sut al
denetle "ekle (tırnaksız çok sözcük)" 0 "Eklendi: #1 Sut al"
calistir ekle "Ekmek al"
denetle "ekle (tırnaklı)" 0 "Eklendi: #2 Ekmek al"
calistir ekle "Fatura ode"
denetle "üçüncü ekle" 0 "Eklendi: #3 Fatura ode"
[ -f "$DOSYA" ] && { gecen=$((gecen + 1)); echo "geçti: veri dosyası oluştu"; } \
    || { kalan=$((kalan + 1)); echo "KALDI: veri dosyası oluşmadı"; }

# --- Listeleme ---
calistir listele
denetle "listele: ilk görev" 0 "1  [ ]  Sut al"
denetle "listele: toplam" 0 "Toplam 3 görev, 0 tamamlandı."

# --- Tamamlama ---
calistir tamamla 2
denetle "tamamla" 0 "Tamamlandı: #2 Ekmek al"
calistir listele
denetle "tamamlanan işareti" 0 "2  [x]  Ekmek al"
denetle "tamamlanan sayacı" 0 "Toplam 3 görev, 1 tamamlandı."
calistir listele bekleyen
denetle "listele bekleyen" 0 "Sut al"
yok_mu "bekleyen listesinde tamamlanan yok" "Ekmek al"
calistir listele tamamlanan
denetle "listele tamamlanan" 0 "Ekmek al"
yok_mu "tamamlanan listesinde bekleyen yok" "Sut al"
calistir listele kalmamis
denetle "geçersiz süzgeç" 2 "Bilinmeyen süzgeç"

# --- Arama ---
calistir ara SUT
denetle "ara (büyük/küçük harf ayrımsız)" 0 "Sut al"
yok_mu "ara: eşleşmeyen yok" "Fatura"
calistir ara yokboyle
denetle "ara: sonuç yok" 0 "Eşleşen görev yok."
calistir ara
denetle "ara: argümansız" 2 "aranacak metni ister"

# --- Silme ve numaralandırma ---
calistir sil 1
denetle "sil" 0 "Silindi: #1 Sut al"
calistir listele
yok_mu "silinen listede yok" "Sut al"
calistir ekle "Yeni is"
denetle "silinen numara yeniden kullanılmaz" 0 "Eklendi: #4 Yeni is"

# --- Hatalı kullanım ---
calistir sil 99
denetle "olmayan görev" 1 "Görev bulunamadı: 99"
calistir tamamla 99
denetle "olmayan görevi tamamlama" 1 "Görev bulunamadı: 99"
calistir sil abc
denetle "numara metin" 2 "Geçersiz görev numarası: 'abc'"
calistir sil 1.5
denetle "numara ondalık" 2 "pozitif tam sayı"
calistir sil 0
denetle "numara sıfır" 2 "Geçersiz görev numarası"
calistir sil
denetle "numarasız sil" 2 "tam olarak bir görev numarası"
calistir sil 1 2
denetle "fazla argüman" 2 "tam olarak bir görev numarası"
calistir ekle
denetle "başlıksız ekle" 2 "bir başlık ister"
calistir ekle "   "
denetle "boşluk başlık" 2 "boş olamaz"
calistir uc
denetle "bilinmeyen komut" 2 "Bilinmeyen komut: 'uc'"
ham "$JUS" yapilacaklar.jus --dosya
denetle "--dosya değersiz" 2 "bir dosya yolu ister"

# --- Yardım ---
calistir yardim
denetle "yardım" 0 "Komutlar:"
CALISMA="$GECICI" ham "$JUS" "$KLASOR/yapilacaklar.jus"
denetle "argümansız çalıştırma" 2 "Kullanım:"

# --- Seçenek biçimleri ve varsayılan dosya ---
DIGER="$GECICI/diger.json"
ham "$JUS" yapilacaklar.jus ekle "Baska liste" --dosya="$DIGER"
denetle "--dosya=yol biçimi" 0 "Eklendi: #1 Baska liste"
[ -f "$DIGER" ] && { gecen=$((gecen + 1)); echo "geçti: --dosya=yol dosyası oluştu"; } \
    || { kalan=$((kalan + 1)); echo "KALDI: --dosya=yol dosyası oluşmadı"; }
CALISMA="$GECICI" ham "$JUS" "$KLASOR/yapilacaklar.jus" ekle "Varsayilan"
denetle "varsayılan dosya" 0 "Eklendi: #1 Varsayilan"
[ -f "$GECICI/yapilacaklar.json" ] && { gecen=$((gecen + 1)); echo "geçti: varsayılan dosya çalışma klasöründe"; } \
    || { kalan=$((kalan + 1)); echo "KALDI: varsayılan dosya çalışma klasöründe oluşmadı"; }

# --- Veri dosyasındaki Türkçe karakterler (UTF-8) ---
TR="$GECICI/turkce.json"
printf '{"sonraki_no": 2, "görevler": [{"no": 1, "başlık": "Çiçekleri suladım, şeker al", "tamam": false, "tarih": "2025-01-31"}]}' > "$TR"
ham "$JUS" yapilacaklar.jus --dosya "$TR" listele
denetle "Türkçe başlık listelenir" 0 "Çiçekleri suladım, şeker al"
ham "$JUS" yapilacaklar.jus --dosya "$TR" ara cicek
denetle "ara: ASCII karşılığı eşleşmez" 0 "Eşleşen görev yok."
ham "$JUS" yapilacaklar.jus --dosya "$TR" ekle "Ikinci"
denetle "Türkçe veriye ekleme" 0 "Eklendi: #2 Ikinci"
grep -q "Çiçekleri suladım" "$TR" && { gecen=$((gecen + 1)); echo "geçti: Türkçe karakterler dosyada korunur"; } \
    || { kalan=$((kalan + 1)); echo "KALDI: Türkçe karakterler dosyada bozuldu"; }

# --- Bozuk veri dosyaları ---
BOZUK="$GECICI/bozuk.json"
printf '{bozuk json' > "$BOZUK"
ham "$JUS" yapilacaklar.jus --dosya "$BOZUK" listele
denetle "bozuk JSON" 1 "geçerli bir JSON dosyası değil"
printf '{"baska": 1}' > "$BOZUK"
ham "$JUS" yapilacaklar.jus --dosya "$BOZUK" listele
denetle "beklenmeyen biçim" 1 "beklenen biçimde değil"
ham "$JUS" yapilacaklar.jus --dosya "$GECICI/yok/liste.json" ekle x
denetle "yazılamayan klasör" 1 "yazılamadı"

echo
if [ "$kalan" = 0 ]; then
    echo "Sonuç: GEÇTİ ($gecen denetim)"
    exit 0
fi
echo "Sonuç: KALDI ($kalan başarısız, $gecen geçti)"
exit 1
