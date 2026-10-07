# 12 - Küçük projeler

Önceki: [11 - Sınıflar](11-siniflar.md) | [İçindekiler](README.md) | Sonraki: [13 - JSON ve veri](13-json-ve-veri.md)

Artık gerekli parçaların hepsini biliyorsunuz. Bu bölümde üç tam program yazacağız. Her programı önce kendiniz yazmayı deneyin; takılırsanız yanındaki açıklamaya bakın. Programlar, önceki bölümlerde öğrendiklerinizi bir arada kullanır.

Programların her satırı yeni bir şey öğretmek için değil, önceki bilgileri birleştirmek için yazıldı. Anlamadığınız bir satır çıkarsa ilgili bölüme dönün.

## Proje 1: Sayı tahmin oyunu

Bilgisayar 1 ile 100 arasında bir sayı tutar. Siz yedi hakta sayıyı bulmaya çalışırsınız. Her tahminden sonra bilgisayar "daha büyük" ya da "daha küçük" der.

Kullanılan konular: `rastgele` modülü, `iken`, `oku`, `dene` / `yakala`, `kır`, `devam`.

`tahmin.jus`:

```jus
kullan rastgele

değişken gizli = rastgele.tam(1, 100)
değişken hak = 7
değişken tahminSayısı = 0
değişken bulundu = yanlış

yaz("1 ile 100 arasında bir sayı tuttum. " + metin(hak) + " hakkınız var.")

iken tahminSayısı < hak:
    değişken girdi = oku("Tahmininiz: ")
    eğer girdi == boş:
        kır

    değişken tahmin = 0
    dene:
        tahmin = sayı(girdi)
    yakala:
        yaz("Lütfen bir sayı yazın.")
        devam

    tahminSayısı += 1
    eğer tahmin < gizli:
        yaz("Daha büyük bir sayı deneyin.")
    değilse eğer tahmin > gizli:
        yaz("Daha küçük bir sayı deneyin.")
    değilse:
        bulundu = doğru
        kır

eğer bulundu:
    yaz("Tebrikler! " + metin(tahminSayısı) + " tahminde buldunuz.")
değilse eğer tahminSayısı >= hak:
    yaz("Hakkınız bitti. Tuttuğum sayı " + metin(gizli) + " idi.")
değilse:
    yaz("Oyun yarıda kaldı.")
```

### Nasıl çalışır?

- `rastgele.tam(1, 100)` her çalıştırmada farklı bir gizli sayı üretir.
- Ana döngü, hak bitene kadar (`tahminSayısı < hak`) döner.
- `oku` girdi bittiğinde `boş` verir. Bu durumda oyunu `kır` ile bırakırız.
- `sayı(girdi)` kullanıcı sayı olmayan bir şey yazdığında hata verir. `dene` ile bunu yakalarız. Hatalı girdi hak yemez: `devam` döngünün başına döner ve `tahminSayısı` artmaz.
- Doğru tahminde `bulundu` işaretlenir ve `kır` ile döngüden çıkılır.
- Döngüden sonra üç durum ayrılır: bulundu, hak bitti ya da oyun yarıda kaldı.
- `değişken tahmin = 0` satırını döngünün içinde, `dene`'den önce yazdık. Çünkü `dene` bloğunda tanımlanan bir değişken yalnızca o blokta geçerli olurdu.

### Örnek oturumlar

Çıktıyı önceden bilebilmek için bu denemelerde programın başına, `kullan rastgele` satırından sonra `rastgele.tohum(42)` satırı ekledik. (Bu yöntem, aynı tohumla aynı sayıyı üretir. Denememizde gizli sayı 45 çıktı.) Siz normal programı çalıştırınca sayı her seferinde farklı olacaktır.

Girdileri `printf '50\nabc\n20\n45\n' | jus tahmin.jus` gibi bir komutla verdiğimiz için yazdıklarınız çıktıda görünmez; terminalde kendiniz yazarsanız her tahmin istemin yanında görünür.

