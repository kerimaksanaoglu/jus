# 11 - Sınıflar

Önceki: [10 - Modüller](10-moduller.md) | [İçindekiler](README.md) | Sonraki: [12 - Küçük projeler](12-kucuk-projeler.md)

Bir banka hesabını düşünün. Sahibi, bakiyesi var; para yatırılabilir, çekilebilir. Bu bilgileri ve işlemleri ayrı ayrı değişkenlerde ve fonksiyonlarda tutarsanız, program büyüdükçe dağınıklaşır. **Sınıf**, birbiriyle ilgili bilgiyi ve bu bilgi üzerinde çalışan fonksiyonları tek bir kalıpta toplar. Sınıftan üretilen her somut örneğe **nesne** denir.

## Sınıf tanımlamak

```jus
sınıf Hesap:
    fonksiyon kur(sahip, bakiye):
        bu.sahip = sahip
        bu.bakiye = bakiye

    fonksiyon yatır(tutar):
        bu.bakiye += tutar

    fonksiyon çek(tutar):
        eğer tutar > bu.bakiye:
            fırlat "Yetersiz bakiye."
        bu.bakiye -= tutar

    fonksiyon özet():
        dön bu.sahip + ": " + metin(bu.bakiye) + " TL"

değişken h1 = Hesap("Ayşe", 100)
değişken h2 = Hesap("Mehmet", 50)

h1.yatır(25)
h2.çek(20)
yaz(h1.özet())
yaz(h2.özet())

yaz(h1.bakiye)
h1.bakiye = 500
yaz(h1.özet())

dene:
    h2.çek(1000)
yakala hata:
    yaz("Hata:", hata)

yaz(tür(h1))
yaz(h1 == h2)

değişken h3 = h1
h3.yatır(1)
yaz(h1.bakiye)
yaz(h1 == h3)
```

```
Ayşe: 125 TL
Mehmet: 30 TL
125
Ayşe: 500 TL
Hata: Yetersiz bakiye.
Hesap
yanlış
501
doğru
```

Parçaları tek tek inceleyelim:

- `sınıf Hesap:` bir sınıf tanımlar. Sınıfın gövdesi yalnızca `fonksiyon` tanımlarından oluşur. Sınıfın içindeki fonksiyonlara **yöntem** denir. Her sınıfın en az bir yöntemi olmalıdır. Sınıf adlarını büyük harfle başlatmak bir alışkanlıktır (zorunluluk değil).
- `Hesap("Ayşe", 100)` sınıfı bir fonksiyon gibi çağırarak yeni bir nesne oluşturur.
- `kur` özel bir yöntemdir: **kurucu**. Nesne oluşturulurken otomatik çağrılır ve sınıfı çağırırken verdiğiniz argümanlar ona gider. `kur` değer döndüremez. `kur` yazmazsanız sınıf argümansız çağrılır.
- `bu`, yöntemin çağrıldığı nesnedir. Parametre listesine yazılmaz; yöntemin içinde hazırdır. `bu.sahip = sahip` ifadesi nesnenin `sahip` **alanını** oluşturur.
- **Alan**, nesnenin içinde sakladığı bir değerdir. Alanlar önceden bildirilmez; ilk atamada oluşur. `nesne.alan` ile okunur, `nesne.alan = değer` ile değiştirilir.
- `h1.yatır(25)` nesnenin yöntemini çağırır. Yöntem, `bu` olarak `h1`'i görür; `h1`'in bakiyesi artar, `h2`'ninki değişmez. Her nesne kendi alanlarını taşır.
- `tür(h1)` nesnenin sınıf adını verir.
- İki nesne yalnızca **aynı nesneyse** eşittir. Aynı bakiyeye sahip iki ayrı hesap bile eşit sayılmaz. `h3 = h1` bir kopya üretmez; `h3` ile `h1` aynı nesneyi gösterir (`h3.yatır(1)` `h1.bakiye`'yi 501 yaptı).

Aynı mantıkla, küçük bir sınıf daha:

```jus
sınıf Selam:
    fonksiyon söyle():
        yaz("Selam!")

değişken s = Selam()
s.söyle()

sınıf Sayaç:
    fonksiyon kur():
        bu.değer = 0

    fonksiyon artır():
        bu.değer += 1
        dön bu.değer

değişken a = Sayaç()
değişken b = Sayaç()
a.artır()
a.artır()
b.artır()
yaz(a.değer, b.değer)

# nesnenin yöntemi bir değer olarak taşınabilir
değişken art = a.artır
yaz(art())
yaz(a.değer)

yaz(tür(Sayaç))
yaz(tür(a))
```

```
Selam!
2 1
3
3
sınıf
Sayaç
```

`Selam` sınıfında `kur` yok; bu yüzden `Selam()` argümansız çağrıldı. `a.artır` parantezsiz yazılınca, `a` nesnesine bağlı bir fonksiyon değeri elde edilir; `art()` çağrısı hâlâ `a`'nın sayacını artırır. `tür(Sayaç)` sınıfın kendisi için `sınıf` verir.

## Kalıtım

Bir sınıf başka bir sınıfın özelliklerini devralabilir. `sınıf Alt(Üst):` yazarsınız: `Alt` sınıfı, `Üst` sınıfın tüm yöntemlerini devralır. Alt sınıf, aynı adla bir yöntem tanımlayarak bunları **değiştirebilir**. Bir sınıfın tek bir üst sınıfı olabilir.

`üst.yöntem(...)` ifadesi, yöntemin üst sınıftaki tanımını çağırır. Bu, devralınan davranışı tamamen atmak yerine genişletmeye yarar.

```jus
sınıf Hayvan:
    fonksiyon kur(ad):
        bu.ad = ad

    fonksiyon tanıt():
        dön bu.ad + ": " + bu.ses()

    fonksiyon ses():
        dön "..."

sınıf Köpek(Hayvan):
    fonksiyon ses():
        dön "Hav"

sınıf Kedi(Hayvan):
    fonksiyon kur(ad, renk):
        üst.kur(ad)
        bu.renk = renk

    fonksiyon ses():
        dön "Miyav"

    fonksiyon tanıt():
        dön üst.tanıt() + " (" + bu.renk + ")"

değişken k = Köpek("Karabaş")
değişken c = Kedi("Pamuk", "beyaz")
değişken h = Hayvan("Bilinmeyen")

yaz(k.tanıt())
yaz(c.tanıt())
yaz(h.tanıt())

her hayvan içinde [k, c, h]:
    yaz(tür(hayvan), "-", hayvan.ses())

yaz(örneği_mi(k, Köpek))
yaz(örneği_mi(k, Hayvan))
yaz(örneği_mi(k, Kedi))
yaz(örneği_mi(5, Hayvan))
```

```
Karabaş: Hav
Pamuk: Miyav (beyaz)
Bilinmeyen: ...
Köpek - Hav
Kedi - Miyav
Hayvan - ...
doğru
doğru
yanlış
yanlış
```

Neler oluyor:

- `Köpek` sınıfında `kur` ve `tanıt` yok; ikisi de `Hayvan`'dan devralındı. `Köpek` yalnızca `ses`'i değiştirdi.
- `k.tanıt()` çağrıldığında, devralınan `tanıt` yöntemi `bu.ses()`'i çağırır. `bu` bir `Köpek` olduğundan `Köpek`'in `ses` yöntemi çalışır ve sonuç `Hav` olur. Bu yüzden `Hayvan`'ın `tanıt` yöntemi her alt sınıfta doğru sesi söyler.
- `Kedi`, `kur` yöntemini yeniden tanımlıyor ve `üst.kur(ad)` ile üst sınıfın kurucusunu çağırıyor; böylece `ad` alanı ayarlanıyor. Sonra kendi alanı olan `renk`'i ekliyor.
- `Kedi`'nin `tanıt` yöntemi `üst.tanıt()` ile üst sınıfın sonucunu alıp üzerine ekleme yapıyor.
- `örneği_mi(değer, sınıf)`, değerin o sınıfın ya da ondan türeyen bir sınıfın nesnesi olup olmadığını söyler. Bir `Köpek` hem `Köpek` hem `Hayvan`'dır ama `Kedi` değildir.

## Sınıf mı, fonksiyon mu?

Her şeyi sınıfla yazmanız gerekmez. Küçük işler için fonksiyonlar ve sözlükler yeterlidir. Sınıf şu durumlarda işe yarar:

- Aynı bilgiler ve onlara ait işlemler birlikte yaşıyorsa (hesap: bakiye + yatır/çek),
- Aynı türden birçok bağımsız nesne gerekiyorsa (iki ayrı sayaç, iki ayrı hesap),
- Ortak davranışı paylaşan farklı türler varsa (hayvanlar).

## Sık yapılan hatalar

**Olmayan alanı okumak.**

```jus
sınıf Kişi:
    fonksiyon kur(ad):
        bu.ad = ad

değişken k = Kişi("Ayşe")
yaz(k.yaş)
```

```
ornek.jus:6: çalışma zamanı hatası: 'Kişi' nesnesinde 'yaş' adında bir alan ya da yöntem yok.
```

**`kur` için yanlış sayıda argüman.**

```jus
sınıf Kişi:
    fonksiyon kur(ad):
        bu.ad = ad

değişken k = Kişi()
yaz(k.ad)
```

```
ornek.jus:5: çalışma zamanı hatası: 'kur' fonksiyonu 1 argüman bekliyor, 0 verildi.
```

**Sınıf gövdesine değişken yazmak.** Sınıf gövdesinde yalnızca `fonksiyon` bulunabilir. Alanları `kur` içinde `bu.alan = ...` ile oluşturun.

```jus
sınıf Kişi:
    değişken ad = "Ayşe"

yaz("hiç çalışmaz")
```

```
ornek.jus:2:5: sözdizimi hatası: Sınıf gövdesinde yalnızca 'fonksiyon' tanımları bulunabilir.
        değişken ad = "Ayşe"
        ^
```

**`bu.` yazmayı unutmak.** `ad = ad` yalnızca parametreyi kendine atar; nesnenin alanı oluşmaz. Alan oluşturmak için `bu.ad = ad` yazın:

```jus
sınıf Kişi:
    fonksiyon kur(ad):
        ad = ad

    fonksiyon selam():
        yaz("Merhaba, " + bu.ad)

değişken k = Kişi("Ayşe")
k.selam()
```

```
ornek.jus:6: çalışma zamanı hatası: 'Kişi' nesnesinde 'ad' adında bir alan ya da yöntem yok.
    satır 6, 'selam' fonksiyonu
    satır 9, ana program
```

**Olmayan yöntemi çağırmak.**

```jus
sınıf Kişi:
    fonksiyon selam():
        yaz("Merhaba")

değişken k = Kişi()
k.veda()
```

```
ornek.jus:6: çalışma zamanı hatası: 'Kişi' nesnesinde 'veda' adında bir yöntem yok.
```

## Alıştırmalar

1. `en` ve `boy` alanlarına sahip bir `Dikdörtgen` sınıfı yazın; `alan()` ve `çevre()` yöntemleri olsun. 3x5'lik bir nesneyle deneyin.
2. `koy(öğe)`, `al()` ve `boşMu()` yöntemlerine sahip bir `Yığın` sınıfı yazın. Yığın, son konulanın ilk alındığı bir listedir. `"a"`, `"b"`, `"c"` koyup boşalana kadar alın.
3. `alan()` yöntemi olan `Şekil` sınıfından `Kare` ve `Daire` sınıflarını türetin. `Şekil`'in `tanıt()` yöntemi sınıf adını ve alanı yazsın. (Daire için `pi` yerine 3 kullanın.)
4. Önceki `Hesap` sınıfına benzer bir sınıf yazıp `faizEkle(oran)` yöntemi ekleyin (oran yüzde olarak). 1000 liraya iki kez %10 faiz ekleyin.
5. `Çalışan` sınıfından `ekipSayısı` alanı olan bir `Yönetici` türetin. `bilgi()` yöntemi yöneticide `üst.bilgi()` çıktısına ekip bilgisini eklesin.

## Çözümler

**1.**

```jus
sınıf Dikdörtgen:
    fonksiyon kur(en, boy):
        bu.en = en
        bu.boy = boy

    fonksiyon alan():
        dön bu.en * bu.boy

    fonksiyon çevre():
        dön 2 * (bu.en + bu.boy)

değişken d = Dikdörtgen(3, 5)
yaz(d.alan())
yaz(d.çevre())
```

```
15
16
```

**2.**

```jus
sınıf Yığın:
    fonksiyon kur():
        bu.öğeler = []

    fonksiyon koy(öğe):
        ekle(bu.öğeler, öğe)

    fonksiyon al():
        dön çıkar(bu.öğeler)

    fonksiyon boşMu():
        dön uzunluk(bu.öğeler) == 0

değişken y = Yığın()
y.koy("a")
y.koy("b")
y.koy("c")
iken değil y.boşMu():
    yaz(y.al())
```

```
c
b
a
```

**3.**

```jus
sınıf Şekil:
    fonksiyon tanıt():
        dön tür(bu) + ", alan: " + metin(bu.alan())

    fonksiyon alan():
        dön 0

sınıf Kare(Şekil):
    fonksiyon kur(kenar):
        bu.kenar = kenar

    fonksiyon alan():
        dön bu.kenar * bu.kenar

sınıf Daire(Şekil):
    fonksiyon kur(yarıçap):
        bu.yarıçap = yarıçap

    fonksiyon alan():
        dön 3 * bu.yarıçap * bu.yarıçap

her ş içinde [Kare(4), Daire(2)]:
    yaz(ş.tanıt())
```

```
Kare, alan: 16
Daire, alan: 12
```

**4.**

```jus
sınıf Hesap:
    fonksiyon kur(bakiye):
        bu.bakiye = bakiye

    fonksiyon faizEkle(oran):
        bu.bakiye += bu.bakiye * oran / 100

değişken h = Hesap(1000)
h.faizEkle(10)
yaz(h.bakiye)
h.faizEkle(10)
yaz(h.bakiye)
```

```
1100
1210
```

**5.**

```jus
sınıf Çalışan:
    fonksiyon kur(ad, maaş):
        bu.ad = ad
        bu.maaş = maaş

    fonksiyon bilgi():
        dön bu.ad + " - " + metin(bu.maaş) + " TL"

sınıf Yönetici(Çalışan):
    fonksiyon kur(ad, maaş, ekipSayısı):
        üst.kur(ad, maaş)
        bu.ekipSayısı = ekipSayısı

    fonksiyon bilgi():
        dön üst.bilgi() + " (ekip: " + metin(bu.ekipSayısı) + ")"

yaz(Çalışan("Ali", 20000).bilgi())
yaz(Yönetici("Ayşe", 35000, 4).bilgi())
```

```
Ali - 20000 TL
Ayşe - 35000 TL (ekip: 4)
```

---

Önceki: [10 - Modüller](10-moduller.md) | [İçindekiler](README.md) | Sonraki: [12 - Küçük projeler](12-kucuk-projeler.md)
