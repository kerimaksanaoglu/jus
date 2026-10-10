# Değerlendirme: iki sınav örneği

[Öğretmen kiti](README.md)

Bu belge iki kısa yazılı/uygulamalı sınav örneği içerir:

- **Sınav 1** (Hafta 5'in sonunda ya da Hafta 6'nın başında): Hafta 1-5 konuları. Süre 40 dakika, 100 puan.
- **Sınav 2** (Hafta 9'un sonunda ya da Hafta 10'un başında): Hafta 1-9 konuları, ağırlık Hafta 6-9. Süre 40 dakika, 100 puan.

Her sınav üç bölümden oluşur: **A. Kod okuma** (çıktıyı tahmin etme), **B. Hata bulma** (hata türü, satır ve düzeltme), **C. Kod yazma**. Her bölümün cevap anahtarı, ilgili sınavın sonundaki "Cevap anahtarı ve puanlama" başlığındadır. Tüm kodlar JUS 1.0.0 ile çalıştırılmış, gösterilen çıktılar ve hata iletileri gerçektir; hata iletilerinde dosya adı `ornek.jus` olarak sadeleştirilmiştir.

## Uygulama önerileri

- **Kâğıt mı bilgisayar mı?** A ve B bölümleri kâğıtla yapılabilir. C bölümünü bilgisayarla yaptırıyorsanız öğrencinin yazdığı programı çalıştırıp çıktısını denetleyin; kâğıtla yaptırıyorsanız küçük sözdizimi hatalarına (eksik iki nokta, tırnak) puan kırmak yerine "fikir doğru mu" ölçütünü öne çıkarın. İlk hafta öğrenciler için sözdizimi hataları normaldir.
- **Çalıştırma izni:** Bilgisayarla yapılan sınavda "A bölümünün çıktısını programı çalıştırarak bulabilirsiniz" derseniz A bölümü anlamsızlaşır. A ve B bölümünü kâğıtta, C bölümünü bilgisayarda yaptırmak dengeli bir seçimdir.
- **İki form:** Kopya riskini azaltmak için sayıları ve adları değiştirerek ikinci bir form hazırlayın. A bölümü sorularında yalnızca sayıları değiştirirseniz sonucu yeniden hesaplamayı (ya da kodu çalıştırmayı) unutmayın.
- **Kapsam dışı kalanlar:** `eşle` / `süz` (Hafta 6, 8. adım), özyineleme (Hafta 5, 7. adım) ve `oku` ile girdi sınavlarda sorulmaz.
- **Ölçüt dengesi:** Sınav, kavramın anlaşılıp anlaşılmadığını ölçer; ezber ölçmek için tasarlanmamıştır. Fonksiyon adlarını hatırlamayan öğrenciye kısa bir "yardımcı kâğıt" (yerleşik fonksiyon adları ve tek satır açıklama) verebilirsiniz.
- **Puanlama:** Puanlar öneridir; kısmi puan ölçütleri cevap anahtarında verilmiştir.

---

## Sınav 1: Hafta 1-5

Ad-soyad: ____________ Tarih: ________ Süre: 40 dakika

### A. Kod okuma (6 x 5 = 30 puan)

Her programın çıktısını yazın. Bir program hata veriyorsa "hata" yazın. Çıktı birden çok satırsa her satırı ayrı yazın.

**A1.**

```jus
yaz(2 + 3 * 4)
yaz((2 + 3) * 4)
yaz(17 % 5)
yaz("7" + "3")
```

**A2.**

```jus
değişken x = 10
x += 5
x = x - 3
yaz(x)
yaz(x > 10 ve x < 20)
```

**A3.**

```jus
değişken puan = 65

eğer puan >= 85:
    yaz("A")
değilse eğer puan >= 60:
    yaz("B")
değilse eğer puan >= 50:
    yaz("C")
değilse:
    yaz("D")
```

**A4.**

```jus
değişken toplam = 0
her i içinde aralık(1, 6):
    eğer i % 2 == 0:
        devam
    toplam += i
yaz(toplam)
```

**A5.**

```jus
fonksiyon f(a, b):
    eğer a > b:
        dön a - b
    dön b - a

yaz(f(3, 10))
yaz(f(10, 3))
yaz(f(4, 4))
```

**A6.**

```jus
fonksiyon g(x):
    yaz(x * 2)

değişken s = g(4)
yaz(s)
```

### B. Hata bulma (3 x 10 = 30 puan)

Her program için: (a) Hata sözdizimi hatası mı, çalışma zamanı hatası mı, yoksa hata iletisi yok ama sonuç yanlış mı? (b) JUS hatayı hangi satırda bildirir (ya da yanlış sonuç hangi satırda ortaya çıkar)? (c) Programın düzeltilmiş hâlini yazın.

