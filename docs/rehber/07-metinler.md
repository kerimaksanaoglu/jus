# 07 - Metinler

Önceki: [06 - Listeler](06-listeler.md) | [İçindekiler](README.md) | Sonraki: [08 - Sözlükler](08-sozlukler.md)

Metinle ikinci bölümde tanıştınız. Metin, çift tırnak içine yazılan karakter dizisidir. Bu bölümde metinlerin içine bakmayı, onları parçalamayı, aramayı ve dönüştürmeyi öğreneceksiniz. JUS'un Türkçe harflere gösterdiği özen de bu bölümde görünür.

## Dizin, dilim ve uzunluk

Metinler listelere çok benzer. Her **karakter** (harf, rakam ya da işaret) bir dizinde durur; dizinler 0'dan başlar ve negatif dizinler sondan sayar. `metin[baş:son]` bir parça (dilim) verir.

```jus
değişken m = "çalışkan"
yaz(m[0])
yaz(m[1])
yaz(m[-1])
yaz(m[0:4])
yaz(m[4:])
yaz(m[:3])
yaz(uzunluk(m))
yaz(uzunluk(""))
yaz("ağaç"[1])
yaz("Ayşe" + " " + "Yılmaz")
```

```
ç
a
n
çalı
şkan
çal
8
0
ğ
Ayşe Yılmaz
```

Birim bayt değil **karakterdir**: `"çalışkan"` 8 karakterdir ve `m[0]` sonucu `"ç"` olur. `uzunluk` karakter sayısını verir. Türkçe harfler tek karakter sayılır.

**Metinler değiştirilemez.** `m[0] = "p"` yazarsanız hata alırsınız. Bir metni değiştirmek için yeni bir metin oluşturursunuz (örneğin `"p" + m[1:]`).

## Bölmek, birleştirmek, kırpmak

- `böl(metin, ayraç)` metni ayraçtan keserek bir liste yapar. Ayraç boş metin (`""`) ise metin karakterlerine bölünür.
- `birleştir(liste, ayraç)` bir listenin öğelerini metne çevirip aralarına ayraç koyarak tek metin yapar.
- `kırp(metin)` baştaki ve sondaki boşlukları atar.

```jus
değişken cümle = "bugün hava çok güzel"
değişken kelimeler = böl(cümle, " ")
yaz(kelimeler)
yaz(uzunluk(kelimeler))

yaz(birleştir(kelimeler, "-"))
yaz(birleştir(["a", "b", "c"], ""))
yaz(birleştir([1, 2, 3], ", "))

yaz(böl("elma,armut,kiraz", ","))
yaz(böl("abc", ""))

yaz("[" + kırp("   merhaba   ") + "]")
yaz(böl("  a  b ", " "))
```

```
["bugün", "hava", "çok", "güzel"]
4
bugün-hava-çok-güzel
abc
1, 2, 3
["elma", "armut", "kiraz"]
["a", "b", "c"]
[merhaba]
["", "", "a", "", "b", ""]
```

Son satıra dikkat: art arda iki ayraç, aralarında boş bir parça üretir. Bu yüzden kullanıcıdan gelen metni bölmeden önce `kırp` ile temizlemek iyi bir alışkanlıktır.

## Aramak ve değiştirmek

- `bul(metin, aranan)` alt metnin ilk geçtiği karakter dizinini verir; yoksa `-1`.
- `aranan içinde metin` alt metin var mı diye `doğru`/`yanlış` söyler.
- `değiştir(metin, eski, yeni)` `eski`'nin geçtiği her yeri `yeni` ile değiştirir ve yeni bir metin verir.
- `başlar_mı(metin, ön)` ve `biter_mi(metin, son)` metnin başını ve sonunu denetler.

```jus
değişken m = "kara kedi kara kuş"
yaz(bul(m, "kara"))
yaz(bul(m, "kedi"))
yaz(bul(m, "köpek"))
yaz("kuş" içinde m)
yaz("köpek" içinde m)

yaz(değiştir(m, "kara", "ak"))
yaz(değiştir("a b c", " ", ""))

yaz(başlar_mı("merhaba dünya", "mer"))
yaz(başlar_mı("merhaba dünya", "dünya"))
yaz(biter_mi("rapor.txt", ".txt"))
yaz(biter_mi("rapor.txt", ".jus"))
```

