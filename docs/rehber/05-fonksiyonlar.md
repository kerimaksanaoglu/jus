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

Varsayılan değeri olmayan her parametre için çağrıda bir argüman vermelisiniz; parametre sayısından fazla argüman vermek de hatadır (aşağıda göreceğiz). Varsayılan değeri olan parametreler ise isteğe bağlıdır; bir sonraki bölüm onları anlatıyor.

## Varsayılan değerler

Bir parametrenin adından sonra `=` ve bir ifade yazarsanız o parametre **isteğe bağlı** olur. Çağrıda argüman verilmezse varsayılan değer kullanılır, verilirse onun yerine verilen değer:

```jus
fonksiyon selamla(ad, selam = "Merhaba"):
    yaz(selam + ", " + ad + "!")

selamla("Ayşe")
selamla("Mehmet", "Günaydın")
```

```
Merhaba, Ayşe!
Günaydın, Mehmet!
```

Kurallar şunlardır:

- Varsayılan değeri olan parametrelerden sonra varsayılan değeri olmayan parametre gelemez. `fonksiyon f(a = 1, b)` yazılamaz; isteğe bağlı parametreler listenin sonunda toplanır.
- Varsayılan ifade, argüman verilmediğinde **her çağrıda**, fonksiyonun gövdesi başlamadan önce yeniden hesaplanır. Argüman verilirse hiç hesaplanmaz.
- Varsayılan ifade kendinden önceki parametreleri kullanabilir.
- Açıkça `boş` vermek varsayılanı devreye sokmaz: parametre `boş` olur.
- Varsayılan değerler yöntemlerde (`kur` dahil, 11. bölüme bakın) ve iç fonksiyonlarda da aynı biçimde çalışır.

İlk iki kuralı bir arada görelim. `yeniNo` her hesaplandığında sayacı bir artırır; sonuçta kaç kez hesaplandığına bakarak varsayılanın ne zaman çalıştığını anlarız:

```jus
değişken sayaç = 0

fonksiyon yeniNo():
    sayaç += 1
    dön sayaç

fonksiyon fiş(ad, no = yeniNo()):
    dön ad + " #" + metin(no)

yaz(fiş("elma"))
yaz(fiş("armut"))
yaz(fiş("kiraz", 99))
yaz(fiş("üzüm"))
yaz(sayaç)
```

```
elma #1
armut #2
kiraz #99
üzüm #3
3
```

`fiş("kiraz", 99)` çağrısında argüman verildiği için `yeniNo()` çalışmadı; sayaç yalnızca üç kez arttı. Üçüncü ve dördüncü kurallar için:

```jus
fonksiyon kayıt(ad, etiket = ad + "-1"):
    dön ad + ":" + etiket

yaz(kayıt("a"))
yaz(kayıt("b", "özel"))

fonksiyon göster(x = 5):
    yaz(x)

göster()
göster(boş)
```

```
a:a-1
b:özel
5
boş
```

`etiket` varsayılanı `ad` parametresini kullandı. `göster(boş)` çağrısında `x` varsayılan olan 5 değil, açıkça verilen `boş` oldu.

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

## Değişken sayıda argüman

Bazen bir fonksiyona kaç argüman verileceği önceden bilinmez. Son parametrenin adının önüne `*` koyarsanız, adlı parametrelerden sonra gelen bütün argümanlar **yeni bir liste** olarak o parametreye toplanır. Listeleri altıncı bölümde ayrıntılı göreceksiniz; burada `her` ile gezmek ve `yaz` ile yazdırmak yeterli.

```jus
fonksiyon topla(*sayılar):
    değişken t = 0
    her s içinde sayılar:
        t += s
    dön t

yaz(topla())
yaz(topla(5))
yaz(topla(1, 2, 3, 4))

fonksiyon etiketle(etiket, *öğeler):
    yaz(etiket + ":", öğeler)

etiketle("meyveler", "elma", "armut")
etiketle("boş")
```

```
0
5
10
meyveler: ["elma", "armut"]
boş: []
```

