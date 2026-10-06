# 03 - Kararlar

Önceki: [02 - Değişkenler ve değerler](02-degiskenler-ve-degerler.md) | [İçindekiler](README.md) | Sonraki: [04 - Döngüler](04-donguler.md)

Şimdiye kadar yazdığımız programlar her zaman aynı satırları sırayla çalıştırdı. Gerçek programlar ise duruma göre karar verir: "Yaş 18'den büyükse şunu yap, değilse bunu yap." Bu bölümde karşılaştırmaları ve `eğer` deyimini öğreneceksiniz.

## Karşılaştırma işleçleri

Karşılaştırma, iki değeri kıyaslayıp `doğru` ya da `yanlış` sonucu verir. Bu iki değer **mantıksal** türdedir.

| İşleç | Anlamı |
|-------|--------|
| `<` | küçüktür |
| `<=` | küçük ya da eşittir |
| `>` | büyüktür |
| `>=` | büyük ya da eşittir |
| `==` | eşittir |
| `!=` | eşit değildir |

```jus
yaz(5 > 3)
yaz(5 < 3)
yaz(5 >= 5)
yaz(4 <= 3)
yaz(5 == 5)
yaz(5 != 5)
yaz("elma" == "elma")
yaz("elma" == "armut")
yaz("12" == 12)
yaz("çay" < "dağ")
yaz("ırmak" < "iz")
yaz([1, 2] == [1, 2])
yaz(boş == boş)
```

```
doğru
yanlış
doğru
yanlış
doğru
yanlış
doğru
yanlış
yanlış
doğru
doğru
doğru
doğru
```

Dikkat edilecek noktalar:

- **`=` ile `==` farklıdır.** Tek `=` atama yapar (`x = 5`), çift `==` karşılaştırır (`x == 5`).
- `"12" == 12` sonucu `yanlış`tır. Türleri farklı iki değer hiçbir zaman eşit değildir. Biri metin, biri sayıdır.
- `<`, `<=`, `>`, `>=` iki sayıyı ya da iki metni karşılaştırır. Metinler Türk alfabesi sırasına göre karşılaştırılır: `"çay" < "dağ"` doğrudur, çünkü `ç` harfi `c`'den sonra, `d`'den önce gelir. `"ırmak" < "iz"` de doğrudur, çünkü Türk alfabesinde `ı` harfi `i`'den önce gelir.
- Listeler ve metinler, içerikleri aynıysa eşittir.

## `eğer` deyimi

`eğer` bir koşul yazarsınız. Koşul `doğru` ise girintili blok çalışır.

```jus
değişken sıcaklık = 31

eğer sıcaklık > 30:
    yaz("Hava sıcak.")

eğer sıcaklık < 10:
    yaz("Hava soğuk.")
değilse:
    yaz("Hava soğuk değil.")

yaz("Bitti.")
```

```
Hava sıcak.
Hava soğuk değil.
Bitti.
```

Burada üç yeni şey var:

- **Koşul**, `doğru` ya da `yanlış` veren bir ifadedir. `sıcaklık > 30` bir koşuldur.
- `eğer koşul:` satırı **iki nokta** ile biter. Bu işaret "şimdi bir blok başlıyor" demektir.
- **Blok**, bir arada çalışan satırlar grubudur. Bloğun satırları, başlatan satıra göre içeriden yazılır. Bu içeri girmeye **girinti** denir. Girinti dilin kuralıdır; süs değildir. Blok, girinti bittiğinde sona erer.

### Girinti kuralları

- Aynı bloktaki bütün satırlar aynı girintide olmalıdır.
- Girinti miktarı serbesttir ama önerilen 4 boşluktur.
- Girintiyi boşlukla ya da sekmeyle yapabilirsiniz; aynı dosyada ikisini karıştıramazsınız. Alışkanlık olarak yalnızca boşluk kullanın.
- Her blokta en az bir deyim bulunmalıdır.

### `değilse eğer` ve `değilse`

Birden çok durumu sırayla denemek için `değilse eğer` kullanırsınız. Hiçbir koşul tutmazsa `değilse` bloğu çalışır. Hem `değilse eğer` hem `değilse` isteğe bağlıdır.

```jus
değişken puan = 72

eğer puan >= 90:
    yaz("Harf notu: A")
değilse eğer puan >= 80:
    yaz("Harf notu: B")
değilse eğer puan >= 70:
    yaz("Harf notu: C")
değilse:
    yaz("Harf notu: D")
```

```
Harf notu: C
```

Koşullar yukarıdan aşağıya denenir ve ilk tutan blok çalışır; geri kalanlara bakılmaz. 72 puan `>= 90` ve `>= 80` koşullarını sağlamadığı için `>= 70` bloğuna girer.

### İç içe `eğer`

Bir bloğun içine başka bir `eğer` yazabilirsiniz. Her iç blok bir düzey daha içeriden yazılır:

```jus
değişken yaş = 20
değişken ehliyet = doğru

eğer yaş >= 18:
    eğer ehliyet:
        yaz("Araba kullanabilirsiniz.")
    değilse:
        yaz("Ehliyet almalısınız.")
değilse:
    yaz("Henüz çok gençsiniz.")
```