```
0
5
-1
doğru
yanlış
ak kedi ak kuş
abc
doğru
yanlış
doğru
yanlış
```

`değiştir` asıl metni değiştirmez, yenisini verir. Sonucu bir değişkene atamayı unutmayın: `m = değiştir(m, "kara", "ak")`.

## Büyük ve küçük harf: Türkçe kuralları

Türkçede `i` harfinin büyüğü `İ`, `ı` harfinin büyüğü `I`'dır. Bu yüzden `istanbul` sözcüğünün büyük harfli yazımı `İSTANBUL`'dur. JUS, `büyük_harf` ve `küçük_harf` fonksiyonlarında bu Türkçe kurallarını uygular.

```jus
yaz(büyük_harf("merhaba"))
yaz(büyük_harf("istanbul"))
yaz(büyük_harf("ılık"))
yaz(büyük_harf("çiğdem şeker"))
yaz(küçük_harf("IŞIK"))
yaz(küçük_harf("İZMİR"))
yaz(küçük_harf("ÇAĞRI"))

# büyük/küçük harf ayrımı olmadan karşılaştırma
değişken a = "ISPARTA"
değişken b = "ısparta"
yaz(a == b)
yaz(küçük_harf(a) == küçük_harf(b))

# ilk harfi büyütmek
fonksiyon baştanBüyüt(kelime):
    dön büyük_harf(kelime[0]) + kelime[1:]

yaz(baştanBüyüt("izmir"))
yaz(baştanBüyüt("ıspanak"))
```

```
MERHABA
İSTANBUL
ILIK
ÇİĞDEM ŞEKER
ışık
izmir
çağrı
yanlış
doğru
İzmir
Ispanak
```

İki metni büyük/küçük harfe bakmadan karşılaştırmak için ikisini de aynı harf boyuna çevirip öyle karşılaştırın. Bu kurallar Türk alfabesindeki harfleri ve İngilizce harfleri kapsar; diğer alfabelerin harfleri değiştirilmeden bırakılır (`büyük_harf("αβγ abc")` sonucu `αβγ ABC` olur).

## Alfabetik karşılaştırma ve sıralama

`<`, `>` gibi işleçler metinlerde de çalışır ve Türk alfabesi sırasını kullanır: `ç`, `c`'den sonra; `ğ`, `g`'den sonra; `ı`, `i`'den önce; `ö`, `o`'dan sonra; `ş`, `s`'den sonra; `ü`, `u`'dan sonra gelir. `sırala` da aynı sırayı kullanır.

```jus
yaz("çay" < "dağ")
yaz("ırmak" < "iz")
yaz("zeytin" < "şeker")
yaz("ürün" < "vapur")
yaz("elma" < "elmas")

değişken isimler = ["Şule", "Çağla", "Zeynep", "Ömer", "Işık", "İpek", "Ayşe", "Güneş", "Gül"]
yaz(sırala(isimler))
yaz(ters("merhaba"))
```

```
doğru
doğru
yanlış
doğru
doğru
["Ayşe", "Çağla", "Gül", "Güneş", "Işık", "İpek", "Ömer", "Şule", "Zeynep"]
abahrem
```

`"zeytin" < "şeker"` yanlıştır: Türk alfabesinde `ş`, `z`'den önce gelir. `ters` fonksiyonu bir metni tersine çevirir.

## Metni gezmek

`her` döngüsüyle bir metnin karakterlerini tek tek dolaşabilirsiniz:

```jus
her harf içinde "JUS":
    yaz(harf)

fonksiyon sesliSay(m):
    değişken sayı6 = 0
    her harf içinde küçük_harf(m):
        eğer harf içinde "aeıioöuü":
            sayı6 += 1
    dön sayı6

yaz(sesliSay("Türkiye Cumhuriyeti"))

fonksiyon palindromMu(m):
    değişken temiz = küçük_harf(değiştir(m, " ", ""))
    dön temiz == ters(temiz)

yaz(palindromMu("Ey Edip adanada pide ye"))
yaz(palindromMu("merhaba"))
```

