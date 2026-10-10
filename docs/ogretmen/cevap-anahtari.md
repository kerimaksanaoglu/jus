# Cevap anahtarı

[Öğretmen kiti](README.md)

Bu belge, haftalık dosyalardaki tüm alıştırmaların çözümlerini içerir. Her program JUS 1.0.0 ile çalıştırılmış, gösterilen çıktılar gerçek çıktıdır. Hata iletilerinde dosya adı `ornek.jus` olarak sadeleştirilmiştir.

Kullanım önerileri:

- Çözümler tek olası çözüm değildir. Öğrencinin programı aynı çıktıyı veriyor ve konuya uygun yazılmışsa doğrudur; "çıktı aynı mı" ve "aynı fikri kullanıyor mu" sorularını ayrı tutun.
- Bazı çözümlerde, o hafta öğretilmemiş bir yöntemin kısa yolu gösterilmiştir. Bunlar "Not" olarak belirtilmiştir; öğrenciden beklenmez.
- Girdi (`oku`) isteyen çözümlerde girdi satırları çıktının üstünde belirtilmiştir. Bu çözümler tarayıcıdaki deneme alanında çalışmaz.

| Hafta | Konu |
|-------|------|
| [Hafta 1](#hafta-1) | `yaz`, yorum, hata iletisi |
| [Hafta 2](#hafta-2) | Değişkenler, sayılar, metinler |
| [Hafta 3](#hafta-3) | Kararlar |
| [Hafta 4](#hafta-4) | Döngüler |
| [Hafta 5](#hafta-5) | Fonksiyonlar |
| [Hafta 6](#hafta-6) | Listeler |
| [Hafta 7](#hafta-7) | Metin işlemleri |
| [Hafta 8](#hafta-8) | Sözlükler |
| [Hafta 9](#hafta-9) | Hata yakalama ve oyun |
| [Hafta 10](#hafta-10) | Tekrar ve proje |

## Hafta 1

Alıştırmalar: [hafta-01.md](hafta-01.md)

### Alıştırma 1.1

```jus
yaz("Merhaba, ben Ayşe.")
```

```
Merhaba, ben Ayşe.
```

### Alıştırma 1.2

```jus
yaz("Birinci satır\nİkinci satır\nÜçüncü satır")
```

```
Birinci satır
İkinci satır
Üçüncü satır
```

Aynı çıktı üç ayrı `yaz` satırıyla da elde edilir. Soru "tek çağrı" dediği için `\n` beklenir.

### Alıştırma 1.3

```jus
yaz("7 çarpı 8 =", 7 * 8)
```

```
7 çarpı 8 = 56
```

Sık yapılan yanlış: `yaz("7 çarpı 8 = 7 * 8")`. Tırnağın içindeki ifade hesaplanmaz.

### Alıştırma 1.4

```jus
yaz("A")
# yaz("B")
yaz("C")  # yaz("D")
yaz("E", "F")
```

```
A
C
E F
```

`D` yazılmadı, çünkü `#` işaretinden sonraki her şey, aynı satırda olsa bile yorumdur.

### Alıştırma 1.5

```jus
yaz("Toplam:", 3 * 7.5 + 2 * 12)
```

```
Toplam: 46.5
```

### Alıştırma 1.6

```jus
yaz("   /\\")
yaz("  /  \\")
yaz(" /    \\")
yaz("/______\\")
yaz("|      |")
yaz("|  []  |")
yaz("|      |")
yaz("|______|")
```

```
   /\
  /  \
 /    \
/______\
|      |
|  []  |
|      |
|______|
```

Ters bölü karakterini yazdırmak için metinde `\\` yazılır. Tek `\` yazan öğrenci, metnin sonundaki ters bölünün kapanış tırnağını "kaçırdığını" görür:

```jus
yaz("   /\")
```

```
ornek.jus:1:5: sözdizimi hatası: Metin kapatılmamış; kapanış tırnağı (") eksik.
    yaz("   /\")
        ^
```

Ters bölüden sonra tablodaki beş karakterden biri gelmiyorsa ("/\x" gibi) ileti "Geçersiz kaçış dizisi" olur.

### Alıştırma 1.7

Birinci program: kapanış tırnağı eksikti. Düzeltilmiş hâli:

```jus
yaz("Merhaba")
yaz("Dünya")
```

```
Merhaba
Dünya
```

İkinci program: ilk satırda parantez kapatılmamıştı; JUS bunu ikinci satırda fark etti (sözdizimi hatası). Düzeltilmiş hâli:

```jus
yaz("Toplam:", (2 + 3) * 4)
yaz("Bitti")
```

```
Toplam: 20
Bitti
```

Üçüncü program: `Yaz` yerine `yaz` yazılmalıydı (çalışma zamanı hatası; ilk satır çalıştı). Düzeltilmiş hâli:

```jus
yaz("Hesap başlıyor")
yaz(5 * 5)
```

```
Hesap başlıyor
25
```

## Hafta 2

Alıştırmalar: [hafta-02.md](hafta-02.md)

### Alıştırma 2.1

```jus
değişken en = 12
değişken boy = 5
yaz("Alan:", en * boy)
yaz("Çevre:", 2 * (en + boy))
```

```
Alan: 60
Çevre: 34
```

Sık yapılan yanlış: `2 * en + boy`. Çarpma önce yapıldığı için çevre `29` çıkar; parantez gerekir.

### Alıştırma 2.2

Programın çıktısı yukarıda hafta dosyasında verilmiştir:

```
26
```

`x` değeri sırasıyla 10, 15, 30, 26 olur.

### Alıştırma 2.3

```jus
değişken ad = "Deniz"
değişken yaş = 13
değişken şehir = "Bursa"
yaz("Ben " + ad + ", " + metin(yaş) + " yaşındayım. Şehir: " + şehir + ".")
```

```
Ben Deniz, 13 yaşındayım. Şehir: Bursa.
```

Öğrenci cümleyi `yaz` içinde virgülle ayırarak da yazabilir (`yaz("Ben", ad, ...)`); bu durumda `metin()` gerekmez, ama noktalama işaretleri önündeki boşlukla ayrılır. İki yazım da kabul edilir; önemli olan değişkenlerin kullanılması ve sayıyı `+` ile birleştirirken `metin()` unutulmamasıdır.

### Alıştırma 2.4

```jus
değişken c = 25
değişken f = c * 9 / 5 + 32
yaz(metin(c) + " derece Celsius = " + metin(f) + " derece Fahrenheit")
```

```
25 derece Celsius = 77 derece Fahrenheit
```

### Alıştırma 2.5

```jus
değişken toplamSaniye = 135
değişken dakika = taban(toplamSaniye / 60)
değişken saniye = toplamSaniye % 60
yaz(dakika, "dakika", saniye, "saniye")
```

```
2 dakika 15 saniye
```

`135 / 60` sonucu `2.25` olduğu için `taban` ile ondalık kısım atılır. `%` ise doğrudan kalanı verir.

### Alıştırma 2.6

```jus
değişken a = 3
değişken b = 8
yaz("Önce:", a, b)
değişken geçici = a
a = b
b = geçici
yaz("Sonra:", a, b)
```

```
Önce: 3 8
Sonra: 8 3
```

Yalnızca `a = b` ve `b = a` yazan öğrencide iki değişken de `8` olur; çünkü `a`'nın eski değeri kutudan silinmiştir. Etkinlikteki kutu gösterimine dönün.

### Alıştırma 2.7

```jus
değişken n = 482
değişken birler = n % 10
değişken onlar = taban(n / 10) % 10
değişken yüzler = taban(n / 100)
yaz("Rakamlar:", yüzler, onlar, birler)
yaz("Rakamlar toplamı:", yüzler + onlar + birler)
```

```
Rakamlar: 4 8 2
Rakamlar toplamı: 14
```

Neden `taban(n / 10) % 10`? `n / 10` sonucu `48.2`; `taban` ile `48` olur; `48 % 10` onlar basamağı olan `8`'i verir.

### Alıştırma 2.8

```jus
değişken toplam = 2 * 7.5 + 1 * 32.9 + 2 * 125.75
değişken indirimli = toplam * 0.9
yaz("Toplam:", biçimle(toplam, 2), "TL")
yaz("İndirimli:", biçimle(indirimli, 2), "TL")
```

```
Toplam: 299.40 TL
İndirimli: 269.46 TL
```

`yaz(toplam)` yazan öğrenci `299.4` görür; para tutarında sondaki sıfırın kaybolması `biçimle` ihtiyacını gösterir.

## Hafta 3

Alıştırmalar: [hafta-03.md](hafta-03.md)

### Alıştırma 3.1

```jus
değişken sayı1 = -4

eğer sayı1 > 0:
    yaz("Pozitif")
değilse eğer sayı1 < 0:
    yaz("Negatif")
değilse:
    yaz("Sıfır")
```

sayı1 = -4 için:

```
Negatif
```

sayı1 = 5 için:

```
Pozitif
```

sayı1 = 0 için:

```
Sıfır
```

Aynı programı üç değerle çalıştırdık; ilk satırdaki sayıyı değiştirmek yeterlidir. Üç yollu zincirde "Sıfır" yolunun son `değilse` olduğuna dikkat edin: iki koşul (`> 0`, `< 0`) yanlışsa geriye yalnızca sıfır kalır.

### Alıştırma 3.2

```jus
değişken n = 17

eğer n % 2 == 0:
    yaz(metin(n) + " çift sayıdır.")
değilse:
    yaz(metin(n) + " tek sayıdır.")
```

```
17 tek sayıdır.
```

### Alıştırma 3.3

```jus
değişken cevap = "Ankara"

eğer cevap == "Ankara":
    yaz("Doğru!")
değilse:
    yaz("Yanlış.")
```

```
Doğru!
```

Büyük-küçük harf ayrımı sürpriz olabilir: `cevap = "ankara"` yazılırsa `Yanlış.` çıkar. Bu, Hafta 7'de `küçük_harf` ile ele alınır.

### Alıştırma 3.4

```jus
değişken puan = 72

eğer puan >= 85:
    yaz("Pekiyi")
değilse eğer puan >= 70:
    yaz("İyi")
değilse eğer puan >= 55:
    yaz("Orta")
değilse eğer puan >= 45:
    yaz("Geçer")
değilse:
    yaz("Zayıf")
```

puan = 72 için:

```
İyi
```

puan = 50 için:

```
Geçer
```

puan = 30 için:

```
Zayıf
```

puan = 90 için:

```
Pekiyi
```

### Alıştırma 3.5

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

Öğrencilerin tipik ikinci çözümü, `fiyat` değişkenini hiç baştan tanımlamayıp her yolda `yaz` çağırmaktır; o da doğrudur.

### Alıştırma 3.6

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

İki `eğer` ayrı ayrıdır (`değilse` ile bağlı değil): `b` ve `c` aynı anda bakılır. Bu, bir değeri "şimdiye kadarki en büyük" olarak taşıma kalıbıdır ve Hafta 4'te döngüyle tekrar edilir.

### Alıştırma 3.7

```jus
değişken yıl = 2024

eğer yıl % 4 == 0 ve (yıl % 100 != 0 veya yıl % 400 == 0):
    yaz(metin(yıl) + " artık yıldır.")
değilse:
    yaz(metin(yıl) + " artık yıl değildir.")
```

yıl = 2024 için:

```
2024 artık yıldır.
```

yıl = 1900 için:

```
1900 artık yıl değildir.
```

yıl = 2000 için:

```
2000 artık yıldır.
```

yıl = 2023 için:

```
2023 artık yıl değildir.
```

Parantezi unutan öğrencide `ve` önce, `veya` sonra hesaplandığı için 1900 yanlış sınıflanır. Parantez, `ve` ile `veya` karışık kullanıldığında zorunlu bir alışkanlık olsun.

### Alıştırma 3.8

```jus
değişken a = 3
değişken b = 4
değişken c = 5

eğer a + b <= c veya a + c <= b veya b + c <= a:
    yaz("Üçgen olmaz.")
değilse:
    eğer a == b ve b == c:
        yaz("Eşkenar")
    değilse eğer a == b veya b == c veya a == c:
        yaz("İkizkenar")
    değilse:
        yaz("Çeşitkenar")
    eğer a * a + b * b == c * c:
        yaz("Dik üçgen")
```

a, b, c = 3, 4, 5 için:

```
Çeşitkenar
Dik üçgen
```

a, b, c = 5, 5, 5 için:

```
Eşkenar
```

a, b, c = 5, 5, 8 için:

```
İkizkenar
```

a, b, c = 4, 6, 7 için:

```
Çeşitkenar
```

a, b, c = 2, 3, 9 için:

```
Üçgen olmaz.
```

Not: Dik üçgen denetimi `c`'nin en uzun kenar olduğunu varsayar. Öğrenci üç olasılığı da denetlemek isterse (`a*a + b*b == c*c` veya `a*a + c*c == b*b` veya `b*b + c*c == a*a`) bu daha eksiksiz bir çözümdür. Çözümde bir `değilse` bloğunun içine ikinci bir `eğer` zinciri yazılmıştır (iç içe karar; rehberde 3. bölümün "İç içe eğer" başlığı). Bu alıştırma zor olarak işaretlidir; iç içe karar yazmakta zorlanan öğrenciye önce "üçgen olur mu?" sorusunu ayrı bir programda çözdürün.

## Hafta 4

Alıştırmalar: [hafta-04.md](hafta-04.md)

### Alıştırma 4.1

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

`aralık(1, 10)` yazan öğrenci 9'a kadar yazar; bitiş değeri dahil değildir.

### Alıştırma 4.2

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

Alternatif çözüm: `iken` ile `n = 5` den başlayıp `n -= 1`.

### Alıştırma 4.3

```jus
her i içinde aralık(1, 11):
    yaz("7 x " + metin(i) + " = " + metin(7 * i))
```

```
7 x 1 = 7
7 x 2 = 14
7 x 3 = 21
7 x 4 = 28
7 x 5 = 35
7 x 6 = 42
7 x 7 = 49
7 x 8 = 56
7 x 9 = 63
7 x 10 = 70
```

Aynı çıktı `yaz("7 x", i, "=", 7 * i)` ile de elde edilir (virgüllü yazım aralara birer boşluk koyar). Çarpılan sayıyı `sayı3 = 7` gibi bir değişkene yazmak (sabit sayıyı kodun içine gömmemek) iyi bir alışkanlıktır.

### Alıştırma 4.4

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

Hem 3'e hem 5'e bölünen sayıları iki kez saymak sık yapılan hatadır; `veya` bunu önler, çünkü tek bir koşuldur.

### Alıştırma 4.5

```jus
değişken kat = 3
iken kat <= 40:
    yaz(kat)
    kat += 3
```

```
3
6
9
12
15
18
21
24
27
30
33
36
39
```

### Alıştırma 4.6

```jus
her n içinde aralık(1, 16):
    eğer n % 15 == 0:
        yaz("FizzBuzz")
    değilse eğer n % 3 == 0:
        yaz("Fizz")
    değilse eğer n % 5 == 0:
        yaz("Buzz")
    değilse:
        yaz(n)
```

```
1
2
Fizz
4
Buzz
Fizz
7
8
Fizz
Buzz
11
Fizz
13
14
FizzBuzz
```

`n % 15 == 0` koşulu (ya da `n % 3 == 0 ve n % 5 == 0`) en başta olmalıdır; 3 ya da 5 koşulu önce gelirse 15 için `FizzBuzz` hiç yazılmaz. Etkinlikteki sıra tartışmasını hatırlatın.

### Alıştırma 4.7

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

Not: `tekrarla("*", i)` tek satırda aynı satırı üretir (Hafta 7'de görülür). Burada amaç iç içe döngüdür.

### Alıştırma 4.8

```jus
değişken n = 29
değişken asal = n > 1
her bölen içinde aralık(2, n):
    eğer n % bölen == 0:
        asal = yanlış
        kır
eğer asal:
    yaz(metin(n) + " asaldır.")
değilse:
    yaz(metin(n) + " asal değildir.")
```

n = 29 için:

```
29 asaldır.
```

n = 30 için:

```
30 asal değildir.
```

n = 97 için:

```
97 asaldır.
```

n = 91 için:

```
91 asal değildir.
```

n = 2 için:

```
2 asaldır.
```

n = 1 için:

```
1 asal değildir.
```

Not: `asal` değişkeni `doğru` ile başlayan bir çözüm 2 ve üzeri sayılarda doğru çalışır; 1 için `1 asaldır.` yazar, bu yanlıştır. `n > 1` ile başlatmak bunu çözer. `kır` olmadan da sonuç doğru çıkar (yalnızca gereksiz yere tüm sayılara bakılır).

### Alıştırma 4.9

```jus
değişken n = 6
değişken adım = 0
yaz(n)
iken n != 1:
    eğer n % 2 == 0:
        n = n / 2
    değilse:
        n = 3 * n + 1
    adım += 1
    yaz(n)
yaz("Adım sayısı:", adım)
```

```
6
3
10
5
16
8
4
2
1
Adım sayısı: 8
```

İzleme tablosu: `n` sırasıyla 6, 3, 10, 5, 16, 8, 4, 2, 1. Sekiz değişiklik, sekiz adım.

## Hafta 5

Alıştırmalar: [hafta-05.md](hafta-05.md)

### Alıştırma 5.1

```jus
fonksiyon kare(x):
    dön x * x

yaz(kare(7))
yaz(kare(1.5))
```

```
49
2.25
```

### Alıştırma 5.2

```jus
fonksiyon selamla(ad):
    yaz("Merhaba, " + ad + "!")

selamla("Ali")
selamla("Zeynep")
```

```
Merhaba, Ali!
Merhaba, Zeynep!
```

### Alıştırma 5.3

```jus
fonksiyon ortalama(a, b, c):
    dön (a + b + c) / 3

yaz(ortalama(70, 85, 100))
yaz(ortalama(1, 2, 2))
```

```
85
1.6666666666667
```

İkinci sonuç `1.6666666666667` olarak görünür; JUS ondalık sayıları en çok 14 anlamlı basamakla yazar. Yuvarlamak isteyen öğrenci `yuvarla(ortalama(1, 2, 2), 2)` yazabilir.

Parantezi unutan öğrencide (`a + b + c / 3`) bölme önce yapılır; ilk çağrıda sonuç `85` yerine `188.33333333333` çıkar. İşlem önceliğini Hafta 2'ye bağlayın.

### Alıştırma 5.4

```jus
fonksiyon çiftMi(n):
    dön n % 2 == 0

her i içinde aralık(1, 6):
    eğer çiftMi(i):
        yaz(i, "çift")
    değilse:
        yaz(i, "tek")
```

```
1 tek
2 çift
3 tek
4 çift
5 tek
```

### Alıştırma 5.5

```jus
fonksiyon enBüyük(a, b, c):
    değişken en = a
    eğer b > en:
        en = b
    eğer c > en:
        en = c
    dön en

yaz(enBüyük(14, 29, 8))
yaz(enBüyük(5, 5, 2))
```

```
29
5
```

### Alıştırma 5.6

```jus
fonksiyon biletFiyatı(yaş, öğrenci):
    eğer yaş < 7:
        dön 0
    değilse eğer öğrenci veya yaş >= 65:
        dön 50
    dön 100

yaz(biletFiyatı(5, yanlış))
yaz(biletFiyatı(16, doğru))
yaz(biletFiyatı(40, yanlış))
```

```
0
50
100
```

### Alıştırma 5.7

```jus
fonksiyon asalMı(n):
    eğer n < 2:
        dön yanlış
    her bölen içinde aralık(2, n):
        eğer n % bölen == 0:
            dön yanlış
    dön doğru

değişken satır = ""
her i içinde aralık(1, 31):
    eğer asalMı(i):
        satır = satır + metin(i) + " "
yaz(kırp(satır))
```

```
2 3 5 7 11 13 17 19 23 29
```

Not: `kırp` Hafta 7'de öğretilir; burada yalnızca sondaki fazla boşluğu atmak için kullanıldı. Öğrenci `yaz(satır)` yazarsa çıktı aynı görünür (sonda görünmeyen bir boşluk kalır). `dön yanlış` döngünün içindedir: bölen bulunduğu anda fonksiyon biter, `kır` gerekmez.

### Alıştırma 5.8

```jus
fonksiyon ebob(a, b):
    iken b != 0:
        değişken kalan = a % b
        a = b
        b = kalan
    dön a

yaz(ebob(48, 18))
yaz(ebob(17, 5))
yaz(ebob(100, 75))
```

```
6
1
25
```

İzleme (`ebob(48, 18)`): (48, 18) -> (18, 12) -> (12, 6) -> (6, 0) ve `a` = 6.

### Alıştırma 5.9

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

Taban durumu (`n == 0`) olmayan çözüm sonsuz özyinelemeye girer ve "Yığın taştı" hatasıyla biter. Çok büyük `n` değerleri (yaklaşık bin üzeri) aynı hatayı taban durumu olsa bile verir; bu sınır için döngü kullanmak daha uygundur.

## Hafta 6

Alıştırmalar: [hafta-06.md](hafta-06.md)

### Alıştırma 6.1

```jus
değişken l = [10, 20, 30, 40, 50]
yaz("İlk:", l[0])
yaz("Son:", l[-1])
yaz("Öğe sayısı:", uzunluk(l))
yaz("Ortadaki:", l[2])
```

```
İlk: 10
Son: 50
Öğe sayısı: 5
Ortadaki: 30
```

### Alıştırma 6.2

```jus
değişken meyveler = ["elma", "armut", "muz"]
yaz("Önce:", meyveler)
ekle(meyveler, "kiraz")
sil(meyveler, 1)
yaz("Sonra:", meyveler)
```

```
Önce: ["elma", "armut", "muz"]
Sonra: ["elma", "muz", "kiraz"]
```

`sil` öğeyi dizinle siler. Metinle silmek isteyen öğrenci `sil(meyveler, "armut")` yazarsa hata alır; önce `bul(meyveler, "armut")` ile dizini bulabilir.

### Alıştırma 6.3

```jus
fonksiyon topla(liste):
    değişken toplam = 0
    her öğe içinde liste:
        toplam += öğe
    dön toplam

yaz(topla([3, 8, 12, 7, 20, 5, 6]))
yaz(topla([]))
```

```
61
0
```

Not: `matematik.toplam` modülü de vardır; ancak bu hafta amaç döngüyle biriktirmektir.

### Alıştırma 6.4

```jus
değişken sayılar = [3, 8, 12, 7, 20, 5, 6]
değişken çiftler = []

her s içinde sayılar:
    eğer s % 2 == 0:
        ekle(çiftler, s)

yaz(çiftler)
```

```
[8, 12, 20, 6]
```

Boş bir listeyle başlayıp döngüde `ekle` ile doldurmak, listelerin en yaygın kalıbıdır.

### Alıştırma 6.5

```jus
değişken sayılar = [18, 24, 9, 31, 15]
değişken enKüçük = sayılar[0]
değişken dizin = 0

her i içinde aralık(uzunluk(sayılar)):
    eğer sayılar[i] < enKüçük:
        enKüçük = sayılar[i]
        dizin = i

yaz("En küçük:", enKüçük, "dizin:", dizin)
```

```
En küçük: 9 dizin: 2
```

Dizin de gerektiği için `aralık(uzunluk(...))` ile gezildi. Yalnızca en küçük değer gerekseydi `her s içinde sayılar` yeterdi.

### Alıştırma 6.6

```jus
değişken notlar = [60, 75, 90, 45, 80]

değişken toplam = 0
her n içinde notlar:
    toplam += n
değişken ortalama = toplam / uzunluk(notlar)

değişken üstündekiler = []
her n içinde notlar:
    eğer n > ortalama:
        ekle(üstündekiler, n)

yaz("Ortalama:", ortalama)
yaz("Ortalamanın üstünde:", üstündekiler)
```

```
Ortalama: 70
Ortalamanın üstünde: [75, 90, 80]
```

Önce ortalamayı bulmak, sonra ikinci bir geçişte karşılaştırmak gerekir. Öğrenci tek döngüde yapmaya çalışırsa ortalama henüz bilinmediği için takılır; bu iki aşamalı düşünmeye iyi bir örnektir.

### Alıştırma 6.7

```jus
değişken asıl = [1, 2, 3, 4]
değişken tersi = []

her öğe içinde asıl:
    araya_ekle(tersi, 0, öğe)

yaz("Asıl:", asıl)
yaz("Ters:", tersi)
```

```
Asıl: [1, 2, 3, 4]
Ters: [4, 3, 2, 1]
```

Her öğe başa eklendiği için son gelen öğe en öne geçer. `asıl` değişmedi; çünkü döngü yalnızca `tersi` listesini değiştirdi.

### Alıştırma 6.8

```jus
değişken a = [1, 2, 3, 4]
değişken b = [3, 4, 5, 6]
değişken tümü = a + b

değişken sonuç = []
her s içinde tümü:
    eğer değil s içinde sonuç:
        ekle(sonuç, s)

yaz(sonuç)
```

```
[1, 2, 3, 4, 5, 6]
```

`eğer değil s içinde sonuç:` "s sonuçta yoksa" diye okunur. `+` iki listeyi birleştirip yeni liste verir.

### Alıştırma 6.9

```jus
fonksiyon sıralıMı(liste):
    her i içinde aralık(1, uzunluk(liste)):
        eğer liste[i] < liste[i - 1]:
            dön yanlış
    dön doğru

yaz(sıralıMı([1, 2, 2, 5]))
yaz(sıralıMı([3, 1, 2]))
yaz(sıralıMı([]))
```

```
doğru
yanlış
doğru
```

Boş liste ve tek öğeli liste için döngü hiç çalışmaz; sonuç `doğru` olur. Bu, "boş küme" gibi sınır durumlarını düşünmeye güzel bir örnektir. Öğrenci `sırala(liste) == liste` yazarsa da doğru sonucu alır (içerik eşitliği); bu çözüm de kabul edilir.

## Hafta 7

Alıştırmalar: [hafta-07.md](hafta-07.md)

### Alıştırma 7.1

```jus
değişken m = "merhaba"
yaz("İlk harf:", m[0])
yaz("Son harf:", m[-1])
yaz("Uzunluk:", uzunluk(m))
yaz("Ortadaki üç harf:", m[2:5])
```

```
İlk harf: m
Son harf: a
Uzunluk: 7
Ortadaki üç harf: rha
```

### Alıştırma 7.2

```jus
değişken cümle = "bir iki üç dört beş altı"
yaz("Kelime sayısı:", uzunluk(böl(cümle, " ")))
```

```
Kelime sayısı: 6
```

### Alıştırma 7.3

```jus
değişken ad = "ayşe"
değişken soyad = "yılmaz"
yaz(büyük_harf(ad) + " " + büyük_harf(soyad))
```

```
AYŞE YILMAZ
```

`Y` ve `A` değil `AYŞE` ve `YILMAZ`: `ş` ve `ı` harfleri de doğru büyütülür. `ı` büyüyünce `I` olur.

### Alıştırma 7.4

```jus
fonksiyon harfSay(m, aranan):
    değişken adet = 0
    her harf içinde m:
        eğer harf == aranan:
            adet += 1
    dön adet

yaz(harfSay("kardeşlerimiz", "e"))
yaz(harfSay("kardeşlerimiz", "r"))
yaz(harfSay("kardeşlerimiz", "x"))
```

```
2
2
0
```

### Alıştırma 7.5

```jus
fonksiyon başHarfBüyük(kelime):
    eğer uzunluk(kelime) == 0:
        dön kelime
    dön büyük_harf(kelime[0]) + kelime[1:]

değişken sonuç = []
her kelime içinde böl("ışıl ışıl parlayan istanbul", " "):
    ekle(sonuç, başHarfBüyük(kelime))

yaz(birleştir(sonuç, " "))
```

```
Işıl Işıl Parlayan İstanbul
```

Beklenen sonucun `Işıl` ve `İstanbul` ile başladığına dikkat: ilk harf `ı` olduğunda `I`, `i` olduğunda `İ` olur.

### Alıştırma 7.6

```jus
fonksiyon palindromMu(m):
    dön m == ters(m)

yaz(palindromMu("kayak"))
yaz(palindromMu("araba"))
yaz(palindromMu("radar"))
```

```
doğru
yanlış
doğru
```

`ters` metinle de çalışır ve yeni bir metin verir. Öğrenci karşılıklı harfleri karşılaştıran bir döngü yazarsa (ilk harf ile son harf, ikinci ile sondan ikinci...) bu da kabul edilir ve daha zordur.

### Alıştırma 7.7

```jus
fonksiyon satırYaz(ad, tutar):
    yaz(sağa_doldur(ad, 12) + sola_doldur(biçimle(tutar, 2), 8))

yaz(tekrarla("=", 20))
yaz(tekrarla(" ", 8) + "FİŞ")
yaz(tekrarla("-", 20))
satırYaz("Ekmek", 7.5)
satırYaz("Süt", 32)
satırYaz("Peynir", 125.75)
yaz(tekrarla("-", 20))
satırYaz("TOPLAM", 7.5 + 32 + 125.75)
```

```
====================
        FİŞ
--------------------
Ekmek           7.50
Süt            32.00
Peynir        125.75
--------------------
TOPLAM        165.25
```

Fonksiyon yazmak zorunlu değildir; üç satırı da doğrudan yazan çözüm de doğrudur. Fonksiyon, tekrarı azalttığı için önerilir. `biçimle` zaten metin döndürdüğü için `metin()` gerekmez.

### Alıştırma 7.8

```jus
değişken sesliler = "aeıioöuü"
değişken adet = 0

her harf içinde küçük_harf("Çanakkale Boğazı"):
    eğer harf içinde sesliler:
        adet += 1

yaz("Sesli harf sayısı:", adet)
```

```
Sesli harf sayısı: 7
```

`küçük_harf` çağrısı atlanırsa baştaki `Ç` ve `B` zaten sesli değildir; ama başka cümlelerde büyük harfle başlayan sesli (ör. `Ali`'deki `A`) atlanır. Bu yüzden küçük harfe çevirmek bir alışkanlık olmalıdır.

### Alıştırma 7.9

```jus
değişken alfabe = "abcçdefgğhıijklmnoöprsştuüvyz"

fonksiyon sezar(m, kaydır):
    değişken sonuç = ""
    her harf içinde m:
        değişken yer = bul(alfabe, harf)
        eğer yer == -1:
            sonuç = sonuç + harf
        değilse:
            sonuç = sonuç + alfabe[(yer + kaydır) % 29]
    dön sonuç

değişken şifreli = sezar("merhaba dünya", 3)
yaz(şifreli)
yaz(sezar(şifreli, 26))
yaz(uzunluk(alfabe))
```

```
öğtjçdç gzpbç
merhaba dünya
29
```

Alfabe 29 harftir, bu yüzden geri çözmek için -3 değil 26 (29 - 3) kaydırılır. `eğer yer == -1` denetimi olmayan çözümde `bul` boşluk için `-1` verir ve boşluk bir harfe dönüşür; "nerede takılır" bölümünün 5. maddesindeki tuzağın aynısıdır.

## Hafta 8

Alıştırmalar: [hafta-08.md](hafta-08.md)

### Alıştırma 8.1

```jus
değişken kişi = {"ad": "Deniz", "yaş": 13, "şehir": "Bursa"}
kişi["yaş"] += 1
kişi["sınıf"] = "7-A"
yaz(kişi)
```

```
{"ad": "Deniz", "yaş": 14, "şehir": "Bursa", "sınıf": "7-A"}
```

Sözlük eklenme sırasıyla yazılır; yeni anahtar sona eklenir.

### Alıştırma 8.2

```jus
değişken notlar = {"Ayşe": 90, "Mehmet": 70, "Zeynep": 80}
değişken toplam = 0
her ad içinde notlar:
    toplam += notlar[ad]
yaz("Ortalama:", toplam / uzunluk(notlar))
```

```
Ortalama: 80
```

### Alıştırma 8.3

```jus
değişken rehber = {"Ayşe": "555 111 22 33", "Veli": "555 444 55 66"}

her ad içinde ["Veli", "Selim"]:
    eğer ad içinde rehber:
        yaz(ad + ":", rehber[ad])
    değilse:
        yaz(ad + ": kayıtlı değil")
```

```
Veli: 555 444 55 66
Selim: kayıtlı değil
```

Burada telefon numaraları uydurmadır. Aynı iş `yaz(ad + ":", al(rehber, ad, "kayıtlı değil"))` ile tek satırda yapılabilir; kabul edilir.

### Alıştırma 8.4

```jus
değişken sayım = {}
her harf içinde "mississippi":
    sayım[harf] = al(sayım, harf, 0) + 1

her harf içinde sırala(anahtarlar(sayım)):
    yaz(harf + ":", sayım[harf])
```

```
i: 4
m: 1
p: 2
s: 4
```

Sıralama olmadan sözlük eklenme sırasıyla (`m`, `i`, `s`, `p`) yazılırdı; alfabetik çıktı için anahtarlar `sırala` ile sıralandı.

### Alıştırma 8.5

```jus
değişken trEn = {"elma": "apple", "kitap": "book", "su": "water"}
değişken enTr = {}

her tr içinde trEn:
    enTr[trEn[tr]] = tr

yaz(enTr)
```

```
{"apple": "elma", "book": "kitap", "water": "su"}
```

Değerler benzersiz değilse (iki Türkçe kelimenin aynı İngilizcesi varsa) son yazılan kazanır ve bir girdi kaybolur; bu, yer değiştirme işleminin sınırıdır.

### Alıştırma 8.6

```jus
değişken sayım = {}
her kelime içinde böl("kedi köpek kedi kuş köpek kedi", " "):
    sayım[kelime] = al(sayım, kelime, 0) + 1

değişken enÇok = ""
değişken adet = 0
her kelime içinde anahtarlar(sayım):
    eğer sayım[kelime] > adet:
        enÇok = kelime
        adet = sayım[kelime]

yaz("En çok geçen:", enÇok, "sayısı:", adet)
```

```
En çok geçen: kedi sayısı: 3
```

Eşitlik durumunda (iki kelime de aynı sayıda geçerse) bu çözüm ilk eklenen kelimeyi verir; çünkü `>` kullanıldı. Öğrenciyle "eşitlik olursa ne olmalı?" sorusunu konuşun.

### Alıştırma 8.7

```jus
değişken kişiler = [
    {"ad": "Ali", "yaş": 31},
    {"ad": "Ece", "yaş": 24},
    {"ad": "Deniz", "yaş": 45},
]

fonksiyon yaşaGöre(k):
    dön k["yaş"]

her k içinde sırala(kişiler, yaşaGöre):
    yaz(k["ad"], k["yaş"])
```

```
Ece 24
Ali 31
Deniz 45
```

### Alıştırma 8.8

```jus
değişken fiyatlar = {"ekmek": 7.5, "süt": 32, "peynir": 125.75}
değişken sepet = [
    {"ürün": "ekmek", "adet": 2},
    {"ürün": "süt", "adet": 1},
    {"ürün": "peynir", "adet": 2},
]

değişken toplam = 0
her satır içinde sepet:
    toplam += fiyatlar[satır["ürün"]] * satır["adet"]

yaz("Toplam:", biçimle(toplam, 2), "TL")
```

```
Toplam: 298.50 TL
```

`fiyatlar[satır["ürün"]]` ifadesi iki adımdır: önce `satır["ürün"]` (ürün adı), sonra o adla fiyat. Öğrenci anlamakta zorlanırsa ara değişkene (`değişken ad = satır["ürün"]`) bölmesini önerin.

### Alıştırma 8.9

```jus
fonksiyon harfSayımı(m):
    değişken sayım = {}
    her harf içinde m:
        sayım[harf] = al(sayım, harf, 0) + 1
    dön sayım

fonksiyon anagramMı(a, b):
    dön harfSayımı(a) == harfSayımı(b)

yaz(anagramMı("kalem", "melak"))
yaz(anagramMı("kitap", "patik"))
yaz(anagramMı("ev", "el"))
```

```
doğru
doğru
yanlış
```

İçeriği aynı olan iki sözlük, girdilerin sırası farklı olsa bile eşittir. Alternatif çözüm: iki kelimenin harflerini `sırala(böl(kelime, ""))` ile sıralayıp karşılaştırmak (boş ayraç metni karakterlerine böler).

## Hafta 9

Alıştırmalar: [hafta-09.md](hafta-09.md)

### Alıştırma 9.1

```jus
fonksiyon güvenliBöl(a, b):
    dene:
        dön a / b
    yakala:
        dön boş

yaz(güvenliBöl(10, 4))
yaz(güvenliBöl(1, 0))
```

```
2.5
boş
```

Alternatif çözüm: `eğer b == 0: dön boş` ile önceden denetlemek de doğrudur. Bu alıştırmanın amacı `dene` / `yakala` yapısını yazmaktır; ikisini karşılaştırmak iyi bir tartışmadır.

### Alıştırma 9.2

```jus
fonksiyon sayıOku(m):
    dene:
        dön sayı(m)
    yakala:
        dön boş

yaz(sayıOku("42"))
yaz(sayıOku("3.5"))
yaz(sayıOku("elma"))
yaz(sayıOku(""))
```

```
42
3.5
boş
boş
```

### Alıştırma 9.3

```jus
değişken l = [1, 2, 3]

dene:
    yaz(l[10])
yakala hata:
    yaz("Hata:", hata)

yaz("Program sürüyor.")
```

```
Hata: Dizin sınırların dışında: uzunluk 3, istenen dizin 10.
Program sürüyor.
```

### Alıştırma 9.4

```jus
fonksiyon karekökAl(x):
    eğer x < 0:
        fırlat "Negatif sayının karekökü alınamaz."
    dön karekök(x)

her x içinde [16, -4, 2]:
    dene:
        yaz("Karekök:", yuvarla(karekökAl(x), 4))
    yakala hata:
        yaz("Hata:", hata)
```

```
Karekök: 4
Hata: Negatif sayının karekökü alınamaz.
Karekök: 1.4142
```

`karekök` negatif sayıda zaten hata verir; `fırlat` ile kendi ileti metnimizi yazdık. Döngünün bir turundaki hata yakalandığı için sonraki tura geçilir.

### Alıştırma 9.5

```jus
değişken girdiler = ["12", "abc", "7", "", "3.5"]
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
Toplam: 22.5
Geçersiz: 2
```

### Alıştırma 9.6

```jus
kullan rastgele

rastgele.tohum(3)

değişken ayşePuan = 0
değişken boraPuan = 0

her tur içinde aralık(1, 4):
    değişken ayşeZar = rastgele.tam(1, 6)
    değişken boraZar = rastgele.tam(1, 6)
    eğer ayşeZar > boraZar:
        ayşePuan += 1
    değilse eğer boraZar > ayşeZar:
        boraPuan += 1
    yaz("Tur", tur, ":", ayşeZar, boraZar, "| Puan:", ayşePuan, boraPuan)
```

```
Tur 1 : 4 3 | Puan: 1 0
Tur 2 : 6 3 | Puan: 2 0
Tur 3 : 6 2 | Puan: 3 0
```

Öğrencilerin çıktısı farklı olacaktır; kabul ölçütü, aynı programın aynı tohumla her çalıştırmada aynı çıktıyı vermesidir. Öğrenci `tohum` satırını silerse her çalıştırmada farklı çıktı alır.

### Alıştırma 9.7

```jus
değişken gizli = 14
değişken düşük = 1
değişken yüksek = 100
değişken adım = 0

iken doğru:
    değişken tahmin = taban((düşük + yüksek) / 2)
    adım += 1
    yaz("Tahmin", adım, ":", tahmin)
    eğer tahmin == gizli:
        yaz("Buldum!", adım, "adımda.")
        kır
    değilse eğer tahmin < gizli:
        düşük = tahmin + 1
    değilse:
        yüksek = tahmin - 1
```

gizli = 14 için:

```
Tahmin 1 : 50
Tahmin 2 : 25
Tahmin 3 : 12
Tahmin 4 : 18
Tahmin 5 : 15
Tahmin 6 : 13
Tahmin 7 : 14
Buldum! 7 adımda.
```

gizli = 100 için:

```
Tahmin 1 : 50
Tahmin 2 : 75
Tahmin 3 : 88
Tahmin 4 : 94
Tahmin 5 : 97
Tahmin 6 : 99
Tahmin 7 : 100
Buldum! 7 adımda.
```

gizli = 1 için:

```
Tahmin 1 : 50
Tahmin 2 : 25
Tahmin 3 : 12
Tahmin 4 : 6
Tahmin 5 : 3
Tahmin 6 : 1
Buldum! 6 adımda.
```

Her adımda aralık yarıya indiği için 100 sayı içinde en fazla 7 adım gerekir (2'nin 7. kuvveti 128'dir). `kır` olmayan çözüm, sayıyı bulduktan sonra da dönmeye devam eder; sonsuz döngüye girer. Bu, `iken doğru:` döngülerinde çıkış koşulunun şart olduğunu gösteren iyi bir örnektir.

### Alıştırma 9.8

```jus
kullan rastgele

fonksiyon sayıyaÇevir(girdi):
    dene:
        dön sayı(girdi)
    yakala:
        dön boş

rastgele.tohum(42)
değişken gizli = rastgele.tam(1, 100)
değişken hak = 7
değişken tahminSayısı = 0
değişken bulundu = yanlış

yaz("1 ile 100 arasında bir sayı tuttum.", hak, "hakkınız var.")

iken tahminSayısı < hak ve değil bulundu:
    değişken girdi = oku("Tahmin: ")
    eğer girdi == boş:
        yaz("Girdi bitti.")
        kır
    değişken tahmin = sayıyaÇevir(girdi)
    eğer tahmin == boş:
        yaz("Bu bir sayı değil, hak kaybetmediniz.")
        devam
    tahminSayısı += 1
    eğer tahmin == gizli:
        bulundu = doğru
    değilse eğer tahmin < gizli:
        yaz("Daha büyük bir sayı deneyin.")
    değilse:
        yaz("Daha küçük bir sayı deneyin.")

eğer bulundu:
    yaz("Tebrikler!", tahminSayısı, "tahminde buldunuz.")
değilse:
    yaz("Olmadı. Sayı", gizli, "idi.")
```

Girdi satırları: `50`, `abc`, `20`, `45`

```
1 ile 100 arasında bir sayı tuttum. 7 hakkınız var.
Tahmin: Daha küçük bir sayı deneyin.
Tahmin: Bu bir sayı değil, hak kaybetmediniz.
Tahmin: Daha büyük bir sayı deneyin.
Tahmin: Tebrikler! 3 tahminde buldunuz.
```

`abc` yazılan turda `tahminSayısı` artmaz; çünkü `devam`, döngünün geri kalanını atlar.

### Alıştırma 9.9

```jus
değişken satırlar = ["Ayşe:90", "Mehmet:abc", "Zeynep:80", "Can"]
değişken notlar = {}

her satır içinde satırlar:
    dene:
        değişken parçalar = böl(satır, ":")
        eğer uzunluk(parçalar) != 2:
            fırlat "':' işareti yok"
        notlar[parçalar[0]] = sayı(parçalar[1])
    yakala hata:
        yaz("Hatalı satır:", satır, "-", hata)

değişken toplam = 0
her ad içinde notlar:
    toplam += notlar[ad]

yaz(notlar)
yaz("Ortalama:", toplam / uzunluk(notlar))
```

```
Hatalı satır: Mehmet:abc - "abc" bir sayıya dönüştürülemez.
Hatalı satır: Can - ':' işareti yok
{"Ayşe": 90, "Zeynep": 80}
Ortalama: 85
```

İki farklı hata tek `yakala` ile karşılandı: dilin kendi hatası (`sayı("abc")`) ve bizim `fırlat` ile oluşturduğumuz hata. Her ikisinde de `hata` değişkeni metindir.

## Hafta 10

Alıştırmalar: [hafta-10.md](hafta-10.md)

### Alıştırma 10.1

```jus
değişken vize = 70
değişken final = 85
değişken ortalama = vize * 0.4 + final * 0.6

yaz("Ortalama:", ortalama)
eğer ortalama >= 50:
    yaz("Geçti")
değilse:
    yaz("Kaldı")
```

```
Ortalama: 79
Geçti
```

### Alıştırma 10.2

```jus
fonksiyon çarpımTablosu(n):
    her i içinde aralık(1, 6):
        yaz(n, "x", i, "=", n * i)

çarpımTablosu(3)
çarpımTablosu(12)
```

```
3 x 1 = 3
3 x 2 = 6
3 x 3 = 9
3 x 4 = 12
3 x 5 = 15
12 x 1 = 12
12 x 2 = 24
12 x 3 = 36
12 x 4 = 48
12 x 5 = 60
```

### Alıştırma 10.3

```jus
fonksiyon özet(liste):
    değişken enKüçük = liste[0]
    değişken enBüyük = liste[0]
    değişken toplam = 0
    her s içinde liste:
        eğer s < enKüçük:
            enKüçük = s
        eğer s > enBüyük:
            enBüyük = s
        toplam += s
    dön [enKüçük, enBüyük, toplam / uzunluk(liste)]

yaz(özet([4, 8, 15, 16, 23, 42]))
```

```
[4, 42, 18]
```

Bir fonksiyon tek değer döndürür; birden çok sonucu bir liste içinde döndürmek yaygın çözümdür. Çağıran taraf `değişken sonuç = özet(...)` yazıp `sonuç[0]`, `sonuç[1]`, `sonuç[2]` ile ulaşır.

### Alıştırma 10.4

```jus
değişken cümle = "bugün hava çok güzel ve güneşli"
değişken kelimeler = böl(cümle, " ")
değişken enUzun = ""

her k içinde kelimeler:
    eğer uzunluk(k) > uzunluk(enUzun):
        enUzun = k

yaz("Kelime sayısı:", uzunluk(kelimeler))
yaz("En uzun kelime:", enUzun)
```

```
Kelime sayısı: 6
En uzun kelime: güneşli
```

### Alıştırma 10.5

```jus
değişken oylar = ["Fen", "Matematik", "Fen", "Türkçe", "Matematik", "Fen"]
değişken sayım = {}

her oy içinde oylar:
    sayım[oy] = al(sayım, oy, 0) + 1

fonksiyon oyaGöre(ders):
    dön -sayım[ders]

her ders içinde sırala(anahtarlar(sayım), oyaGöre):
    yaz(ders + ":", sayım[ders])
```

```
Fen: 3
Matematik: 2
Türkçe: 1
```

Oy sayısının eksisini döndürmek, büyükten küçüğe sıralamanın bilinen kısa yoludur (Hafta 8, Adım 6).

### Alıştırma 10.6

```jus
değişken girdiler = ["80", "yok", "65", "90", "", "75"]
değişken geçerli = []
değişken geçersiz = 0

her g içinde girdiler:
    dene:
        ekle(geçerli, sayı(g))
    yakala:
        geçersiz += 1

eğer uzunluk(geçerli) == 0:
    yaz("Not yok")
değilse:
    değişken toplam = 0
    her n içinde geçerli:
        toplam += n
    yaz("Ortalama:", toplam / uzunluk(geçerli))

yaz("Geçersiz girdi:", geçersiz)
```

```
Ortalama: 77.5
Geçersiz girdi: 2
```

`uzunluk(geçerli) == 0` denetimi olmayan çözüm, hiç geçerli not yokken sıfıra bölme hatası verir (`0 / 0`). Bu, "uç durumu düşünmek" için iyi bir örnektir.

### Alıştırma 10.7

```jus
değişken öğrenciler = [
    {"ad": "Ayşe", "notlar": [90, 85, 70]},
    {"ad": "Mehmet", "notlar": [35, 60, 40]},
    {"ad": "Zeynep", "notlar": [100, 95, 80]},
]

fonksiyon ortalama(liste):
    değişken toplam = 0
    her n içinde liste:
        toplam += n
    dön toplam / uzunluk(liste)

değişken enİyi = ""
değişken enYüksek = -1

her ö içinde öğrenciler:
    değişken ort = ortalama(ö["notlar"])
    değişken durum = "Geçti"
    eğer ort < 50:
        durum = "Kaldı"
    yaz(sağa_doldur(ö["ad"], 8), biçimle(ort, 1), durum)
    eğer ort > enYüksek:
        enYüksek = ort
        enİyi = ö["ad"]

yaz("En yüksek ortalama:", enİyi)
```

```
Ayşe     81.7 Geçti
Mehmet   45.0 Kaldı
Zeynep   91.7 Geçti
En yüksek ortalama: Zeynep
```

Bu program sözlük listesi, iç içe liste, fonksiyon, karar ve en büyüğü bulma kalıbını birlikte kullanır; proje çalışmasının küçük bir örneğidir.

### Alıştırma 10.8

```jus
fonksiyon gücüBul(şifre):
    değişken puan = 0
    değişken rakamVar = yanlış
    değişken büyükVar = yanlış

    eğer uzunluk(şifre) >= 8:
        puan += 1

    her harf içinde şifre:
        eğer harf içinde "0123456789":
            rakamVar = doğru
        eğer harf != küçük_harf(harf):
            büyükVar = doğru

    eğer rakamVar:
        puan += 1
    eğer büyükVar:
        puan += 1

    eğer puan >= 3:
        dön "güçlü"
    değilse eğer puan == 2:
        dön "orta"
    dön "zayıf"

yaz(gücüBul("elma"))
yaz(gücüBul("elma1234"))
yaz(gücüBul("Elma1234"))
```

```
zayıf
orta
güçlü
```

`harf != küçük_harf(harf)` yalnızca büyük harflerde `doğru` olur; rakam ve küçük harflerde harf kendisine eşittir. Bu yöntem `Ç`, `İ` gibi Türkçe büyük harflerde de çalışır.