```
Araba kullanabilirsiniz.
```

`eğer ehliyet:` satırında koşul bir karşılaştırma değil, doğrudan bir mantıksal değişkendir. Mantıksal bir değer zaten `doğru` ya da `yanlış` olduğu için olduğu gibi koşul olabilir. `eğer ehliyet == doğru:` yazmaya gerek yoktur.

## Mantıksal işleçler: `ve`, `veya`, `değil`

Birden çok koşulu birleştirmek için üç işleç vardır:

- `a ve b`: iki taraf da `doğru` ise `doğru`.
- `a veya b`: taraflardan biri `doğru` ise `doğru`.
- `değil a`: `doğru`yu `yanlış`, `yanlış`ı `doğru` yapar.

```jus
değişken yaş = 20
değişken öğrenci = doğru

eğer yaş >= 18 ve yaş < 65:
    yaz("Tam bilet")

eğer yaş < 7 veya yaş >= 65:
    yaz("Ücretsiz")
değilse:
    yaz("Ücretli")

eğer değil öğrenci:
    yaz("Öğrenci değil")
değilse:
    yaz("Öğrenci indirimi var")

eğer yaş >= 18 ve öğrenci:
    yaz("Öğrenci bileti")

yaz(doğru ve yanlış)
yaz(doğru veya yanlış)
yaz(değil doğru)
yaz(değil (3 > 5))
```

```
Tam bilet
Ücretli
Öğrenci indirimi var
Öğrenci bileti
yanlış
doğru
yanlış
doğru
```

Öncelik sırası: karşılaştırmalar önce, sonra `değil`, sonra `ve`, en son `veya` değerlendirilir. Yani `değil 3 == 4`, `değil (3 == 4)` anlamına gelir ve `doğru` sonucunu verir. Emin olmadığınızda parantez kullanın; hem sonucu garanti edersiniz hem kodu okuyana yardım edersiniz.

```jus
yaz(değil 3 == 4)
yaz(doğru veya yanlış ve yanlış)
yaz((doğru veya yanlış) ve yanlış)
```

```
doğru
doğru
yanlış
```

İkinci satırda `ve` önce çalıştığı için `yanlış ve yanlış` hesaplanır, sonra `doğru veya yanlış` olur. Üçüncü satırda parantez sırayı değiştirir.

`ve`, `veya` ve `değil` yalnızca mantıksal değerlerle çalışır. `5 ve doğru` yazarsanız hata alırsınız (aşağıda göreceksiniz).

### `içinde`: üyelik denetimi

`öğe içinde kap` ifadesi, öğenin bir listede, metinde ya da sözlükte bulunup bulunmadığını söyler:

```jus
yaz(3 içinde [1, 2, 3])
yaz("ay" içinde "ayşe")
yaz("kedi" içinde ["köpek", "kuş"])
yaz("ad" içinde {"ad": "Ayşe"})
```

```
doğru
doğru
yanlış
doğru
```

Listede öğeyi, metinde alt metni, sözlükte anahtarı arar. Bunlara ilerideki bölümlerde döneceğiz.

### Kısa devre

`ve` ile `veya`, sonucu sol taraftan belli olduğunda sağ tarafı hiç hesaplamaz. Buna **kısa devre** denir. Aşağıdaki örnek bunu gösterir; `sesli` fonksiyonu çağrıldığında bir iz bırakıyor (fonksiyonları beşinci bölümde öğreneceksiniz):

```jus
fonksiyon sesli(m):
    yaz("  sesli() çağrıldı:", m)
    dön doğru

yaz("Birinci deneme")
yaz(yanlış ve sesli("a"))

yaz("İkinci deneme")
yaz(doğru ve sesli("b"))

yaz("Üçüncü deneme")
yaz(doğru veya sesli("c"))

değişken liste = [4, 5]
eğer uzunluk(liste) > 2 ve liste[2] == 9:
    yaz("var")
değilse:
    yaz("yok, ama hata da çıkmadı")
```

```
Birinci deneme
yanlış
İkinci deneme
  sesli() çağrıldı: b
doğru
Üçüncü deneme
doğru
yok, ama hata da çıkmadı
```

`yanlış ve ...` ifadesinin sonucu baştan `yanlış` olduğu için `sesli("a")` hiç çağrılmadı. Aynı şekilde `doğru veya ...` için `sesli("c")` çağrılmadı. Bu özellik pratikte de işe yarar: son örnekte `liste[2]` ifadesi, listede üçüncü öğe olmadığı için normalde hata verirdi. Ama `uzunluk(liste) > 2` önce `yanlış` çıktığından o taraf hiç hesaplanmadı.

## Sık yapılan hatalar

**Koşulun mantıksal olmaması.** JUS, koşul olarak sayı, metin ya da `boş` kabul etmez; `doğru`/`yanlış` ister.

```jus
değişken sayı4 = 1
eğer sayı4:
    yaz("bir")
```

```
ornek.jus:2: çalışma zamanı hatası: Koşul mantıksal bir değer (doğru/yanlış) olmalı; sayı verildi.
```

