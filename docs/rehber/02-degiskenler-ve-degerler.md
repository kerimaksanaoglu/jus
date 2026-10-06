# 02 - Değişkenler ve değerler

Önceki: [01 - İlk program](01-ilk-program.md) | [İçindekiler](README.md) | Sonraki: [03 - Kararlar](03-kararlar.md)

Programlar bilgiyle çalışır: sayılar, adlar, cevaplar. Bu bölümde JUS'un temel bilgi türlerini, bilgiyi saklamak için kullanılan **değişkenleri**, hesap yapmayı ve kullanıcıdan bilgi almayı öğreneceksiniz.

## Değer türleri

Her değerin bir **türü** vardır. Tür, değerin ne çeşit bir bilgi olduğunu söyler. JUS'un dört temel türü şunlardır:

| Tür | Örnekler | Ne işe yarar |
|-----|----------|--------------|
| `sayı` | `42`, `3.14`, `-7` | Hesap yapılan bilgi |
| `metin` | `"merhaba"`, `"42"` | Yazı |
| `mantıksal` | `doğru`, `yanlış` | Bir şeyin evet/hayır cevabı |
| `boş` | `boş` | "Değer yok" anlamında özel değer |

Ondalık sayılarda ayırıcı virgül değil **noktadır**: `3.14`. Noktanın iki yanında da rakam olmalıdır.

Bir değerin türünü `tür` fonksiyonu söyler:

```jus
yaz(tür(42))
yaz(tür(3.14))
yaz(tür("merhaba"))
yaz(tür(doğru))
yaz(tür(boş))
yaz(tür([1, 2, 3]))
yaz(tür({"ad": "Ayşe"}))
yaz(tür(yaz))
```

```
sayı
sayı
metin
mantıksal
boş
liste
sözlük
fonksiyon
```

JUS'ta tam sayı ile ondalık sayı ayrı türler değildir; ikisi de `sayı`dır. `liste` ve `sözlük` türlerini 6. ve 8. bölümlerde öğreneceksiniz. `yaz` bile bir değerdir: türü `fonksiyon`.

`"42"` ile `42` farklı şeylerdir. Biri yazı, biri sayıdır. Yazıyla toplama ya da çarpma yapamazsınız.

## Değişkenler

**Değişken**, bir değere verilen addır. Değeri bir kez saklayıp sonra adıyla kullanırsınız. Değişkeni `değişken` anahtar kelimesiyle tanımlarsınız:

```jus
değişken ad = "Ayşe"
değişken yaş = 25
yaz(ad)
yaz(yaş)

yaş = 26
yaz("Yeni yaş:", yaş)

değişken henüz
yaz(henüz)

değişken sayaç = 0
sayaç += 1
sayaç += 1
yaz(sayaç)
```

```
Ayşe
25
Yeni yaş: 26
boş
2
```

Adım adım:

- `değişken ad = "Ayşe"` yeni bir değişken tanımlar ve ona `"Ayşe"` değerini verir. `=` işaretine **atama** denir: sağdaki değeri soldaki değişkene yerleştirir. Matematikteki "eşittir" anlamında değildir.
- `yaş = 26` var olan değişkenin değerini değiştirir. Bu kez `değişken` yazmayız; çünkü değişken zaten vardır.
- `değişken henüz` yalnızca ad tanımlar; değeri `boş` olur.
- `sayaç += 1`, `sayaç = sayaç + 1` ile aynıdır. `-=`, `*=` ve `/=` da vardır.

### Değişken adı kuralları

- Ad bir harf ya da alt çizgi (`_`) ile başlar. Sonrasında harf, rakam ve alt çizgi gelebilir.
- Türkçe harfler kullanılabilir: `yaş`, `öğrenci`, `şehir`.
- Büyük küçük harf ayrımı vardır: `yaş` ile `Yaş` farklı adlardır.
- Aşağıdaki 24 anahtar kelime ad olarak kullanılamaz:

```
boş      bu       değil    değilse   değişken   dene     devam    doğru
dön      eğer     fırlat   fonksiyon her        içinde   iken     kır
kullan   olarak   sınıf    üst       ve         veya     yakala   yanlış
```

İyi ad, değişkenin ne tuttuğunu anlatır: `x` yerine `öğrenciSayısı` gibi.

## Aritmetik

JUS'ta dört işlem ve kalan alma vardır:

| İşleç | Anlamı |
|-------|--------|
| `+` | toplama |
| `-` | çıkarma |
| `*` | çarpma |
| `/` | bölme |
| `%` | bölümden kalan |

