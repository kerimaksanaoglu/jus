# Hafta 2 - Değişkenler, sayılar ve metinler

[Öğretmen kiti](README.md) | Önceki: [Hafta 1](hafta-01.md) | Sonraki: [Hafta 3 - Kararlar](hafta-03.md)

İlgili rehber bölümü: [02 - Değişkenler ve değerler](../rehber/02-degiskenler-ve-degerler.md)

Bu hafta öğrenciler "bilgiyi saklamayı" öğrenir. Haftanın en önemli fikri, `=` işaretinin matematikteki eşitlik değil **atama** olduğudur: sağdaki değeri soldaki kutuya koyar. Bu fikri kâğıt üzerinde "izleme tablosu" ile yerleştirirseniz döngülerde (Hafta 4) ve fonksiyonlarda (Hafta 5) çok zaman kazanırsınız.

## Kazanımlar

Hafta sonunda öğrenci:

1. `değişken` ile sayı, metin ve mantıksal değer saklayan değişkenler tanımlayabilir; değerini `=` ve `+=` ile değiştirebilir.
2. `tür()` ile bir değerin türünü sorgulayabilir; `"5"` ile `5` arasındaki farkı bir örnekle açıklayabilir.
3. Toplama, çıkarma, çarpma, bölme, `%` ve parantez içeren bir ifadenin sonucunu çalıştırmadan tahmin edebilir.
4. Metinleri `+` ile birleştirebilir; sayıyı metne `metin()` ile çevirmesi gerektiğini bilir.
5. Ondalık sonuçları `yuvarla` ve `biçimle` ile düzenleyebilir.

## Ön hazırlık

- Tarayıcıda deneme alanı: <https://kerimaksanaoglu.github.io/jus/>.
- Etkinlik için: her masaya üç dört küçük kâğıt kutu (ya da yapışkan not), üzerlerine yazmak için kalem. Her öğrenciye bir "izleme tablosu" kâğıdı (aşağıdaki etkinlikte tarif edilir; tahtaya çizip öğrencilerin deftere çizmesi de yeterlidir).
- Bu haftanın bir adımı (`oku`) deneme alanında çalışmaz. Dersi yalnızca tarayıcıyla yürütüyorsanız o adımı atlayın ya da tahtada gösterip geçin; haftanın hiçbir kazanımı `oku`ya bağlı değildir.
- Hafta 1'in çıkış biletinden akılda kalan en sık yanlışı (parantez, tırnak) ısınma için not edin.

## Ders akışı

| Süre | Evre | Ne yapılır |
|------|------|------------|
| 0-8 | Isınma | Tahtaya Hafta 1'deki konulardan üç `yaz` satırı yazın (bir işlem, bir metin, bir yorum). Öğrenciler çıktıyı tahmin eder. Ardından sorun: "Bir sayıyı bilgisayara bir kere söyleyip sonra adıyla çağırabilir miyiz?" |
| 8-18 | Yeni kavram | Değişken = adı olan kutu. `değişken yaş = 12` okunuşu: "yaş kutusuna 12 koy". Kutunun içi sonradan değişebilir. |
| 18-30 | Canlı kodlama 1 | Adım 1-3: değişken, `tür`, aritmetik. |
| 30-40 | Etkinlik | "Kutular ve bilgisayar" (bilgisayarsız, izleme tablosu). |
| 40-52 | Canlı kodlama 2 | Adım 4-6: metin birleştirme, `metin()`, `yuvarla` ve `biçimle`. |
| 52-72 | Öğrenci uygulaması | Eşli çalışma: Alıştırma 2.1-2.6. Hızlılar 2.7 ve 2.8'e geçer. |
| 72-76 | İsteğe bağlı | Adım 7: `oku` (yalnızca kurulu bilgisayarlarda). Kurulum yoksa tahtada gösterip geçin. |
| 76-80 | Kapanış | Çıkış bileti. |

## Canlı kodlama betiği

### Adım 1 - Değişken tanımlamak

```jus
değişken ad = "Ayşe"
değişken yaş = 12
yaz(ad)
yaz(yaş)
yaş = 13
yaz("Yeni yaş:", yaş)
```

```
Ayşe
12
Yeni yaş: 13
```

