# Hafta 3 - Kararlar: `eğer` ve `değilse`

[Öğretmen kiti](README.md) | Önceki: [Hafta 2](hafta-02.md) | Sonraki: [Hafta 4 - Döngüler](hafta-04.md)

İlgili rehber bölümü: [03 - Kararlar](../rehber/03-kararlar.md)

Bu hafta programlar ilk kez "duruma göre farklı davranır". İki fikri yerleştirmeye çalışın: (1) koşul her zaman `doğru` ya da `yanlış` üretir, (2) bir `eğer / değilse eğer / değilse` zincirinde **yalnızca bir** yol çalışır. Ayrıca bu hafta girinti ilk kez bir kural olarak karşınıza çıkar: bir bloğun hangi satırlara ait olduğunu girinti belirler.

## Kazanımlar

Hafta sonunda öğrenci:

1. Karşılaştırma işleçleriyle (`==`, `!=`, `<`, `<=`, `>`, `>=`) `doğru` ya da `yanlış` üreten ifadeler yazabilir; `=` ile `==` arasındaki farkı açıklayabilir.
2. `eğer`, `değilse eğer` ve `değilse` ile iki ve daha çok yollu karar yazabilir.
3. `ve`, `veya`, `değil` ile birleşik koşul kurabilir.
4. Girintinin bir bloğa hangi satırların ait olduğunu belirlediğini bilir; girinti hatası içeren bir programı düzeltebilir.
5. `%` ile bir sayının çift ya da tek olduğunu, başka bir sayıya bölünüp bölünmediğini denetleyebilir.

## Ön hazırlık

- Tarayıcıda deneme alanı: <https://kerimaksanaoglu.github.io/jus/>.
- "Ayağa kalk" etkinliği için sınıfta serbest alan; kart gerekmez, koşulları siz söylersiniz (aşağıda).
- Girinti için öğrencilere önerin: her blok içinde **4 boşluk**. Deneme alanında Tab tuşunun imleci sonraki alana taşıyıp taşımadığını dersten önce deneyin; taşıyorsa öğrenciler boşluk tuşuyla yazar.
- Hafta 2'den geçen bir hata: `değişken` unutulması. Isınmada tahtada bir tanesini hatalı bırakın.

## Ders akışı

| Süre | Evre | Ne yapılır |
|------|------|------------|
| 0-8 | Isınma | Hafta 2 izleme tablosu: üç satırlık bir programın çıktısını tahmin edin. Hatalı bir satır (`değişken` unutulmuş) bırakın ve ileti okutun. |
| 8-14 | Yeni kavram | "Evet/hayır sorusu": `5 > 3` bir sorudur; cevabı `doğru` ya da `yanlış`. Günlük hayattan karar örnekleri: "yağmur yağıyorsa şemsiye al". |
| 14-26 | Canlı kodlama 1 | Adım 1-3: karşılaştırma, `eğer`, `değilse`, girinti. |
| 26-40 | Etkinlik | "Ayağa kalk" (bilgisayarsız). |
| 40-52 | Canlı kodlama 2 | Adım 4-6: `değilse eğer` zinciri, koşul sırası, `ve` / `veya`. |
| 52-72 | Öğrenci uygulaması | Alıştırma 3.1-3.6, eşli. Hızlılar 3.7 ve 3.8'e geçer. |
| 72-76 | Paylaşım | Bir öğrenci 3.4'teki aralık sırasını tahtada açıklar. |
| 76-80 | Kapanış | Çıkış bileti. |

## Canlı kodlama betiği

### Adım 1 - Karşılaştırma bir sorudur

```jus
yaz(5 > 3)
yaz(5 < 3)
yaz(5 == 5)
yaz(5 != 5)
yaz("elma" == "elma")
yaz("elma" == "armut")
yaz(10 >= 10)
```

```
doğru
yanlış
doğru
yanlış
doğru
yanlış
doğru
```

