# 05 - Fonksiyonlar

Önceki: [04 - Döngüler](04-donguler.md) | [İçindekiler](README.md) | Sonraki: [06 - Listeler](06-listeler.md)

`yaz`, `uzunluk`, `sayı` gibi fonksiyonları zaten kullandınız. Bu bölümde kendi fonksiyonlarınızı yazacaksınız. Fonksiyon, adı olan ve tekrar tekrar çağrılabilen bir talimat grubudur. Programı küçük, adlandırılmış parçalara bölmek, onu anlaşılır ve düzenli tutmanın en iyi yoludur.

## Fonksiyon tanımlamak

```jus
fonksiyon selamla(ad):
    yaz("Merhaba, " + ad + "!")

selamla("Ayşe")
selamla("Mehmet")
```

```
Merhaba, Ayşe!
Merhaba, Mehmet!
```

- `fonksiyon selamla(ad):` fonksiyonu tanımlar. `selamla` fonksiyonun adıdır.
- Parantez içindeki `ad` bir **parametredir**: fonksiyonun dışarıdan alacağı değerin adıdır. Fonksiyonun içinde sıradan bir değişken gibi kullanılır.
- Girintili satırlar fonksiyonun **gövdesidir**.
- `selamla("Ayşe")` fonksiyonu **çağırır**. Çağrıda verdiğiniz `"Ayşe"` değerine argüman denir; parametrenin değeri olur.

Tanımlamak fonksiyonu çalıştırmaz; yalnızca ne yapacağını anlatır. Fonksiyon, siz çağırdığınızda çalışır. Çağırmadan önce tanımlamış olmalısınız.

## Değer döndürmek: `dön`

Bir fonksiyon sonucunu `dön` ile çağırana verir. `dön` fonksiyonu da hemen bitirir.

```jus
fonksiyon topla(a, b):
    dön a + b

değişken sonuç = topla(3, 4)
yaz(sonuç)
yaz(topla(10, 20) * 2)

fonksiyon büyüğü(a, b):
    eğer a > b:
        dön a
    dön b

yaz(büyüğü(8, 12))
yaz(büyüğü(15, 2))
```

```
7
60
12
15
```

`büyüğü` fonksiyonunda `a > b` doğruysa `dön a` fonksiyonu bitirir; ikinci `dön` hiç çalışmaz. Böylece ayrıca `değilse` bloğu yazmaya gerek kalmaz.

`dön` kullanmazsanız ya da yalnız `dön` yazarsanız fonksiyon `boş` değerini verir:

```jus
fonksiyon sadeceYaz(m):
    yaz(m)

değişken x = sadeceYaz("selam")
yaz(x)

fonksiyon erken(n):
    eğer n < 0:
        dön
    yaz("n negatif değil")

yaz(erken(-1))
erken(5)
```

```
selam
boş
boş
n negatif değil
```