```
J
U
S
8
doğru
yanlış
```

`sesliSay` önce metni küçük harfe çevirir, sonra her karakterin sesli harfler arasında olup olmadığına bakar. `palindromMu`, tersten de aynı okunan ifadeleri (palindrom) bulur.

## Tekrarlamak ve hizalamak

Çıktıyı düzenli göstermek için üç fonksiyon vardır:

- `tekrarla(m, adet)` metni arka arkaya `adet` kez yazar.
- `sola_doldur(m, genişlik)` metni sağa yaslar: `genişlik` karaktere tamamlamak için soluna boşluk ekler.
- `sağa_doldur(m, genişlik)` metni sola yaslar: sağına boşluk ekler.

İkisine de üçüncü argüman olarak tek karakterlik bir dolgu verebilirsiniz. Metin zaten genişlikten uzunsa olduğu gibi kalır; kesilmez.

```jus
yaz(tekrarla("-", 20))
yaz(tekrarla("ab", 3))
yaz("[" + sola_doldur("7", 3) + "]")
yaz(sola_doldur("7", 3, "0"))
yaz("[" + sağa_doldur("ad", 6) + "]")
yaz(sağa_doldur("ad", 6, ".") + "|")
yaz(sola_doldur("çilek", 3))
```

```
--------------------
ababab
[  7]
007
[ad    ]
ad....|
çilek
```

Bu fonksiyonlarla hizalı bir tablo yazdırabilirsiniz. Yazı sütunlarını sola, sayı sütunlarını sağa yaslamak okumayı kolaylaştırır. Sayıları ondalık basamak sayısı sabit olacak şekilde `biçimle` ile (ikinci bölüm) metne çeviriyoruz:

```jus
değişken ürünler = [["Elma", 12.5, 3], ["Karpuz", 45, 1], ["Çilek", 80.25, 2]]

yaz(sağa_doldur("Ürün", 10) + sola_doldur("Fiyat", 8) + sola_doldur("Adet", 6))
yaz(tekrarla("-", 24))
her ü içinde ürünler:
    yaz(sağa_doldur(ü[0], 10) + sola_doldur(biçimle(ü[1], 2), 8) + sola_doldur(metin(ü[2]), 6))
```

```
Ürün         Fiyat  Adet
------------------------
Elma         12.50     3
Karpuz       45.00     1
Çilek        80.25     2
```

Türkçe harfler tek karakter sayıldığı için `Çilek` ve `Ürün` gibi sözcükler de doğru hizalanır. `sola_doldur` sayı değil metin ister; sayıyı önce `metin()` ya da `biçimle()` ile çevirin. Sıra numarası gibi baştan sıfırlı yazımlar için dolgu karakteri `"0"` verilir:

```jus
her n içinde [1, 7, 42, 108]:
    yaz("Sıra " + sola_doldur(metin(n), 4, "0"))
```

```
Sıra 0001
Sıra 0007
Sıra 0042
Sıra 0108
```

## Sık yapılan hatalar

**Metnin karakterini değiştirmeye çalışmak.**

```jus
değişken m = "kedi"
m[0] = "p"
yaz(m)
```

```
ornek.jus:2: çalışma zamanı hatası: Metinler değiştirilemez; yeni bir metin oluşturun.
```

Çözüm: `m = "p" + m[1:]`.

**Sınır dışı dizin.**

```jus
değişken m = "kedi"
yaz(m[4])
```

```
ornek.jus:2: çalışma zamanı hatası: Dizin sınırların dışında: uzunluk 4, istenen dizin 4.
```

**Metin fonksiyonlarına metin vermemek.**

```jus
yaz(büyük_harf(42))
```

```
ornek.jus:1: çalışma zamanı hatası: 'büyük_harf' fonksiyonu metin ister; sayı verildi.
```

**`sola_doldur`'a sayı vermek.**

```jus
yaz(sola_doldur(5, 3))
```

```
ornek.jus:1: çalışma zamanı hatası: 'sola_doldur' fonksiyonu metin ister; sayı verildi.
```

