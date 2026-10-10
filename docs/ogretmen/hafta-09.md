# Hafta 9 - Hata yakalama ve küçük bir oyun

[Öğretmen kiti](README.md) | Önceki: [Hafta 8](hafta-08.md) | Sonraki: [Hafta 10 - Dönem projesi](hafta-10.md)

İlgili rehber bölümleri: [09 - Hatalar](../rehber/09-hatalar.md) ve [12 - Küçük projeler](../rehber/12-kucuk-projeler.md) (Proje 1: sayı tahmin oyunu)

Bu hafta iki parçalıdır. Birinci ders saatinde öğrenciler, program hata verdiğinde **durmak** yerine hatayı **ele almayı** öğrenir (`dene` / `yakala` / `fırlat`). İkinci ders saatinde şimdiye kadar öğrendiklerini bir küçük oyunda birleştirirler. İkinci parça, dönem projesi için ısınmadır.

Önemli bir kısıt: tarayıcıdaki deneme alanında `oku` çalışmaz. Bu yüzden haftanın ana oyunları girdi istemeden çalışan iki oyundur (zar yarışı ve kendi kendine tahmin eden bilgisayar). Girdi isteyen klasik tahmin oyunu, bilgisayara JUS kuran sınıflar için üçüncü seçenek olarak verilmiştir.

Bu haftanın sonunda [değerlendirme.md](degerlendirme.md) içindeki 2. sınavı uygulayabilirsiniz (Hafta 1-9). Zaman darsa sınavı Hafta 10'un ilk ders saatinde yapın.

## Kazanımlar

Hafta sonunda öğrenci:

1. `dene` / `yakala` ile çalışma zamanı hatasını yakalayıp programı sürdürebilir.
2. `yakala hata:` ile hata metnini okuyup kullanıcıya anlamlı bir ileti gösterebilir.
3. `fırlat` ile kendi kuralı bozulduğunda hata oluşturabilir ve bunu `yakala` ile karşılayabilir.
4. Sözdizimi hatalarının yakalanamadığını; yazım hatasını `yakala` ile susturmanın sorunu saklayacağını açıklayabilir.
5. Döngü, karar, fonksiyon ve liste bilgisini birleştirerek `rastgele` modülünü kullanan küçük bir oyun yazabilir.

## Ön hazırlık

- Tarayıcıda deneme alanı: <https://kerimaksanaoglu.github.io/jus/>. `rastgele` modülü deneme alanının "kullanılamaz" listesinde yer almıyor, ama dersten önce `kullan rastgele` ve `rastgele.tam(1, 6)` ifadelerini bir kez çalıştırarak deneyin.
- Etkinlik için: her çifte bir kâğıt, kalem, bir "gizli sayı" kartı.
- Dönem projesi için: [proje.md](proje.md) dosyasını okuyup bir sonraki hafta öğrencilerin proje seçeceğini duyurun.

## Ders akışı

| Süre | Evre | Ne yapılır |
|------|------|------------|
| 0-6 | Isınma | Hafta 8'den: sözlükte olmayan anahtarı okuyunca ne oluyor? Program dururken "durmasa olmaz mı?" sorusunu açın. |
| 6-12 | Yeni kavram | `dene`: "şunu dene". `yakala`: "olmazsa şunu yap". Hatanın iki türü ve yalnızca birinin yakalanabildiği. |
| 12-26 | Canlı kodlama 1 | Adım 1-5: `dene`/`yakala`, hata değeri, `fırlat`. |
| 26-36 | Etkinlik | "Yakala mı, düzelt mi?" kartları ve kâğıtta tahmin oyunu (bilgisayarsız). |
| 36-48 | Canlı kodlama 2 | Adım 6-7: zar yarışı ve kendi kendine tahmin eden bilgisayar. |
| 48-72 | Öğrenci uygulaması | Alıştırma 9.1-9.6, eşli. Hızlılar 9.7-9.9'a geçer. |
| 72-76 | Paylaşım | Bir çift ikili arama sonucunu (9.7) anlatır: "Kaç adımda buldu?" |
| 76-80 | Kapanış | Çıkış bileti. Proje duyurusu. |