Söyleyin: "`değişken` yeni kutu açar, `=` içine değer koyar. Üçüncü deyimde `değişken` yazmadım, çünkü kutu zaten var; yalnızca içindekini değiştiriyorum."

Sorun: "`yaş = 13` satırını `değişken yaş = 13` olarak yazsak ne olur?" (Üst düzeyde aynı adı yeniden tanımlamak hata değildir; yeni değer eskisinin yerini alır. Ama bu alışkanlığı teşvik etmeyin: blok içinde aynı ad iki kez tanımlanırsa hata verir.)

### Adım 2 - Değerin türü

```jus
yaz(tür(12))
yaz(tür("12"))
yaz(tür(3.5))
yaz(tür(12 > 5))
yaz(tür(boş))
```

```
sayı
metin
sayı
mantıksal
boş
```

Söyleyin: "Bilgisayar `12` ile `"12"`nin farkını bilir. Biri hesap yapılacak bir sayı, diğeri yazıdır." Dört türü tahtaya yazdırın: `sayı`, `metin`, `mantıksal`, `boş`. Ondalıklı sayının da `sayı` olduğunu vurgulayın.

### Adım 3 - Aritmetik

```jus
yaz(10 + 4)
yaz(10 - 4)
yaz(10 * 4)
yaz(10 / 4)
yaz(10 % 4)
yaz(2 + 3 * 4)
yaz((2 + 3) * 4)

değişken sayaç = 0
sayaç += 1
sayaç += 1
yaz(sayaç)
```

```
14
6
40
2.5
2
14
20
2
```

Söyleyin: "`%` bölümden kalanı verir: 10'u 4'e bölünce kalan 2." Sorun: "`sayaç += 1` ile `sayaç = sayaç + 1` arasında fark var mı?" (Yok; ilki kısa yazımdır.) Burada izleme tablosunu tahtada gösterin: `sayaç` sütununda 0, 1, 2.

### Adım 4 - Metin birleştirme

```jus
değişken ad = "Ayşe"
değişken yaş = 12
yaz("Merhaba, " + ad + "!")
yaz(ad + " " + metin(yaş) + " yaşında.")
```

```
Merhaba, Ayşe!
Ayşe 12 yaşında.
```

Sorun: "Neden `yaş` yerine `metin(yaş)` yazdım?" Yanıt gelmeden önce `metin()` olmadan deneyin ve hata iletisini birlikte okuyun:

```jus
değişken yaş = 12
yaz("Yaş: " + yaş)
```

```
ornek.jus:2: çalışma zamanı hatası: '+' işleci iki sayı, iki metin ya da iki liste ister; metin ve sayı verildi. Dönüştürmek için metin() ya da sayı() kullanılabilir.
```

Söyleyin: "İleti çözümü de söylüyor: `metin()` kullanılabilir. JUS türleri sessizce çevirmez; hatayı erken yakalatır."

### Adım 5 - `metin()` ve `sayı()`

```jus
değişken girdi = "40"
yaz(girdi + "2")
yaz(sayı(girdi) + 2)
yaz("5" + "3")
yaz(5 + 3)
```

```
402
42
53
8
```

Sorun: "Aynı `+` işareti neden bir yerde 402, bir yerde 42 verdi?" Beklenen yanıt: iki metni birleştirir, iki sayıyı toplar. `"5" + "3"` ile `5 + 3` farkı sınıfın hatırlayacağı bir örnektir.

### Adım 6 - Ondalık sayıları düzenlemek

```jus
değişken fiyat = 19.9
değişken adet = 3
yaz(fiyat * adet)
yaz(yuvarla(1 / 3, 2))
yaz("Toplam: " + biçimle(fiyat * adet, 2) + " TL")
yaz(biçimle(2, 2))
```

```
59.7
0.33
Toplam: 59.70 TL
2.00
```

Söyleyin: "`yuvarla` sayıyı verilen basamağa yuvarlar, sonuç yine sayıdır. `biçimle` ise sayıyı verilen basamakla **metne** çevirir; `59.70` gibi sondaki sıfırı korumak için kullanılır." Para yazarken `biçimle`'yi önerin.

### Adım 7 - İsteğe bağlı: kullanıcıdan girdi almak (`oku`)

