# Hafta 7 - Metin işlemleri

[Öğretmen kiti](README.md) | Önceki: [Hafta 6](hafta-06.md) | Sonraki: [Hafta 8 - Sözlükler](hafta-08.md)

İlgili rehber bölümü: [07 - Metinler](../rehber/07-metinler.md)

Metin, karakterlerden oluşan bir "liste" gibi düşünülebilir: dizin, dilim, `uzunluk` ve `her ... içinde` listelerdeki gibi çalışır. Bu benzerliği kullanın; Hafta 6'daki bilgi doğrudan taşınır. Bu haftanın Türkçe'ye özgü zenginliği: `büyük_harf` ve `küçük_harf` Türkçe kurallarını (i/İ, ı/I) uygular; sıralama Türk alfabesine göredir. Başka bir dilde bu iki işte çoğu zaman özel çaba gerekir; sınıfta bunu fark ettirin.

## Kazanımlar

Hafta sonunda öğrenci:

1. `uzunluk`, dizin ve dilimle bir metnin karakterlerine ve parçalarına erişebilir; metnin değiştirilemediğini bilir.
2. `böl`, `birleştir`, `kırp`, `değiştir` ve `bul` ile bir cümleyi işleyebilir (ör. kelime sayma).
3. `büyük_harf` ve `küçük_harf` ile Türkçe harf dönüşümlerini yapabilir; büyük-küçük harf farkı olmadan metin karşılaştırabilir.
4. `her harf içinde metin` ile bir metni gezerek harf sayabilir.
5. `tekrarla`, `sola_doldur` ve `sağa_doldur` ile hizalı çıktı üretebilir.

## Ön hazırlık

- Tarayıcıda deneme alanı: <https://kerimaksanaoglu.github.io/jus/>. Türkçe karakter yazımında klavye sorunu olabilecek öğrenciler için (özellikle tabletlerde) önceden denemeniz iyi olur.
- Sezar şifresi etkinliği için: her öğrenciye kâğıt şerit ya da A4 üzerine yazılmış Türk alfabesi (29 harf): `a b c ç d e f g ğ h ı i j k l m n o ö p r s ş t u ü v y z`. İki şerit ya da bir şerit ve kesilmiş ikinci bir şerit.
- Ders sonunda alfabetik sıralama gösterisi için öğrencilerin ad listesi.

## Ders akışı

| Süre | Evre | Ne yapılır |
|------|------|------------|
| 0-6 | Isınma | Hafta 6'dan: `l = [10, 20, 30]` için `l[0]`, `l[-1]`, `l[0:2]`. "Aynısı bir kelimede de çalışsa?" |
| 6-12 | Yeni kavram | Metin = karakter dizisi. Dizin, dilim, uzunluk aynı. Değiştirilemez: yeni metin üretilir. |
| 12-26 | Canlı kodlama 1 | Adım 1-4: dizin/dilim, Türkçe harf, `böl`/`birleştir`, `kırp`/`değiştir`/`bul`. |
| 26-38 | Etkinlik | Sezar şifresi çarkı (bilgisayarsız). |
| 38-50 | Canlı kodlama 2 | Adım 5-7: harf harf gezme, ilk harfi büyütme, hizalama, alfabetik sıralama. |
| 50-72 | Öğrenci uygulaması | Alıştırma 7.1-7.6, eşli. Hızlılar 7.7-7.9'a geçer. |
| 72-76 | Paylaşım | Bir çift 7.5'te `ı`/`İ` sorununu anlatır. |
| 76-80 | Kapanış | Çıkış bileti. |

## Canlı kodlama betiği

### Adım 1 - Dizin, dilim, uzunluk

```jus
değişken m = "merhaba"
yaz(uzunluk(m))
yaz(m[0])
yaz(m[-1])
yaz(m[1:4])
yaz(m[:3])
yaz(m[4:])
yaz(uzunluk("çay"))
```

```
7
m
a
erh
mer
aba
3
```

Söyleyin: "Metin de sıfırdan numaralanır. `uzunluk("çay")` 3'tür: JUS harfleri bayt değil karakter sayar, Türkçe harfler tek harftir." Sorun: "`m[1:4]` hangi harfleri verir? Dördüncü dizindeki harf dahil mi?" (Hayır; liste dilimiyle aynı.)

### Adım 2 - Türkçe büyük-küçük harf

```jus
yaz(büyük_harf("istanbul ışık"))
yaz(küçük_harf("IĞDIR İZMİR"))
yaz("Ankara" == "ankara")
yaz(küçük_harf("Ankara") == küçük_harf("ANKARA"))
```