Söyleyin: "Her karşılaştırmanın cevabı `doğru` ya da `yanlış`. Bunlar yeni bir tür: `mantıksal`." Sorun: "`==` iki tane eşittir. Peki tek eşittir ne yapıyordu?" (Atama: kutuya değer koyar. `==` ise soru sorar: 'bunlar eşit mi?'.)

Tuzak olarak ekleyin:

```jus
yaz("5" == 5)
```

```
yanlış
```

Söyleyin: "Hata vermedi ama `yanlış` dedi. Çünkü biri metin, biri sayı. Türleri farklı iki değer hiçbir zaman eşit değildir."

### Adım 2 - İlk `eğer`

```jus
değişken sıcaklık = 31

eğer sıcaklık > 30:
    yaz("Hava çok sıcak.")
yaz("Program bitti.")
```

```
Hava çok sıcak.
Program bitti.
```

Söyleyin: "Sonu iki nokta ile biten satır 'bundan sonra gelen girintili satırlar koşula bağlı' demek. Girintili olan satır yalnızca koşul doğruysa çalışır. Girintisiz satır her zaman çalışır."

Şimdi `sıcaklık` değerini `20` yapın ve çalıştırın:

```jus
değişken sıcaklık = 20

eğer sıcaklık > 30:
    yaz("Hava çok sıcak.")
yaz("Program bitti.")
```

```
Program bitti.
```

Sorun: "Bu sefer neden yalnızca son satır çıktı?"

### Adım 3 - `değilse` ve girinti

```jus
değişken sayı1 = 17

eğer sayı1 % 2 == 0:
    yaz("Çift sayı")
    yaz("İki ile tam bölünür.")
değilse:
    yaz("Tek sayı")
    yaz("İki ile bölününce 1 kalır.")
yaz("Bitti")
```

```
Tek sayı
İki ile bölününce 1 kalır.
Bitti
```

Söyleyin: "`değilse`, `eğer` ile aynı hizada başlar; ikisi aynı kararın iki yarısıdır. Her yolda iki satır var; ikisi de girintili olduğu için birlikte hareket ediyorlar." Sorun: "17'yi 18 yapsak hangi iki satır çıkar?"

Girinti hatasını göstermek için ikinci satırın girintisini bozun:

```jus
değişken sayı1 = 17

eğer sayı1 % 2 == 0:
    yaz("Çift sayı")
      yaz("İki ile tam bölünür.")
değilse:
    yaz("Tek sayı")
```

```
ornek.jus:5:7: sözdizimi hatası: Beklenmeyen girinti; bu satır bir bloğun içinde değil.
          yaz("İki ile tam bölünür.")
          ^
```

Okuyun: "5. satır, 7. sütun: beklenmeyen girinti." Aynı bloktaki satırların başlangıcı aynı sütunda olmalı.

### Adım 4 - Çok yollu karar: `değilse eğer`

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

```
İyi
```

Söyleyin: "Bilgisayar yukarıdan başlar. İlk doğru koşulun yolunu çalıştırır, kalanlara hiç bakmaz. 72 için ilk koşul yanlış, ikincisi doğru: `İyi`." (Bu puan aralıkları yalnızca örnektir; kendi okulunuzun kuralını kullanabilirsiniz.)

### Adım 5 - Koşulların sırası önemlidir

Aynı programda koşulların sırasını tersine çevirin ve çalıştırmadan önce çıktıyı sorun:

```jus
değişken puan = 90

eğer puan >= 45:
    yaz("Geçer")
değilse eğer puan >= 55:
    yaz("Orta")
değilse eğer puan >= 70:
    yaz("İyi")
değilse eğer puan >= 85:
    yaz("Pekiyi")
değilse:
    yaz("Zayıf")
```

```
Geçer
```

