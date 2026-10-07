# 09 - Hatalar

Önceki: [08 - Sözlükler](08-sozlukler.md) | [İçindekiler](README.md) | Sonraki: [10 - Modüller](10-moduller.md)

Önceki bölümlerde hata iletileri gördünüz. O hatalarda program durdu. Ama her hata felaket değildir. Kullanıcı sayı yerine "abc" yazmış olabilir; bir dosya bulunmuyor olabilir. İyi bir program bunları beklenen durumlar olarak ele alır. Bu bölümde hataları **yakalamayı** ve kendi hatalarınızı **fırlatmayı** öğreneceksiniz.

## İki tür hata

Birinci bölümde gördüğünüz ayrımı hatırlayın:

| Tür | Ne zaman oluşur | Yakalanabilir mi? |
|-----|-----------------|-------------------|
| Sözdizimi hatası | Program başlamadan, yazım denetiminde | Hayır; program hiç başlamaz |
| Çalışma zamanı hatası | Program çalışırken (sıfıra bölme, olmayan dizin, tanımsız ad, türlerin uyuşmaması vb.) | Evet |

Bu bölümde yakalanabilen çalışma zamanı hatalarından söz edeceğiz.

## `dene` ve `yakala`

`dene` bloğundaki bir deyim hata verirse bloğun kalanı atlanır ve `yakala` bloğu çalışır. Hata yoksa `yakala` bloğu çalışmaz.

```jus
fonksiyon böl2(a, b):
    dön a / b

dene:
    yaz("Hesaplanıyor...")
    değişken sonuç = böl2(10, 0)
    yaz("Sonuç:", sonuç)
yakala hata:
    yaz("Hesaplanamadı:", hata)

yaz("Program devam ediyor.")

dene:
    yaz(böl2(10, 4))
yakala hata:
    yaz("Bu satır çalışmaz.")

dene:
    yaz(sayı("abc"))
yakala hata:
    yaz("Hata metni:", hata)
    yaz(tür(hata))

dene:
    değişken l = [1, 2, 3]
    yaz(l[10])
yakala:
    yaz("Hata oldu, ayrıntıya gerek yok.")
```

```
Hesaplanıyor...
Hesaplanamadı: Sıfıra bölünemez.
Program devam ediyor.
2.5
Hata metni: "abc" bir sayıya dönüştürülemez.
metin
Hata oldu, ayrıntıya gerek yok.
```

Adım adım:

- İlk `dene` bloğunda `böl2(10, 0)` hata verdi. `yaz("Sonuç:", sonuç)` hiç çalışmadı; `yakala` bloğu çalıştı ve program sürdü.
- `yakala hata:` ifadesindeki `hata`, hata değerini tutan bir değişkendir. Adı siz seçersiniz. Yalnızca `yakala` bloğunda geçerlidir.
- İkinci blokta hata olmadığı için `yakala` hiç çalışmadı.
- Dilin kendi ürettiği hatalarda hata değeri, hatayı anlatan bir **metindir** (`tür(hata)` sonucu `metin`).
- Hata değerine ihtiyacınız yoksa adı yazmayabilirsiniz: `yakala:`.

## Hata fırlatmak: `fırlat`

Kendi kurallarınız bozulduğunda siz de hata oluşturabilirsiniz. `fırlat ifade` bir hata başlatır. Fırlatılan değer her türden olabilir (metin, sayı, sözlük...) ve `yakala` bloğuna olduğu gibi ulaşır.

```jus
fonksiyon yaşKontrol(yaş):
    eğer yaş < 0:
        fırlat "Yaş negatif olamaz."
    eğer yaş > 150:
        fırlat "Yaş gerçekçi değil."
    dön yaş

her deneme içinde [25, -3, 200]:
    dene:
        yaz("Geçerli yaş:", yaşKontrol(deneme))
    yakala sebep:
        yaz("Reddedildi:", sebep)

# fırlatılan değer her türden olabilir
dene:
    fırlat {"kod": 404, "mesaj": "bulunamadı"}
yakala h:
    yaz(h["kod"], h["mesaj"])

dene:
    fırlat 42
yakala h:
    yaz(h + 1)
```

```
Geçerli yaş: 25
Reddedildi: Yaş negatif olamaz.
Reddedildi: Yaş gerçekçi değil.
404 bulunamadı
43
```

`yaşKontrol` fonksiyonu geçersiz değerde `fırlat` ile durur. Çağıran taraf `dene` / `yakala` ile bunu karşılar.

## Hatanın yolculuğu