Dört girdi (`50`, `abc`, `20`, `45`) verdiğimizde:

```
1 ile 100 arasında bir sayı tuttum. 7 hakkınız var.
Tahmininiz: Daha küçük bir sayı deneyin.
Tahmininiz: Lütfen bir sayı yazın.
Tahmininiz: Daha büyük bir sayı deneyin.
Tahmininiz: Tebrikler! 3 tahminde buldunuz.
```

`abc` hak yemediği için "3 tahminde" buldu: 50, 20 ve 45.

Yedi yanlış tahminle (`10`, `90`, `60`, `30`, `40`, `44`, `46`):

```
1 ile 100 arasında bir sayı tuttum. 7 hakkınız var.
Tahmininiz: Daha büyük bir sayı deneyin.
Tahmininiz: Daha küçük bir sayı deneyin.
Tahmininiz: Daha küçük bir sayı deneyin.
Tahmininiz: Daha büyük bir sayı deneyin.
Tahmininiz: Daha büyük bir sayı deneyin.
Tahmininiz: Daha büyük bir sayı deneyin.
Tahmininiz: Daha küçük bir sayı deneyin.
Hakkınız bitti. Tuttuğum sayı 45 idi.
```

Girdi iki tahminden sonra biterse (`90`, `20`):

```
1 ile 100 arasında bir sayı tuttum. 7 hakkınız var.
Tahmininiz: Daha küçük bir sayı deneyin.
Tahmininiz: Daha büyük bir sayı deneyin.
Tahmininiz: Oyun yarıda kaldı.
```

### Genişletme fikirleri

- Hak sayısını ve sayı aralığını değişkenlere bağlayıp zorluk seçtirin.
- Oyun bitince "Tekrar oynamak ister misiniz?" diye sorun.
- Tahmin ile gizli sayı arasındaki farka göre "çok yaklaştınız" gibi ipuçları verin.

## Proje 2: Not ortalaması hesaplayıcı

Kullanıcı her satıra bir öğrencinin adını ve notunu yazar. Boş bir satırla bitirir. Program ortalamayı, en yüksek ve en düşük notu, geçen öğrenci sayısını ve her öğrencinin harf notunu yazar.

Kullanılan konular: fonksiyonlar, liste, sözlük, `böl`, `kırp`, `dene` / `yakala`, `fırlat`, döngüler, `sırala` (anahtar fonksiyonuyla), `yuvarla`, `tekrarla` ve `sağa_doldur` / `sola_doldur`.

`notlar.jus`:

```jus
fonksiyon harfNotu(ortalama):
    eğer ortalama >= 90:
        dön "A"
    değilse eğer ortalama >= 80:
        dön "B"
    değilse eğer ortalama >= 70:
        dön "C"
    değilse eğer ortalama >= 50:
        dön "D"
    dön "F"

fonksiyon notunEksisi(ö):
    dön -ö["not"]

değişken öğrenciler = []

yaz("Her satıra bir öğrencinin adını ve notunu yazın (örnek: Ayşe 85).")
yaz("Bitirmek için boş bir satır bırakın.")

iken doğru:
    değişken satır = oku("> ")
    eğer satır == boş:
        kır
    satır = kırp(satır)
    eğer satır == "":
        kır

    değişken parçalar = böl(satır, " ")
    eğer uzunluk(parçalar) != 2:
        yaz("Biçim hatalı. 'ad not' şeklinde yazın.")
        devam

    dene:
        değişken puan = sayı(parçalar[1])
        eğer puan < 0 veya puan > 100:
            fırlat "Not 0 ile 100 arasında olmalı."
        ekle(öğrenciler, {"ad": parçalar[0], "not": puan})
    yakala sebep:
        yaz("Kaydedilmedi:", sebep)

eğer uzunluk(öğrenciler) == 0:
    yaz("Hiç öğrenci girilmedi.")
değilse:
    değişken sıralı = sırala(öğrenciler, notunEksisi)
    değişken toplam = 0
    değişken geçen = 0

    yaz("")
    yaz("Sonuçlar (nota göre)")
    yaz(tekrarla("-", 20))
    her ö içinde sıralı:
        yaz(sağa_doldur(ö["ad"] + ":", 9) + sola_doldur(metin(ö["not"]), 5) + "  (" + harfNotu(ö["not"]) + ")")
        toplam += ö["not"]
        eğer ö["not"] >= 50:
            geçen += 1

    değişken ortalama = toplam / uzunluk(sıralı)
    yaz("")
    yaz("Öğrenci sayısı:", uzunluk(sıralı))
    yaz("Sınıf ortalaması:", yuvarla(ortalama, 1), "(" + harfNotu(ortalama) + ")")
    yaz("En yüksek:", sıralı[0]["ad"], sıralı[0]["not"])
    yaz("En düşük:", sıralı[-1]["ad"], sıralı[-1]["not"])
    yaz("Geçen öğrenci sayısı:", geçen)
```

