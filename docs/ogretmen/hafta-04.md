# Hafta 4 - Döngüler: `iken` ve `her`

[Öğretmen kiti](README.md) | Önceki: [Hafta 3](hafta-03.md) | Sonraki: [Hafta 5 - Fonksiyonlar](hafta-05.md)

İlgili rehber bölümü: [04 - Döngüler](../rehber/04-donguler.md)

Döngü, bir işi tekrar tekrar yazmak yerine bilgisayara "bunu şu kadar kez ya da şu koşul sürdükçe yap" demektir. Bu hafta üç kalıbı yerleştirmeye çalışın: **sayma** (`her i içinde aralık(...)`), **biriktirme** (toplam, sayaç) ve **koşul bitene kadar** (`iken`). Öğrencilerin çoğu için en zor şey, döngünün içinde değişkenlerin tur tur değiştiğini zihinde izlemektir; bu yüzden hafta boyunca izleme tablosunu kullanın.

## Kazanımlar

Hafta sonunda öğrenci:

1. `iken` ile bir koşul doğru olduğu sürece tekrar eden bir döngü yazabilir; koşulu değiştiren satırın neden gerekli olduğunu açıklayabilir.
2. `her ... içinde aralık(...)` ile belirli sayıda tekrar yazabilir; `aralık(baş, son)` çağrısının `son` değerini içermediğini bilir.
3. Toplam, sayaç ve "şimdiye kadarki en büyük" kalıplarını döngüyle uygulayabilir.
4. `kır` ile döngüyü erken bitirebilir, `devam` ile bir turu atlayabilir.
5. Bir döngünün ilk turlarını kâğıtta izleme tablosuyla izleyip sonucu tahmin edebilir.

## Ön hazırlık

- Tarayıcıda deneme alanı: <https://kerimaksanaoglu.github.io/jus/>. Sayfada "Durdur" düğmesi vardır; bir program sonsuza kadar dönerse onunla durdurulur. Dersin başında düğmenin yerini gösterin. Terminalde çalışan sınıflarda aynı iş Ctrl+C iledir.
- İzleme tablosu için tahtada boş bir tablo hazır olsun.
- Etkinlik için sınıf bir çember ya da sıra oluşturabilmeli.
- Hafta 3'ün `%` ile bölünebilirlik kalıbını ısınmada yineleyin: `n % 3 == 0`.

## Ders akışı

| Süre | Evre | Ne yapılır |
|------|------|------------|
| 0-6 | Isınma | Tahtada `yaz("Merhaba")` satırını beş kez yazın (kopyalama). Sorun: "Bunu 1000 kere yazmak istesek?" |
| 6-12 | Yeni kavram | Döngü = tekrar. Üç kalıp: sayma, biriktirme, koşul sürdükçe. |
| 12-26 | Canlı kodlama 1 | Adım 1-4: `aralık`, toplam, `iken`. |
| 26-38 | Etkinlik | FizzBuzz çemberi ve izleme tablosu (bilgisayarsız). |
| 38-50 | Canlı kodlama 2 | Adım 5-7: `kır`, `devam`, iç içe döngü, sonsuz döngü belirtisi. |
| 50-72 | Öğrenci uygulaması | Alıştırma 4.1-4.6, eşli. Hızlılar 4.7-4.9'a geçer. |
| 72-76 | Paylaşım | Bir çift Collatz dizisini (4.9) tahtada izleme tablosuyla gösterir. |
| 76-80 | Kapanış | Çıkış bileti. |

## Canlı kodlama betiği

### Adım 1 - Sayarak tekrar

```jus
her i içinde aralık(5):
    yaz("Merhaba", i)
```

```
Merhaba 0
Merhaba 1
Merhaba 2
Merhaba 3
Merhaba 4
```

Söyleyin: "`aralık(5)` 0'dan başlayıp 5'e **kadar** sayar, 5 dahil değil. `i` her turda sıradaki sayıyı alır. Girintili satır her turda çalışır."

Sorun: "Kaç tur döndü? Son tur hangi sayıyla?" (5 tur; sonuncusu 4.) İsteyen öğrenci `aralık(5)`'in kendisine bakmak için `yaz(aralık(5))` yazabilir:

```jus
yaz(aralık(5))
yaz(aralık(1, 6))
yaz(aralık(10, 0, -3))
```

```
[0, 1, 2, 3, 4]
[1, 2, 3, 4, 5]
[10, 7, 4, 1]
```

Söyleyin: "`aralık(baş, son)`: baştan sona kadar, son dahil değil. Üçüncü sayı adımdır; eksi olursa geriye sayar." Köşeli parantezli bu yapıya Hafta 6'da liste diyeceğiz.

### Adım 2 - Biriktirme: toplam

