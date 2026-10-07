# 06 - Listeler

Önceki: [05 - Fonksiyonlar](05-fonksiyonlar.md) | [İçindekiler](README.md) | Sonraki: [07 - Metinler](07-metinler.md)

Şimdiye kadar her değişkende tek bir değer sakladık. Peki 30 öğrencinin notunu saklamak isterseniz? 30 ayrı değişken tanımlamak yerine bir **liste** kullanırsınız. Liste, sıralı bir değer dizisidir.

## Liste oluşturmak ve öğelere erişmek

Liste, köşeli parantez içinde virgülle ayrılmış değerlerle yazılır. Listedeki her değere **öğe** denir. Öğeye sırasını gösteren **dizin** ile erişirsiniz. Dizinler **0'dan** başlar: ilk öğenin dizini 0, ikincinin 1'dir. Negatif dizin sondan sayar: `-1` son öğedir.

```jus
değişken notlar = [85, 92, 78]
yaz(notlar)
yaz(notlar[0])
yaz(notlar[2])
yaz(notlar[-1])
yaz(notlar[-3])

notlar[1] = 95
yaz(notlar)
notlar[0] += 5
yaz(notlar)

yaz(uzunluk(notlar))

değişken karışık = [1, "iki", doğru, boş, [3, 4]]
yaz(karışık)
yaz(uzunluk(karışık))

değişken boşListe = []
yaz(boşListe)
yaz(uzunluk(boşListe))

yaz([1, 2] + [3, 4])
```

```
[85, 92, 78]
85
78
78
85
[85, 95, 78]
[90, 95, 78]
3
[1, "iki", doğru, boş, [3, 4]]
5
[]
0
[1, 2, 3, 4]
```

Önemli noktalar:

- `notlar[1] = 95` bir öğeyi değiştirir. Liste değiştirilebilir bir yapıdır.
- `uzunluk(liste)` öğe sayısını verir.
- Bir liste farklı türlerden öğeler taşıyabilir; bir listenin içinde başka bir liste de olabilir.
- `+` iki listeyi birleştirip yeni bir liste verir.
- Listeler yazdırıldığında metinler tırnak içinde görünür (`"iki"`). Bu, `"3"` ile `3`'ü ayırt etmenize yardım eder.
- Son öğeden sonra virgül konabilir. Uzun listeleri birden çok satıra bölebilirsiniz.

## Dilimler

`liste[baş:son]`, `baş` dizininden `son` dizinine kadar (son hariç) yeni bir liste verir. Bir ucu yazmazsanız listenin başı ya da sonu alınır. Sınırlar liste dışına taşarsa hata olmaz; kırpılır.

```jus
değişken harfler = ["a", "b", "c", "d", "e", "f"]
yaz(harfler[1:4])
yaz(harfler[:2])
yaz(harfler[3:])
yaz(harfler[-2:])
yaz(harfler[:])
yaz(harfler[2:100])
yaz(harfler[4:2])
```

```
["b", "c", "d"]
["a", "b"]
["d", "e", "f"]
["e", "f"]
["a", "b", "c", "d", "e", "f"]
["c", "d", "e", "f"]
[]
```

## Ekleme, çıkarma, silme

| Fonksiyon | Ne yapar |
|-----------|----------|
| `ekle(liste, öğe)` | Öğeyi listenin sonuna ekler |
| `araya_ekle(liste, dizin, öğe)` | Öğeyi verilen dizine yerleştirir, sonrakileri kaydırır |
| `çıkar(liste)` | Son öğeyi listeden çıkarır ve döndürür |
| `sil(liste, dizin)` | Dizindeki öğeyi siler ve döndürür |
| `bul(liste, öğe)` | Öğenin ilk dizinini verir; yoksa `-1` |

Bunların hepsi (`bul` hariç) listeyi **yerinde** değiştirir; yeni liste üretmez.