## Canlı kodlama betiği

### Adım 1 - Hatada durmak

```jus
yaz("Başlıyoruz")
değişken x = sayı("abc")
yaz("Devam ediyoruz")
```

```
Başlıyoruz
ornek.jus:2: çalışma zamanı hatası: "abc" bir sayıya dönüştürülemez.
```

Sorun: "Kullanıcı `abc` yazdı diye programın kapanması iyi mi? Ne olmasını isterdik?"

### Adım 2 - `dene` ve `yakala`

```jus
yaz("Başlıyoruz")
dene:
    değişken x = sayı("abc")
    yaz("Sayı:", x)
yakala:
    yaz("Bu bir sayı değil.")
yaz("Devam ediyoruz")
```

```
Başlıyoruz
Bu bir sayı değil.
Devam ediyoruz
```

Söyleyin: "`dene` bloğundaki bir satır hata verirse bloğun **kalanı atlanır** ve `yakala` bloğu çalışır. Program ölmez, sonraki satırlara devam eder." Sorun: "`yaz("Sayı:", x)` neden çalışmadı?"

### Adım 3 - Hata değerine bakmak

```jus
dene:
    yaz(10 / 0)
yakala hata:
    yaz("Hata:", hata)

dene:
    yaz(sayı("abc"))
yakala hata:
    yaz("Hata:", hata)

dene:
    değişken l = [1, 2, 3]
    yaz(l[10])
yakala hata:
    yaz("Hata:", hata)
    yaz(tür(hata))
```

```
Hata: Sıfıra bölünemez.
Hata: "abc" bir sayıya dönüştürülemez.
Hata: Dizin sınırların dışında: uzunluk 3, istenen dizin 10.
metin
```

Söyleyin: "`yakala hata:` yazarsak hata iletisi `hata` adlı değişkende toplanır. Adı biz seçeriz; `h` de olur. Bu ileti, daha önce ekranda gördüğümüz iletinin aynısıdır; dilin kendi ürettiği hatalar **metindir**."

### Adım 4 - Fonksiyonda yakalamak

```jus
fonksiyon güvenliBöl(a, b):
    dene:
        dön a / b
    yakala:
        dön boş

yaz(güvenliBöl(10, 4))
yaz(güvenliBöl(1, 0))
yaz(güvenliBöl(9, 3))
```

```
2.5
boş
3
```