### Nasıl çalışır?

- Her öğrenci bir sözlük olarak (`{"ad": ..., "not": ...}`) `öğrenciler` listesine eklenir.
- Her satır `kırp` ile temizlenir ve `böl(satır, " ")` ile ikiye ayrılır. İki parça değilse uyarı verilir.
- Not, `sayı()` ile sayıya çevrilir. Çevrilemezse ya da 0-100 aralığında değilse `dene` bloğu hatayı yakalar ve öğrenci kaydedilmez. Aralık denetimini `fırlat` ile kendimiz yaptık; böylece iki farklı sorun aynı `yakala` bloğunda ele alındı.
- `sırala(öğrenciler, notunEksisi)` öğrencileri nota göre büyükten küçüğe dizer. Anahtar fonksiyonu notun eksisini döndürdüğü için en yüksek not başa gelir (sekizinci bölüm). Böylece en yüksek öğrenci `sıralı[0]`, en düşük öğrenci `sıralı[-1]` olur; en yüksek ve en düşüğü bulmak için ayrıca döngü yazmamız gerekmedi.
- Sonuç satırları `sağa_doldur` ve `sola_doldur` ile hizalanır: adlar sola, notlar sağa yaslıdır. Başlığın altındaki çizgiyi `tekrarla` çizer.
- Toplam ve geçen sayısı tek bir `her` döngüsünde hesaplanır.
- `yuvarla(ortalama, 1)` ortalamayı bir ondalık basamağa yuvarlar.
- Kısa bir hatırlatma: `harfNotu` içindeki son `dön "F"` satırı, hiçbir koşul tutmazsa çalışır; ayrı bir `değilse` yazmaya gerek yoktur, çünkü önceki dönüşler fonksiyonu zaten bitirmiştir.

### Örnek oturum

Girdi olarak şu satırları verdik: `Ayşe 90`, `Mehmet 45`, `Zeynep 78.5`, `hatalı`, `Ali yüz`, `Can 120`, `Elif 62` ve boş bir satır.

```
Her satıra bir öğrencinin adını ve notunu yazın (örnek: Ayşe 85).
Bitirmek için boş bir satır bırakın.
> > > > Biçim hatalı. 'ad not' şeklinde yazın.
> Kaydedilmedi: "yüz" bir sayıya dönüştürülemez.
> Kaydedilmedi: Not 0 ile 100 arasında olmalı.
> > 
Sonuçlar (nota göre)
--------------------
Ayşe:       90  (A)
Zeynep:   78.5  (C)
Elif:       62  (D)
Mehmet:     45  (F)

Öğrenci sayısı: 4
Sınıf ortalaması: 68.9 (D)
En yüksek: Ayşe 90
En düşük: Mehmet 45
Geçen öğrenci sayısı: 3
```