Sorun: "90 puan alan öğrenci neden `Geçer` aldı?" Yanıt: ilk doğru koşul çalışır, gerisine bakılmaz; `90 >= 45` ilk koşul olarak doğrudur. Kural: dar koşuldan geniş koşula doğru sıralayın. Hata iletisi çıkmadığı için bu hata öğrenci tarafından zor fark edilir; "çıktıyı beklediğiniz değerle karşılaştırma" alışkanlığını vurgulayın.

### Adım 6 - `ve`, `veya`, `değil`

```jus
değişken yaş = 15
değişken öğrenci = doğru

eğer yaş < 7:
    yaz("Ücretsiz")
değilse eğer öğrenci veya yaş >= 65:
    yaz("Yarım bilet")
değilse:
    yaz("Tam bilet")

eğer yaş >= 13 ve yaş <= 17:
    yaz("Ergenlik yaş aralığında")

eğer değil öğrenci:
    yaz("Öğrenci değil")
değilse:
    yaz("Öğrenci")
```

```
Yarım bilet
Ergenlik yaş aralığında
Öğrenci
```

Söyleyin: "`ve`: iki taraf da doğru olmalı. `veya`: en az biri doğru olsa yeter. `değil`: doğruyu yanlışa, yanlışı doğruya çevirir." Tahtaya doğruluk tablosu çizdirin:

| a | b | `a ve b` | `a veya b` |
|---|---|----------|------------|
| doğru | doğru | doğru | doğru |
| doğru | yanlış | yanlış | doğru |
| yanlış | doğru | yanlış | doğru |
| yanlış | yanlış | yanlış | yanlış |

## Sınıf içi etkinlik: Ayağa kalk (bilgisayarsız, 14 dk)

Amaç: Koşulu, hangi yolun çalıştığını ve zincirde yalnızca bir yolun çalıştığını bedenle yaşamak.

Hazırlık: Sınıfta öğrencilerin ayağa kalkacağı boş bir alan. Tahtaya her turun kodunu yazın (ya da yansıtın); öğrenciler kodu "bilgisayar" gibi okur.

| Tur | Tahtadaki kod | Öğrenciler ne yapar |
|-----|---------------|---------------------|
| 1 | `eğer gözlüğün var:` / `    ayağa kalk` | Gözlüklüler kalkar, diğerleri bekler. |
| 2 | `eğer ayakkabın bağcıklı:` / `    ayağa kalk` / `değilse:` / `    el çırp` | Herkes iki yoldan birini yapar. Kimse ikisini birden yapmaz. |
| 3 | `eğer kardeşin var ve yazın doğdun:` / `    el kaldır` | İki koşul birden doğru olanlar el kaldırır. Kaç kişi kaldırdı? `ve` yerine `veya` deyince sayı nasıl değişir? |
| 4 | `eğer doğum ayın 1-4 arası:` / `    sağa dön` / `değilse eğer doğum ayın 5-8 arası:` / `    sola dön` / `değilse:` / `    arkanı dön` | Zincir: herkes tam olarak bir yol seçer. |
| 5 | Aynı zincirin sırasını bozun: ilk koşulu `doğum ayın 1-8 arası` yapın, ikinciyi `1-4 arası` bırakın. | Kimler ikinci yola hiç giremez? Neden? (Çünkü ilk koşul zaten doğru; zincir orada durur.) |

Gözlem soruları:

- Bir turda herkes aynı şeyi yaptıysa koşul nasıl bir koşul?
- 2. turdaki `değilse` olmasaydı gözlüksüzler ne yapardı? (Hiçbir şey.)
- 5. turdaki sıra hatası, canlı kodlama Adım 5'teki puan hatasıyla aynı mı?

## Alıştırmalar

