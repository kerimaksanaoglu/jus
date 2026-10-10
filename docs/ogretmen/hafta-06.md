# Hafta 6 - Listeler

[Öğretmen kiti](README.md) | Önceki: [Hafta 5](hafta-05.md) | Sonraki: [Hafta 7 - Metin işlemleri](hafta-07.md)

İlgili rehber bölümü: [06 - Listeler](../rehber/06-listeler.md)

Şimdiye kadar her değişken tek bir değer tutuyordu. Liste, birçok değeri sıralı biçimde tek bir adın altında tutar; döngülerle birlikte kullanıldığında öğrencilerin ilk "gerçek" programlarını mümkün kılar. Bu haftanın iki riskli noktası: dizinin **0'dan** başlaması ve listelerin **başvuruyla** taşınması (`b = a` kopyalama değildir).

## Kazanımlar

Hafta sonunda öğrenci:

1. Liste oluşturabilir; `liste[i]`, negatif dizin ve dilimle öğelere erişebilir; ilk öğenin dizininin 0 olduğunu bilir.
2. `ekle`, `sil`, `çıkar` ve `araya_ekle` ile listeyi değiştirebilir.
3. `her öğe içinde liste` ile listeyi gezerek toplam, sayma ve en büyük/en küçük bulma işlerini yapabilir.
4. `sırala`, `ters`, `uzunluk`, `içinde` ve `bul` fonksiyonlarını kullanabilir; `sırala`'nın yeni bir liste verdiğini bilir.
5. İki değişkenin aynı listeyi gösterebileceğini küçük bir örnekle gösterebilir.

## Ön hazırlık

- Tarayıcıda deneme alanı: <https://kerimaksanaoglu.github.io/jus/>.
- "Dizin kartları" etkinliği için: 6 numaralı kart (içlerinde sayılar yazılı), yere yapıştırılacak 0'dan 5'e kadar dizin kâğıtları, 4 boş kart (ekleme için).
- Sınav 1 yapıldıysa sonuçları ilk 10 dakikada değerlendirin: en çok yanlış yapılan 2-3 soruyu tahtada birlikte çözün. Yanlış cevapların çoğu bu kitin "Öğrenciler nerede takılır" bölümlerindeki hatalarla örtüşecektir.

## Ders akışı

| Süre | Evre | Ne yapılır |
|------|------|------------|
| 0-8 | Isınma | Sınav ya da geçen haftadan bir kısa fonksiyon (ör. `kare`). Sorun: "5 sınav notunu tek tek `not1`, `not2`... diye değişkende mi tutarız? 30 öğrenci olsa?" |
| 8-14 | Yeni kavram | Liste = sıralı kutular dizisi; dizin = kutunun numarası, 0'dan başlar. |
| 14-28 | Canlı kodlama 1 | Adım 1-3: oluşturma, dizin, değiştirme, ekleme, gezme. |
| 28-40 | Etkinlik | "Dizin kartları" (bilgisayarsız). |
| 40-52 | Canlı kodlama 2 | Adım 4-7: dilim, `sırala`, en büyüğü bulma, başvuru tuzağı. |
| 52-72 | Öğrenci uygulaması | Alıştırma 6.1-6.6, eşli. Hızlılar 6.7-6.9'a geçer. |
| 72-76 | Paylaşım | Bir çift 6.7'nin "asıl liste değişmedi" kısmını anlatır. |
| 76-80 | Kapanış | Çıkış bileti. |

## Canlı kodlama betiği

### Adım 1 - Liste ve dizin

```jus
değişken notlar = [85, 92, 78]
yaz(notlar)
yaz(notlar[0])
yaz(notlar[2])
yaz(notlar[-1])
yaz(uzunluk(notlar))
```

```
[85, 92, 78]
85
78
78
3
```

Söyleyin: "Köşeli parantez içinde virgülle ayrılmış değerler: liste. Her öğenin bir numarası var ve numara **sıfırdan** başlıyor. Eksi sayılar sondan sayar: `-1` son öğe."

Sorun: "Üç öğeli listede en büyük dizin kaç?" (2.) "Listeye üç öğe varken `notlar[3]` yazarsak?" (Hata; aşağıda bölümünde.)

### Adım 2 - Değiştirmek, eklemek, silmek

```jus
değişken meyveler = ["elma", "armut"]
ekle(meyveler, "muz")
yaz(meyveler)
meyveler[0] = "kiraz"
yaz(meyveler)
sil(meyveler, 1)
yaz(meyveler)
araya_ekle(meyveler, 0, "üzüm")
yaz(meyveler)
yaz(çıkar(meyveler))
yaz(meyveler)
```

```
["elma", "armut", "muz"]
["kiraz", "armut", "muz"]
["kiraz", "muz"]
["üzüm", "kiraz", "muz"]
muz
["üzüm", "kiraz"]
```