```
İSTANBUL IŞIK
ığdır izmir
yanlış
doğru
```

Söyleyin: "`i` büyüyünce `İ`, `ı` büyüyünce `I` oluyor; Türkçe'nin kuralı bu. JUS bunu bilir." Karşılaştırmada büyük-küçük harfi yok saymak için iki tarafı da `küçük_harf`'e çevirdik. Üçüncü satır `yanlış` çıktı: büyük-küçük harf farkı karşılaştırmayı bozar.

### Adım 3 - Bölmek ve birleştirmek

```jus
değişken cümle = "bir iki üç dört beş"
değişken kelimeler = böl(cümle, " ")
yaz(kelimeler)
yaz("Kelime sayısı:", uzunluk(kelimeler))
yaz(birleştir(kelimeler, "-"))
yaz(kelimeler[2])
```

```
["bir", "iki", "üç", "dört", "beş"]
Kelime sayısı: 5
bir-iki-üç-dört-beş
üç
```

Söyleyin: "`böl` metni belirtilen ayraçtan keser ve **liste** verir. `birleştir` listeyi tekrar metne çevirir. Hafta 6'da öğrendiğimiz liste bilgisi burada devreye giriyor."

### Adım 4 - Temizlemek ve aramak

```jus
değişken ham = "   Merhaba Dünya   "
yaz("[" + kırp(ham) + "]")
yaz(değiştir("kedi kedi kedi", "kedi", "köpek"))
yaz(bul("elma armut", "armut"))
yaz(bul("elma armut", "muz"))
yaz("arm" içinde "elma armut")
yaz(başlar_mı("merhaba", "mer"), biter_mi("merhaba", "ba"))
```

```
[Merhaba Dünya]
köpek köpek köpek
5
-1
doğru
doğru doğru
```

Söyleyin: "`kırp` baştaki ve sondaki boşlukları atar; köşeli parantezleri boşluğu görmek için kullandım. `bul` alt metnin ilk geçtiği dizini verir, yoksa `-1`."

### Adım 5 - Metni harf harf gezmek

```jus
fonksiyon harfSay(m, aranan):
    değişken adet = 0
    her harf içinde m:
        eğer harf == aranan:
            adet += 1
    dön adet

yaz(harfSay("kardeşlerimiz", "e"))
yaz(harfSay("kardeşlerimiz", "r"))
yaz(harfSay("kardeşlerimiz", "x"))
```

```
2
2
0
```

Söyleyin: "`her harf içinde m` metindeki karakterleri sırayla verir; liste gezmeyle aynı. Hafta 4'teki sayaç kalıbını, Hafta 5'teki fonksiyonla birleştirdik."

### Adım 6 - İlk harfi büyütmek

```jus
fonksiyon başHarfBüyük(kelime):
    eğer uzunluk(kelime) == 0:
        dön kelime
    dön büyük_harf(kelime[0]) + kelime[1:]

yaz(başHarfBüyük("istanbul"))
yaz(başHarfBüyük("ışık"))
yaz("[" + başHarfBüyük("") + "]")
```

```
İstanbul
Işık
[]
```

Sorun: "Neden `ışık`ın ilk harfi `I` oldu, `İ` değil?" (Çünkü Türkçe kuralı: `ı` büyüyünce `I`.) Boş metin kontrolünü neden yaptık? (`""[0]` sınır dışı hata verirdi.)

### Adım 7 - Hizalama ve alfabetik sıralama

```jus
yaz(tekrarla("-", 20))
yaz(sağa_doldur("Ekmek", 10) + sola_doldur(biçimle(7.5, 2), 8))
yaz(sağa_doldur("Süt", 10) + sola_doldur(biçimle(32, 2), 8))
yaz(tekrarla("-", 20))

yaz("çay" < "dağ", "ırmak" < "iz")
yaz(sırala(["Şule", "İpek", "Ali", "Çağla", "Ece", "Ömer", "Ilgaz"]))
```

```
--------------------
Ekmek         7.50
Süt          32.00
--------------------
doğru doğru
["Ali", "Çağla", "Ece", "Ilgaz", "İpek", "Ömer", "Şule"]
```

Söyleyin: "`sağa_doldur` metni sola yaslayıp sağına boşluk ekler; `sola_doldur` tersini yapar. Böylece sütunlar hizalanır. `sırala` ve `<` Türk alfabesine göre çalışır: `Ç`, `C`'den sonra; `Ilgaz`, `İpek`ten önce gelir (`ı` alfabede `i`'den önce)."

## Sınıf içi etkinlik: Sezar şifresi çarkı (bilgisayarsız, 12 dk)