Çözüm: `eğer sayı4 != 0:` gibi bir karşılaştırma yazın.

**`==` yerine `=` yazmak.** `=` bir atamadır ve atanan değeri sonuç verir. Bu yüzden `eğer x = 5:` yazarsanız koşul `5` olur, bu da mantıksal olmadığı için hata verir. (Ayrıca bu işlem `x`'e 5'i atar.)

```jus
değişken x = 3
eğer x = 5:
    yaz("beş")
```

```
ornek.jus:2: çalışma zamanı hatası: Koşul mantıksal bir değer (doğru/yanlış) olmalı; sayı verildi.
```

**İki noktayı unutmak.**

```jus
değişken x = 3
eğer x > 2
    yaz("büyük")
```

```
ornek.jus:2:11: sözdizimi hatası: Blok başlatmak için ':' bekleniyor.
    eğer x > 2
              ^
```

**Girintiyi unutmak.**

```jus
değişken x = 3
eğer x > 2:
yaz("büyük")
```

```
ornek.jus:3:1: sözdizimi hatası: ':' işaretinden sonra girintili bir blok bekleniyor.
    yaz("büyük")
    ^
```

**Girintiyi tutarsız yapmak.** Aynı bloktaki satırlar aynı girintide olmalıdır:

```jus
değişken x = 3
eğer x > 2:
    yaz("büyük")
      yaz("çok büyük")
```

```
ornek.jus:4:7: sözdizimi hatası: Beklenmeyen girinti; bu satır bir bloğun içinde değil.
          yaz("çok büyük")
          ^
```

**`ve` ile mantıksal olmayan değer kullanmak.**

```jus
değişken x = 3
eğer x > 2 ve 5:
    yaz("büyük")
```

```
ornek.jus:2: çalışma zamanı hatası: 've' / 'veya' mantıksal değerler ister; sayı verildi.
```

## Alıştırmalar

1. Bir sayı değişkeni tanımlayın. Sayı pozitifse `Pozitif`, negatifse `Negatif`, sıfırsa `Sıfır` yazdırın.
2. Bir yıl değişkeni için artık yıl olup olmadığını yazdırın. Kural: yıl 4'e bölünür ve (100'e bölünmez ya da 400'e bölünür). 2024, 1900 ve 2000 ile deneyin.
3. Bilet fiyatı: 7 yaşından küçükler ücretsiz, öğrenciler ve 65 yaş üstü yarı fiyat (50), diğerleri 100. `yaş` ve `öğrenci` değişkenlerine göre fiyatı yazdırın.
4. `a`, `b`, `c` adlı üç sayıdan en büyüğünü bulup yazdırın.
5. Doğru şifre `kiraz42` olsun. Kullanıcıdan şifre isteyin; doğruysa `Hoş geldiniz.`, değilse `Şifre yanlış.` yazdırın.

## Çözümler

**1.**

```jus
değişken sayı1 = -4

eğer sayı1 > 0:
    yaz("Pozitif")
değilse eğer sayı1 < 0:
    yaz("Negatif")
değilse:
    yaz("Sıfır")
```

```
Negatif
```

**2.** `yıl = 1900` için:

```jus
değişken yıl = 1900

eğer yıl % 4 == 0 ve (yıl % 100 != 0 veya yıl % 400 == 0):
    yaz(metin(yıl) + " artık yıldır.")
değilse:
    yaz(metin(yıl) + " artık yıl değildir.")
```

```
1900 artık yıl değildir.
```

İlk satırdaki değeri `2024`, `2000` ve `2023` yapıp çalıştırdığımızda sonuçlar sırasıyla `2024 artık yıldır.`, `2000 artık yıldır.` ve `2023 artık yıl değildir.` oldu.

**3.**

```jus
değişken yaş = 16
değişken öğrenci = doğru
değişken fiyat = 100

eğer yaş < 7:
    fiyat = 0
değilse eğer öğrenci veya yaş >= 65:
    fiyat = 50

yaz("Bilet fiyatı:", fiyat)
```

```
Bilet fiyatı: 50
```

**4.**

```jus
değişken a = 14
değişken b = 29
değişken c = 8

değişken enBüyük = a
eğer b > enBüyük:
    enBüyük = b
eğer c > enBüyük:
    enBüyük = c

yaz("En büyük:", enBüyük)
```

```
En büyük: 29
```

**5.**

```jus
değişken doğruŞifre = "kiraz42"
değişken girilen = oku("Şifre: ")

eğer girilen == doğruŞifre:
    yaz("Hoş geldiniz.")
değilse:
    yaz("Şifre yanlış.")
```

`printf 'kiraz42\n' | jus ornek.jus` ile:

```
Şifre: Hoş geldiniz.
```

`printf 'elma\n' | jus ornek.jus` ile:

```
Şifre: Şifre yanlış.
```

---

Önceki: [02 - Değişkenler ve değerler](02-degiskenler-ve-degerler.md) | [İçindekiler](README.md) | Sonraki: [04 - Döngüler](04-donguler.md)