Söyleyin: "Hata olursa `boş` döndürüyoruz. Çağıran taraf `boş` gelip gelmediğine bakarak ne olduğunu anlar." (Hafta 5'teki `dön` bilgisinin tekrarıdır.)

### Adım 5 - Kendi hatamızı fırlatmak

```jus
fonksiyon yaşKontrol(yaş):
    eğer yaş < 0:
        fırlat "Yaş negatif olamaz."
    eğer yaş > 150:
        fırlat "Yaş gerçekçi değil."
    dön yaş

her deneme içinde [25, -3, 200]:
    dene:
        yaz("Geçerli yaş:", yaşKontrol(deneme))
    yakala sebep:
        yaz("Reddedildi:", sebep)
```

```
Geçerli yaş: 25
Reddedildi: Yaş negatif olamaz.
Reddedildi: Yaş gerçekçi değil.
```

Söyleyin: "Kendi kuralımız bozulduğunda `fırlat` ile hata başlatırız. Fırlatılan değer `yakala`'daki değişkene aynen gelir." Hafta 5'te yazdıkları fonksiyonlara girdi denetimi eklemeyi önerin.

### Adım 6 - Oyun 1: Zar yarışı

Aşağıdaki programda `rastgele.tohum(7)` satırı, **aynı sonucu her çalıştırmada almak** içindir; böylece sınıfta herkes aynı çıktıyı görür. Gerçek bir oyun için bu satır silinir.

```jus
kullan rastgele

rastgele.tohum(7)

değişken ayşePuan = 0
değişken boraPuan = 0

her tur içinde aralık(1, 6):
    değişken ayşeZar = rastgele.tam(1, 6)
    değişken boraZar = rastgele.tam(1, 6)
    yaz("Tur", tur, "- Ayşe:", ayşeZar, "Bora:", boraZar)
    eğer ayşeZar > boraZar:
        ayşePuan += 1
    değilse eğer boraZar > ayşeZar:
        boraPuan += 1

yaz("Puanlar - Ayşe:", ayşePuan, "Bora:", boraPuan)
eğer ayşePuan > boraPuan:
    yaz("Kazanan: Ayşe")
değilse eğer boraPuan > ayşePuan:
    yaz("Kazanan: Bora")
değilse:
    yaz("Berabere")
```

```
Tur 1 - Ayşe: 5 Bora: 4
Tur 2 - Ayşe: 3 Bora: 5
Tur 3 - Ayşe: 1 Bora: 2
Tur 4 - Ayşe: 5 Bora: 6
Tur 5 - Ayşe: 2 Bora: 4
Puanlar - Ayşe: 1 Bora: 4
Kazanan: Bora
```

Sorun: "Bu programda hangi hafta konuları var?" (Döngü, karar, değişken, `rastgele` modülü.) `tohum(7)` satırını silip birkaç kez çalıştırmalarını isteyin: her seferinde farklı çıktı alırlar. Bu çıktı sizin ekranınızdan farklı olabilir; sorun değildir.

### Adım 7 - Oyun 2: Kendi kendine tahmin eden bilgisayar

Bilgisayar 1-100 arasında gizli bir sayıyı (burada 73) bulmaya çalışıyor. Her adımda aralığın ortasını deniyor ve aralığı yarıya indiriyor. Önce sınıfla kâğıtta oynadıkları oyunu hatırlatın (etkinlik).

```jus
değişken gizli = 73
değişken düşük = 1
değişken yüksek = 100
değişken adım = 0

iken doğru:
    değişken tahmin = taban((düşük + yüksek) / 2)
    adım += 1
    yaz("Tahmin", adım, ":", tahmin)
    eğer tahmin == gizli:
        yaz("Buldum!", adım, "adımda.")
        kır
    değilse eğer tahmin < gizli:
        düşük = tahmin + 1
    değilse:
        yüksek = tahmin - 1
```

```
Tahmin 1 : 50
Tahmin 2 : 75
Tahmin 3 : 62
Tahmin 4 : 68
Tahmin 5 : 71
Tahmin 6 : 73
Buldum! 6 adımda.
```

Sorun: "Her adımda aralık nasıl değişti? Bu yöntemle 100 sayı içinden en çok kaç adımda buluruz?" (Her adımda yarıya indiği için en fazla 7 adım: 2'nin yedinci kuvveti 128.) Değişken adlarında `alt` / `üst` yerine `düşük` / `yüksek` kullanılmasının nedenini açıklayın: `üst` JUS'ta ayrılmış bir kelimedir (bkz. aşağıda, "Öğrenciler nerede takılır", 5. madde).

### Adım 8 - Kurulu bilgisayarlar için: girdili tahmin oyunu

Bu adım deneme alanında çalışmaz; JUS'un bilgisayara kurulu olduğu sınıflarda gösterin. `sayıyaÇevir` fonksiyonu `dene`/`yakala` ile kullanıcının geçersiz girdisini karşılar. Bu çıktılar girdi dışarıdan verilerek alındığı için yazılanlar çıktıda görünmez; `rastgele.tohum(42)` satırı gizli sayıyı sabitler (bu tohumla gizli sayı 23 olur).

```jus
kullan rastgele

fonksiyon sayıyaÇevir(girdi):
    dene:
        dön sayı(girdi)
    yakala:
        dön boş

rastgele.tohum(42)
değişken gizli = rastgele.tam(1, 50)
değişken hak = 5
değişken bulundu = yanlış

yaz("1 ile 50 arasında bir sayı tuttum.", hak, "hakkınız var.")

iken hak > 0 ve değil bulundu:
    değişken girdi = oku("Tahmin: ")
    eğer girdi == boş:
        yaz("Girdi bitti.")
        kır
    değişken tahmin = sayıyaÇevir(girdi)
    eğer tahmin == boş:
        yaz("Bu bir sayı değil, hak kaybetmediniz.")
        devam
    hak -= 1
    eğer tahmin == gizli:
        bulundu = doğru
    değilse eğer tahmin < gizli:
        yaz("Daha büyük bir sayı deneyin.")
    değilse:
        yaz("Daha küçük bir sayı deneyin.")

eğer bulundu:
    yaz("Tebrikler!")
değilse:
    yaz("Olmadı. Sayı", gizli, "idi.")
```

Girdi satırları: `30`, `abc`, `10`, `23`

```
1 ile 50 arasında bir sayı tuttum. 5 hakkınız var.
Tahmin: Daha küçük bir sayı deneyin.
Tahmin: Bu bir sayı değil, hak kaybetmediniz.
Tahmin: Daha büyük bir sayı deneyin.
Tahmin: Tebrikler!
```

Girdi satırları: `1`, `2`, `3`, `4`, `5`

```
1 ile 50 arasında bir sayı tuttum. 5 hakkınız var.
Tahmin: Daha büyük bir sayı deneyin.
Tahmin: Daha büyük bir sayı deneyin.
Tahmin: Daha büyük bir sayı deneyin.
Tahmin: Daha büyük bir sayı deneyin.
Tahmin: Daha büyük bir sayı deneyin.
Olmadı. Sayı 23 idi.
```

Girdi satırları: `30`

```
1 ile 50 arasında bir sayı tuttum. 5 hakkınız var.
Tahmin: Daha küçük bir sayı deneyin.
Tahmin: Girdi bitti.
Olmadı. Sayı 23 idi.
```

Söyleyin: "Üç çalıştırmada üç farklı son: bulundu, hak bitti, girdi yarıda bitti. `abc` yazan kullanıcı hak kaybetmedi; `devam` döngünün başına döndürdü."

## Sınıf içi etkinlik: Yakala mı, düzelt mi? ve kâğıtta tahmin oyunu (bilgisayarsız, 10 dk)

Amaç: Hangi hataların `yakala` ile karşılanması, hangilerinin düzeltilmesi gerektiğini ayırt etmek; oyunun stratejisini koddan önce bulmak.

**Bölüm 1 - Kartlar (5 dk).** Aşağıdaki durumları tahtaya yazın. Öğrenciler her biri için "yakala" (kullanıcıdan ya da dışarıdan gelebilen, beklenen bir durum) ya da "düzelt" (programcının hatası) der. Yanıtı gerekçesiyle konuşun:

| Durum | Yakala mı, düzelt mi? |
|-------|-----------------------|
| Kullanıcı yaş yerine `abc` yazdı. | Yakala: beklenen bir kullanıcı hatası. |
| Programda `yazz("selam")` yazılmış. | Düzelt: yazım hatası; yakalarsanız hatayı saklamış olursunuz. |
| Bölen kullanıcıdan geliyor ve sıfır olabilir. | Yakala (ya da `eğer` ile önceden denetle). |
| Program bir listenin 10. öğesini okuyor ama liste 3 öğeli; liste programcının kendi listesi. | Düzelt: mantık hatası. |
| `eğer x > 3` satırında iki nokta unutulmuş. | Düzelt: sözdizimi hatası; zaten yakalanamaz, program hiç başlamaz. |

**Bölüm 2 - Kâğıtta tahmin oyunu (5 dk).** Çiftler: biri 1-100 arası bir sayıyı gizlice kâğıda yazar. Diğeri en az adımda bulmaya çalışır; her tahminden sonra "daha büyük" ya da "daha küçük" yanıtı verilir. Tahmin sayısını not edin.

Sınıfa sorun: "En az adımda bulanlar ilk tahminlerinde ne dedi?" Çoğu zaman 50. Sonra: "Cevap 'daha büyük' ise bir sonraki tahmin ne olmalı?" (50 ile 100'ün ortası, yaklaşık 75.) Bu, Adım 7'deki programın stratejisidir. Önce kâğıtta bulunan strateji, kodu yazmayı kolaylaştırır.

## Alıştırmalar

Çözümler [cevap anahtarında](cevap-anahtari.md#hafta-9).

### Kolay

**Alıştırma 9.1.** Bir bölme yapıp bölen sıfırsa `boş` döndüren `güvenliBöl(a, b)` fonksiyonunu yazın. `güvenliBöl(10, 4)` ve `güvenliBöl(1, 0)` sonuçlarını yazdırın.

Beklenen çıktı:

```
2.5
boş
```

**Alıştırma 9.2.** Bir metni sayıya çeviren, çeviremezse `boş` döndüren `sayıOku(m)` fonksiyonunu yazın. `"42"`, `"3.5"`, `"elma"` ve `""` (boş metin) için çağırın.

Beklenen çıktı:

```
42
3.5
boş
boş
```

**Alıştırma 9.3.** `[1, 2, 3]` listesinin 10. öğesini okumaya çalışın ve hatayı yakalayıp `Hata:` ile yazdırın. Program devam edip `Program sürüyor.` yazdırsın.

Beklenen çıktı:

```
Hata: Dizin sınırların dışında: uzunluk 3, istenen dizin 10.
Program sürüyor.
```

### Orta

**Alıştırma 9.4.** Negatif sayı verilirse `fırlat` ile `Negatif sayının karekökü alınamaz.` hatası veren, değilse `karekök` döndüren `karekökAl(x)` fonksiyonunu yazın. `16`, `-4` ve `2` ile `dene`/`yakala` içinde deneyin. (`karekök` yerleşik fonksiyondur; sonuçları `yuvarla(..., 4)` ile düzenleyin.)

Beklenen çıktı:

```
Karekök: 4
Hata: Negatif sayının karekökü alınamaz.
Karekök: 1.4142
```

**Alıştırma 9.5.** `["12", "abc", "7", "", "3.5"]` listesindeki metinleri sayıya çevirip geçerli olanların toplamını bulun; geçersiz kaç tane olduğunu da yazdırın.

Beklenen çıktı:

```
Toplam: 22.5
Geçersiz: 2
```

**Alıştırma 9.6 - Zar yarışı.** Adım 6'daki programı değiştirin: yarış 3 turluk olsun, `rastgele.tohum(3)` kullanın ve her turun sonunda o ana kadarki puanları da yazdırın. (Öğrencinin çıktısı farklı olabilir; ilke tohumla aynı çıktıyı yeniden üretmektir. Beklenen çıktı çözümdeki programa aittir.)

Beklenen çıktı:

```
Tur 1 : 4 3 | Puan: 1 0
Tur 2 : 6 3 | Puan: 2 0
Tur 3 : 6 2 | Puan: 3 0
```

### Zor

**Alıştırma 9.7 - İkili arama.** Adım 7'deki programı `gizli = 14` için çalıştırın ve kaç adımda bulduğunu yazın. Sonra `gizli = 100` ve `gizli = 1` ile deneyin. (Üç sonuç da en fazla 7 adım olmalı.)

Beklenen çıktılar:

gizli = 14 için:

```
Tahmin 1 : 50
Tahmin 2 : 25
Tahmin 3 : 12
Tahmin 4 : 18
Tahmin 5 : 15
Tahmin 6 : 13
Tahmin 7 : 14
Buldum! 7 adımda.
```

gizli = 100 için:

```
Tahmin 1 : 50
Tahmin 2 : 75
Tahmin 3 : 88
Tahmin 4 : 94
Tahmin 5 : 97
Tahmin 6 : 99
Tahmin 7 : 100
Buldum! 7 adımda.
```

gizli = 1 için:

```
Tahmin 1 : 50
Tahmin 2 : 25
Tahmin 3 : 12
Tahmin 4 : 6
Tahmin 5 : 3
Tahmin 6 : 1
Buldum! 6 adımda.
```

**Alıştırma 9.8 - Tahmin oyunu (kurulu bilgisayar).** Adım 8'deki oyunu, hak sayısını 7 ve aralığı 1-100 yapacak şekilde değiştirin. Oyun bittiğinde kaç tahmin yaptığınızı da yazdırsın. (Bu alıştırma deneme alanında çalışmaz.)

Beklenen çıktı (girdi: `50`, `abc`, `20`, `45`; tohum 42 ile gizli sayı 45):

Girdi satırları: `50`, `abc`, `20`, `45`

```
1 ile 100 arasında bir sayı tuttum. 7 hakkınız var.
Tahmin: Daha küçük bir sayı deneyin.
Tahmin: Bu bir sayı değil, hak kaybetmediniz.
Tahmin: Daha büyük bir sayı deneyin.
Tahmin: Tebrikler! 3 tahminde buldunuz.
```

**Alıştırma 9.9 - Not satırları.** `"Ayşe:90"` gibi `ad:not` biçiminde yazılmış satırlardan oluşan `["Ayşe:90", "Mehmet:abc", "Zeynep:80", "Can"]` listesini işleyin. Geçerli satırları bir sözlüğe `{ad: not}` olarak ekleyin. Notu sayıya çevrilemeyen ya da `:` içermeyen satırlar için `fırlat` ile bir hata oluşturup `yakala` içinde `Hatalı satır:` ile yazdırın. En sonda sözlüğü ve ortalamayı yazdırın.

Beklenen çıktı:

```
Hatalı satır: Mehmet:abc - "abc" bir sayıya dönüştürülemez.
Hatalı satır: Can - ':' işareti yok
{"Ayşe": 90, "Zeynep": 80}
Ortalama: 85
```

## Öğrenciler nerede takılır

### 1. `yakala`'yı unutmak

```jus
yaz("Bu satır hiç çalışmaz.")
dene:
    yaz("içeride")
yaz("yakala yok")
```

```
ornek.jus:4:1: sözdizimi hatası: 'dene' bloğundan sonra 'yakala' bekleniyor.
    yaz("yakala yok")
    ^
```

Her `dene` bloğunu bir `yakala` bloğu izlemelidir. Sözdizimi hatası olduğu için **ilk `yaz` satırı bile çalışmaz**; öğrenci bunu fark etmediğinde şaşırır. Yönlendirme: "İletide hangi satır gösteriliyor? Neyi bekliyormuş?"

### 2. Sözdizimi hatasını yakalamaya çalışmak

```jus
yaz("Bu satır çalışmaz")
dene:
    değişken x =
yakala:
    yaz("yakalandı")
```

```
ornek.jus:3:17: sözdizimi hatası: İfade bekleniyor.
        değişken x =
                    ^
```

Sözdizimi hatası program başlamadan önce bulunur; `dene` bloğu onu yakalayamaz. Yakalanabilenler yalnızca program çalışırken oluşan hatalardır (Hafta 1'deki ayrım).

### 3. Her şeyi `yakala:` ile susturmak

```jus
dene:
    değişken puan = 80
    yaz("Puan:", puna)
yakala:
    yaz("Bir hata oldu.")
```

```
Bir hata oldu.
```

`puna` yazım hatasıdır (`puan` olmalıydı), ama program "Bir hata oldu" der; asıl neden saklanır. Hata metnini yazdırmak sorunu görünür kılar:

```jus
dene:
    değişken puan = 80
    yaz("Puan:", puna)
yakala hata:
    yaz("Bir hata oldu:", hata)
```

```
Bir hata oldu: 'puna' adında bir değişken ya da fonksiyon tanımlı değil. 'puan' mı demek istediniz?
```

Yönlendirme: "Hata mı bekliyorduk, yoksa kendi yazım hatamız mı? Kendi yazım hatamızı yakalarsak ne kaybederiz?" Kural: kullanıcıdan ya da dışarıdan gelen veriyi yakalayın, kendi mantık hatalarınızı düzeltin. `dene` bloğunu küçük tutun.

### 4. `dene` içinde tanımlanan değişkeni dışarıda kullanmak

```jus
dene:
    değişken sayı1 = sayı("12")
yakala:
    yaz("sayı değil")
yaz(sayı1 + 1)
```

```
ornek.jus:5: çalışma zamanı hatası: 'sayı1' adında bir değişken ya da fonksiyon tanımlı değil. 'sayı' mı demek istediniz?
```

Blok içinde tanımlanan değişken blok bitince geçerliliğini yitirir (`eğer` ve döngü bloklarıyla aynı kural); `yakala hata:` ile tanımlanan `hata` adı da yalnızca `yakala` bloğunda geçerlidir. Hata iletisi "tanımlı değil" der; öğrenci "tanımlamıştım" der. Çözüm: değişkeni `dene`'den önce tanımlayıp bloğun içinde değerini değiştirmek:

```jus
değişken sayı1 = 0
dene:
    sayı1 = sayı("12")
yakala:
    yaz("sayı değil")
yaz(sayı1 + 1)
```

```
13
```

### 5. `üst` gibi ayrılmış bir kelimeyi değişken adı yapmak

İkili aramada doğal olan `alt` ve `üst` adları, `üst` bir ayrılmış kelime (üst sınıfı çağırmak için) olduğu için çalışmaz:

```jus
değişken alt = 1
değişken üst = 100
yaz((alt + üst) / 2)
```

```
ornek.jus:2:10: sözdizimi hatası: Değişken adı bekleniyor.
    değişken üst = 100
             ^
ornek.jus:3:12: sözdizimi hatası: 'üst' yalnızca bir sınıfın yöntemleri içinde kullanılabilir.
    yaz((alt + üst) / 2)
               ^
```

Tek hatadan üç ileti çıkar; ilk ileti ("Değişken adı bekleniyor") asıl nedeni söylemez. Bunu Hafta 2'de gördüyseniz kısa tutun: "Hangi kelimenin altında `^` var? O kelimeyi ayrılmış kelimeler tablosundan arayalım." Çözüm: `düşük` / `yüksek`.

## Çıkış bileti

1. `dene` bloğunda hata olursa bloğun kalan satırları ne olur?
2. Şu program ne yazar?
3. Hangisini `yakala` ile karşılamalı, hangisini düzeltmeli: (a) kullanıcının yazdığı `abc`'yi sayıya çevirmek, (b) kendi yazdığınız değişken adındaki harf hatası?

Soru 2'nin programı:

```jus
fonksiyon bölüm(a, b):
    dene:
        dön a / b
    yakala:
        dön -1

yaz(bölüm(8, 2))
yaz(bölüm(8, 0))
```

```
4
-1
```

Cevaplar:

1. Atlanır; `yakala` bloğu çalışır.
2. `4` ve `-1`.
3. (a) yakalanır; (b) düzeltilir.

## Ev çalışması (isteğe bağlı)

1. Hafta 5'te yazdığınız bir fonksiyona (ör. `ebob` ya da `asalMı`) `fırlat` ile girdi denetimi ekleyin (negatif sayı verilirse hata). Bir `dene` / `yakala` ile hem geçerli hem geçersiz değerle deneyin.
2. Zar yarışını iki yerine üç oyuncuya genişletin. Kazananı bulma kısmı en çok hangi satırları değiştirmenizi gerektirdi?
3. Dönem projeniz için [proje.md](proje.md) içindeki fikirlerden iki tanesini seçin ve seçme nedenlerinizi bir cümleyle yazın.