Girdiyi dosyadan verdiğimiz için istemler (`> `) arka arkaya sıralanmış görünüyor. Terminalde her istemin yanında sizin yazdığınız satır olur. `hatalı` tek parça olduğu için "Biçim hatalı" uyarısı aldı. `Ali yüz` ve `Can 120` kaydedilmedi. Ortalama 275,5 / 4 = 68,875'tir; bir ondalık basamağa yuvarlanınca 68.9 olur.

Hiç öğrenci girmeden boş satırla çıkarsanız:

```
Her satıra bir öğrencinin adını ve notunu yazın (örnek: Ayşe 85).
Bitirmek için boş bir satır bırakın.
> Hiç öğrenci girilmedi.
```

### Genişletme fikirleri

- Öğrencileri nota göre değil, ada göre (Türk alfabesine uygun) sıralayın.
- Her harf notundan kaç öğrenci olduğunu bir sözlükle sayın.
- Sonuçları `dosya.yaz` ile bir dosyaya kaydedin.

## Proje 3: Yapılacaklar listesi

Bir komut satırı programı: iş ekler, listeler, bitirildi olarak işaretler ve siler. Liste bir dosyada saklanır; program kapanıp tekrar açıldığında işler yerinde durur.

Kullanılan konular: `dosya` modülü, fonksiyonlar, liste ve sözlük, `böl` / `birleştir`, `bul`, dilim, `dene` / `yakala` / `fırlat`.

`yapilacaklar.jus`:

```jus
kullan dosya

değişken DOSYA = "yapilacaklar.txt"
değişken işler = []

# Dosyadaki işleri okur. Her satır "0|metin" ya da "1|metin" biçimindedir.
# 1 işin bittiği anlamına gelir.
fonksiyon yükle():
    eğer değil dosya.var_mı(DOSYA):
        dön
    her satır içinde dosya.satırlar(DOSYA):
        eğer satır == "":
            devam
        değişken parçalar = böl(satır, "|")
        ekle(işler, {"metin": birleştir(parçalar[1:], "|"), "bitti": parçalar[0] == "1"})

fonksiyon kaydet():
    değişken satırlar = []
    her iş içinde işler:
        değişken işaret = "0"
        eğer iş["bitti"]:
            işaret = "1"
        ekle(satırlar, işaret + "|" + iş["metin"])
    dosya.yaz(DOSYA, birleştir(satırlar, "\n") + "\n")

fonksiyon listele():
    eğer uzunluk(işler) == 0:
        yaz("Liste boş.")
        dön
    her i içinde aralık(uzunluk(işler)):
        değişken kutu = "[ ]"
        eğer işler[i]["bitti"]:
            kutu = "[x]"
        yaz(metin(i + 1) + ". " + kutu + " " + işler[i]["metin"])

# "3" gibi bir metni listedeki dizine çevirir; geçersizse hata fırlatır.
fonksiyon dizinAl(metin2):
    değişken no = sayı(metin2)
    eğer no < 1 veya no > uzunluk(işler):
        fırlat "Böyle bir numara yok: " + metin2
    dön no - 1

yükle()
yaz("Komutlar: ekle <iş>, listele, bitir <no>, sil <no>, çık")

iken doğru:
    değişken girdi = oku("> ")
    eğer girdi == boş:
        kır
    girdi = kırp(girdi)

    değişken komut = girdi
    değişken argüman = ""
    değişken boşlukYeri = bul(girdi, " ")
    eğer boşlukYeri != -1:
        komut = girdi[:boşlukYeri]
        argüman = kırp(girdi[boşlukYeri + 1:])

    eğer komut == "çık":
        kır
    değilse eğer komut == "listele":
        listele()
    değilse eğer komut == "ekle":
        eğer argüman == "":
            yaz("Neyi ekleyeceğinizi yazın.")
        değilse:
            ekle(işler, {"metin": argüman, "bitti": yanlış})
            kaydet()
            yaz("Eklendi.")
    değilse eğer komut == "bitir" veya komut == "sil":
        dene:
            değişken dizin = dizinAl(argüman)
            eğer komut == "bitir":
                işler[dizin]["bitti"] = doğru
            değilse:
                sil(işler, dizin)
            kaydet()
            yaz("Tamam.")
        yakala sebep:
            yaz("Olmadı:", sebep)
    değilse eğer komut != "":
        yaz("Bilinmeyen komut: " + komut)

yaz("Görüşürüz.")
```