**B1.**

```jus
değişken yaş = 12
eğer yaş >= 18
    yaz("Yetişkin")
değilse:
    yaz("Çocuk")
```

**B2.**

```jus
değişken yaş = 12
yaz("Yaşınız: " + yaş)
```

**B3.** Program `12` yazdırmalıydı.

```jus
fonksiyon alan(en, boy):
    en * boy

yaz(alan(3, 4))
```

### C. Kod yazma (40 puan)

**C1. (10 puan)** `n = 14` değişkeni verilsin. Sayı çiftse `Çift`, değilse `Tek` yazdıran bir program yazın.

**C2. (15 puan)** 1'den 20'ye kadar (20 dahil) olan sayılardan 4'e bölünenlerin toplamını bulup `Toplam: ...` biçiminde yazdıran bir program yazın.

**C3. (15 puan)** Üç sayının en küçüğünü döndüren `enKüçük(a, b, c)` fonksiyonunu yazın. `enKüçük(7, 2, 9)` ve `enKüçük(5, 5, 1)` sonuçlarını yazdırın.

---

### Sınav 1: Cevap anahtarı ve puanlama

#### A. Kod okuma

Her soru 5 puan; çıktının tamamı doğruysa tam puan, bir satır yanlışsa 3 puan, çoğu yanlışsa 0 puan.

**A1.** Çıktı:

```
14
20
2
73
```

Ölçülen: işlem önceliği, `%`, metin birleştirme. `"7" + "3"` iki metni birleştirdiği için `73` olur (sayı toplaması değil).

**A2.** Çıktı:

```
12
doğru
```

`x` sırasıyla 10, 15, 12 olur. `12 > 10 ve 12 < 20` doğrudur.

**A3.** Çıktı:

```
B
```

Yaygın yanlış cevap `C`: `65 >= 50` de doğrudur, ama zincirde ilk doğru koşul (`65 >= 60`) çalışır ve gerisine bakılmaz.

**A4.** Çıktı:

```
9
```

`i` 1, 2, 3, 4, 5 değerlerini alır; çift olanlarda `devam` ile toplama atlanır: 1 + 3 + 5 = 9. `aralık(1, 6)` 6'yı içermez.

**A5.** Çıktı:

```
7
7
0
```

Fonksiyon iki sayının farkının mutlak değerini verir. `f(4, 4)` için `a > b` yanlıştır, `b - a` sıfırdır.

**A6.** Çıktı:

```
8
boş
```

`g` ekrana `8` yazar ama değer döndürmez; `s` `boş` olur. (`yaz` ile `dön` farkı.)

#### B. Hata bulma

Her soru 10 puan: (a) hata türü 3 puan, (b) satır numarası 3 puan, (c) düzeltme 4 puan.

**B1.** Gerçek JUS iletisi:

```
ornek.jus:2:15: sözdizimi hatası: Blok başlatmak için ':' bekleniyor.
    eğer yaş >= 18
                  ^
```

(a) Sözdizimi hatası. (b) JUS 2. satırda bildirir; asıl eksik `eğer` satırının sonundaki iki noktadır. (c) `eğer yaş >= 18:` yazılır. Cevap olarak "2. satır" ya da "eğer satırı" kabul edilir.

**B2.** Gerçek JUS iletisi:

```
ornek.jus:2: çalışma zamanı hatası: '+' işleci iki sayı, iki metin ya da iki liste ister; metin ve sayı verildi. Dönüştürmek için metin() ya da sayı() kullanılabilir.
```

(a) Çalışma zamanı hatası. (b) 2. satır. (c) `yaz("Yaşınız: " + metin(yaş))` ya da `yaz("Yaşınız:", yaş)`.

**B3.** Programın çıktısı:

```
boş
```

(a) Hata iletisi yok; sonuç yanlış (`boş`). (b) `yaz(alan(3, 4))` satırında görülür; asıl neden fonksiyonun 2. satırında `dön` eksikliğidir. (c) `dön en * boy`. Öğrenci "dön unutulmuş" der ya da yanlış sonucun nedenini açıklarsa (b) tam puan alır.

#### C. Kod yazma

**C1. (10 puan)** Örnek çözüm:

```jus
değişken n = 14
eğer n % 2 == 0:
    yaz("Çift")
değilse:
    yaz("Tek")
```

```
Çift
```

Puanlama: koşul (`n % 2 == 0`) 4 puan, `eğer`/`değilse` yapısı 4 puan, doğru çıktı 2 puan.

**C2. (15 puan)** Örnek çözüm:

```jus
değişken toplam = 0
her i içinde aralık(1, 21):
    eğer i % 4 == 0:
        toplam += i
yaz("Toplam:", toplam)
```

```
Toplam: 60
```

Puanlama: döngü aralığı (21'e kadar, yani 20 dahil) 4 puan, bölünebilirlik koşulu 4 puan, toplamı biriktirme 4 puan, doğru çıktı 3 puan. `aralık(1, 20)` yazan öğrenci 20'yi atladığı için `40` bulur; bu "bitiş değeri dahil değil" yanlışıdır, 3 puan kırın.

**C3. (15 puan)** Örnek çözüm:

```jus
fonksiyon enKüçük(a, b, c):
    değişken en = a
    eğer b < en:
        en = b
    eğer c < en:
        en = c
    dön en

yaz(enKüçük(7, 2, 9))
yaz(enKüçük(5, 5, 1))
```

```
2
1
```

Puanlama: fonksiyon tanımı ve parametreler 4 puan, karşılaştırma mantığı 5 puan, `dön` kullanımı 3 puan, iki doğru çağrı 3 puan. `yaz` ile yazan ama `dön` kullanmayan çözüm (ekrana yazıyor ama değer vermiyor) çağrılar `yaz(...)` içinde `boş` yazdıracağı için en fazla 9 puan alır.

---

## Sınav 2: Hafta 1-9

Ad-soyad: ____________ Tarih: ________ Süre: 40 dakika

### A. Kod okuma (6 x 5 = 30 puan)

Her programın çıktısını yazın. Bir program hata veriyorsa "hata" yazın.

**A1.**

```jus
değişken l = [5, 3, 8, 1]
ekle(l, 7)
yaz(l[1], l[-1], uzunluk(l))
yaz(l[1:3])
yaz(sırala(l))
yaz(l)
```

**A2.**

```jus
değişken a = [1, 2]
değişken b = a
ekle(b, 3)
yaz(a)
yaz(a == b)
```

**A3.**

```jus
değişken m = "Merhaba Dünya"
yaz(uzunluk(m))
yaz(m[0:3])
yaz(büyük_harf("ışık"))
yaz(böl("a-b-c", "-"))
yaz(birleştir(["x", "y"], "+"))
```

**A4.**

```jus
değişken s = {}
her h içinde "abca":
    s[h] = al(s, h, 0) + 1
yaz(s)
yaz("z" içinde s)
```

**A5.**

```jus
fonksiyon f(x):
    dene:
        dön 10 / x
    yakala:
        dön -1

yaz(f(2))
yaz(f(0))
```

**A6.**

```jus
değişken k = [{"ad": "Ali", "yaş": 31}, {"ad": "Ece", "yaş": 24}]

fonksiyon yaşa(x):
    dön x["yaş"]

her x içinde sırala(k, yaşa):
    yaz(x["ad"])
```

### B. Hata bulma (3 x 10 = 30 puan)

Her program için: (a) Hata türü nedir (sözdizimi, çalışma zamanı ya da hata iletisi yok ama sonuç yanlış)? (b) Hangi satırda ortaya çıkar? (c) Düzeltilmiş hâlini yazın.

**B1.** Program `10`, `20`, `30` yazdırmalıydı.

```jus
değişken l = [10, 20, 30]
her i içinde aralık(1, 4):
    yaz(l[i])
```

**B2.**

```jus
değişken sayım = {}
her harf içinde "kedi":
    sayım[harf] += 1
yaz(sayım)
```

**B3.** Program en düşük puanı (`10`) yazdırmalıydı.

```jus
değişken puanlar = [40, 10, 30]
sırala(puanlar)
yaz(puanlar[0])
```

### C. Kod yazma (40 puan)

**C1. (10 puan)** Bir cümledeki kelime sayısını döndüren `kelimeSay(cümle)` fonksiyonunu yazın. `kelimeSay("bir iki üç")` sonucunu yazdırın.

**C2. (15 puan)** `"elma armut elma muz elma armut"` metninde en çok geçen kelimeyi ve kaç kez geçtiğini bulup `elma 3` biçiminde yazdıran bir program yazın.

**C3. (15 puan)** `["10", "x", "5"]` listesindeki metinleri sayıya çevirip geçerli olanların toplamını, geçersiz kaç tane olduğunu `Toplam: 15` ve `Geçersiz: 1` biçiminde yazdıran bir program yazın. Geçersiz girdi programı durdurmamalıdır.

---

### Sınav 2: Cevap anahtarı ve puanlama

#### A. Kod okuma

Her soru 5 puan; çıktının tamamı doğruysa tam puan, bir satır yanlışsa 3 puan, çoğu yanlışsa 0 puan.

**A1.** Çıktı:

```
3 7 5
[3, 8]
[1, 3, 5, 7, 8]
[5, 3, 8, 1, 7]
```

`ekle` ile `l` `[5, 3, 8, 1, 7]` olur. `sırala` yeni bir liste verir; `l` değişmez (son satır).

**A2.** Çıktı:

```
[1, 2, 3]
doğru
```

`b = a` kopyalama değildir; iki ad aynı listeyi gösterir. Bu yüzden `a` da `[1, 2, 3]` olur.

**A3.** Çıktı:

```
13
Mer
IŞIK
["a", "b", "c"]
x+y
```

`Merhaba Dünya` 13 karakterdir (boşluk dahil). `büyük_harf("ışık")` Türkçe kurala göre `IŞIK` verir. `böl` liste döndürür.

**A4.** Çıktı:

```
{"a": 2, "b": 1, "c": 1}
yanlış
```

Sözlük, anahtarların eklenme sırasıyla yazılır (`a`, `b`, `c`). `"z" içinde s` sözlükte anahtar arar.

**A5.** Çıktı:

```
5
-1
```

**A6.** Çıktı:

```
Ece
Ali
```

`sırala` sözlükleri `yaşa` fonksiyonunun verdiği değere göre dizer: önce 24, sonra 31.

#### B. Hata bulma

Her soru 10 puan: (a) hata türü 3 puan, (b) satır numarası 3 puan, (c) düzeltme 4 puan.

**B1.** Gerçek çıktı:

```
20
30
ornek.jus:3: çalışma zamanı hatası: Dizin sınırların dışında: uzunluk 3, istenen dizin 3.
```

(a) Çalışma zamanı hatası: önce `20` ve `30` yazılır, sonra dizin 3 sınırın dışında kalır. (b) 3. satır. (c) `aralık(0, 3)` ya da `aralık(uzunluk(l))`. `aralık(1, 4)` 0. öğeyi atlar ve 3. dizini ister.

**B2.** Gerçek çıktı:

```
ornek.jus:3: çalışma zamanı hatası: Sözlükte "k" anahtarı yok.
```

(a) Çalışma zamanı hatası. (b) 3. satır. (c) `sayım[harf] = al(sayım, harf, 0) + 1` ya da önce `eğer harf içinde sayım:` denetimi.

**B3.** Programın çıktısı:

```
40
```

(a) Hata iletisi yok; sonuç yanlış (`40`). (b) Sonuç 3. satırda görülür; neden 2. satırdadır: `sırala` yeni bir liste döndürür ve sonuç atılmıştır. (c) `puanlar = sırala(puanlar)` ya da `yaz(sırala(puanlar)[0])`.

#### C. Kod yazma

**C1. (10 puan)** Örnek çözüm:

```jus
fonksiyon kelimeSay(cümle):
    dön uzunluk(böl(cümle, " "))

yaz(kelimeSay("bir iki üç"))
```

```
3
```

Puanlama: `böl` ile bölme 4 puan, `uzunluk` 3 puan, `dön` 3 puan. Döngüyle sayan çözüm de doğrudur. (Bilinen sınır: çok boşluklu cümlelerde boş kelimeler sayılır; bu sınavda beklenmez.)

**C2. (15 puan)** Örnek çözüm:

```jus
değişken sayım = {}
her kelime içinde böl("elma armut elma muz elma armut", " "):
    sayım[kelime] = al(sayım, kelime, 0) + 1

değişken enÇok = ""
değişken adet = 0
her kelime içinde anahtarlar(sayım):
    eğer sayım[kelime] > adet:
        enÇok = kelime
        adet = sayım[kelime]

yaz(enÇok, adet)
```

```
elma 3
```

Puanlama: kelimelere bölme 3 puan, sözlükle sayma 6 puan, en büyüğü bulma 4 puan, doğru çıktı 2 puan.

**C3. (15 puan)** Örnek çözüm:

```jus
değişken girdiler = ["10", "x", "5"]
değişken toplam = 0
değişken geçersiz = 0

her g içinde girdiler:
    dene:
        toplam += sayı(g)
    yakala:
        geçersiz += 1

yaz("Toplam:", toplam)
yaz("Geçersiz:", geçersiz)
```

```
Toplam: 15
Geçersiz: 1
```

Puanlama: `dene` / `yakala` yapısı 6 puan, `sayı()` ile çeviri 3 puan, iki sayaç 3 puan, doğru çıktı 3 puan. `yakala` olmadan yazılan çözüm `x` için hata verir ve programı durdurur; bu, soruda istenen koşulu karşılamaz (0 puan).