```jus
değişken alışveriş = ["ekmek", "süt"]

ekle(alışveriş, "yumurta")
yaz(alışveriş)

araya_ekle(alışveriş, 1, "peynir")
yaz(alışveriş)

değişken son = çıkar(alışveriş)
yaz("Çıkarılan:", son)
yaz(alışveriş)

değişken silinen = sil(alışveriş, 0)
yaz("Silinen:", silinen)
yaz(alışveriş)

yaz(bul(alışveriş, "süt"))
yaz(bul(alışveriş, "çay"))
yaz("süt" içinde alışveriş)
yaz("çay" içinde alışveriş)

alışveriş = alışveriş + ["çay", "şeker"]
yaz(alışveriş)
yaz(ekle(alışveriş, "tuz"))
```

```
["ekmek", "süt", "yumurta"]
["ekmek", "peynir", "süt", "yumurta"]
Çıkarılan: yumurta
["ekmek", "peynir", "süt"]
Silinen: ekmek
["peynir", "süt"]
1
-1
doğru
yanlış
["peynir", "süt", "çay", "şeker"]
boş
```

Son satıra dikkat: `ekle` listeyi değiştirir ama bir şey döndürmez; `yaz(ekle(...))` `boş` yazar. Yeni bir liste beklemeyin: `değişken yeni = ekle(liste, 4)` yazarsanız `yeni` `boş` olur.

## Sıralama ve ters çevirme

`sırala(liste)` küçükten büyüğe sıralı **yeni** bir liste verir; asıl listeyi değiştirmez. `ters(liste)` öğeleri ters sırada yeni bir liste olarak verir. Metinler Türk alfabesine göre sıralanır.

```jus
değişken sayılar = [5, 2, 9, 1, 7]
değişken sıralı = sırala(sayılar)
yaz(sıralı)
yaz(sayılar)
yaz(ters(sıralı))
yaz(ters(sayılar))

yaz(sırala(["muz", "çilek", "armut", "ılık", "iğde", "şeftali", "elma"]))

# büyükten küçüğe
yaz(ters(sırala(sayılar)))
```

```
[1, 2, 5, 7, 9]
[5, 2, 9, 1, 7]
[9, 7, 5, 2, 1]
[7, 1, 9, 2, 5]
["armut", "çilek", "elma", "ılık", "iğde", "muz", "şeftali"]
[9, 7, 5, 2, 1]
```

`sırala` için listenin tüm öğeleri sayı ya da tüm öğeleri metin olmalıdır. Türk alfabesinde `ı` harfi `i`'den önce geldiği için `ılık` kelimesi `iğde`'den önce sıralandı.

## Ölçüte göre sıralama: `sırala(liste, anahtar)`

Bazen öğeleri kendi değerlerine göre değil, onlardan hesaplanan bir ölçüte göre sıralamak istersiniz: kelimeleri uzunluğuna göre, sayıları büyükten küçüğe... `sırala`'ya ikinci argüman olarak bir **fonksiyon** verirseniz, `sırala` her öğe için o fonksiyonu çağırır ve öğeleri dönen değere göre dizer. Fonksiyonun adı parantezsiz yazılır; çünkü onu çağırmıyor, `sırala`'ya değer olarak veriyorsunuz (beşinci bölümde gördüğünüz gibi).

```jus
değişken kelimeler = ["muz", "elma", "kayısı", "çilek", "incir"]

fonksiyon harfSayısı(m):
    dön uzunluk(m)

yaz(sırala(kelimeler, harfSayısı))
yaz(sırala(kelimeler, uzunluk))
yaz(ters(sırala(kelimeler, uzunluk)))

fonksiyon eksisi(n):
    dön -n

yaz(sırala([5, 2, 9, 1, 7], eksisi))
```

```
["muz", "elma", "çilek", "incir", "kayısı"]
["muz", "elma", "çilek", "incir", "kayısı"]
["kayısı", "incir", "çilek", "elma", "muz"]
[9, 7, 5, 2, 1]
```