Söyleyin: "`ekle` sona ekler. `sil(liste, dizin)` o numaradaki öğeyi çıkarır; ondan sonraki öğelerin numarası bir azalır. `çıkar` son öğeyi alıp geri verir." Öğrencilerin çıktının her satırını elleriyle takip etmesini isteyin.

### Adım 3 - Listeyi gezmek

```jus
değişken notlar = [85, 92, 78, 64]

değişken toplam = 0
her puan içinde notlar:
    toplam += puan
yaz("Toplam:", toplam)
yaz("Ortalama:", toplam / uzunluk(notlar))

her i içinde aralık(uzunluk(notlar)):
    yaz(i, "->", notlar[i])
```

```
Toplam: 319
Ortalama: 79.75
0 -> 85
1 -> 92
2 -> 78
3 -> 64
```

Söyleyin: "`her öğe içinde liste`: sırayla her öğeyi alır. Dizin numarası da lazımsa `aralık(uzunluk(liste))` kullanırız." Hafta 4'teki toplam kalıbının aynısı olduğunu vurgulayın.

### Adım 4 - En büyük değeri bulmak

```jus
değişken sayılar = [18, 24, 9, 31, 15]
değişken enBüyük = sayılar[0]
her s içinde sayılar:
    eğer s > enBüyük:
        enBüyük = s
yaz("En büyük:", enBüyük)
```

```
En büyük: 31
```

Hatırlatın: Hafta 3'te `a`, `b`, `c` için yaptığımız kalıp bu; liste uzasa da program değişmiyor.

### Adım 5 - Dilim

```jus
değişken l = [10, 20, 30, 40, 50]
yaz(l[1:3])
yaz(l[:2])
yaz(l[2:])
yaz(l[-2:])
yaz(l)
```

```
[20, 30]
[10, 20]
[30, 40, 50]
[40, 50]
[10, 20, 30, 40, 50]
```

Söyleyin: "`l[baş:son]` baştan sona kadar, **son dahil değil** (`aralık` gibi). Baş ya da son yazılmazsa uçlar alınır. Dilim yeni bir liste verir; `l` değişmedi."

### Adım 6 - Sıralama, ters çevirme, arama

```jus
değişken puanlar = [40, 10, 30, 20]
yaz(sırala(puanlar))
yaz(puanlar)
yaz(ters(puanlar))
yaz(30 içinde puanlar)
yaz(35 içinde puanlar)
yaz(bul(puanlar, 30))
yaz(bul(puanlar, 35))
```

```
[10, 20, 30, 40]
[40, 10, 30, 20]
[20, 30, 10, 40]
doğru
yanlış
2
-1
```

Söyleyin: "`sırala` ve `ters` **yeni bir liste** verir; `puanlar` hâlâ eski sırada. `içinde` 'var mı' sorusunu cevaplar. `bul` öğenin dizinini verir, yoksa `-1`."

### Adım 7 - Listeler başvuruyla taşınır

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
```

```
[1, 2, 3, 4]
[1, 2, 3, 4]
[1, 2, 3, 4]
[1, 2, 3, 4, 5]
```

Söyleyin: "`b = a` yeni bir liste yapmadı; `b` ve `a` aynı listenin iki adı. Birinden eklediğimiz öğeyi diğerinde de görürüz. `a + []` ise yeni bir liste üretir, `c` bağımsızdır." Benzetme: aynı ev, iki anahtar. Fonksiyona liste verildiğinde de aynı şey olur; fonksiyon çağıranın listesini değiştirebilir.

### Adım 8 - İsteğe bağlı: `eşle` ve `süz`

```jus
fonksiyon kare(x):
    dön x * x

fonksiyon büyükMü(x):
    dön x > 10