```jus
değişken toplam = 0
her sayı2 içinde aralık(1, 11):
    toplam += sayı2
yaz("1'den 10'a toplam:", toplam)
```

```
1'den 10'a toplam: 55
```

Burada izleme tablosunu çizdirin (ilk dört tur yeterlidir):

| Tur | `sayı2` | `toplam` (tur sonunda) |
|-----|---------|-------------------------|
| 1 | 1 | 1 |
| 2 | 2 | 3 |
| 3 | 3 | 6 |
| 4 | 4 | 10 |

Sorun: "`toplam = 0` satırını döngünün içine alırsak ne olur?" (Hafta içinde "nerede takılır" bölümünde bu hata var.)

### Adım 3 - `iken` ile gerisayım

```jus
değişken n = 5
iken n > 0:
    yaz(n)
    n -= 1
yaz("Fırlatıldı!")
```

```
5
4
3
2
1
Fırlatıldı!
```

Söyleyin: "`iken` koşul doğru olduğu sürece tekrar eder. `n -= 1` olmasaydı koşul hiç değişmez, döngü hiç bitmezdi." Bu haftanın en önemli cümlesi: **döngünün içinde koşulu değiştiren bir şey olmalı.**

Sorun: "Ne zaman `her`, ne zaman `iken`?" Yanıt: kaç kez döneceğimizi baştan biliyorsak `her ... aralık`, "şu olana kadar" diyorsak `iken`.

### Adım 4 - `iken` ile "şu olana kadar"

```jus
değişken kuvvet = 1
iken kuvvet <= 1000:
    yaz(kuvvet)
    kuvvet *= 2
```

```
1
2
4
8
16
32
64
128
256
512
```

Sorun: "Kaç tur döneceğini önceden biliyor muyduk?" Hayır; bu yüzden `iken` uygun.

### Adım 5 - `kır` ve `devam`

```jus
her i içinde aralık(1, 21):
    eğer i % 2 == 0:
        devam
    yaz(i)
    eğer i >= 9:
        kır
yaz("Döngü bitti")
```

```
1
3
5
7
9
Döngü bitti
```

Söyleyin: "`devam`: bu turun kalanını atla, sonraki tura geç. `kır`: döngüden tamamen çık." Sorun: "20'ye kadar sayacaktık ama neden 9'da durduk? Çift sayılar neden yazılmadı?"

### Adım 6 - İç içe döngü

```jus
her i içinde aralık(1, 6):
    değişken satır = ""
    her j içinde aralık(i):
        satır = satır + "*"
    yaz(satır)
```

```
*
**
***
****
*****
```

Söyleyin: "İçteki döngü bir satırı doldurur, dıştaki döngü satırları sayar. `i` 3 olduğunda içteki döngü 3 tur döner." Öğrencilerden `i = 3` için içteki döngünün tablosunu çizmelerini isteyin.

### Adım 7 - Sonsuz döngünün belirtisi

Gerçek bir sonsuz döngü ekranı doldurur; bu yüzden belirtiyi sınırlı bir sürümle gösterelim. Sayacı artırmayı unutan program her turda aynı şeyi yazar:

```jus
değişken i = 0
değişken tur = 0
iken i < 3:
    yaz("i =", i)
    tur += 1
    eğer tur == 5:
        yaz("5 turdur i değişmiyor, durduruyorum")
        kır
```

```
i = 0
i = 0
i = 0
i = 0
i = 0
5 turdur i değişmiyor, durduruyorum
```

Sorun: "`i` neden hep 0? Hangi satır eksik?" (`i += 1`.) Öğrencilere, programları gerçekten sonsuz döngüye girdiğinde "Durdur" düğmesine basmalarını (terminalde Ctrl+C) ve **hata yapmanın normal olduğunu** söyleyin.

## Sınıf içi etkinlik: FizzBuzz çemberi ve izleme tablosu (bilgisayarsız, 12 dk)

Amaç: Bir döngünün her turunda aynı kuralın uygulandığını yaşamak; kuralı koşula çevirmek; döngüyü kâğıtta izlemek.

**Bölüm 1 - FizzBuzz çemberi (6 dk).** Sınıf çember olur. Sırayla 1'den saymaya başlarlar. Kurallar:

- 3'ün katı olan sayıda "Fizz" denir.
- 5'in katı olan sayıda "Buzz" denir.
- Hem 3'ün hem 5'in katı olan sayıda "FizzBuzz" denir.
- Hata yapan oturur.