Çözümler [cevap anahtarında](cevap-anahtari.md#hafta-3).

### Kolay

**Alıştırma 3.1.** `sayı1` değişkenine `-4` değerini verin. Sayı pozitifse `Pozitif`, negatifse `Negatif`, sıfırsa `Sıfır` yazdırın. Programı `5` ve `0` ile de deneyin.

Beklenen çıktılar:

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

**Alıştırma 3.2.** `n = 17` için sayının çift mi tek mi olduğunu `17 tek sayıdır.` biçiminde yazdırın.

Beklenen çıktı:

```
17 tek sayıdır.
```

**Alıştırma 3.3.** `cevap` değişkenine `"Ankara"` yazın. Türkiye'nin başkenti sorusunun cevabı `"Ankara"` ise `Doğru!`, değilse `Yanlış.` yazdırın.

Beklenen çıktı:

```
Doğru!
```

### Orta

**Alıştırma 3.4.** `puan = 72` için Adım 4'teki zincirin aynısını, kendi yazdığınız aralıklarla kurun. Programı 50 ve 30 puanla da deneyin. (Beklenen çıktı Adım 4'ün 72 için verdiği çıktıdır.)

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

**Alıştırma 3.5.** Bilet fiyatı: 7 yaşından küçükler ücretsiz (0), öğrenciler ya da 65 yaş ve üstü yarı fiyat (50), diğerleri 100. `yaş = 16` ve `öğrenci = doğru` için fiyatı yazdırın. (İpucu: `fiyat` değişkenini önce 100 yapıp koşullarla değiştirebilirsiniz.)

Beklenen çıktı:

```
Bilet fiyatı: 50
```