- `*` yalnızca **son** parametrenin önüne yazılabilir.
- Toplanacak argüman kalmadıysa parametre boş listedir (`[]`); `topla()` bu yüzden 0 verir.
- `*` parametresi varsayılan değerli parametrelerle birlikte kullanılabilir. Sıra: önce zorunlu parametreler, sonra varsayılanlı olanlar, en sonda `*` parametresi.

```jus
fonksiyon f(a, b = 2, *kalan):
    yaz(a, b, kalan)

f(1)
f(1, 5)
f(1, 5, 7, 9)
```

```
1 2 []
1 5 []
1 5 [7, 9]
```

Argümanlar önce `a`'ya, sonra `b`'ye, artanlar `kalan` listesine gider. `*` çağrının tarafında kullanılamaz: elinizdeki bir listeyi `f(*liste)` biçiminde argümanlara açamazsınız. Listeyi gezmek istiyorsanız onu sıradan bir parametre olarak, tek argüman halinde verin.

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

Varsayılan değerli ya da `*` parametreli fonksiyonlarda ileti, kabul edilen aralığı söyler:

```jus
fonksiyon alan(en, boy = 1):
    dön en * boy

yaz(alan(4, 3, 2))
```

```
ornek.jus:4: çalışma zamanı hatası: 'alan' fonksiyonu en az 1, en çok 2 argüman bekliyor, 3 verildi.
```

`*` parametresi olan fonksiyonun üst sınırı olmadığı için ileti yalnızca alt sınırı söyler: `'etiketle' fonksiyonu en az 1 argüman bekliyor, 0 verildi.`

**Varsayılanı olmayan parametreyi varsayılanlıdan sonra yazmak.**

```jus
fonksiyon alan(en = 1, boy):
    dön en * boy
```

```
ornek.jus:1:24: sözdizimi hatası: Varsayılan değeri olan parametreden sonra varsayılan değeri olmayan parametre gelemez.
    fonksiyon alan(en = 1, boy):
                           ^
```

Çözüm: `fonksiyon alan(boy, en = 1)`.

**`*` parametresinden sonra başka parametre yazmak.**

```jus
fonksiyon topla(*sayılar, ek):
    dön ek
```

```
ornek.jus:1:27: sözdizimi hatası: '*' ile işaretlenen parametre sonuncu olmalıdır.
    fonksiyon topla(*sayılar, ek):
                              ^
```

**Çağrıda listeyi `*` ile açmaya çalışmak.**

```jus
fonksiyon topla(*sayılar):
    dön uzunluk(sayılar)

değişken liste = [1, 2, 3]
yaz(topla(*liste))
```

```
ornek.jus:5:11: sözdizimi hatası: İfade bekleniyor.
    yaz(topla(*liste))
              ^
```

`*` yalnızca parametre listesinde anlam taşır. Çağrıda yazılan `*` çarpma işleci sayılır ve solunda bir ifade beklenir.

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
6. `indirim(fiyat, oran = 10)` fonksiyonunu yazın: fiyattan yüzde `oran` kadar indirim yapılmış tutarı versin. `indirim(200)` ve `indirim(200, 25)` ne verir?
7. En az bir sayı alan `enBüyük(ilk, *diğerleri)` fonksiyonunu yazın; aldığı sayıların en büyüğünü versin. `enBüyük(3)` ve `enBüyük(4, 9, 2)` ile deneyin.

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

**6.**

```jus
fonksiyon indirim(fiyat, oran = 10):
    dön fiyat - fiyat * oran / 100

yaz(indirim(200))
yaz(indirim(200, 25))
```

```
180
150
```

**7.**

```jus
fonksiyon enBüyük(ilk, *diğerleri):
    değişken büyük = ilk
    her s içinde diğerleri:
        eğer s > büyük:
            büyük = s
    dön büyük

yaz(enBüyük(3))
yaz(enBüyük(4, 9, 2))
```

```
3
9
```

`ilk` parametresi en az bir argüman verilmesini zorunlu kılar; böylece karşılaştıracak bir başlangıç değeri hep vardır.

---

Önceki: [04 - Döngüler](04-donguler.md) | [İçindekiler](README.md) | Sonraki: [06 - Listeler](06-listeler.md)