Gözlemler:

- Yerleşik fonksiyonlar da değerdir; ikinci satırda `uzunluk` doğrudan anahtar olarak verildi.
- Anahtarı eşit olan öğeler özgün sıralarını korur: `çilek` ve `incir` ikisi de 5 harflidir; listede `çilek` önce olduğu için sonuçta da önce gelir.
- Sayıları büyükten küçüğe dizmek için anahtar olarak sayının eksisini verdik.
- Sonuç yine yeni bir listedir; asıl liste değişmez.

Sözlük ve nesne listelerini sıralamak için de aynı yol izlenir; sekizinci bölümde göreceksiniz.

## `eşle` ve `süz`

Listelerle en sık yapılan iki iş, her öğeyi dönüştürmek ve bazı öğeleri seçmektir. İkisi için de elle döngü yazabilirsiniz, ama yerleşik fonksiyonlar daha kısadır:

- `eşle(liste, f)`, her öğe için `f(öğe)` sonucunu içeren yeni bir liste verir.
- `süz(liste, f)`, `f(öğe)` sonucu `doğru` olan öğelerden oluşan yeni bir liste verir. `f` mantıksal değer döndürmelidir.

```jus
fonksiyon kare(n):
    dön n * n

fonksiyon çift_mi(n):
    dön n % 2 == 0

değişken sayılar = [1, 2, 3, 4, 5, 6]
yaz(eşle(sayılar, kare))
yaz(süz(sayılar, çift_mi))
yaz(eşle(süz(sayılar, çift_mi), kare))
yaz(eşle(["ali", "ayşe"], büyük_harf))
yaz(sayılar)
```

```
[1, 4, 9, 16, 25, 36]
[2, 4, 6]
[4, 16, 36]
["ALİ", "AYŞE"]
[1, 2, 3, 4, 5, 6]
```

İkisi de asıl listeyi değiştirmez. Çağrıları iç içe yazabilirsiniz: üçüncü satır önce çift sayıları seçti, sonra karelerini aldı.

## Listeyi gezmek

`her ... içinde` ile öğeleri tek tek dolaşırsınız. Dizine de ihtiyacınız varsa `aralık(uzunluk(liste))` ile dizinleri dolaşın.

```jus
değişken notlar = [70, 85, 90, 65]

# öğeleri doğrudan gezmek
değişken toplam = 0
her not1 içinde notlar:
    toplam += not1
yaz("Toplam:", toplam)
yaz("Ortalama:", toplam / uzunluk(notlar))

# dizinle gezmek
her i içinde aralık(uzunluk(notlar)):
    yaz(metin(i + 1) + ". öğrenci:", notlar[i])

# en büyük öğeyi bulmak
değişken enBüyük = notlar[0]
her n içinde notlar:
    eğer n > enBüyük:
        enBüyük = n
yaz("En büyük:", enBüyük)

# yeni liste oluşturmak
değişken kareler = []
her n içinde aralık(1, 6):
    ekle(kareler, n * n)
yaz(kareler)

# listeyi tersten gezmek
her n içinde ters(notlar):
    yaz(n)
```

```
Toplam: 310
Ortalama: 77.5
1. öğrenci: 70
2. öğrenci: 85
3. öğrenci: 90
4. öğrenci: 65
En büyük: 90
[1, 4, 9, 16, 25]
65
90
85
70
```

Boş bir liste (`[]`) kurup döngüyle doldurmak çok yaygın bir kalıptır; `kareler` örneğinde bunu gördünüz.

## Listeler başvuruyla taşınır

Bir listeyi başka bir değişkene atadığınızda liste kopyalanmaz; iki ad **aynı listeyi** gösterir. Aynı şey bir listeyi fonksiyona verdiğinizde de olur. Bu yüzden birinden yapılan değişiklik diğerinde de görünür.