Oyundan sonra sorun: "Bu kuralı bilgisayara nasıl söylerdik?" Öğrenciler `sayı % 3 == 0` ve `sayı % 5 == 0` koşullarını önerir; "ikisi birden" için `ve` gerektiğini kendileri bulur. Hangi koşulun **önce** kontrol edilmesi gerektiğini tartışın (Hafta 3, Adım 5'te gördükleri sıra sorunu!). Bu bilgi Alıştırma 4.6'da kullanılır.

**Bölüm 2 - İzleme tablosu (6 dk).** Aşağıdaki programı öğrenciler kâğıtta izler (kodu tahtaya yazın, çıktıyı sonra gösterin):

```jus
değişken toplam = 0
her i içinde aralık(1, 5):
    toplam += i
    yaz("i =", i, "toplam =", toplam)
yaz("Sonuç:", toplam)
```

```
i = 1 toplam = 1
i = 2 toplam = 3
i = 3 toplam = 6
i = 4 toplam = 10
Sonuç: 10
```

Tablo sütunları: tur, `i`, `toplam`. Öğrenciler tabloyu doldurduktan sonra çıktıyla karşılaştırır. `aralık(1, 5)` neden 5'i içermiyor ve sonuç neden 10? (1+2+3+4.)

## Alıştırmalar

Çözümler [cevap anahtarında](cevap-anahtari.md#hafta-4).

### Kolay

**Alıştırma 4.1.** `aralık` kullanarak 1'den 10'a kadar sayıları alt alta yazdırın.

Beklenen çıktı:

```
1
2
3
4
5
6
7
8
9
10
```

**Alıştırma 4.2.** 5'ten 1'e geriye sayın ve sonunda `Fırlatıldı!` yazdırın.

Beklenen çıktı:

```
5
4
3
2
1
Fırlatıldı!
```

**Alıştırma 4.3.** 7'nin çarpım tablosunu `7 x 1 = 7` biçiminde 1'den 10'a kadar yazdırın.

Beklenen çıktı:

```
7 x 1 = 7
7 x 2 = 14
7 x 3 = 21
7 x 4 = 28
7 x 5 = 35
7 x 6 = 42
7 x 7 = 49
7 x 8 = 56
7 x 9 = 63
7 x 10 = 70
```

### Orta

**Alıştırma 4.4.** 1'den 100'e kadar olan sayılardan 3'e **ya da** 5'e bölünenlerin toplamını bulup yazdırın.

Beklenen çıktı:

```
2418
```

**Alıştırma 4.5.** `iken` kullanarak 3'ün katlarını (3, 6, 9, ...) 40'ı geçmeden yazdırın.

Beklenen çıktı:

```
3
6
9
12
15
18
21
24
27
30
33
36
39
```

**Alıştırma 4.6 - FizzBuzz.** 1'den 15'e kadar sayıları yazdırın; 3'ün katı için sayı yerine `Fizz`, 5'in katı için `Buzz`, ikisinin katı için `FizzBuzz` yazdırın.

Beklenen çıktı:

```
1
2
Fizz
4
Buzz
Fizz
7
8
Fizz
Buzz
11
Fizz
13
14
FizzBuzz
```

### Zor

**Alıştırma 4.7 - Üçgen.** İç içe iki döngüyle şu şekli yazdırın (ilk satırda 1, son satırda 5 yıldız):

```
*
**
***
****
*****
```

**Alıştırma 4.8 - Asal mı?** `n = 29` için sayının asal olup olmadığını döngüyle bulun: 2'den `n - 1`'e kadar sayılarda `n`'i tam bölen varsa asal değildir. Çıktı `29 asaldır.` ya da `... asal değildir.` olsun. Programı 30, 97, 91, 2 ve 1 ile de deneyin. (İpucu: bir `asal` değişkeni olsun; bölen bulunca `yanlış` yapıp `kır` ile çıkın. 1'in asal sayılmadığını unutmayın.)

Beklenen çıktılar:

n = 29 için:

```
29 asaldır.
```

n = 30 için:

```
30 asal değildir.
```

n = 97 için:

```
97 asaldır.
```

n = 91 için:

```
91 asal değildir.
```

n = 2 için:

```
2 asaldır.
```

n = 1 için:

```
1 asal değildir.
```

**Alıştırma 4.9 - Collatz dizisi.** `n = 6` ile başlayın. `n` çiftse ikiye bölün, tekse üçle çarpıp bir ekleyin; `n` 1 olana kadar her değeri yazdırın ve kaç adımda 1'e ulaşıldığını bildirin.

Beklenen çıktı:

```
6
3
10
5
16
8
4
2
1
Adım sayısı: 8
```

## Öğrenciler nerede takılır

### 1. `aralık`'ın bitiş değerini içerdiğini sanmak

Hata iletisi çıkmaz; sonuç beklenenden bir eksik gelir.

```jus
her i içinde aralık(1, 4):
    yaz(i)
```

```
1
2
3
```

Öğrenci "1'den 4'e kadar yazdırmak istedim ama 4 yok" der. Yönlendirme: "`aralık(1, 4)` sayıları nerede bırakıyor? Sonu dahil mi?" Çözüm `aralık(1, 5)`. Hatırlatma: `aralık(5)` beş tur sayar (0'dan 4'e).

### 2. Sayı üzerinde `her` kullanmak

```jus
her i içinde 5:
    yaz(i)
```

```
ornek.jus:1: çalışma zamanı hatası: 'her' döngüsü liste, metin ya da sözlük üzerinde gezinir; sayı verildi.
```

İleti `her`'in neyi gezebileceğini söyler (liste, metin, sözlük). Beş kez tekrar için `aralık(5)` gerekir. Bu hata genellikle "5 kere yaz" cümlesinin doğrudan çevrilmesinden gelir.

### 3. Toplamı döngünün içinde sıfırlamak

```jus
her i içinde aralık(1, 5):
    değişken toplam = 0
    toplam += i
yaz("Toplam:", toplam)
```

```
ornek.jus:4: çalışma zamanı hatası: 'toplam' adında bir değişken ya da fonksiyon tanımlı değil.
```

İleti `toplam`'ın tanımlı olmadığını söyler: döngü içinde tanımlanan değişken döngü bitince yok olur. Öğrenci "ama tanımlamıştım" der. Başka bir öğrenci aynı yanlışı iletisiz yapar:

```jus
değişken toplam = 0
her i içinde aralık(1, 5):
    toplam = 0
    toplam += i
yaz("Toplam:", toplam)
```

```
Toplam: 4
```

Son program hata vermez ama `10` yerine `4` yazar: her turda toplam sıfırlandığı için yalnızca son sayı kalır. Yönlendirme: izleme tablosu. "Her tur sonunda `toplam` kaç? 3. turda kaçtı?" Kural: biriktirdiğiniz değişkeni döngüden **önce** başlatın.

### 4. Döngü değişkenini döngü dışında kullanmak

```jus
her i içinde aralık(3):
    yaz(i)
yaz("Döngüden sonra:", i)
```

```
0
1
2
ornek.jus:3: çalışma zamanı hatası: 'i' adında bir değişken ya da fonksiyon tanımlı değil.
```

Döngü değişkeni yalnızca döngünün içinde geçerlidir. Son değeri dışarıda kullanmak isteyen öğrenci, değeri döngüden önce tanımlanmış bir değişkene aktarmalıdır (`son = i`).

### 5. Sayacı artırmayı unutmak

```jus
değişken i = 0
değişken tur = 0
iken i < 3:
    yaz("i =", i)
    tur += 1
    eğer tur == 3:
        yaz("i hâlâ", i, "- sayacı artırmayı unuttuk")
        kır
```

```
i = 0
i = 0
i = 0
i hâlâ 0 - sayacı artırmayı unuttuk
```

Gerçek programda bu koruma yoktur ve döngü sonsuza kadar sürer; deneme alanında "Durdur" düğmesine, terminalde Ctrl+C'ye basılır. Yönlendirme: "Koşulda hangi değişken var? O değişken döngünün içinde değişiyor mu?" Hata iletisi çıkmayan, programın "takıldığı" bu durum öğrencileri korkutur; kısa sürede kendi başlarına tanımaları için ilk seferinde birlikte çözün.

## Çıkış bileti

1. Aşağıdaki program ne yazar?
2. `aralık(2, 6)` hangi sayıları üretir?
3. Bir `iken` döngüsü hiç bitmiyor. İlk olarak neye bakarsınız?

Soru 1 ve 2 için programlar:

```jus
her i içinde aralık(3):
    yaz(i * 2)
```

```
0
2
4
```

```jus
yaz(aralık(2, 6))
```

```
[2, 3, 4, 5]
```

Cevaplar:

1. `0`, `2`, `4` (her satırda bir sayı).
2. `2, 3, 4, 5`. Bitiş değeri (6) dahil değildir.
3. Koşuldaki değişkenin döngü içinde değişip değişmediğine; değişmiyorsa koşulu değiştiren satırı (örneğin `i += 1`) eklemeye bakılır.

## Ev çalışması (isteğe bağlı)

1. 1'den 50'ye kadar sayıların toplamını döngüyle hesaplayın. Sonucu `50 * 51 / 2` formülüyle kontrol edin.
2. İstediğiniz sayının çarpım tablosunu yazan bir program yazın; sayıyı bir değişkenden alsın. Üç farklı sayıyla deneyin.
3. Bir sayının kaç basamaklı olduğunu `iken` ile bulun (sayıyı 10'a bölüp `taban` alarak 0'a kadar inin). `n = 12345` için sonuç 5 olmalı.