Çözüm: `sola_doldur(metin(5), 3)`.

**Metin fonksiyonlarını nokta ile çağırmak.** JUS'ta metin işlemleri nesne yöntemi değil, sıradan fonksiyonlardır. `m.büyük_harf()` yerine `büyük_harf(m)` yazın.

```jus
değişken m = "merhaba"
yaz(m.büyük_harf())
```

```
ornek.jus:2: çalışma zamanı hatası: metin türündeki değerlerin 'büyük_harf' adında bir yöntemi yok.
```

## Alıştırmalar

1. `"bir iki üç dört beş"` cümlesindeki kelime sayısını yazdırın.
2. `"ışıl ışıl parlayan istanbul"` içindeki her kelimenin ilk harfini büyütüp birleştirin.
3. Bir metinde belirli bir harfin kaç kez geçtiğini veren `harfSay(m, aranan)` fonksiyonunu yazın. `"kardeşlerimiz"` içinde `e`, `r` ve `x` harflerini sayın.
4. Kullanıcıdan adını isteyin; başındaki sonundaki boşlukları atıp büyük harfle `Merhaba, ...!` yazdırın ve harf sayısını belirtin.
5. `["Şanlıurfa", "İzmir", "Ankara", "Çorum", "Iğdır", "Ordu"]` listesini alfabetik sıralayın.
6. `tekrarla` ile bir üst ve alt çizgi çizerek şu çıktıyı üretin: `Fiş` başlığı yaklaşık ortada olsun, altında `Ekmek` ve `Süt` satırlarının fiyatları (`7.50` ve `32.00`) sağa yaslı olsun.

## Çözümler

**1.**

```jus
değişken cümle = "bir iki üç dört beş"
yaz(uzunluk(böl(cümle, " ")))
```

```
5
```

**2.**

```jus
fonksiyon baştanBüyüt(kelime):
    dön büyük_harf(kelime[0]) + kelime[1:]

değişken kelimeler = böl("ışıl ışıl parlayan istanbul", " ")
değişken yeni = []
her k içinde kelimeler:
    ekle(yeni, baştanBüyüt(k))
yaz(birleştir(yeni, " "))
```

```
Işıl Işıl Parlayan İstanbul
```

**3.**

```jus
fonksiyon harfSay(m, aranan):
    değişken sayaç = 0
    her h içinde m:
        eğer h == aranan:
            sayaç += 1
    dön sayaç

yaz(harfSay("kardeşlerimiz", "e"))
yaz(harfSay("kardeşlerimiz", "r"))
yaz(harfSay("kardeşlerimiz", "x"))
```

```
2
2
0
```

**4.**

```jus
değişken ad = oku("Adınız: ")
yaz("Merhaba, " + büyük_harf(kırp(ad)) + "!")
yaz("Adınız " + metin(uzunluk(kırp(ad))) + " harfli.")
```

Girdi olarak `  ışıl  ` (başında ve sonunda boşluk) verildiğinde (`printf '  ışıl  \n' | jus ornek.jus`):

```
Adınız: Merhaba, IŞIL!
Adınız 4 harfli.
```

**5.**

```jus
değişken şehirler = ["Şanlıurfa", "İzmir", "Ankara", "Çorum", "Iğdır", "Ordu"]
yaz(sırala(şehirler))
```

```
["Ankara", "Çorum", "Iğdır", "İzmir", "Ordu", "Şanlıurfa"]
```

**6.** Bir olası çözüm:

```jus
fonksiyon satır(ad, tutar):
    dön sağa_doldur(ad, 10, ".") + sola_doldur(biçimle(tutar, 2), 8)

yaz(tekrarla("=", 18))
yaz(sola_doldur("Fiş", 11))
yaz(tekrarla("=", 18))
yaz(satır("Ekmek", 7.5))
yaz(satır("Süt", 32))
yaz(tekrarla("=", 18))
```

```
==================
        Fiş
==================
Ekmek.....    7.50
Süt.......   32.00
==================
```

---

Önceki: [06 - Listeler](06-listeler.md) | [İçindekiler](README.md) | Sonraki: [08 - Sözlükler](08-sozlukler.md)