yaz(eşle([1, 2, 3, 4], kare))
yaz(süz([4, 12, 7, 20], büyükMü))
```

```
[1, 4, 9, 16]
[12, 20]
```

Söyleyin: "`eşle` her öğeye fonksiyonu uygular, `süz` fonksiyonun `doğru` dediği öğeleri tutar. Fonksiyonu **adıyla, parantezsiz** veriyoruz: çağırmıyoruz, fonksiyonu teslim ediyoruz." Bu adım yalnızca ilgili öğrenciler içindir; sınavda sorulmaz.

## Sınıf içi etkinlik: Dizin kartları (bilgisayarsız, 12 dk)

Amaç: Dizinin sıfırdan başlamasını, negatif dizini, dilimi ve ekleme/silmenin dizinleri kaydırdığını bedenle görmek.

Hazırlık: Yere 0'dan 5'e kadar yazılı altı kâğıt dizin; öğrenciler soldan sağa dizine göre dururlar. Her birinin elinde bir sayı kartı var: `[7, 3, 9, 4, 8, 1]`. Bir öğrenci "bilgisayar"dır ve komutları söyler.

| Komut | Beklenen eylem |
|-------|----------------|
| `liste[0]` | 0. dizindeki öğrenci elindeki sayıyı söyler (7). |
| `liste[5]` ve `liste[-1]` | Aynı kişi (1). Eksi dizin sondan sayar. |
| `liste[1:4]` | 1., 2. ve 3. dizindeki öğrenciler öne bir adım çıkar (3, 9, 4). 4. dizindeki dahil değildir. |
| `ekle(liste, 6)` | Boş kartlı bir öğrenci en sona katılır; dizini 6 olur. |
| `sil(liste, 0)` | 0. dizindeki öğrenci çıkar; **herkes bir adım sola kayar**, dizinler bir azalır. Şimdi `liste[0]` kim? (3) |
| `araya_ekle(liste, 1, 5)` | 1. dizine bir öğrenci girer; sonrakiler sağa kayar. |
| `sırala(liste)` | Sıra, sayılara göre yeni bir sıra olarak kurulur. Sorun: "Eski sırayı bozdunuz mu, yoksa yeni bir sıra mı kurdunuz?" (Yeni bir liste kurulur; `liste` olduğu gibi kalır.) |

Gözlem soruları:

- Dizi neden sıfırdan başlıyor? ("Kaç adım ileride?" olarak düşünün: ilk öğe başlangıçta, yani 0 adım ileride.)
- `liste[6]` dendiğinde kimse yok. Bilgisayar ne yapmalı? (Hata verir; aşağıdaki "nerede takılır" bölümüne bağlayın.)

## Alıştırmalar

Çözümler [cevap anahtarında](cevap-anahtari.md#hafta-6).

### Kolay

**Alıştırma 6.1.** `l = [10, 20, 30, 40, 50]` listesinde ilk öğeyi, son öğeyi, öğe sayısını ve ortadaki (2. dizin) öğeyi aşağıdaki gibi yazdırın.

Beklenen çıktı:

```
İlk: 10
Son: 50
Öğe sayısı: 5
Ortadaki: 30
```

**Alıştırma 6.2.** `["elma", "armut", "muz"]` listesine `"kiraz"` ekleyin, `"armut"`'u silin (dizinini kendiniz bulmak zorunda değilsiniz, 1. dizindir) ve listeyi önce ve sonra yazdırın.

Beklenen çıktı:

```
Önce: ["elma", "armut", "muz"]
Sonra: ["elma", "muz", "kiraz"]
```

**Alıştırma 6.3.** Bir sayı listesinin toplamını veren `topla(liste)` fonksiyonunu yazın (boş listede 0). `[3, 8, 12, 7, 20, 5, 6]` ve `[]` için çağırın.

Beklenen çıktı:

```
61
0
```

### Orta

**Alıştırma 6.4.** `[3, 8, 12, 7, 20, 5, 6]` listesindeki çift sayıları yeni bir listeye toplayıp yazdırın.

Beklenen çıktı:

```
[8, 12, 20, 6]
```

**Alıştırma 6.5.** `[18, 24, 9, 31, 15]` listesindeki en küçük sayıyı ve bu sayının listedeki dizinini yazdırın.

Beklenen çıktı:

```
En küçük: 9 dizin: 2
```

**Alıştırma 6.6.** `[60, 75, 90, 45, 80]` notlarının ortalamasını bulun, ortalamanın üstündeki notlardan yeni bir liste yapıp ortalamayla birlikte yazdırın.

Beklenen çıktı:

```
Ortalama: 70
Ortalamanın üstünde: [75, 90, 80]
```

### Zor

**Alıştırma 6.7.** Bir listeyi `ters` fonksiyonunu kullanmadan, bir döngüyle ters çevirin. (İpucu: `araya_ekle(yeni, 0, öğe)` hep başa ekler.) Asıl liste değişmesin. `[1, 2, 3, 4]` için ikisini de yazdırın.

Beklenen çıktı:

```
Asıl: [1, 2, 3, 4]
Ters: [4, 3, 2, 1]
```

**Alıştırma 6.8.** `[1, 2, 3, 4]` ile `[3, 4, 5, 6]` listelerini birleştirin ve tekrar eden öğeleri atarak yeni bir liste yapın.

Beklenen çıktı:

```
[1, 2, 3, 4, 5, 6]
```

**Alıştırma 6.9.** Bir listenin küçükten büyüğe sıralı olup olmadığını döndüren `sıralıMı(liste)` fonksiyonunu yazın. `[1, 2, 2, 5]`, `[3, 1, 2]` ve `[]` için çağırın.

Beklenen çıktı:

```
doğru
yanlış
doğru
```

## Öğrenciler nerede takılır

### 1. Sınır dışı dizin

```jus
değişken liste = [10, 20, 30]
yaz(liste[3])
```

```
ornek.jus:2: çalışma zamanı hatası: Dizin sınırların dışında: uzunluk 3, istenen dizin 3.
```

Üç öğeli listenin son dizini 2'dir. İleti hem uzunluğu hem istenen dizini söylediği için öğrenci çoğunlukla kendi bulur. Yönlendirme: "Listenin uzunluğu kaç? Son dizin kaç?" Sondaki öğeye ulaşmanın güvenli yolu `liste[-1]`'dir.

### 2. `ekle`'nin sonucunu kullanmak

```jus
değişken liste = [1, 2, 3]
değişken yeni = ekle(liste, 4)
yaz(yeni[0])
```

```
ornek.jus:3: çalışma zamanı hatası: Yalnızca liste, metin ve sözlük dizinlenebilir; boş verildi.
```

`ekle` listeyi **yerinde** değiştirir ve hiçbir şey (`boş`) döndürür. İleti "boş dizinlenemez" der; öğrenci `yeni`'nin neden boş olduğunu anlamakta zorlanır. Yönlendirme: "`ekle` bir değer mi veriyor yoksa listeyi mi değiştiriyor? Hafta 5'teki `yaz` ve `dön` farkını hatırlayalım." Çözüm: `ekle(liste, 4)` tek başına yazılır ve `liste` kullanılır.

### 3. `sırala` listeyi değiştirdi sanmak

```jus
değişken puanlar = [40, 10, 30]
sırala(puanlar)
yaz(puanlar)
```

```
[40, 10, 30]
```

Hata yoktur; liste sıralanmamıştır. `sırala` yeni bir liste döndürür ve burada sonuç atılmıştır. Çözüm: `puanlar = sırala(puanlar)` ya da `değişken sıralı = sırala(puanlar)`. Aynı durum `ters` için de geçerlidir.

### 4. Listeyi kopyaladığını sanmak

```jus
değişken asıl = [1, 2, 3]
değişken kopya = asıl
ekle(kopya, 99)
yaz("asıl:", asıl)
```

```
asıl: [1, 2, 3, 99]
```

Hata yoktur; ama `asıl` değişmişti. Yönlendirme: etkinlikteki "iki anahtar, tek ev" benzetmesi. Gerçek kopya için `asıl + []` yazılır. Fonksiyona verilen listenin de aynı şekilde değişebileceğini, bunun bazen istenen davranış olduğunu (ör. `ekle` yazan bir fonksiyon) söyleyin.

### 5. Başka bir dilin yöntemlerini denemek

Python bilen öğrenciler `liste.append(4)` yazar:

```jus
değişken liste = [1, 2, 3]
liste.append(4)
```

```
ornek.jus:2: çalışma zamanı hatası: liste türündeki değerlerin 'append' adında bir yöntemi yok. JUS'ta bunun karşılığı 'ekle' yerleşik fonksiyonudur: ekle(liste, ...).
```

JUS'ta liste işlemleri nesnenin yöntemi değil, sıradan fonksiyonlardır: `ekle(liste, 4)`. İleti türün ve yöntem adının ne olduğunu söyler. Bu, [Python köprüsü](../python-koprusu.md) için de güzel bir konuşma fırsatıdır: aynı fikir (listeye ekleme), farklı yazım.

## Çıkış bileti

1. `l = [4, 8, 15]` için `l[1]`, `l[-1]` ve `uzunluk(l)` ne verir?
2. `a = [1, 2]` ve `b = a` yazdıktan sonra `ekle(b, 3)` yaparsak `a` ne olur?
3. `sırala(puanlar)` yazdık ama `puanlar` sıralanmadı. Neden?

Soru 1 ve 2 için program:

```jus
değişken l = [4, 8, 15]
yaz(l[1], l[-1], uzunluk(l))

değişken a = [1, 2]
değişken b = a
ekle(b, 3)
yaz(a)
```

```
8 15 3
[1, 2, 3]
```

Cevaplar:

1. `8`, `15`, `3`.
2. `a` da `[1, 2, 3]` olur; `b` ve `a` aynı listeyi gösterir.
3. `sırala` yeni bir sıralı liste döndürür, `puanlar`'ı değiştirmez. Sonucu bir değişkene atamak gerekir.

## Ev çalışması (isteğe bağlı)

1. Sınıfta sevdiğiniz beş şarkı, film ya da yemekten bir liste yapın; ilkini, sonuncusunu ve kaç öğe olduğunu yazdırın; sonra bir öğe ekleyip birini silin.
2. Beş kişinin boyunu listeye yazın. Boyların ortalamasını, en uzununu ve en kısasını bulun.
3. Hafta 5'te yazdığınız `asalMı` fonksiyonunu kullanarak 1'den 50'ye kadar asal sayıların listesini yapın ve kaç tane olduğunu `uzunluk` ile yazdırın.