Bu adım tarayıcıdaki deneme alanında çalışmaz; JUS'un bilgisayara kurulu olduğu durumda gösterin. Terminalde program klavyeden iki satır bekler. Aşağıdaki çıktı, girdiler dışarıdan verilerek alındığı için yazdıklarınız çıktıda görünmez.

```jus
değişken ad = oku("Adınız? ")
yaz("Merhaba, " + ad + "!")
değişken yaşMetni = oku("Yaşınız? ")
değişken yaş = sayı(yaşMetni)
yaz("Gelecek yıl " + metin(yaş + 1) + " yaşında olacaksınız.")
```

Girdi satırları: `Ayşe`, `12`

```
Adınız? Merhaba, Ayşe!
Yaşınız? Gelecek yıl 13 yaşında olacaksınız.
```

Vurgulayın: `oku` **her zaman metin** verir; sayı gerekiyorsa `sayı()` ile çevrilir.

## Sınıf içi etkinlik: Kutular ve bilgisayar (bilgisayarsız, 10 dk)

Amaç: Atamayı ve bir programın satır satır ilerlediğini fiziksel olarak görmek.

Hazırlık: Tahtaya iki sütunlu bir **izleme tablosu** çizin. Sütun başlıkları programdaki değişken adları, her satır "programın bir adımı".

Program:

```jus
değişken a = 4
değişken b = a + 3
a = b * 2
b = a - 5
yaz(a, b)
```

```
14 9
```

Adımlar:

1. İki öğrenci "a kutusu" ve "b kutusu" olur; her biri elinde kâğıt tutar. Üçüncü bir öğrenci "bilgisayar"dır ve programı satır satır okur.
2. Her satırda "bilgisayar" kutuların değerini bilerek yüksek sesle hesaplar, kutu öğrencisi kâğıdındaki sayıyı **silip** yenisini yazar ("a kutusu şimdi 14").
3. Sınıf, tahtadaki tabloya her adımda yeni değerleri ekler. Doğru tablo şöyledir:

| Adım | Satır | a | b |
|------|-------|---|---|
| 1 | `değişken a = 4` | 4 | |
| 2 | `değişken b = a + 3` | 4 | 7 |
| 3 | `a = b * 2` | 14 | 7 |
| 4 | `b = a - 5` | 14 | 9 |
| 5 | `yaz(a, b)` | 14 | 9 |

4. Sınıfa programı çalıştırtın ve çıktının `14 9` olduğunu doğrulayın.
5. İkinci tur: iki kutunun içeriğini **takas** etme problemi. Sorun: "a ve b'nin değerini yer değiştirmek istiyoruz. `a = b` yazarsak ne olur?" Öğrenciler kutuyla dener: a'nın eski değeri kaybolur. Çözüm için üçüncü bir kutu (`geçici`) gerekir (Alıştırma 2.6).

Tartışma: Matematikte `a = a + 1` yanlış bir eşitliktir. Programda ise "a kutusuna, a kutusundaki sayının bir fazlasını koy" demektir.

## Alıştırmalar

