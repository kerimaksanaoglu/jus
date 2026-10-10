# Dönem projesi

[Öğretmen kiti](README.md)

Dönem projesi, öğrencilerin 9 hafta boyunca ayrı ayrı öğrendikleri parçaları kendi seçtikleri küçük bir programda birleştirmesidir. Proje, sınavdan farklı olarak öğrencinin bir fikri baştan sona götürmesini ister: seçmek, planlamak, yazmak, denemek, hata ayıklamak ve anlatmak. Bu belge proje fikirlerini, takvimi, teslim listesini ve değerlendirme ölçeğini içerir.

## Genel çerçeve

- **Kim:** Bireysel ya da ikili. İkili çalışmada iki öğrenci aynı ekranda dönüşümlü yazar; teslimde her ikisi de programı anlatabilmelidir.
- **Boyut:** 50-150 satır. Çok uzun projeler çoğu zaman daha iyi değil, daha dağınıktır.
- **Ortam:** Tarayıcıdaki deneme alanı yeterlidir. `oku`, `dosya`, `ağ` ve `http` orada çalışmadığı için, projelerin asgari sürümleri **girdi istemeyen** biçimde tasarlanmıştır (veri program içinde verilir; ya da kendi kendine oynayan bir "bot" kullanılır). Bilgisayarlara JUS kurulu sınıflarda aynı fikirler `oku` ile etkileşimli hâle getirilebilir; bu, genişletilmiş sürüm olarak sunulmuştur.
- **Onay:** Öğrenci projesine başlamadan önce kısa bir **tasarım kâğıdı** yazar (ne yapacak, hangi veriler, hangi çıktı) ve siz onaylarsınız. Kapsamı çok geniş seçenleri asgari sürüme yönlendirin.
- **Kopyalama ve yardım:** Rehberden, kitten ya da arkadaştan fikir almak serbesttir; ama öğrenci her satırı açıklayabilmelidir. Yardım aldığı yeri kod başındaki yorumda belirtir.

Aşağıdaki örnek çözümler **öğretmen içindir**: asgari sürümün ne kadar büyük olması gerektiğini göstermek ve beklenen çıktıyı bilmeniz için yazılmıştır. Öğrencilere vermeyin; verirseniz yalnızca bir "bu seviyede beklenir" göstergesi olarak, çalışırken değil, teslimden sonra gösterin.

## Proje fikirleri

Aşağıdaki tabloda her projenin dayandığı hafta konuları özetlenmiştir.

| # | Proje | Ağırlıklı konular | Hafta |
|---|-------|--------------------|-------|
| 1 | Karne ve not hesaplayıcı | sözlük listesi, fonksiyon, döngü, karar, hizalama | 3, 4, 5, 6, 7, 8 |
| 2 | Metin analizcisi | metin işlemleri, sözlük, liste | 4, 6, 7, 8 |
| 3 | Zar ve şans simülasyonu | `rastgele`, döngü, sözlük | 4, 8, 9 |
| 4 | Tahmin eden bilgisayar (ve tahmin oyunu) | döngü, karar, fonksiyon, `rastgele`, hata yakalama | 3, 4, 5, 9 |
| 5 | Alışveriş sepeti ve fiş | sözlük, liste, hizalama, `biçimle` | 2, 6, 7, 8 |
| 6 | Şifre makinesi | metin, fonksiyon, `%`, döngü | 2, 4, 5, 7 |
| 7 | Kütüphane kayıt sistemi | sözlük listesi, arama, sıralama, hata yakalama | 6, 8, 9 |
| 8 | Mini sınav (quiz) | liste, sözlük, metin karşılaştırma, döngü | 3, 4, 6, 7, 8 |

### Proje 1 - Karne ve not hesaplayıcı

**Kapsam:** Birkaç öğrencinin ders notlarından ortalama, harf notu ve sınıf ortalaması hesaplayan, sonucu hizalı bir tablo olarak yazdıran program.