### Nasıl çalışır?

- **Veri biçimi.** Her iş dosyada bir satırdır: `0|Ekmek al` (yapılmadı) ya da `1|Ekmek al` (yapıldı). Bellekte ise her iş bir sözlüktür: `{"metin": ..., "bitti": ...}`.
- `yükle()` program başlarken dosya varsa onu okur. `böl(satır, "|")` satırı ikiye ayırır; iş metninin kendisinde de `|` bulunabileceği için metni `birleştir(parçalar[1:], "|")` ile yeniden kurarız.
- `kaydet()` bellekteki listeyi satırlara çevirir ve `dosya.yaz` ile dosyanın üzerine yazar. Her değişiklikten sonra çağrılır; böylece program beklenmedik biçimde kapansa bile kayıtlar durur.
- `listele()` her işin başına numara ve `[ ]` ya da `[x]` kutusu koyar. Kullanıcıya gösterilen numaralar 1'den başlar, listedeki dizinler 0'dan; `dizinAl` bu dönüşümü yapar.
- `dizinAl` kullanıcının yazdığı metni sayıya çevirir ve geçerli aralıkta olup olmadığına bakar. Geçersizse `fırlat` ile hata verir. `sayı("abc")` de hata verir. İki hata türü de ana döngüdeki `dene` / `yakala` içinde tek yerde ele alınır.
- Ana döngü, girdi satırını komut ve argüman olarak ayırır: ilk boşluktan öncesi komut, sonrası argümandır. `bul(girdi, " ")` boşluk yoksa `-1` verir; bu durumda tüm girdi komuttur.
- Komut seçimi `değilse eğer` zinciriyle yapılır. `bitir` ve `sil` komutları benzer olduğu için aynı blokta işlenip `komut == "bitir"` ile ayrılır.

### Örnek oturumlar

Önce dosya yokken şu girdileri verdik:

```
listele
ekle Ekmek al
ekle Raporu bitir | gönder
ekle Dişçiyi ara
listele
bitir 1
bitir 7
bitir abc
sil 3
uçur
listele
çık
```

Çıktı:

```
Komutlar: ekle <iş>, listele, bitir <no>, sil <no>, çık
> Liste boş.
> Eklendi.
> Eklendi.
> Eklendi.
> 1. [ ] Ekmek al
2. [ ] Raporu bitir | gönder
3. [ ] Dişçiyi ara
> Tamam.
> Olmadı: Böyle bir numara yok: 7
> Olmadı: "abc" bir sayıya dönüştürülemez.
> Tamam.
> Bilinmeyen komut: uçur
> 1. [x] Ekmek al
2. [ ] Raporu bitir | gönder
> Görüşürüz.
```

Program bittikten sonra `yapilacaklar.txt` dosyasının içeriği:

```
1|Ekmek al
0|Raporu bitir | gönder
```

Programı yeniden çalıştırıp `listele` ve `çık` yazdığımızda işler dosyadan geri geldi:

```
Komutlar: ekle <iş>, listele, bitir <no>, sil <no>, çık
> 1. [x] Ekmek al
2. [ ] Raporu bitir | gönder
> Görüşürüz.
```

### Genişletme fikirleri