Çözümler [cevap anahtarında](cevap-anahtari.md#hafta-2).

### Kolay

**Alıştırma 2.1.** Bir dikdörtgenin eni 12, boyu 5'tir. `en` ve `boy` değişkenlerini tanımlayıp alanını ve çevresini şu biçimde yazdırın.

Beklenen çıktı:

```
Alan: 60
Çevre: 34
```

**Alıştırma 2.2.** Aşağıdaki programın çıktısını çalıştırmadan tahmin edin:

```jus
değişken x = 10
x += 5
x = x * 2
x -= 4
yaz(x)
```

```
26
```

(İzleme tablosu çizmek tahmin etmeyi kolaylaştırır: `x` sırasıyla 10, 15, 30, 26 olur.)

**Alıştırma 2.3.** `ad`, `yaş` ve `şehir` değişkenlerini tanımlayıp tek `yaz` satırıyla aşağıdaki gibi bir cümle yazdırın. (Kendi bilgilerinizi kullanın; cümledeki sayıyı `metin()` ile çevirmeyi unutmayın.)

Beklenen çıktı:

```
Ben Deniz, 13 yaşındayım. Şehir: Bursa.
```

### Orta

**Alıştırma 2.4.** 25 derece Celsius'u Fahrenheit'e çevirin (formül: `C * 9 / 5 + 32`). Çıktı şöyle olsun:

```
25 derece Celsius = 77 derece Fahrenheit
```

**Alıştırma 2.5.** 135 saniyeyi dakika ve saniye olarak yazdırın. (İpucu: `taban(x)`, `x`'ten büyük olmayan en büyük tam sayıyı verir; `%` kalanı verir.)

Beklenen çıktı:

```
2 dakika 15 saniye
```

**Alıştırma 2.6.** `a` değişkeni 3, `b` değişkeni 8 olsun. Üçüncü bir değişken kullanarak değerlerini yer değiştirin. Önce ve sonra her iki değeri de yazdırın.

Beklenen çıktı:

```
Önce: 3 8
Sonra: 8 3
```

### Zor

**Alıştırma 2.7.** `n = 482` değişkeninin rakamlarını `%` ve `taban` ile ayırıp toplamlarını yazdırın. (İpucu: `n % 10` birler basamağıdır.)

Beklenen çıktı:

```
Rakamlar: 4 8 2
Rakamlar toplamı: 14
```

**Alıştırma 2.8.** Bir market fişi hazırlayın: 2 adet ekmek (7.5 TL), 1 adet süt (32.9 TL), 2 adet peynir (125.75 TL). Toplamı hesaplayın, yüzde 10 indirim uygulayın ve iki tutarı da iki ondalık basamakla yazdırın.

Beklenen çıktı:

```
Toplam: 299.40 TL
İndirimli: 269.46 TL
```

## Öğrenciler nerede takılır

### 1. Sayı ile metni `+` ile birleştirmek

```jus
değişken yaş = 12
yaz("Yaşınız: " + yaş)
```

```
ornek.jus:2: çalışma zamanı hatası: '+' işleci iki sayı, iki metin ya da iki liste ister; metin ve sayı verildi. Dönüştürmek için metin() ya da sayı() kullanılabilir.
```

İleti çözümü içerir: `metin()` kullanılabilir. Yönlendirme: "İleti hangi iki türün birleşemediğini söylüyor? Hangisini hangisine çevireceğiz?" Alternatif olarak virgülle ayırın: `yaz("Yaşınız:", yaş)` hata vermez ve her iki biçim de doğrudur.

### 2. `değişken` yazmadan ilk atama

```jus
sayaç = 1
yaz(sayaç)
```

```
ornek.jus:1: çalışma zamanı hatası: 'sayaç' adında bir değişken tanımlı değil. Yeni değişken için 'değişken sayaç = ...' yazın.
```

İleti yeni değişken için ne yazılacağını söyler. Bu, öğrencilerin kendi kendine okuyup düzeltebildiği ileti türüdür; hemen çözümü vermeyin, "ileti sana ne öneriyor?" diye sorun.

### 3. Ondalık sayıda virgül kullanmak

```jus
değişken fiyat = 12,5
yaz(fiyat)
```

```
ornek.jus:1:20: sözdizimi hatası: Deyimden sonra satır sonu bekleniyor.
    değişken fiyat = 12,5
                       ^
```

Türkçe alışkanlıkla yazılan `12,5` JUS'ta sayı değildir; ondalık ayırıcı noktadır. İleti sorunu doğrudan söylemez ("satır sonu bekleniyor"); `^` işareti virgülü gösterir. Yönlendirme: "İşaret hangi karakterin üstünde? Sayıların içinde bu karakteri JUS neye benzetiyor?"

Dikkat: Aynı hata `yaz` içinde sessiz kalır, çünkü virgül orada argümanları ayırır:

```jus
yaz("Fiyat:", 12,5)
```

```
Fiyat: 12 5
```

Çıktı `Fiyat: 12 5` olur; hata iletisi çıkmaz. Öğrenci "5 nereden geldi" diye şaşırır.

### 4. Yerleşik bir fonksiyonun adını değişken yapmak

`sayı`, `metin`, `tür`, `ekle`, `sil`, `bul`, `al`, `ters` gibi adlar yerleşik fonksiyonlara aittir. Üst düzeyde bir değişkene aynı ad verilebilir; hata verilmez, ama o fonksiyon artık çağrılamaz.

```jus
değişken sayı = 5
yaz(sayı("3") + 1)
```

```
ornek.jus:2: çalışma zamanı hatası: Yalnızca fonksiyonlar çağrılabilir; bu değerin türü sayı.
```

İleti yanıltıcıdır: öğrenci `sayı("3")`'ü bir fonksiyon çağrısı olarak yazdığını bilir; "türü sayı" ifadesinin kendi değişkenini kastettiğini fark etmez. Yönlendirme: "Bu programda `sayı` adını ilk satırda ne yaptık?" Çözüm: değişkene başka bir ad verin (`sayı1`, `adet` gibi). Öğrencilere, yerleşik fonksiyon adlarını kullanmamaları için liste vermek yerine şunu söyleyin: "Bir ad hem fonksiyon hem değişken gibi görünüyorsa değişkenin adını değiştirin."

### 5. Ayrılmış bir kelimeyi değişken adı yapmak

JUS'un 25 ayrılmış kelimesi vardır (`değişken`, `eğer`, `iken`, `her`, `dön`, `geç`, `sınıf`, `üst`, `bu`, `ve`, `veya`, `değil`, `boş` ve diğerleri; tam liste kitin [ana sayfasındaki](README.md) "Ayrılmış kelimeler" bölümündedir). Öğrenciler günlük dilde çok kullandıkları `sınıf`, `geç`, `dön`, `üst` gibi kelimeleri değişken adı yapmak ister:

```jus
değişken sınıf = 7
yaz(sınıf)
```

```
ornek.jus:1:10: sözdizimi hatası: Değişken adı bekleniyor.
    değişken sınıf = 7
             ^
ornek.jus:2:5: sözdizimi hatası: İfade bekleniyor.
    yaz(sınıf)
        ^
```

İleti yanıltıcıdır: "Değişken adı bekleniyor" der, ama `sınıf` kelimesinin ayrılmış olduğunu söylemez; ayrıca aynı hatadan ikinci bir ileti daha üretir. Öğrenci "ad yazdım ki" der. Yönlendirme: "`^` işareti hangi kelimenin altında? O kelimeyi başka bir yerde kullandığımızda ne iş yapıyordu?" Çözüm: `sınıfı`, `sınıfNo`, `seviye` gibi başka bir ad. Deneyiminize göre bu hatayı tahtaya "yasak kelimeler" listesi olarak asabilirsiniz.

Aynı sınıfta büyük-küçük harf farkı da (`yaş` ile `Yaş`) hata kaynağıdır:

```jus
değişken yaş = 12
yaz(Yaş)
```

```
ornek.jus:2: çalışma zamanı hatası: 'Yaş' adında bir değişken ya da fonksiyon tanımlı değil. 'yaş' mı demek istediniz?
```

Türkçe klavyede `İ/I` ve `ı/i` karışıklığına özellikle dikkat ettirin.

## Çıkış bileti

1. Aşağıdaki program ne yazar?
2. `yaz("5" + "3")` ve `yaz(5 + 3)` ne yazar? Neden farklı?
3. `yaz("Yaş: " + 12)` neden hata verir? Nasıl düzeltilir?

Soru 1'in programı:

```jus
değişken a = 5
a = a + 2
yaz(a)
```

```
7
```

Cevaplar:

1. `7`.
2. `53` ve `8`. İlkinde iki metin birleşir, ikincisinde iki sayı toplanır.
3. Metin ile sayı `+` ile birleşmez. `yaz("Yaş: " + metin(12))` ya da `yaz("Yaş:", 12)` yazılır.

## Ev çalışması (isteğe bağlı)

1. Hafta 1'deki tanıtım programınızı `ad`, `sınıf`, `yaş` değişkenleriyle yeniden yazın; çıktı aynı kalsın.
2. Yaşınızı bir değişkene yazın ve yaklaşık kaç gün yaşadığınızı (yaş x 365) hesaplayıp yazdırın.
3. Kendi seçtiğiniz bir formülü (bir kenarı verilen karenin alanı, bir sürat-zaman-yol problemi) değişkenlerle yazın. Değerleri değiştirip sonucun nasıl değiştiğini defterinize not edin.