**Gereken konular:** Sözlük listesi (Hafta 8), `ortalama` ve `harfNotu` fonksiyonları (Hafta 5), döngü ve toplama (Hafta 4), karar zinciri (Hafta 3), `sağa_doldur`, `sola_doldur`, `biçimle` (Hafta 2 ve 7).

**Asgari sürüm:**

- En az 4 öğrenci, her birinin 3 notu; veriler programın içinde.
- Her öğrenci için ortalama ve harf notu.
- Hizalı tablo ve sınıf ortalaması.

**Genişletilmiş sürüm:**

- Ortalamaya göre sıralı liste (`sırala` ile anahtar fonksiyonu).
- En yüksek ve en düşük ortalamalı öğrenci.
- Notu geçersiz (negatif ya da 100'den büyük) girişi `fırlat` ile reddetme.
- `*` karakterleriyle ortalamaların çubuk grafiği (`tekrarla`).
- Kurulu bilgisayarlarda notları `oku` ile alma.

**Öğretmen için örnek asgari çözüm:** (Harf aralıkları bu örnek için uydurulmuştur; okulunuzun ölçeğini kullanın.)

```jus
değişken öğrenciler = [
    {"ad": "Ayşe", "notlar": [90, 85, 70]},
    {"ad": "Mehmet", "notlar": [35, 60, 40]},
    {"ad": "Zeynep", "notlar": [100, 95, 80]},
    {"ad": "Can", "notlar": [60, 75, 55]},
]

fonksiyon ortalama(liste):
    değişken toplam = 0
    her n içinde liste:
        toplam += n
    dön toplam / uzunluk(liste)

fonksiyon harfNotu(ort):
    eğer ort >= 85:
        dön "A"
    değilse eğer ort >= 70:
        dön "B"
    değilse eğer ort >= 50:
        dön "C"
    dön "F"

yaz(sağa_doldur("Ad", 10) + sola_doldur("Ort.", 7) + "  Harf")
yaz(tekrarla("-", 24))

değişken sınıfToplamı = 0
her ö içinde öğrenciler:
    değişken ort = ortalama(ö["notlar"])
    sınıfToplamı += ort
    yaz(sağa_doldur(ö["ad"], 10) + sola_doldur(biçimle(ort, 1), 7) + "  " + harfNotu(ort))

yaz(tekrarla("-", 24))
yaz("Sınıf ortalaması:", biçimle(sınıfToplamı / uzunluk(öğrenciler), 1))
```

```
Ad           Ort.  Harf
------------------------
Ayşe         81.7  B
Mehmet       45.0  F
Zeynep       91.7  A
Can          63.3  C
------------------------
Sınıf ortalaması: 70.4
```

### Proje 2 - Metin analizcisi

**Kapsam:** Programın içinde verilen bir paragrafın kelime sayısı, farklı kelime sayısı, en uzun kelimesi ve en sık geçen kelimesi gibi istatistiklerini çıkaran program.

**Gereken konular:** `küçük_harf`, `değiştir`, `böl` (Hafta 7); sözlükle sayma (Hafta 8); liste ve döngü (Hafta 4, 6).

**Asgari sürüm:**

- Noktalama temizleme ve küçük harfe çevirme.
- Kelime sayısı, en uzun kelime, en sık kelime.

**Genişletilmiş sürüm:**

- Harf sıklığı tablosu (en sık 5 harf).
- Sesli/sessiz harf oranı (Türkçe sesli harfler).
- Palindrom kelimeleri bulma.
- Cümle sayısı ve ortalama cümle uzunluğu (`böl(yazı, ".")`).

**Öğretmen için örnek asgari çözüm:**

```jus
değişken yazı = "Kitap okumak insanı geliştirir. Okuyan insan düşünür, düşünen insan üretir."

değişken temiz = küçük_harf(yazı)
her işaret içinde [".", ",", "!", "?"]:
    temiz = değiştir(temiz, işaret, "")

değişken kelimeler = böl(temiz, " ")
değişken sayım = {}
değişken enUzun = ""
her k içinde kelimeler:
    sayım[k] = al(sayım, k, 0) + 1
    eğer uzunluk(k) > uzunluk(enUzun):
        enUzun = k

değişken enSık = ""
değişken enSıkAdet = 0
her k içinde anahtarlar(sayım):
    eğer sayım[k] > enSıkAdet:
        enSık = k
        enSıkAdet = sayım[k]

yaz("Kelime sayısı:", uzunluk(kelimeler))
yaz("Farklı kelime:", uzunluk(sayım))
yaz("En uzun kelime:", enUzun)
yaz("En sık kelime:", enSık, "(" + metin(enSıkAdet) + " kez)")
```

```
Kelime sayısı: 10
Farklı kelime: 9
En uzun kelime: geliştirir
En sık kelime: insan (2 kez)
```

### Proje 3 - Zar ve şans simülasyonu

**Kapsam:** Bir zarı çok kez atıp her yüzün kaç kez geldiğini sayan ve sonucu çubuk grafikle gösteren program. Öğrenciler "adil zar eşit dağılım verir mi?" sorusunu denemeyle görür.

**Gereken konular:** `kullan rastgele`, `rastgele.tam` (Hafta 9), döngü (Hafta 4), sözlük (Hafta 8), `tekrarla` (Hafta 7).

**Asgari sürüm:**

- 600 atış, her yüzün sayısı.
- Her sayı için çubuk grafik (10 atış = 1 yıldız).

**Genişletilmiş sürüm:**

- İki zarın toplamının dağılımı (2'den 12'ye).
- Atış sayısını 60, 600 ve 6000 yapıp farkı karşılaştırma: sayı arttıkça dağılım eşitliğe yaklaşıyor mu?
- En uzun art arda aynı sayı serisini bulma.
- Yazı-tura serisi simülasyonu.

**Öğretmen için örnek asgari çözüm:** `tohum` her çalıştırmada aynı çıktıyı almak içindir; çıktının sizin ekranınızda aynı olması beklenir.

```jus
kullan rastgele

rastgele.tohum(2024)

değişken sayım = {}
her yüz içinde aralık(1, 7):
    sayım[yüz] = 0

her atış içinde aralık(600):
    değişken zar = rastgele.tam(1, 6)
    sayım[zar] += 1

her yüz içinde aralık(1, 7):
    yaz(yüz, ":", sağa_doldur(tekrarla("*", taban(sayım[yüz] / 10)), 14), sayım[yüz])
```

```
1 : **********     105
2 : **********     106
3 : *********      91
4 : **********     102
5 : ********       81
6 : ***********    115
```

### Proje 4 - Tahmin eden bilgisayar (ve tahmin oyunu)

**Kapsam:** Bilgisayarın 1-100 arasındaki gizli bir sayıyı en az adımda bulmaya çalıştığı program. Öğrenci stratejiyi kâğıtta keşfeder (Hafta 9 etkinliği), sonra koda döker.

**Gereken konular:** Döngü ve karar (Hafta 3-4), fonksiyon (Hafta 5), `rastgele` (Hafta 9). Girdili sürüm için `oku` ve `dene` / `yakala` (Hafta 9).

**Asgari sürüm:**

- `adımSayısı(gizli)` fonksiyonu: ortayı deneyerek gizli sayıyı bulan ve adım sayısını döndüren.
- 1'den 100'e kadar her gizli sayı için adım sayısı; ortalama ve en çok adım.

**Genişletilmiş sürüm:**

- Alternatif strateji (hep 1'den başlayıp sırayla denemek) ile karşılaştırma: hangisi ortalama kaç adımda buluyor?
- Kurulu bilgisayarlarda insan oyuncu: bilgisayar sayı tutar, oyuncu `oku` ile tahmin eder, hak sınırı vardır (bkz. [Hafta 9, Adım 8](hafta-09.md)).
- Aralığı ve hak sayısını değiştirilebilir yapma.

**Öğretmen için örnek asgari çözüm:**

```jus
fonksiyon adımSayısı(gizli):
    değişken düşük = 1
    değişken yüksek = 100
    değişken adım = 0
    iken doğru:
        değişken tahmin = taban((düşük + yüksek) / 2)
        adım += 1
        eğer tahmin == gizli:
            dön adım
        değilse eğer tahmin < gizli:
            düşük = tahmin + 1
        değilse:
            yüksek = tahmin - 1

değişken toplam = 0
değişken enÇok = 0
her g içinde aralık(1, 101):
    değişken a = adımSayısı(g)
    toplam += a
    eğer a > enÇok:
        enÇok = a

yaz("Ortalama adım:", biçimle(toplam / 100, 2))
yaz("En çok adım:", enÇok)
```

```
Ortalama adım: 5.80
En çok adım: 7
```

### Proje 5 - Alışveriş sepeti ve fiş

**Kapsam:** Fiyat listesinden ve sepetten fiş hazırlayan program: ürün satırları, ara toplam, indirim ve genel toplam.

**Gereken konular:** Sözlük ve sözlük listesi (Hafta 8), `sağa_doldur` / `sola_doldur` / `biçimle` / `tekrarla` (Hafta 7), fonksiyon (Hafta 5).

**Asgari sürüm:**

- En az 4 ürünlük fiyat sözlüğü, 3 satırlık sepet.
- Hizalı fiş: ürün, adet, birim fiyat, satır tutarı; genel toplam.
- Para tutarları iki ondalık basamakla.

**Genişletilmiş sürüm:**

- Belirli bir tutarın üzerinde yüzde indirim.
- Sepette olmayan ya da fiyatı olmayan ürün için `fırlat` ile anlamlı hata.
- Satırları tutara göre sıralı yazdırma.
- Para biçimi için `tr` modülü: `kullan tr`, `tr.para(tutar)` (aşağıdaki örneğe bakın).

**Öğretmen için örnek asgari çözüm:**

```jus
değişken fiyatlar = {"ekmek": 7.5, "süt": 32, "peynir": 125.75, "zeytin": 89.9}
değişken sepet = [
    {"ürün": "ekmek", "adet": 2},
    {"ürün": "süt", "adet": 1},
    {"ürün": "peynir", "adet": 2},
]

yaz(sağa_doldur("Ürün", 10) + sola_doldur("Adet", 5) + sola_doldur("Fiyat", 9) + sola_doldur("Tutar", 10))
yaz(tekrarla("-", 34))

değişken toplam = 0
her satır içinde sepet:
    değişken ad = satır["ürün"]
    değişken tutar = fiyatlar[ad] * satır["adet"]
    toplam += tutar
    yaz(sağa_doldur(ad, 10) + sola_doldur(metin(satır["adet"]), 5) + sola_doldur(biçimle(fiyatlar[ad], 2), 9) + sola_doldur(biçimle(tutar, 2), 10))

yaz(tekrarla("-", 34))
yaz(sağa_doldur("TOPLAM", 24) + sola_doldur(biçimle(toplam, 2), 10))
```

```
Ürün       Adet    Fiyat     Tutar
----------------------------------
ekmek         2     7.50     15.00
süt           1    32.00     32.00
peynir        2   125.75    251.50
----------------------------------
TOPLAM                      298.50
```

`tr` modülüyle genişletme (isteğe bağlı) şöyle görünür:

```jus
kullan tr

yaz(tr.para(298.5))
yaz(tr.para(1234.5))
```

```
298,50 ₺
1.234,50 ₺
```

### Proje 6 - Şifre makinesi

**Kapsam:** Sezar şifresiyle metin şifreleyen ve çözen program. Hafta 7'deki etkinlik ve 7.9 alıştırması başlangıç noktasıdır.

**Gereken konular:** Metin gezme, `bul`, `%` (Hafta 2, 7), fonksiyon (Hafta 5).

**Asgari sürüm:**

- `şifrele(metin, anahtar)` ve `çöz(metin, anahtar)` fonksiyonları.
- Bir cümleyi şifreleyip çözerek aynı cümlenin geri geldiğini gösteren demo.

**Genişletilmiş sürüm:**

- Şifreli bir metni anahtar bilmeden çözme: 28 anahtarın hepsini deneyip sonuçları yazdırma ("kaba kuvvet").
- Büyük harfleri koruma.
- Her harfin farklı kaydırma kullandığı (anahtar sözcüklü) şifre.
- Harf sıklığı ile anahtarı tahmin etme (Proje 2 ile birleştirme).

**Öğretmen için örnek asgari çözüm:**

```jus
değişken alfabe = "abcçdefgğhıijklmnoöprsştuüvyz"

fonksiyon kaydır(m, adet):
    değişken sonuç = ""
    her harf içinde m:
        değişken yer = bul(alfabe, harf)
        eğer yer == -1:
            sonuç = sonuç + harf
        değilse:
            sonuç = sonuç + alfabe[(yer + adet) % 29]
    dön sonuç

fonksiyon şifrele(m, anahtar):
    dön kaydır(m, anahtar)

fonksiyon çöz(m, anahtar):
    dön kaydır(m, 29 - anahtar)

değişken gizli = şifrele("buluşma saat beşte", 5)
yaz(gizli)
yaz(çöz(gizli, 5))
```

```
fapayre veez fıyzı
buluşma saat beşte
```

### Proje 7 - Kütüphane kayıt sistemi

**Kapsam:** Küçük bir kitap listesini yöneten program: listeleme, arama, ödünç verme.

**Gereken konular:** Sözlük listesi, sıralama (Hafta 8), arama döngüsü (Hafta 6), `fırlat` / `yakala` (Hafta 9), fonksiyon (Hafta 5).

**Asgari sürüm:**

- En az 5 kitap; her kitapta `ad`, `yazar`, `ödünçte` alanları.
- Kitapları ada göre sıralı listeleyen fonksiyon.
- Ada göre arayıp bulunan kitabı döndüren fonksiyon; bulunamazsa anlamlı bir ileti.
- Ödünç verme: kitap zaten ödünçteyse reddeden fonksiyon.

**Genişletilmiş sürüm:**

- İade etme ve ödünçteki kitapların listesi.
- Yazara göre arama (küçük/büyük harf farkı gözetmeden).
- Üye sistemi: her kitabın kimde olduğu (`ödünçte` yerine üye adı).
- Kayıtları `json` modülüyle dosyaya yazma (kurulu bilgisayarlarda, [rehber 13](../rehber/13-json-ve-veri.md)).

**Öğretmen için örnek asgari çözüm:**

```jus
değişken kitaplar = [
    {"ad": "Küçük Prens", "yazar": "Antoine de Saint-Exupéry", "ödünçte": yanlış},
    {"ad": "Çalıkuşu", "yazar": "Reşat Nuri Güntekin", "ödünçte": yanlış},
    {"ad": "Kuyucaklı Yusuf", "yazar": "Sabahattin Ali", "ödünçte": yanlış},
    {"ad": "Simyacı", "yazar": "Paulo Coelho", "ödünçte": yanlış},
    {"ad": "Sefiller", "yazar": "Victor Hugo", "ödünçte": yanlış},
]

fonksiyon adaGöre(kitap):
    dön kitap["ad"]

fonksiyon listele():
    her k içinde sırala(kitaplar, adaGöre):
        değişken durum = "rafta"
        eğer k["ödünçte"]:
            durum = "ödünçte"
        yaz(sağa_doldur(k["ad"], 18), durum)

fonksiyon bul_kitap(ad):
    her k içinde kitaplar:
        eğer k["ad"] == ad:
            dön k
    fırlat "Kitap bulunamadı: " + ad

fonksiyon ödünçVer(ad):
    değişken kitap = bul_kitap(ad)
    eğer kitap["ödünçte"]:
        fırlat ad + " zaten ödünçte."
    kitap["ödünçte"] = doğru
    yaz(ad, "ödünç verildi.")

ödünçVer("Simyacı")
her istek içinde ["Simyacı", "Dönüşüm", "Sefiller"]:
    dene:
        ödünçVer(istek)
    yakala hata:
        yaz("Hata:", hata)

listele()
```

```
Simyacı ödünç verildi.
Hata: Simyacı zaten ödünçte.
Hata: Kitap bulunamadı: Dönüşüm
Sefiller ödünç verildi.
Çalıkuşu           rafta
Kuyucaklı Yusuf    rafta
Küçük Prens        rafta
Sefiller           ödünçte
Simyacı            ödünçte
```

### Proje 8 - Mini sınav (quiz)

**Kapsam:** Soru-cevap listesinden sınav yapan, puan hesaplayıp geri bildirim veren program. Deneme alanında öğrenci cevapları programın içinde bir liste olarak verilir (bir "sanal öğrenci"); kurulu bilgisayarlarda cevaplar `oku` ile alınır.

**Gereken konular:** Sözlük listesi (Hafta 8), `küçük_harf` ve `kırp` ile karşılaştırma (Hafta 7), döngü (Hafta 4), `eğer` (Hafta 3).

**Asgari sürüm:**

- En az 4 soru; her soru `soru` ve `cevap` alanlı sözlük.
- Her soru için öğrencinin cevabı, doğru mu yanlış mı, ve son puan.
- Büyük-küçük harf ve baştaki/sondaki boşluk farkı cevabı bozmamalı.

**Genişletilmiş sürüm:**

- Soruları `rastgele.karıştır` ile karışık sırayla sorma.
- Yanlış cevapta doğru cevabı gösterme; son puana göre geri bildirim zinciri.
- Birden çok kabul edilen cevap (cevap bir liste olur).
- Kurulu bilgisayarlarda `oku` ile gerçek sınav.

**Öğretmen için örnek asgari çözüm:**

```jus
değişken sorular = [
    {"soru": "Türkiye'nin başkenti neresidir?", "cevap": "ankara"},
    {"soru": "7 x 8 kaçtır?", "cevap": "56"},
    {"soru": "Bir yılda kaç ay vardır?", "cevap": "12"},
    {"soru": "Suyun donma sıcaklığı kaç derecedir?", "cevap": "0"},
]

değişken öğrenciCevapları = ["  Ankara ", "54", "12", "0"]
değişken puan = 0

her i içinde aralık(uzunluk(sorular)):
    değişken cevap = küçük_harf(kırp(öğrenciCevapları[i]))
    yaz("Soru", i + 1, "-", sorular[i]["soru"])
    eğer cevap == sorular[i]["cevap"]:
        puan += 1
        yaz("  Doğru")
    değilse:
        yaz("  Yanlış. Doğru cevap:", sorular[i]["cevap"])

yaz("Puanınız:", puan, "/", uzunluk(sorular))
```

```
Soru 1 - Türkiye'nin başkenti neresidir?
  Doğru
Soru 2 - 7 x 8 kaçtır?
  Yanlış. Doğru cevap: 56
Soru 3 - Bir yılda kaç ay vardır?
  Doğru
Soru 4 - Suyun donma sıcaklığı kaç derecedir?
  Doğru
Puanınız: 3 / 4
```

## Proje takvimi

Aşağıdaki takvim, haftada iki ders saatinin 10 haftalık dönemi içindir. Her hafta proje için yaklaşık 15-30 dakika ayırın; kalan süre ev çalışmasıdır.

| Hafta | Proje ile ilgili iş | Öğrenci çıktısı |
|-------|---------------------|-----------------|
| 6-7 | Fikir tohumları: dersin sonunda 5 dakika "bu konuyla ne yapılabilir?" konuşması. | Ev çalışması: iki proje fikri (bu belgeden ya da kendinden). |
| 8 | Proje seçimi ve tasarım kâğıdı (ilk 15 dakika). Veri nerede? Hangi fonksiyonlar? Asgari sürüm maddeleri neler? Öğretmen onayı. | Onaylı tasarım kâğıdı. |
| 9 | İlk çalışan sürüm (asgari sürümün yarısı). Öğretmen ara kontrolü (her çift 2 dakika). | En az 30 satırlık çalışan kod; test tablosu başlangıcı. |
| 10 | Bitirme, test, akran kod gezisi, sunum. | Teslim listesi tamam; sunum yapılmış. |

Daha kısa dönemlerde (ör. 6 hafta) projeyi "mini projeye" dönüştürün: yalnızca Proje 1, 2 ya da 8'in asgari sürümü, 2 ders saatinde. Daha uzun dönemlerde (ör. 14 hafta) projeyi iki aşamaya bölün: asgari sürüm ara teslim, genişletilmiş sürüm final.

### Haftalık ara kontrol soruları

- Programın bugün çalışan bir parçası var mı? Göster.
- Son yaptığın değişiklik ne? Neden?
- Takıldığın yer nerede? Hata iletisi ne diyor?
- Gelecek hafta bitirmek için hangi üç iş kaldı?

## Öğrenci teslim listesi

Öğrenciye bu listeyi proje seçiminde verin; teslimde işaretleyip getirsin.

- [ ] Program tek dosya hâlinde çalışıyor (`.jus`). Dosyanın en başında yorum satırlarında: proje adı, yazar(lar), programın ne yaptığı (3-5 satır).
- [ ] Programı baştan sona çalıştırdığımda hata iletisi çıkmıyor (ya da bilerek yakaladığım hatalar var).
- [ ] En az iki fonksiyon yazdım ve anlamlı adlar verdim.
- [ ] Kodda en az bir kez döngü, karar ve liste ya da sözlük kullandım.
- [ ] Bilerek yanlış ya da uç bir girdi düşündüm (boş liste, sıfır, olmayan anahtar) ve ne olacağına karar verdim.
- [ ] En az üç farklı veriyle denedim; test tablosu yazdım (veri / beklenen çıktı / gerçek çıktı).
- [ ] Programın bir çıktı örneğini defterime ya da dosyama kopyaladım.
- [ ] Sunum notum hazır (5 cümle: ne yapıyor, nasıl çalışıyor, zorlandığım yer, öğrendiğim şey, bir sonraki adım).
- [ ] Yardım aldığım yerleri (arkadaş, öğretmen, rehber) kodun başında belirttim.

### Test tablosu örneği

| Veri | Beklenen çıktı | Gerçek çıktı | Sorun? |
|------|----------------|---------------|--------|
| `[4, 8, 15]` | ortalama 9 | ortalama 9 | yok |
| `[]` | `Not yok` | sıfıra bölünemez hatası | var: boş liste denetlenmeli |

## Değerlendirme ölçeği

Her ölçüt 1-4 arasında puanlanır. Ağırlıklar öneridir; sınıfınızın önceliğine göre değiştirebilirsiniz. Puanı okul ölçeğinize çevirme sorumluluğu size aittir; aşağıdaki sayılar yalnızca ölçek içi karşılaştırma içindir.

| Ölçüt (ağırlık) | 1 - Başlangıç | 2 - Gelişiyor | 3 - Yeterli | 4 - İleri |
|------------------|---------------|---------------|-------------|-----------|
| **Çalışırlık ve doğruluk** (x3) | Program çalışmıyor ya da yalnızca birkaç satırı çalışıyor. | Program çalışıyor ama çıktıların bir kısmı beklenenden farklı. | Program asgari sürümü hatasız ve doğru çıktıyla çalışıyor. | Genişletilmiş sürümden en az bir özellik de doğru çalışıyor. |
| **Konu kullanımı** (x2) | Öğrenilen konulardan yalnızca biri ya da ikisi kullanılmış. | Dönem konularının yarısı kadarı kullanılmış, bazıları gereksiz ya da yanlış yerde. | Karar, döngü, fonksiyon ve liste/sözlük uygun yerlerde kullanılmış. | Konular yerinde ve öğrencinin kendi kararıyla seçilmiş; sözlük listesi, hata yakalama gibi ileri bir birleşim var. |
| **Kod okunabilirliği** (x2) | Adlar `x`, `a1` gibi anlamsız; yorum ve boşluk yok; kodu başkası okuyamıyor. | Bazı adlar anlamlı; yorum çok az; aynı kod tekrar tekrar kopyalanmış. | Adlar anlamlı, girinti düzgün, başında açıklama var; tekrar eden kod fonksiyona alınmış. | Kod bölümlere ayrılmış, yorumlar "ne" değil "neden"i anlatıyor; başkasının anlaması için ek açıklama gerekmiyor. |
| **Hata ve uç durumlar** (x1) | Hatalı ya da uç girdi düşünülmemiş. | Bir uç durum düşünülmüş ama program hâlâ kolayca çöküyor. | Bilinen uç durumlar (boş liste, sıfır, olmayan anahtar) `eğer` ya da `dene`/`yakala` ile ele alınmış. | `fırlat` ile anlamlı hatalar üretilmiş, hata iletileri kullanıcıya yardımcı. |
| **Test ve hata ayıklama** (x1) | Test kanıtı yok. | Bir ya da iki deneme yapılmış, kayıt yok. | Test tablosu var; en az üç farklı veriyle denenmiş. | Test tablosunda bulunan bir hatanın nasıl düzeltildiği anlatılıyor. |
| **Sunum ve anlatım** (x1) | Programın ne yaptığını anlatamıyor. | Ne yaptığını anlatıyor ama nasıl çalıştığını açıklamakta zorlanıyor. | Hem ne yaptığını hem nasıl çalıştığını açık anlatıyor; sorulara cevap veriyor. | Kod parçasını kendi cümleleriyle açıklıyor, zorlandığı yeri ve çözümünü anlatıyor, soruya ek bilgiyle cevap veriyor. |

**Toplam puan:** (ölçüt puanı x ağırlık) toplamıdır. Ağırlıklar toplamı 10 olduğu için en düşük toplam 10, en yüksek 40'tır.

| Toplam | Genel düzey (öneri) |
|--------|---------------------|
| 10-19 | Başlangıç: projeyi yeniden gözden geçirme ve kısa bir ek görüşme gerekir. |
| 20-27 | Gelişiyor: temel kavramlar yerinde, bir ya da iki alanda destek gerekir. |
| 28-35 | Yeterli: beklenen düzeyi karşılıyor. |
| 36-40 | İleri: dönem hedeflerinin üzerinde. |

**İkili çalışmada** ek olarak "iş birliği" ölçütü (1-4) kullanabilirsiniz: iki öğrenci de kodun her bölümünü açıklayabiliyor mu? Rolleri dönüşümlü mü yaptılar? Bu ölçütü bireysel puan olarak değil, ikiliye birlikte verin.

### Geri bildirim için kısa kalıp

Öğrenciye puan yanında iki cümle yazın: "Programında en iyi yaptığın şey ... Bir sonraki projede şunu dene: ...". Puan tablosu tek başına öğrenciye ne yapacağını söylemez.