```jus
yaz(7 + 3)
yaz(7 - 3)
yaz(7 * 3)
yaz(15 / 4)
yaz(15 % 4)
yaz(-7 % 3)
yaz(1 / 3)
yaz(2 + 3 * 4)
yaz((2 + 3) * 4)
yaz(-5 + 2)
yaz(10 / 2)

değişken x = 10
x -= 3
yaz(x)
x *= 4
yaz(x)
x /= 7
yaz(x)
```

```
10
4
21
3.75
3
2
0.33333333333333
14
20
-3
5
7
28
4
```

Önemli noktalar:

- `/` her zaman ondalıklı bölme yapar: `15 / 4` sonucu `3.75`'tir.
- Tam sayı sonuçlar ondalık kısım olmadan yazılır (`10 / 2` için `5`). Diğer sonuçlar en çok 14 anlamlı basamakla yazılır.
- `%` bölümden kalanı verir. Çift sayı kontrolü gibi işlerde çok kullanılır: `n % 2 == 0`.
- `-7 % 3` sonucu `2`'dir; sonuç bölenin işaretini taşır.
- Çarpma ve bölme, toplama ve çıkarmadan önce yapılır. Sırayı değiştirmek için parantez kullanın: `(2 + 3) * 4`.

## Metinleri birleştirmek

İki metni `+` ile birleştirirsiniz:

```jus
değişken ad = "Ayşe"
değişken yaş = 25

yaz("Merhaba, " + ad + "!")
yaz(ad + " " + "Yılmaz")
yaz(ad + " " + metin(yaş) + " yaşında.")
```

```
Merhaba, Ayşe!
Ayşe Yılmaz
Ayşe 25 yaşında.
```

Bir sayıyı metne eklemek istiyorsanız önce `metin()` ile metne çevirmelisiniz. JUS türleri sizin yerinize sessizce çevirmez; bu, hataları erken yakalamanızı sağlar.

## Dönüşümler: `metin()` ve `sayı()`

- `metin(değer)` herhangi bir değeri metne çevirir.
- `sayı(metin)` bir metni sayıya çevirir. Metin geçerli bir sayı değilse hata verir.

```jus
değişken girdi = "40"
değişken sayı2 = sayı(girdi)
yaz(sayı2 + 2)
yaz(girdi + "2")
yaz(metin(doğru))
yaz(metin(3.5))
```

```
42
402
doğru
3.5
```

`sayı2 + 2` iki sayıyı toplar (`42`); `girdi + "2"` ise iki metni birleştirir (`402`). Aynı `+` işareti türe göre farklı iş yapar.

## Kullanıcıdan bilgi almak: `oku`

`oku()` fonksiyonu kullanıcının yazdığı bir satırı okur ve **metin** olarak verir. `oku("istem")` biçiminde yazarsanız önce istemi ekrana yazar.

```jus
değişken ad = oku("Adınız nedir? ")
yaz("Merhaba, " + ad + "!")

değişken yaşMetni = oku("Yaşınız? ")
değişken yaş = sayı(yaşMetni)
yaz("Gelecek yıl " + metin(yaş + 1) + " yaşında olacaksınız.")
```

Bu programı çalıştırınca JUS sizden iki satır yazmanızı bekler: önce adınızı, sonra yaşınızı. Burada girdiyi kendimiz vererek çalıştırdık (`printf` komutu iki satırlık girdiyi programa gönderir):

```sh
printf 'Ayşe\n29\n' | jus ornek.jus
```

```
Adınız nedir? Merhaba, Ayşe!
Yaşınız? Gelecek yıl 30 yaşında olacaksınız.
```

Siz programı normal çalıştırıp klavyeden yazarsanız, yazdıklarınız istemin yanında görünür. Yukarıdaki çıktıda görünmemelerinin nedeni, girdinin klavyeden değil `printf` komutundan gelmesidir.

`oku` her zaman metin verir. Sayı istiyorsanız `sayı()` ile çevirmelisiniz. Girdi bittiyse (örneğin klavyeden girdi kapatıldıysa) `oku` `boş` verir.

## Sık yapılan hatalar

**Sayı ile metni `+` ile birleştirmek.**

```jus
değişken yaş = 25
yaz("Yaşınız: " + yaş)
```

```
ornek.jus:2: çalışma zamanı hatası: '+' işleci iki sayı, iki metin ya da iki liste ister; metin ve sayı verildi. Dönüştürmek için metin() ya da sayı() kullanılabilir.
```

