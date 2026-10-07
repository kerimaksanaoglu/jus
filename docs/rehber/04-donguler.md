# 04 - Döngüler

Önceki: [03 - Kararlar](03-kararlar.md) | [İçindekiler](README.md) | Sonraki: [05 - Fonksiyonlar](05-fonksiyonlar.md)

Bilgisayarlar tekrar işinde insanlardan çok daha iyidir. Aynı satırları yüz kez yazmak yerine bir **döngü** kurarsınız. Döngü, bir bloğu birden çok kez çalıştıran yapıdır. JUS'ta iki döngü vardır: `iken` ve `her ... içinde`.

## `iken` döngüsü

`iken` koşul `doğru` olduğu sürece bloğu tekrarlar. Her turdan önce koşul yeniden denetlenir.

```jus
değişken sayaç = 1
iken sayaç <= 5:
    yaz("Sayaç:", sayaç)
    sayaç += 1
yaz("Döngü bitti, sayaç =", sayaç)
```

```
Sayaç: 1
Sayaç: 2
Sayaç: 3
Sayaç: 4
Sayaç: 5
Döngü bitti, sayaç = 6
```

Adım adım:

1. `sayaç` 1 ile başlar.
2. Koşul (`sayaç <= 5`) denetlenir. Doğruysa blok çalışır.
3. Blokta `sayaç` bir artar.
4. Başa dönülür; koşul yeniden denetlenir.
5. `sayaç` 6 olunca koşul `yanlış` olur ve döngü biter. Program döngünün altındaki satırdan sürer.

Döngü bloğunun içinde koşulu değiştiren bir şey olmalıdır. Aksi halde döngü hiç bitmez. Buna **sonsuz döngü** denir. Sonsuz döngüye girmiş bir programı terminalde `Ctrl+C` tuşlarıyla durdurabilirsiniz.

## `her ... içinde` döngüsü

Çoğu zaman bir koleksiyonun öğelerini tek tek dolaşmak istersiniz. `her` döngüsü bunun için vardır:

```jus
her meyve içinde ["elma", "armut", "kiraz"]:
    yaz("Meyve:", meyve)

her harf içinde "çay":
    yaz(harf)
```

```
Meyve: elma
Meyve: armut
Meyve: kiraz
ç
a
y
```

`her x içinde kap:` yazdığınızda `x` her turda kabın sıradaki öğesini alır. Listede öğeleri, metinde karakterleri, sözlükte anahtarları dolaşır (sözlükleri sekizinci bölümde göreceğiz). Döngü değişkeni (`meyve`, `harf`) yalnızca döngünün içinde geçerlidir; ayrıca `değişken` ile tanımlamanız gerekmez.

## Belirli sayıda tekrar: `aralık`

"Bunu 10 kez yap" demek için `aralık` fonksiyonu kullanılır. `aralık(son)`, 0'dan `son`'a kadar (son hariç) sayıların listesini verir.

```jus
her i içinde aralık(3):
    yaz("Tur", i)

yaz(aralık(5))
yaz(aralık(2, 6))
yaz(aralık(0, 20, 5))
yaz(aralık(5, 0, -2))
yaz(aralık(3, 3))
```

```
Tur 0
Tur 1
Tur 2
[0, 1, 2, 3, 4]
[2, 3, 4, 5]
[0, 5, 10, 15]
[5, 3, 1]
[]
```

Üç biçimi vardır:

- `aralık(son)`: 0'dan `son`'a kadar.
- `aralık(baş, son)`: `baş`'tan `son`'a kadar.
- `aralık(baş, son, adım)`: `adım` kadar artarak. Adım negatif olabilir, böylece geriye sayarsınız.

Her durumda `son` değeri **dahil değildir**. `aralık(1, 6)`, 1'den 5'e kadar sayar. Bu başta şaşırtıcı gelir; alışınca kullanışlı olduğunu görürsünüz: `aralık(5)` tam 5 öğe üretir. Sonucun `[ ]` içinde yazılması, bir **liste** olduğunu gösterir. Listeleri altıncı bölümde ayrıntılı işleyeceğiz.

`aralık` ile `her` döngüsünü birleştirmek, sayı dizileriyle çalışmanın en yaygın yoludur. Aşağıdaki ilk örnek 1'den 100'e kadar sayıları toplar.

## `kır` ve `devam`

- `kır` en içteki döngüyü hemen bitirir.
- `devam` en içteki döngünün o turunun kalanını atlar ve sonraki tura geçer.

İkisi de yalnızca döngü içinde kullanılabilir.

```jus
değişken toplam = 0
her n içinde aralık(1, 101):
    toplam += n
yaz("1'den 100'e kadar toplam:", toplam)

yaz("Çift sayılar:")
her n içinde aralık(1, 11):
    eğer n % 2 == 1:
        devam
    yaz(n)

yaz("30'dan büyük ilk 7 katı:")
her n içinde aralık(1, 100):
    eğer n % 7 == 0 ve n > 30:
        yaz(n)
        kır
```

```
1'den 100'e kadar toplam: 5050
Çift sayılar:
2
4
6
8
10
30'dan büyük ilk 7 katı:
35
```

İkinci döngüde tek sayılar için `devam` çalışır ve `yaz(n)` atlanır. Üçüncü döngüde ilk uygun sayı bulununca `kır` döngüyü bitirir.

Bir başka örnek:

```jus
değişken n = 5
iken n > 0:
    n = n - 1
    eğer n == 2:
        devam
    yaz("n şimdi", n)
```

```
n şimdi 4
n şimdi 3
n şimdi 1
n şimdi 0
```

`n` 2 olduğunda `devam` çalıştı ve `yaz` atlandı; ama `n = n - 1` devamdan önce olduğu için döngü sonsuza gitmedi. `devam` kullanırken sayacı güncellemeyi `devam`'dan önceye almayı unutmayın.

### Koşulu `doğru` olan döngü

Ne zaman biteceğini baştan bilmediğiniz bir döngü için `iken doğru:` yazıp içeriden `kır` ile çıkabilirsiniz:

```jus
değişken deneme = 0
iken doğru:
    deneme += 1
    eğer deneme * deneme > 50:
        kır
yaz("Karesi 50'yi geçen ilk sayı:", deneme)
```

```
Karesi 50'yi geçen ilk sayı: 8
```

## İç içe döngüler

Bir döngünün içine başka bir döngü yazabilirsiniz. İçteki döngü, dıştaki her tur için baştan sona çalışır. `kır` ve `devam` yalnızca en içteki döngüyü etkiler.

```jus
her i içinde aralık(1, 4):
    değişken satır = ""
    her j içinde aralık(1, 4):
        satır = satır + metin(i * j) + "  "
    yaz(satır)
```

```
1  2  3  
2  4  6  
3  6  9  
```

Bu, 3x3'lük bir çarpım tablosudur. Her satırın sonunda iki boşluk kalır; gözle görünmez ama oradadır.

## Boş blok: `geç`

Her blokta en az bir deyim bulunmalıdır. Bazen bir koşulun ya da döngünün gövdesinde yapılacak bir şey yoktur; ya da bir bloğu sonra doldurmak üzere yerinde bırakmak istersiniz. Bu durumda hiçbir şey yapmayan `geç` deyimini yazarsınız:

```jus
her i içinde aralık(1, 7):
    eğer i % 2 == 0:
        geç
    değilse:
        yaz(i, "tek")

iken yanlış:
    geç
yaz("bitti")
```

```
1 tek
3 tek
5 tek
bitti
```

`geç`, `devam` ya da `kır` gibi döngüyü etkilemez; yalnızca "burada bilerek bir şey yok" demektir. `geç` her blokta kullanılabilir: `eğer`, döngü, `dene` / `yakala` ve fonksiyon gövdelerinde.

## Sık yapılan hatalar

**Bloğu boş bırakmak.** Yalnızca yorum içeren satırlar girintiyi etkilemez; bu yüzden aşağıdaki blok da boştur:

```jus
eğer doğru:
    # sonra yazılacak
yaz(1)
```

```
ornek.jus:3:1: sözdizimi hatası: ':' işaretinden sonra girintili bir blok bekleniyor.
    yaz(1)
    ^
```

Çözüm: bloğa `geç` yazın.

**`kır`'ı döngü dışında kullanmak.**

```jus
yaz("başla")
kır
```

```
ornek.jus:2:1: sözdizimi hatası: 'kır' yalnızca bir döngünün içinde kullanılabilir.
    kır
    ^
```

**Sayı üzerinde `her` kullanmak.** Sayı bir kap değildir. Belirli sayıda tekrar için `aralık` kullanın:

```jus
her i içinde 5:
    yaz(i)
```

```
ornek.jus:1: çalışma zamanı hatası: 'her' döngüsü liste, metin ya da sözlük üzerinde gezinir; sayı verildi.
```

Çözüm: `her i içinde aralık(5):`.

**Döngü değişkenini döngü dışında kullanmak.**

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

**Koşulun mantıksal olmaması.**

```jus
değişken n = 3
iken n:
    n = n - 1
```

```
ornek.jus:2: çalışma zamanı hatası: Koşul mantıksal bir değer (doğru/yanlış) olmalı; sayı verildi.
```

Çözüm: `iken n > 0:`.

**Sayacı artırmayı unutmak.** `iken sayaç <= 5:` bloğunda `sayaç += 1` yazmazsanız koşul hiç değişmez ve program sonsuza kadar döner. Böyle bir durumda `Ctrl+C` ile durdurun.

## Alıştırmalar

1. `aralık` kullanarak 1'den 10'a kadar sayıları alt alta yazdırın.
2. 5'ten 1'e geriye sayın ve sonunda `Fırlatıldı!` yazdırın.
3. 1'den 100'e kadar olan sayılardan 3'e ya da 5'e bölünenlerin toplamını bulun.
4. `iken` kullanarak 2'nin kuvvetlerini (1, 2, 4, 8, ...) 1000'i geçmeden yazdırın.
5. İç içe iki döngü kullanarak şu şekli yazdırın:
   ```
   *
   **
   ***
   ****
   *****
   ```

## Çözümler

**1.**

```jus
her i içinde aralık(1, 11):
    yaz(i)
```

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

**2.**

```jus
her i içinde aralık(5, 0, -1):
    yaz(i)
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

**3.**

```jus
değişken toplam = 0
her n içinde aralık(1, 101):
    eğer n % 3 == 0 veya n % 5 == 0:
        toplam += n
yaz(toplam)
```

```
2418
```

**4.**

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

**5.**

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

---

Önceki: [03 - Kararlar](03-kararlar.md) | [İçindekiler](README.md) | Sonraki: [05 - Fonksiyonlar](05-fonksiyonlar.md)