- `temizle` komutuyla bitmiş işleri toplu silin.
- İşlere tarih ekleyin (`zaman.tarih()`).
- İşleri bir sınıfa (`sınıf Iş:`) taşıyın; `yükle` ve `kaydet` de bir `Liste` sınıfının yöntemi olsun.

## Alıştırmalar

1. Not projesinden esinlenin: bir not listesinin ortalamasını veren `ortalama(liste)` ve belirli bir sınırın üstündeki notları listeleyen `geçenler(liste, sınır)` fonksiyonlarını yazın. `[90, 45, 78, 62, 30]` ve sınır 50 ile deneyin.
2. Tahmin oyununun çekirdeğini yazın: gizli sayı 45, tahminler `[50, 20, 45, 10]` listesinde olsun. Her tahmine ipucu verin; doğru tahminde `doğru!` deyip döngüyü bitirin. (Tahmin 45'te durmalı, 10'a hiç gelinmemeli.)
3. Yapılacaklar projesindeki dosya fikrini kullanarak küçük bir günlük programı yazın: `dosya.ekle` ile iki kayıt ekleyin, sonra kayıtları numaralı yazdırın ve dosyayı silin.

## Çözümler

**1.**

```jus
fonksiyon ortalama(liste):
    değişken toplam = 0
    her n içinde liste:
        toplam += n
    dön toplam / uzunluk(liste)

fonksiyon geçenler(liste, sınır):
    değişken sonuç = []
    her n içinde liste:
        eğer n >= sınır:
            ekle(sonuç, n)
    dön sonuç

değişken notlar = [90, 45, 78, 62, 30]
yaz("Ortalama:", ortalama(notlar))
yaz("Geçenler:", geçenler(notlar, 50))
```

```
Ortalama: 61
Geçenler: [90, 78, 62]
```

**2.**

```jus
değişken gizli = 45
değişken tahminler = [50, 20, 45, 10]

her tahmin içinde tahminler:
    eğer tahmin < gizli:
        yaz(tahmin, "-> daha büyük")
    değilse eğer tahmin > gizli:
        yaz(tahmin, "-> daha küçük")
    değilse:
        yaz(tahmin, "-> doğru!")
        kır
```

```
50 -> daha küçük
20 -> daha büyük
45 -> doğru!
```

**3.**

```jus
kullan dosya

değişken günlük = "gunluk_deneme.txt"

fonksiyon kayıtEkle(m):
    dosya.ekle(günlük, m + "\n")

kayıtEkle("Birinci kayıt")
kayıtEkle("İkinci kayıt")

değişken sıra = 1
her satır içinde dosya.satırlar(günlük):
    yaz(metin(sıra) + ". " + satır)
    sıra += 1

dosya.sil(günlük)
```

```
1. Birinci kayıt
2. İkinci kayıt
```

## Sırada ne var?

Temel bölümleri ve üç projeyi bitirdiniz. Sonraki üç bölüm, programlarınızı dış dünyaya açar:

- [13 - JSON ve veri](13-json-ve-veri.md): yapılandırılmış veriyi dosyaya kaydetmek ve okumak.
- [14 - Ağ ve HTTP](14-ag-ve-http.md): web istemcisi ve küçük bir sunucu.
- [15 - Test, biçim ve paket](15-test-bicim-paket.md): programlarınızı sınamak, düzenli tutmak ve paylaşmak.

Bunlardan sonrası için:

- Dilin tam ve bağlayıcı tanımını okuyun: [dil tanımı](../dil-tanimi.md). Rehberde değinmediğimiz ayrıntılar (işleç öncelikleri, dilbilgisi) orada.
- Depodaki `examples` klasöründeki örnek programlara bakın.
- Kendi küçük projelerinizi yazın. Programlamayı öğrenmenin en iyi yolu bir problemi çözmeye çalışmaktır.

---

Önceki: [11 - Sınıflar](11-siniflar.md) | [İçindekiler](README.md) | Sonraki: [13 - JSON ve veri](13-json-ve-veri.md)