**Alıştırma 3.6.** `a = 14`, `b = 29`, `c = 8` sayılarından en büyüğünü bulup yazdırın. (İpucu: `enBüyük` değişkeni `a` ile başlasın; `b` ve `c`'yi sırayla karşılaştırın.)

Beklenen çıktı:

```
En büyük: 29
```

### Zor

**Alıştırma 3.7 - Artık yıl.** Bir yıl 4'e bölünüyor **ve** (100'e bölünmüyor **veya** 400'e bölünüyor) ise artık yıldır. `yıl = 2024` için `2024 artık yıldır.` ya da `... artık yıl değildir.` yazdırın. Programı 1900, 2000 ve 2023 ile de deneyin.

Beklenen çıktılar:

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

**Alıştırma 3.8 - Üçgen.** `a = 3`, `b = 4`, `c = 5` uzunluklu üç kenar verilsin. Önce kenarların bir üçgen oluşturup oluşturmadığını denetleyin (her kenar diğer ikisinin toplamından küçük olmalı). Oluşturmuyorsa `Üçgen olmaz.` yazdırın. Oluşturuyorsa üç kenar da eşitse `Eşkenar`, ikisi eşitse `İkizkenar`, hiçbiri eşit değilse `Çeşitkenar` yazdırın. Bir de dik üçgen mi (`a*a + b*b == c*c`) denetleyin.

Beklenen çıktı:

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

## Öğrenciler nerede takılır

### 1. `==` yerine `=` yazmak

```jus
değişken x = 3
eğer x = 5:
    yaz("beş")
```

```
ornek.jus:2: çalışma zamanı hatası: Koşul mantıksal bir değer (doğru/yanlış) olmalı; sayı verildi.
```

`=` atamadır; atanan değeri (`5`) sonuç olarak verir; `5` bir sayıdır, `doğru`/`yanlış` değildir. İleti "koşul mantıksal olmalı" der; öğrenci "ben mantıksal yazdım ki" diyebilir. Yönlendirme: "Koşulda kaç eşittir var? Soru sormak mı istiyoruz, değer koymak mı?" Bir ek tuzak: bu satır `x`'in değerini de değiştirir.

### 2. İki nokta işaretini unutmak

```jus
değişken x = 3
eğer x > 2
    yaz("büyük")
```

```
ornek.jus:2:11: sözdizimi hatası: Blok başlatmak için ':' bekleniyor.
    eğer x > 2
              ^
```

İleti `^` işaretiyle koşulun bittiği yeri gösterir. Yönlendirme: "`eğer` satırının sonunda ne eksik?" Bu, bu haftanın en sık sözdizimi hatasıdır; iki noktanın "devamı girintili" demek olduğunu hatırlatın.

### 3. Girintiyi unutmak

```jus
değişken x = 3
eğer x > 2:
yaz("büyük")
```

```
ornek.jus:3:1: sözdizimi hatası: ':' işaretinden sonra girintili bir blok bekleniyor.
    yaz("büyük")
    ^
```

Öğrenci iki noktayı koymuş ama sonraki satırı içeri almamıştır. Yönlendirme: "İki nokta ne demişti? Alt satırlar nasıl yazılacaktı?"

### 4. Koşul olarak sayı yazmak

```jus
değişken puan = 80
eğer puan:
    yaz("var")
```

```
ornek.jus:2: çalışma zamanı hatası: Koşul mantıksal bir değer (doğru/yanlış) olmalı; sayı verildi.
```

Başka dillerde "sıfır olmayan sayı doğrudur" gibi kurallar vardır; JUS'ta koşul her zaman `doğru`/`yanlış` olmalıdır. Çözüm: `eğer puan > 0:`.

### 5. `veya` ile kısa yazım

Öğrenciler günlük dilde "yaş 13 ya da 14 ise" derken `yaş == 13 veya 14` yazar:

```jus
değişken yaş = 15
eğer yaş == 13 veya 14:
    yaz("13 ya da 14")
```

```
ornek.jus:2: çalışma zamanı hatası: 've' / 'veya' mantıksal değerler ister; sayı verildi.
```

İleti sağdaki `14`'ün mantıksal olmadığını söyler. Ama dikkat: `yaş` 13 olduğunda sol taraf zaten `doğru` olduğu için sağ taraf hiç hesaplanmaz ve program hata vermeden çalışır. Aynı yanlış yazım bazen çalışıp bazen hata verir:

```jus
değişken yaş = 13
eğer yaş == 13 veya 14:
    yaz("13 ya da 14")
```

```
13 ya da 14
```

Çözüm: iki ayrı karşılaştırma: `yaş == 13 veya yaş == 14`. Aynı tuzak `1 < x < 10` yazımında da vardır; doğrusu `x > 1 ve x < 10`.

## Çıkış bileti

1. `yaz(10 > 3)` ne yazar? `yaz(10 == 3)` ne yazar?
2. Aşağıdaki programda `sayı1` 6 ise ekrana ne yazılır?
3. `eğer x = 5:` yazmak neden yanlıştır?

Soru 2'nin programı:

```jus
değişken sayı1 = 6
eğer sayı1 > 5:
    yaz("A")
değilse eğer sayı1 > 3:
    yaz("B")
değilse:
    yaz("C")
```

```
A
```

Cevaplar:

1. `doğru` ve `yanlış`.
2. Yalnızca `A`. Zincirde ilk doğru koşulun yolu çalışır, `sayı1 > 3` de doğru olsa bile ona bakılmaz.
3. Tek `=` atamadır; koşulun `doğru`/`yanlış` olması gerekir. Karşılaştırma için `==` yazılır.

## Ev çalışması (isteğe bağlı)

1. Bir hava sıcaklığı değişkenine göre ne giyileceğini söyleyen bir karar zinciri yazın (en az üç yol). Üç farklı sıcaklıkla deneyin.
2. Kendi uydurduğunuz bir giriş kuralını (ör. bir oyuna katılmak için yaş ve boy koşulu) `ve`/`veya` kullanarak yazın. Kuralı bir cümleyle de açıklayın ve sınır değerlerle (tam eşitlik durumuyla) deneyin.
3. Sınıf etkinliğindeki 5. turu bir program olarak yazın: `ay` değişkeni verilsin, yanlış sıralanmış zincirin hangi ayları yanlış sınıflandırdığını bulun.