Amaç: Metni karakter karakter işlemeyi, "dizin + kaydırma" fikrini ve gerektiğinde başa dönmeyi (`%`) yaşamak.

Hazırlık: Her çiftin elinde Türk alfabesi yazılı iki kâğıt şerit var (29 harf). Üst şerit sabit, alt şerit sağa ya da sola kaydırılabilir. Alfabenin sırası: `a b c ç d e f g ğ h ı i j k l m n o ö p r s ş t u ü v y z`.

1. Alt şeridi 3 harf kaydırın: üst şeritteki `a`'nın altında `d` olsun. Şifreleme kuralı: düz metindeki harfi üst şeritte bulun, altındaki harfi yazın. `a` yerine `d`, `b` yerine `e` olur.
2. Her öğrenci kendi adını şifreler ve arkadaşına verir. Arkadaşı alt şeridi ters yönde kaydırarak çözer.
3. Tartışma: Alfabenin sonundaki `z` harfi için alt şeritte hangi harf gelir? Şerit bittiği için başa dönmek gerekir. Bunu bilgisayara nasıl söyleriz? Öğrenciler "29'a gelince tekrar 0'dan başla" demeyi bulur; bu `%` işlemidir.
4. Harfin sıra numarasını yazarak formüle çevirin: yeni sıra numarası = (eski sıra numarası + kaydırma) `%` 29. Alıştırma 7.9 bunu programlar.

Gözlem: Programı yazmadan önce kâğıtta kuralı bulmak, kodu yazmayı çok kolaylaştırır.

## Alıştırmalar