```jus
değişken a = [1, 2, 3]
değişken b = a
ekle(b, 4)
yaz(a)
yaz(b)

değişken c = a + []
ekle(c, 5)
yaz(a)
yaz(c)

fonksiyon ikiyleÇarp(liste):
    her i içinde aralık(uzunluk(liste)):
        liste[i] = liste[i] * 2

değişken d = [1, 2, 3]
ikiyleÇarp(d)
yaz(d)

değişken tablo = [[1, 2, 3], [4, 5, 6]]
yaz(tablo[1][2])
tablo[0][0] = 100
yaz(tablo)
```

```
[1, 2, 3, 4]
[1, 2, 3, 4]
[1, 2, 3, 4]
[1, 2, 3, 4, 5]
[2, 4, 6]
6
[[100, 2, 3], [4, 5, 6]]
```

`değişken b = a` yazınca `b` ayrı bir liste olmadı; `ekle(b, 4)` hem `a`'yı hem `b`'yi değiştirdi. `a + []` ise yeni bir liste ürettiği için `c` bağımsızdır. Fonksiyona verilen liste de aynı liste olduğundan `ikiyleÇarp` çağıranın listesini değiştirdi. Listenin içindeki liste, `tablo[1][2]` gibi iki dizinle okunur.

## Sık yapılan hatalar

**Sınır dışı dizin.** Üç öğeli listenin son dizini 2'dir:

```jus
değişken liste = [10, 20, 30]
yaz(liste[3])
```

```
ornek.jus:2: çalışma zamanı hatası: Dizin sınırların dışında: uzunluk 3, istenen dizin 3.
```

**`ekle`'nin sonucunu kullanmak.** `ekle` `boş` döndürür:

```jus
değişken liste = [1, 2, 3]
değişken yeni = ekle(liste, 4)
yaz(yeni[0])
```

```
ornek.jus:3: çalışma zamanı hatası: Yalnızca liste, metin ve sözlük dizinlenebilir; boş verildi.
```

**Karışık türleri sıralamak.**

```jus
yaz(sırala([3, "iki", 1]))
```

```
ornek.jus:1: çalışma zamanı hatası: 'sırala' için listenin tüm öğeleri sayı ya da tüm öğeleri metin olmalı. Başka türde öğeler için ikinci argüman olarak bir anahtar fonksiyonu verin.
```

**`süz` fonksiyonundan mantıksal olmayan değer döndürmek.** `süz`, verdiğiniz fonksiyonun `doğru` ya da `yanlış` döndürmesini bekler:

```jus
fonksiyon bir(n):
    dön 1

yaz(süz([1, 2], bir))
```

```
ornek.jus:4: çalışma zamanı hatası: 'süz' için verilen fonksiyon doğru ya da yanlış döndürmeli; sayı döndürdü.
```

**Fonksiyonu çağırarak vermek.** `eşle([1, 2], kare(2))` yazarsanız `kare(2)` önce çalışır ve `eşle`'ye fonksiyon değil `4` gider:

```
ornek.jus:3: çalışma zamanı hatası: Yalnızca fonksiyonlar çağrılabilir; bu değerin türü sayı.
```

Fonksiyonu parantezsiz, adıyla verin: `eşle([1, 2], kare)`.

**Boş listeden öğe çıkarmak.**

```jus
değişken boşListe = []
yaz(çıkar(boşListe))
```

```
ornek.jus:2: çalışma zamanı hatası: Boş listeden öğe çıkarılamaz.
```

**Listeye `+` ile tek öğe eklemek.** `+` iki listeyi birleştirir; öğe eklemek için `[4]` gibi bir liste yazın ya da `ekle` kullanın.

```jus
değişken liste = [1, 2, 3]
yaz(liste + 4)
```

```
ornek.jus:2: çalışma zamanı hatası: '+' işleci iki sayı, iki metin ya da iki liste ister; liste ve sayı verildi. Dönüştürmek için metin() ya da sayı() kullanılabilir.
```

## Alıştırmalar