Bir hata, onu yakalayan bir `dene` bloğu bulunana kadar çağıran fonksiyonlara doğru ilerler. Bu, hatayı oluştuğu yerde değil, nasıl ele alınacağını bilen yerde yakalamanızı sağlar. Yakalayan blok isterse hatayı yeniden fırlatabilir; `dene` blokları iç içe de yazılabilir.

```jus
fonksiyon sayıOku(metin2):
    dene:
        dön sayı(metin2)
    yakala:
        dön boş

yaz(sayıOku("42"))
yaz(sayıOku("3.5"))
yaz(sayıOku("elma"))

fonksiyon derin():
    fırlat "derinden gelen hata"

fonksiyon orta():
    derin()
    yaz("orta: bu satır çalışmaz")

fonksiyon dış():
    dene:
        orta()
    yakala h:
        yaz("dış yakaladı:", h)
        fırlat "dış yeniden fırlattı"

dene:
    dış()
yakala h:
    yaz("ana yakaladı:", h)

dene:
    dene:
        yaz(1 / 0)
    yakala h:
        yaz("iç:", h)
    yaz(tanımsızAd)
yakala h:
    yaz("dış:", h)
```

```
42
3.5
boş
dış yakaladı: derinden gelen hata
ana yakaladı: dış yeniden fırlattı
iç: Sıfıra bölünemez.
dış: 'tanımsızAd' adında bir değişken ya da fonksiyon tanımlı değil.
```

- `sayıOku` hata durumunda `boş` döndürüyor. Bu, "sayıya çevrilemedi" bilgisini çağırana taşımanın basit bir yoludur.
- `derin()` hatayı fırlattı; `orta()` yakalamadığı için hata yukarı çıktı ve `orta`'nın kalan satırı çalışmadı. `dış()` hatayı yakaladı, bir mesaj yazdı ve yeni bir hata fırlattı; bu kez ana program yakaladı.
- Son örnekte iç `dene` yalnızca sıfıra bölmeyi yakaladı. Sonraki satırdaki tanımsız ad hatasını ise dıştaki `dene` yakaladı.

## Yakalanmayan hatalar

Hiçbir `dene` bloğu yakalamazsa program durur ve hata iletisi çağrı zinciriyle birlikte yazılır. Çıkış kodu 70'tir.

```jus
fonksiyon böl(a, b):
    dön a / b

yaz("Başlıyor")
yaz(böl(1, 0))
yaz("Burası çalışmaz")
```

```
Başlıyor
ornek.jus:2: çalışma zamanı hatası: Sıfıra bölünemez.
    satır 2, 'böl' fonksiyonu
    satır 5, ana program
```

İleti, hatanın 2. satırda oluştuğunu, o satırın `böl` fonksiyonunda olduğunu ve fonksiyonun 5. satırda ana programdan çağrıldığını söylüyor. Yakalanmayan bir `fırlat` da aynı şekilde durdurur; fırlatılan değer iletide görünür:

```jus
fonksiyon kontrol(n):
    eğer n < 0:
        fırlat "negatif sayı"
    dön n

yaz(kontrol(5))
yaz(kontrol(-1))
```

```
5
ornek.jus:3: çalışma zamanı hatası: negatif sayı
    satır 3, 'kontrol' fonksiyonu
    satır 7, ana program
```

## Ne zaman yakalamalı?

- Kullanıcıdan ya da dışarıdan gelen verilerde (girdi, dosya) hata beklenebilir; yakalayın.
- Programınızdaki gerçek bir yazım hatasını (örneğin yanlış yazılmış değişken adı) yakalayıp susturmayın. Bu hata düzeltilmesi gereken bir hatadır; saklamak onu bulmayı zorlaştırır.
- `dene` bloğunu küçük tutun. Yalnızca hata verebilecek satırları içine alın.

## Sık yapılan hatalar

**Sözdizimi hatasını yakalamaya çalışmak.** Sözdizimi hataları yakalanamaz; program hiç başlamaz. Aşağıdaki programda ilk `yaz` bile çalışmaz:

```jus
yaz("Bu satır çalışmaz")
dene:
    değişken x =
yakala:
    yaz("yakalandı")
```

```
ornek.jus:3:17: sözdizimi hatası: İfade bekleniyor.
        değişken x =
                    ^
```

**`yakala`'yı unutmak.** Her `dene` bloğunu bir `yakala` izlemelidir:

```jus
yaz("Bu satır hiç çalışmaz.")
dene:
    yaz("içeride")
yaz("yakala yok")
```

```
ornek.jus:4:1: sözdizimi hatası: 'dene' bloğundan sonra 'yakala' bekleniyor.
    yaz("yakala yok")
    ^
```