Çözümler [cevap anahtarında](cevap-anahtari.md#hafta-7).

### Kolay

**Alıştırma 7.1.** `m = "merhaba"` için ilk harfi, son harfi, uzunluğu ve ortadaki üç harfi (`m[2:5]`) yazdırın.

Beklenen çıktı:

```
İlk harf: m
Son harf: a
Uzunluk: 7
Ortadaki üç harf: rha
```

**Alıştırma 7.2.** `"bir iki üç dört beş altı"` cümlesindeki kelime sayısını `Kelime sayısı: 6` biçiminde yazdırın.

Beklenen çıktı:

```
Kelime sayısı: 6
```

**Alıştırma 7.3.** `ad = "ayşe"` ve `soyad = "yılmaz"` için `AYŞE YILMAZ` yazdırın.

Beklenen çıktı:

```
AYŞE YILMAZ
```

### Orta

**Alıştırma 7.4.** Bir metinde belirli bir harfin kaç kez geçtiğini veren `harfSay(m, aranan)` fonksiyonunu yazın. `"kardeşlerimiz"` içinde `e`, `r` ve `x` için çağırın.

Beklenen çıktı:

```
2
2
0
```

**Alıştırma 7.5.** `"ışıl ışıl parlayan istanbul"` cümlesindeki her kelimenin ilk harfini büyütüp tek cümle olarak yazdırın.

Beklenen çıktı:

```
Işıl Işıl Parlayan İstanbul
```

**Alıştırma 7.6 - Palindrom.** Baştan ve sondan okunuşu aynı olan kelimeye palindrom denir. `palindromMu(m)` fonksiyonu yazın (İpucu: `ters` metinle de çalışır). `"kayak"`, `"araba"` ve `"radar"` için sonucu yazdırın.

Beklenen çıktı:

```
doğru
yanlış
doğru
```

### Zor

**Alıştırma 7.7 - Fiş.** Aşağıdaki fişi yazdırın. Ürün adları 12 karakter genişliğinde sola yaslı, tutarlar 8 karakter genişliğinde sağa yaslı ve iki ondalık basamaklı olsun. Çizgiler `tekrarla` ile yapılsın.

Beklenen çıktı:

```
====================
        FİŞ
--------------------
Ekmek           7.50
Süt            32.00
Peynir        125.75
--------------------
TOPLAM        165.25
```

**Alıştırma 7.8 - Sesli harfler.** `"Çanakkale Boğazı"` metnindeki sesli harflerin (`a e ı i o ö u ü`) sayısını bulun. (İpucu: önce `küçük_harf`'e çevirin; sesli harfleri bir metin olarak tutup `içinde` ile denetleyin.)

Beklenen çıktı:

```
Sesli harf sayısı: 7
```

**Alıştırma 7.9 - Sezar şifresi.** `alfabe = "abcçdefgğhıijklmnoöprsştuüvyz"` metnini kullanarak `sezar(m, kaydır)` fonksiyonunu yazın: metindeki her harf için `bul(alfabe, harf)` ile dizinini bulun, `(dizin + kaydır) % 29` yeni dizindir. Alfabede olmayan karakterler (boşluk gibi) değişmeden kalsın. `"merhaba dünya"` metnini 3 kaydırmayla şifreleyin, sonucu 26 kaydırmayla (yani -3'e denk) geri çözün.

Beklenen çıktı:

```
öğtjçdç gzpbç
merhaba dünya
29
```

## Öğrenciler nerede takılır

### 1. Metnin bir harfini değiştirmeye çalışmak

```jus
değişken m = "kedi"
m[0] = "p"
yaz(m)
```

```
ornek.jus:2: çalışma zamanı hatası: Metinler değiştirilemez; yeni bir metin oluşturun.
```

Metinler değiştirilemez. Yönlendirme: "Liste gibi görünüyor ama farkı ne?" Çözüm: yeni bir metin üretilir: `m = "p" + m[1:]`.

### 2. Metin işlemini nokta ile çağırmak

```jus
değişken m = "merhaba"
yaz(m.büyük_harf())
```

```
ornek.jus:2: çalışma zamanı hatası: metin türündeki değerlerin 'büyük_harf' adında bir yöntemi yok. 'büyük_harf' bir yerleşik fonksiyondur; büyük_harf(metin) biçiminde çağrılır.
```

Başka dillerde alışık olunan `m.upper()` benzeri yazım JUS'ta yoktur; metin işlemleri sıradan fonksiyonlardır: `büyük_harf(m)`. Hata iletisi doğru yazımı söyler. Aynı yaklaşım liste için de geçerlidir (Hafta 6).

### 3. Hizalama fonksiyonuna sayı vermek

```jus
yaz(sola_doldur(5, 3))
```

```
ornek.jus:1: çalışma zamanı hatası: 'sola_doldur' fonksiyonu metin ister; sayı verildi.
```

`sola_doldur`, `sağa_doldur`, `büyük_harf` gibi metin fonksiyonları metin ister. Çözüm: `sola_doldur(metin(5), 3)`. Fiş alıştırmasında bunu `biçimle` ile birlikte kullanırken özellikle görürsünüz: `biçimle` zaten metin döndürür.

### 4. Büyük-küçük harf farkını unutmak

```jus
değişken cevap = "Ankara"
eğer cevap == "ankara":
    yaz("Doğru")
değilse:
    yaz("Yanlış")
```

```
Yanlış
```

Hata iletisi yoktur; kullanıcı doğru yanıt verse de program `Yanlış` der. Çözüm: iki tarafı da aynı biçime getirmek: `küçük_harf(cevap) == "ankara"`. Kullanıcıdan gelen metinlerde bunu alışkanlık edinin.

### 5. `bul`'un -1 sonucunu dizin olarak kullanmak

```jus
değişken m = "kedi"
değişken yer = bul(m, "z")
yaz(yer)
yaz(m[yer])
```

```
-1
i
```

Aranan harf yoksa `bul` `-1` verir. `-1` geçerli bir dizindir (son karakter), bu yüzden hata çıkmaz, ama yanlış sonuç gelir. Yönlendirme: dizin kullanmadan önce `eğer yer != -1:` ile denetleyin. Alfabe şifresi gibi alıştırmalarda alfabede olmayan karakterlerde (boşluk) bu tuzağa düşülür.

## Çıkış bileti

1. `"merhaba"[1:4]` ve `uzunluk("çay")` ne verir?
2. `büyük_harf("ışık")` ne verir? Neden?
3. `böl("a,b,c", ",")` ne verir?

Program:

```jus
yaz("merhaba"[1:4], uzunluk("çay"))
yaz(büyük_harf("ışık"))
yaz(böl("a,b,c", ","))
```

```
erh 3
IŞIK
["a", "b", "c"]
```

Cevaplar:

1. `erh` ve `3`.
2. `IŞIK`. Türkçe kuralı: `ı` büyük harfe çevrilince `I` olur (noktasız).
3. `["a", "b", "c"]`, yani üç öğeli bir liste.

## Ev çalışması (isteğe bağlı)

1. Kendi adınızın ve soyadınızın uzunluğunu, sesli harf sayısını ve ters yazılışını bulan bir program yazın.
2. Sınıftaki beş arkadaşınızın adından bir liste yapın ve alfabetik sıralayıp yazdırın. Adlarda `Ç`, `Ğ`, `İ`, `Ş` gibi harfler varsa sırayı kontrol edin.
3. Sezar şifresiyle bir cümleyi şifreleyip bir arkadaşınıza verin; çözmesi için kaydırma sayısını söylemeyin (en fazla 28 deneme).