1. Bir sayı listesinin toplamını veren `topla(liste)` fonksiyonunu yazın. Boş listede sonuç 0 olsun.
2. `[3, 8, 12, 7, 20, 5, 6]` listesindeki çift sayıları yeni bir listeye toplayıp yazdırın.
3. `[18, 24, 9, 31, 15]` listesindeki en küçük sayıyı ve bu sayının listedeki dizinini yazdırın.
4. Bir listeyi `ters` fonksiyonunu kullanmadan, bir döngüyle ters çevirin. (İpucu: `araya_ekle` ile hep başa ekleyin.) Asıl liste değişmesin.
5. `[1, 2, 3, 4]` ile `[3, 4, 5, 6]` listelerini birleştirin ve tekrar eden öğeleri atarak yeni bir liste yapın.
6. `[3, 8, 12, 7, 20, 5, 6]` listesindeki sayıların iki katlarını `eşle` ile, 5'ten büyük olanları `süz` ile bulun.
7. `["armut", "kiraz", "ayva", "karpuz", "üzüm"]` listesini son harfine göre sıralayın.

## Çözümler

**1.**

```jus
fonksiyon topla(liste):
    değişken t = 0
    her n içinde liste:
        t += n
    dön t

yaz(topla([4, 8, 15, 16, 23, 42]))
yaz(topla([]))
```

```
108
0
```

**2.**

```jus
değişken sayılar = [3, 8, 12, 7, 20, 5, 6]
değişken çiftler = []
her n içinde sayılar:
    eğer n % 2 == 0:
        ekle(çiftler, n)
yaz(çiftler)
```

```
[8, 12, 20, 6]
```

**3.**

```jus
değişken sıcaklıklar = [18, 24, 9, 31, 15]
değişken enKüçük = sıcaklıklar[0]
her s içinde sıcaklıklar:
    eğer s < enKüçük:
        enKüçük = s
yaz("En küçük:", enKüçük)
yaz("Dizini:", bul(sıcaklıklar, enKüçük))
```

```
En küçük: 9
Dizini: 2
```

**4.**

```jus
değişken liste = [1, 2, 3, 4, 5]
değişken tersi = []
her n içinde liste:
    araya_ekle(tersi, 0, n)
yaz(tersi)
yaz(liste)
```

```
[5, 4, 3, 2, 1]
[1, 2, 3, 4, 5]
```

**5.**

```jus
değişken a = [1, 2, 3, 4]
değişken b = [3, 4, 5, 6]
değişken birleşik = a + b
değişken tekrarsız = []
her n içinde birleşik:
    eğer değil (n içinde tekrarsız):
        ekle(tekrarsız, n)
yaz(birleşik)
yaz(tekrarsız)
```

```
[1, 2, 3, 4, 3, 4, 5, 6]
[1, 2, 3, 4, 5, 6]
```

**6.**

```jus
fonksiyon ikiKat(n):
    dön n * 2

fonksiyon beşten_büyük(n):
    dön n > 5

yaz(eşle([3, 8, 12, 7, 20, 5, 6], ikiKat))
yaz(süz([3, 8, 12, 7, 20, 5, 6], beşten_büyük))
```

```
[6, 16, 24, 14, 40, 10, 12]
[8, 12, 7, 20, 6]
```

**7.**

```jus
değişken kelimeler = ["armut", "kiraz", "ayva", "karpuz", "üzüm"]

fonksiyon sonHarf(m):
    dön m[uzunluk(m) - 1]

yaz(sırala(kelimeler, sonHarf))
```

```
["ayva", "üzüm", "armut", "kiraz", "karpuz"]
```

`kiraz` ve `karpuz` aynı harfle (`z`) bittiği için özgün sıralarını korudu.

---

Önceki: [05 - Fonksiyonlar](05-fonksiyonlar.md) | [İçindekiler](README.md) | Sonraki: [07 - Metinler](07-metinler.md)