**Hata değişkenini bloğun dışında kullanmak.** `yakala hata:` ile tanımlanan ad yalnızca `yakala` bloğunun içinde geçerlidir:

```jus
dene:
    yaz(1 / 0)
yakala hata:
    yaz("yakalandı")
yaz(hata)
```

```
yakalandı
ornek.jus:5: çalışma zamanı hatası: 'hata' adında bir değişken ya da fonksiyon tanımlı değil.
```

**Hata fırlatan değeri beklememek.** Fırlatılan değer ne ise `yakala` ona o olarak ulaşır. `fırlat 42` yaptıysanız hata değeri bir sayıdır, metin değil. Dilin kendi hataları ise her zaman metindir.

## Alıştırmalar

1. Bir bölme yapıp sıfıra bölünme durumunda `boş` döndüren `güvenliBöl(a, b)` fonksiyonu yazın.
2. Kullanıcıdan bir sayı isteyin; sayı olmayan bir girdi verirse "Bu bir sayı değil" deyip yeniden sorsun. (`iken` ve `dene` birlikte.)
3. `karekökAl(x)` fonksiyonu negatif sayıda `fırlat` ile anlamlı bir hata versin. `16`, `-4` ve `2` ile deneyin.
4. Bir listede olmayan bir dizine erişin ve hatayı yakalayıp metnini yazdırın. Program sonrasında `Program sürüyor.` yazsın.
5. Bir fonksiyon içinde hatayı yakalayıp bir mesaj yazdıktan sonra aynı hatayı farklı bir mesajla yeniden fırlatsın; ana program onu yakalasın.

## Çözümler

**1.**

```jus
fonksiyon güvenliBöl(a, b):
    dene:
        dön a / b
    yakala:
        dön boş

yaz(güvenliBöl(10, 4))
yaz(güvenliBöl(10, 0))
```

```
2.5
boş
```

**2.**

```jus
fonksiyon sayıİste(istem):
    iken doğru:
        değişken girdi = oku(istem)
        eğer girdi == boş:
            dön boş
        dene:
            dön sayı(girdi)
        yakala:
            yaz("Bu bir sayı değil, tekrar deneyin.")

değişken yaş = sayıİste("Yaşınız: ")
eğer yaş == boş:
    yaz("Girdi bitti.")
değilse:
    yaz("Gelecek yıl", yaş + 1, "yaşında olacaksınız.")
```

Girdi olarak sırasıyla `abc`, boş bir satır ve `29` verildiğinde (`printf 'abc\n\n29\n' | jus ornek.jus`):

```
Yaşınız: Bu bir sayı değil, tekrar deneyin.
Yaşınız: Bu bir sayı değil, tekrar deneyin.
Yaşınız: Gelecek yıl 30 yaşında olacaksınız.
```

`eğer girdi == boş:` denetimi önemlidir: girdi tükendiğinde `oku` `boş` verir, `sayı(boş)` hata verir ve bu hata yakalanıp döngü sonsuza kadar tekrar eder. Denetim olmasaydı program, girdi bittiğinde bile durmazdı. Girdi hiç yokken (`printf '' | jus ornek.jus`) program şunu yazar:

```
Yaşınız: Girdi bitti.
```

**3.**

```jus
fonksiyon karekökAl(x):
    eğer x < 0:
        fırlat "Negatif sayının karekökü alınamaz: " + metin(x)
    dön karekök(x)

her n içinde [16, -4, 2]:
    dene:
        yaz(karekökAl(n))
    yakala sebep:
        yaz("Hata:", sebep)
```

```
4
Hata: Negatif sayının karekökü alınamaz: -4
1.4142135623731
```

**4.**

```jus
değişken meyveler = ["elma", "armut"]
dene:
    yaz(meyveler[5])
yakala hata:
    yaz("Yakalanan hata:", hata)
yaz("Program sürüyor.")
```

```
Yakalanan hata: Dizin sınırların dışında: uzunluk 2, istenen dizin 5.
Program sürüyor.
```

**5.**

```jus
fonksiyon işle(x):
    dene:
        dön 100 / x
    yakala hata:
        yaz("işle içinde görüldü:", hata)
        fırlat "işle başarısız oldu"

dene:
    yaz(işle(4))
    yaz(işle(0))
yakala sebep:
    yaz("Ana program:", sebep)
```

```
25
işle içinde görüldü: Sıfıra bölünemez.
Ana program: işle başarısız oldu
```

---

Önceki: [08 - Sözlükler](08-sozlukler.md) | [İçindekiler](README.md) | Sonraki: [10 - Modüller](10-moduller.md)