Çözüm: `"Yaşınız: " + metin(yaş)`.

**Tanımlamadan atama yapmak.**

```jus
sayaç = 1
yaz(sayaç)
```

```
ornek.jus:1: çalışma zamanı hatası: 'sayaç' adında bir değişken tanımlı değil. Yeni değişken için 'değişken sayaç = ...' yazın.
```

Yeni bir değişken için `değişken` yazmalısınız.

**Sayı olmayan metni sayıya çevirmek.**

```jus
değişken sayı3 = sayı("abc")
yaz(sayı3)
```

```
ornek.jus:1: çalışma zamanı hatası: "abc" bir sayıya dönüştürülemez.
```

Kullanıcıdan gelen girdinin sayı olmayabileceğini unutmayın. Bunu güvenle karşılamayı dokuzuncu bölümde öğreneceksiniz.

**Girdi bittiğinde `sayı(oku())` yazmak.** Yukarıdaki programı yalnızca bir satır girdiyle (`printf 'Ayşe\n'`) çalıştırırsanız ikinci `oku` `boş` verir ve `sayı` bunu çeviremez:

```
Adınız nedir? Merhaba, Ayşe!
Yaşınız? ornek.jus:5: çalışma zamanı hatası: 'sayı' fonksiyonu metin ya da sayı ister; boş verildi.
```

**Sıfıra bölmek.**

```jus
değişken a = 10
yaz(a / 0)
```

```
ornek.jus:2: çalışma zamanı hatası: Sıfıra bölünemez.
```

**Rakamla başlayan ad.**

```jus
değişken 2sayı = 5
```

```
ornek.jus:1:10: sözdizimi hatası: Değişken adı bekleniyor.
    değişken 2sayı = 5
             ^
```

## Alıştırmalar

1. Bir dikdörtgenin eni 12, boyu 5. Alanını ve çevresini iki değişken kullanarak hesaplayıp yazdırın.
2. 25 derece Celsius'u Fahrenheit'e çevirin (formül: `C * 9 / 5 + 32`). Çıktı `25 derece Celsius = 77 derece Fahrenheit` biçiminde olsun.
3. Kullanıcıdan iki sayı isteyin; toplamlarını ve ortalamalarını yazdırın.
4. 135 saniyeyi dakika ve saniye olarak yazdırın: `2 dakika 15 saniye`. (İpucu: `taban` ve `%`. `taban(x)`, `x`'ten büyük olmayan en büyük tam sayıyı verir.)
5. Aşağıdaki program hata veriyor. Düzeltin:
   ```jus
   yaz("Toplam: " + 5 + 3)
   ```
   Ardından `Toplam: 8` ve `Toplam: 53` çıktılarını veren iki ayrı düzeltme yazın.

## Çözümler

**1.**

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

**2.**

```jus
değişken celsius = 25
değişken fahrenheit = celsius * 9 / 5 + 32
yaz(metin(celsius) + " derece Celsius = " + metin(fahrenheit) + " derece Fahrenheit")
```

```
25 derece Celsius = 77 derece Fahrenheit
```

**3.**

```jus
değişken a = sayı(oku("Birinci sayı: "))
değişken b = sayı(oku("İkinci sayı: "))
yaz("Toplam:", a + b)
yaz("Ortalama:", (a + b) / 2)
```

`printf '10\n25\n' | jus ornek.jus` ile çalıştırıldığında:

```
Birinci sayı: İkinci sayı: Toplam: 35
Ortalama: 17.5
```

**4.**

```jus
değişken saniye = 135
değişken dakika = taban(saniye / 60)
değişken kalan = saniye % 60
yaz(metin(dakika) + " dakika " + metin(kalan) + " saniye")
```

```
2 dakika 15 saniye
```

**5.** Özgün program şu hatayı verir:

```
ornek.jus:1: çalışma zamanı hatası: '+' işleci iki sayı, iki metin ya da iki liste ister; metin ve sayı verildi. Dönüştürmek için metin() ya da sayı() kullanılabilir.
```

İki düzeltme:

```jus
yaz("Toplam: " + metin(5 + 3))
yaz("Toplam: " + metin(5) + metin(3))
```

```
Toplam: 8
Toplam: 53
```

---

Önceki: [01 - İlk program](01-ilk-program.md) | [İçindekiler](README.md) | Sonraki: [03 - Kararlar](03-kararlar.md)