`sadeceYaz` bir şey döndürmediği için `x` değişkeni `boş` oldu. `erken(-1)` hemen çıktığı için ekrana yalnızca `boş` yazıldı (bu, `yaz(erken(-1))`'in sonucudur).

Argüman sayısı parametre sayısına eşit olmalıdır. Az ya da çok argüman vermek hatadır (aşağıda göreceğiz).

## Kapsam: değişkenler nerede geçerlidir?

Bir değişkenin görülebildiği yere **kapsam** denir.

- Programın en üstünde tanımlanan değişkenler **geneldir**; her yerden okunabilir.
- Bir fonksiyonun ya da bloğun içinde tanımlanan değişkenler **yereldir**; yalnızca o blokta, tanımlandıkları satırdan sonra geçerlidir.
- İç blokta dış blokla aynı adı yeniden tanımlarsanız dıştakini **gölgelersiniz**: içeride yeni değişken kullanılır, dıştaki değişmez.

```jus
değişken genel = "ben geneldim"

fonksiyon dene1():
    değişken yerel = "ben yerelim"
    yaz(genel)
    yaz(yerel)

dene1()

fonksiyon gölge():
    değişken genel = "fonksiyonun içindeki genel"
    yaz(genel)

gölge()
yaz(genel)

fonksiyon değiştir():
    genel = "değiştirildi"

değiştir()
yaz(genel)

eğer doğru:
    değişken blokta = 1
    yaz(blokta)
```

```
ben geneldim
ben yerelim
fonksiyonun içindeki genel
ben geneldim
değiştirildi
1
```

Üç davranış görüyoruz:

- `dene1` içinden genel değişken okunabildi.
- `gölge` kendi `genel` değişkenini tanımladığı için dıştaki değişmedi.
- `değiştir` içinde `değişken` yazmadan atama yaptığımız için dıştaki değişkenin kendisi değişti.

Genel değişkenleri fonksiyon içinden değiştirmek kolaydır ama programı anlaşılmaz yapar. Mümkünse fonksiyona değeri parametre olarak verin, sonucu `dön` ile alın.

## Özyineleme

Bir fonksiyon kendini çağırabilir. Buna **özyineleme** denir. Özyinelemeli her fonksiyonda iki parça olmalıdır: işin bittiği **taban durum** ve problemi küçülten **özyineleme adımı**.

```jus
fonksiyon faktöriyel(n):
    eğer n <= 1:
        dön 1
    dön n * faktöriyel(n - 1)

yaz(faktöriyel(5))
yaz(faktöriyel(10))

fonksiyon fibonacci(n):
    eğer n < 2:
        dön n
    dön fibonacci(n - 1) + fibonacci(n - 2)

her i içinde aralık(10):
    yaz(fibonacci(i))

fonksiyon gerisayım(n):
    eğer n == 0:
        yaz("Başla!")
        dön
    yaz(n)
    gerisayım(n - 1)

gerisayım(3)
```

```
120
3628800
0
1
1
2
3
5
8
13
21
34
3
2
1
Başla!
```

`faktöriyel(5)` şöyle çalışır: `5 * faktöriyel(4)`, o da `4 * faktöriyel(3)`... `faktöriyel(1)` 1 verince zincir geri çözülür. Taban durumu (`n <= 1`) olmasaydı fonksiyon sonsuza kadar kendini çağırırdı. JUS iç içe çağrı derinliğini sınırlar; sınır aşılırsa hata verir (aşağıda).

## Boş fonksiyon gövdesi: `geç`

Bir fonksiyonun gövdesi de boş olamaz. Henüz yazmadığınız bir fonksiyonun yerini tutmak için `geç` kullanın:

```jus
fonksiyon sonra_yazılacak():
    geç

fonksiyon selamla(ad):
    dön "Merhaba, " + ad

yaz(sonra_yazılacak())
yaz(selamla("Ali"))
```

```
boş
Merhaba, Ali
```

Gövdesi yalnızca `geç` olan fonksiyon `boş` döndürür. (`geç` dördüncü bölümde anlatıldı.)

## Fonksiyonlar birer değerdir

JUS'ta fonksiyonlar sıradan değerlerdir: bir değişkene atanabilir, başka bir fonksiyona argüman olarak verilebilir ve bir fonksiyondan döndürülebilir. Bunun en yararlı kullanımını altıncı bölümde göreceksiniz: `sırala`, `eşle` ve `süz` yerleşik fonksiyonları, ne yapacaklarını bir fonksiyon argümanından öğrenir.

## İç fonksiyonlar (kısaca)

Bir fonksiyonun içinde başka bir fonksiyon tanımlayabilirsiniz. İç fonksiyon, dış fonksiyon bittikten sonra bile dış fonksiyonun değişkenlerine erişmeyi sürdürür. Buna **kapanım** denir.

```jus
fonksiyon sayaçYap():
    değişken sayı5 = 0
    fonksiyon artır():
        sayı5 += 1
        dön sayı5
    dön artır

değişken a = sayaçYap()
değişken b = sayaçYap()
yaz(a())
yaz(a())
yaz(a())
yaz(b())

fonksiyon ikiKat(f, x):
    dön f(f(x))

fonksiyon ekleBir(n):
    dön n + 1

yaz(ikiKat(ekleBir, 10))

değişken kısayol = ekleBir
yaz(kısayol(99))
```

```
1
2
3
1
12
100
```

`sayaçYap` her çağrıldığında kendine ait yeni bir `sayı5` değişkeni oluşturur. `a` ve `b` ayrı sayaçlardır; biri diğerini etkilemez. Fonksiyonun adı (`ekleBir`) parantezsiz yazılınca fonksiyonun kendisidir; `ikiKat(ekleBir, 10)` çağrısında fonksiyon argüman olarak taşındı. Bu konu ileri düzeydir; ilk okumada anlamadıysanız sonraya bırakabilirsiniz.

## Sık yapılan hatalar

**Yanlış sayıda argüman vermek.**

```jus
fonksiyon topla(a, b):
    dön a + b

yaz(topla(1))
```

```
ornek.jus:4: çalışma zamanı hatası: 'topla' fonksiyonu 2 argüman bekliyor, 1 verildi.
```

**Yerel değişkeni dışarıda kullanmak.**

```jus
fonksiyon hesapla():
    değişken gizli = 42

hesapla()
yaz(gizli)
```

```
ornek.jus:5: çalışma zamanı hatası: 'gizli' adında bir değişken ya da fonksiyon tanımlı değil.
```

**`dön` yazmayı unutmak.** Bu hata iletisi vermez, ama sonuç `boş` olur:

```jus
fonksiyon kare(x):
    x * x

yaz(kare(4))
```

```
boş
```

`x * x` hesaplanır ama sonuç atılır. Doğrusu `dön x * x` yazmaktır. Fonksiyondan beklediğiniz değer yerine `boş` görüyorsanız ilk bakacağınız yer `dön` olsun.

**`dön`'ü fonksiyon dışında kullanmak.**

```jus
yaz("başla")
dön 5
```

```
ornek.jus:2:1: sözdizimi hatası: 'dön' yalnızca bir fonksiyonun içinde kullanılabilir.
    dön 5
    ^
```

**Taban durumu olmayan özyineleme.**

```jus
fonksiyon sonsuz(n):
    dön sonsuz(n + 1)

yaz(sonsuz(1))
```

```
ornek.jus:2: çalışma zamanı hatası: Yığın taştı; fonksiyon çağrıları çok derine indi (sonsuz özyineleme olabilir).
    satır 2, 'sonsuz' fonksiyonu
    satır 2, 'sonsuz' fonksiyonu
    satır 2, 'sonsuz' fonksiyonu
    satır 2, 'sonsuz' fonksiyonu
    satır 2, 'sonsuz' fonksiyonu
    satır 2, 'sonsuz' fonksiyonu
    satır 2, 'sonsuz' fonksiyonu
    satır 2, 'sonsuz' fonksiyonu
    satır 2, 'sonsuz' fonksiyonu
    satır 2, 'sonsuz' fonksiyonu
    ... (1013 çağrı daha)
    satır 4, ana program
```

Bu iletiye çağrı zinciri de denir: hatanın hangi fonksiyonun, hangi fonksiyon tarafından çağrılmasıyla oluştuğunu gösterir. Zincirin en altı hep `ana program`dır.

## Alıştırmalar

1. Bir sayının karesini veren `kare` fonksiyonunu yazın. `kare(7)` ve `kare(1.5)` ile deneyin.
2. Üç sayının ortalamasını veren `ortalama(a, b, c)` fonksiyonunu yazın.
3. `üssü(taban, üs)` fonksiyonunu özyinelemeyle yazın (üs sıfırsa sonuç 1'dir). `üssü(2, 10)` ve `üssü(5, 3)` sonuçlarını yazdırın.
4. Sayının çift olup olmadığını `doğru`/`yanlış` olarak veren `çiftMi(n)` fonksiyonunu yazın. 1'den 5'e kadar her sayı için sonucu yazdırın.
5. 1'den `n`'e kadar sayıların toplamını özyinelemeyle bulan `topla(n)` fonksiyonunu yazın. `topla(10)` ve `topla(100)` ne verir?

## Çözümler

**1.**

```jus
fonksiyon kare(n):
    dön n * n

yaz(kare(7))
yaz(kare(1.5))
```

```
49
2.25
```

**2.**

```jus
fonksiyon ortalama(a, b, c):
    dön (a + b + c) / 3

yaz(ortalama(70, 80, 90))
yaz(ortalama(1, 2, 4))
```

```
80
2.3333333333333
```

**3.**

```jus
fonksiyon üssü(taban, üs):
    eğer üs == 0:
        dön 1
    dön taban * üssü(taban, üs - 1)

yaz(üssü(2, 10))
yaz(üssü(5, 3))
```

```
1024
125
```

**4.**

```jus
fonksiyon çiftMi(n):
    dön n % 2 == 0

her n içinde aralık(1, 6):
    yaz(n, çiftMi(n))
```

```
1 yanlış
2 doğru
3 yanlış
4 doğru
5 yanlış
```

**5.**

```jus
fonksiyon topla(n):
    eğer n == 0:
        dön 0
    dön n + topla(n - 1)

yaz(topla(10))
yaz(topla(100))
```

```
55
5050
```

---

Önceki: [04 - Döngüler](04-donguler.md) | [İçindekiler](README.md) | Sonraki: [06 - Listeler](06-listeler.md)
