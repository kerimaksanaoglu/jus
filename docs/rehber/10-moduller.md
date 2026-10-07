# 10 - Modüller

Önceki: [09 - Hatalar](09-hatalar.md) | [İçindekiler](README.md) | Sonraki: [11 - Sınıflar](11-siniflar.md)

Programlar büyüdükçe her şeyi tek dosyaya yazmak zorlaşır. Ayrıca matematik, rastgele sayı, dosya işleme gibi işleri her seferinde sıfırdan yazmak istemezsiniz. **Modül**, hazır ya da kendi yazdığınız bir grup değişken ve fonksiyondur. Modülü `kullan` deyimiyle programınıza çağırırsınız.

## `kullan`

```jus
kullan matematik
kullan matematik olarak m
```

- `kullan matematik`, `matematik` modülünü yükler ve onu `matematik` adlı bir değişkene bağlar.
- `olarak` ile modüle başka bir ad verebilirsiniz: `kullan matematik olarak m`. Uzun adları kısaltmak için kullanışlıdır.
- Modülün üyelerine nokta ile erişirsiniz: `matematik.pi`, `m.karekök(144)`.

Modül, kaç kez `kullan` ile istenirse istensin yalnızca bir kez yüklenir.

JUS ile birlikte gelen modüllere **standart kütüphane** denir. Aşağıda beş modülü sırayla tanıyacağız: `matematik`, `rastgele`, `zaman`, `dosya` ve `sistem`. Standart kütüphanede ayrıca `json`, `http`, `ağ` ve `tr` modülleri vardır; bunlar 13. ve 14. bölümlerde anlatılıyor.

## `matematik`

```jus
kullan matematik
kullan matematik olarak m

yaz(matematik.pi)
yaz(matematik.e)
yaz(matematik.üs(2, 10))
yaz(m.karekök(144))
yaz(m.sin(0))
yaz(m.cos(0))
yaz(m.ln(1))
yaz(m.log10(1000))
yaz(m.en_küçük(4, 9, 2, 7))
yaz(m.en_büyük(4, 9, 2, 7))
yaz(m.en_büyük([3, 8, 5]))
yaz(m.toplam(1, 2, 3, 4))
yaz(m.toplam([10, 20, 30]))

# yerleşik sayı fonksiyonları kullan gerektirmez
yaz(karekök(81))
yaz(mutlak(-7))
yaz(taban(3.7))
yaz(tavan(3.2))
yaz(yuvarla(3.5))
yaz(yuvarla(2.4))

değişken r = 5
yaz("Çevre:", 2 * matematik.pi * r)
yaz("Alan:", yuvarla(matematik.pi * r * r))
```

```
3.1415926535898
2.718281828459
1024
12
0
1
0
3
2
9
8
10
60
9
7
3
4
4
2
Çevre: 31.415926535898
Alan: 79
```

`matematik` modülünün üyeleri:

| Üye | Açıklama |
|-----|----------|
| `pi`, `e` | Sabitler |
| `üs(taban, üs)` | Üs alma |
| `karekök(x)` | Karekök |
| `sin(x)`, `cos(x)`, `tan(x)` | Trigonometrik fonksiyonlar; açı radyan cinsindendir |
| `ln(x)`, `log10(x)` | Doğal ve onluk logaritma |
| `en_küçük(...)`, `en_büyük(...)` | Verilen sayıların ya da bir sayı listesinin en küçüğü, en büyüğü |
| `toplam(...)` | Verilen sayıların ya da bir sayı listesinin toplamı |

Gördüğünüz `karekök`, `mutlak`, `taban`, `tavan`, `yuvarla` gibi sayı fonksiyonları ise yerleşiktir; `kullan` gerektirmez. (`taban(3.7)`, 3.7'den büyük olmayan en büyük tam sayıdır; `tavan(3.2)`, 3.2'den küçük olmayan en küçük tam sayıdır.)

## `rastgele`

Rastgele sayılar oyunlarda ve simülasyonlarda gerekir. Bir rastgele sayının çıktısını önceden bilemeyeceğiniz için aşağıdaki örnek, sonuçları kendi içinde denetleyip `doğru` yazdırır.

```jus
kullan rastgele

# 1 ile 6 arasında (6 dahil) bir zar atışı; 1000 atışın hepsi aralıkta mı?
değişken hepsiAralıkta = doğru
her i içinde aralık(1000):
    değişken zar = rastgele.tam(1, 6)
    eğer zar < 1 veya zar > 6:
        hepsiAralıkta = yanlış
yaz("Hepsi 1-6 arasında:", hepsiAralıkta)

değişken x = rastgele.sayı()
yaz(x >= 0 ve x < 1)

yaz(rastgele.seç(["tek"]))

# aynı tohum, aynı dizi
rastgele.tohum(7)
değişken ilk = [rastgele.tam(1, 100), rastgele.tam(1, 100), rastgele.tam(1, 100)]
rastgele.tohum(7)
değişken ikinci = [rastgele.tam(1, 100), rastgele.tam(1, 100), rastgele.tam(1, 100)]
yaz(ilk == ikinci)

değişken kartlar = [1, 2, 3, 4, 5]
rastgele.karıştır(kartlar)
yaz(uzunluk(kartlar))
yaz(sırala(kartlar))
```

```
Hepsi 1-6 arasında: doğru
doğru
tek
doğru
5
[1, 2, 3, 4, 5]
```

| Üye | Açıklama |
|-----|----------|
| `sayı()` | 0 ile 1 arasında (1 hariç) rastgele sayı |
| `tam(alt, üst)` | `alt` ve `üst` **dahil** rastgele tam sayı |
| `seç(liste)` | Listeden rastgele bir öğe |
| `karıştır(liste)` | Listenin öğelerini **yerinde** karıştırır |
| `tohum(sayı)` | Üreteci sıfırlar; aynı tohum her zaman aynı diziyi üretir |

`tohum`, program her çalıştığında aynı "rastgele" dizinin üretilmesini istediğiniz durumlarda (örneğin hata ayıklarken) işe yarar. Kendi programlarınızda normalde `tohum` çağırmazsınız.

## `zaman`

```jus
kullan zaman

değişken başlangıç = zaman.şimdi()
yaz(tür(başlangıç))
zaman.bekle(2)
değişken geçen = zaman.şimdi() - başlangıç
yaz(geçen >= 1)

değişken t = zaman.tarih()
yaz(anahtarlar(t))
yaz(t["yıl"] >= 2024)
yaz(t["haftanın_günü"] >= 1 ve t["haftanın_günü"] <= 7)
```

```
sayı
doğru
["yıl", "ay", "gün", "saat", "dakika", "saniye", "haftanın_günü"]
doğru
doğru
```

| Üye | Açıklama |
|-----|----------|
| `şimdi()` | 1 Ocak 1970'ten bu yana geçen saniye |
| `bekle(saniye)` | Programı verilen süre kadar durdurur |
| `tarih()` | Yerel tarih ve saat; sözlük olarak verilir. Anahtarlar: `yıl`, `ay`, `gün`, `saat`, `dakika`, `saniye`, `haftanın_günü` (1: Pazartesi) |

Tarih ve saat çalıştırdığınız ana bağlı olduğu için örnekte gerçek değerleri yazdırmak yerine denetimler yaptık. Örnek 2 saniye bekler.

`şimdi()` saniyenin kesirlerini de verir; iki çağrı arasındaki fark, geçen süreyi saniye cinsinden gösterir.

## `dosya`

Dosya modülü metin dosyalarını okur ve yazar. Dosyalar UTF-8 olarak işlenir.

```jus
kullan dosya

değişken yol = "deneme_notlar.txt"
yaz(dosya.var_mı(yol))

dosya.yaz(yol, "Birinci satır\nİkinci satır\n")
yaz(dosya.var_mı(yol))
yaz(dosya.oku(yol))

dosya.ekle(yol, "Üçüncü satır\n")
yaz(dosya.satırlar(yol))
yaz(uzunluk(dosya.satırlar(yol)))

dosya.yaz(yol, "Üzerine yazıldı")
yaz(dosya.oku(yol))

dosya.sil(yol)
yaz(dosya.var_mı(yol))
```

```
yanlış
doğru
Birinci satır
İkinci satır

["Birinci satır", "İkinci satır", "Üçüncü satır"]
3
Üzerine yazıldı
yanlış
```

Dosya adı olarak yazdığınız yol, programı çalıştırdığınız klasöre göre değerlendirilir.

| Üye | Açıklama |
|-----|----------|
| `oku(yol)` | Dosyanın tüm içeriği, metin olarak |
| `satırlar(yol)` | Dosyanın satırları, liste olarak (satır sonu karakterleri olmadan) |
| `yaz(yol, içerik)` | Dosyayı içerikle oluşturur; varsa **üzerine yazar** |
| `ekle(yol, içerik)` | İçeriği dosyanın sonuna ekler |
| `var_mı(yol)` | Dosya varsa `doğru` |
| `sil(yol)` | Dosyayı siler |
| `listele(yol)` | Klasördeki dosya ve klasör adlarının listesi; sıra belirsizdir |
| `klasör_oluştur(yol)` | Klasör oluşturur |
| `klasör_sil(yol)` | **Boş** bir klasörü siler |

İlk `yaz(dosya.oku(yol))` çıktısında dosyanın sonundaki satır sonu yüzünden bir boş satır görünür: dosya `İkinci satır\n` ile bitiyor ve `yaz` bir satır sonu daha ekliyor.

`dosya.yaz` var olan bir dosyanın içeriğini siler. Önemli bir dosya üzerinde denemeden önce yedek alın.

### Klasörlerle çalışmak

`dosya.listele` bir klasördeki dosya ve alt klasör adlarını liste olarak verir. Sırası belirsiz olduğu için, her çalıştırmada aynı çıktıyı istiyorsanız `sırala` ile sıralayın. `dosya.klasör_oluştur` yeni klasör açar; `dosya.klasör_sil` yalnızca **boş** klasörü siler, içinde bir şey varsa önce içindekileri silmeniz gerekir.

```jus
kullan dosya

değişken klasör = "deneme_klasör"
dosya.klasör_oluştur(klasör)
dosya.yaz(klasör + "/b.txt", "ikinci")
dosya.yaz(klasör + "/a.txt", "birinci")
dosya.klasör_oluştur(klasör + "/alt")

değişken adlar = sırala(dosya.listele(klasör))
yaz(adlar)
yaz(uzunluk(adlar))

her ad içinde adlar:
    eğer ad == "alt":
        dosya.klasör_sil(klasör + "/alt")
    değilse:
        dosya.sil(klasör + "/" + ad)

yaz(dosya.listele(klasör))
dosya.klasör_sil(klasör)
```

```
["a.txt", "alt", "b.txt"]
3
[]
```

Listede dosya ile klasör adları karışık gelir; hangisinin klasör olduğunu bu fonksiyon söylemez. Yukarıda `alt` adını elle bildiğimiz için ayırabildik.

Boş olmayan klasörü silmeye çalışırsanız hata alırsınız:

```jus
kullan dosya

dosya.klasör_oluştur("dolu_klasör")
dosya.yaz("dolu_klasör/a.txt", "x")
dosya.klasör_sil("dolu_klasör")
```

```
ornek.jus:5: çalışma zamanı hatası: 'dolu_klasör' klasörü silinemedi; klasör boş olmalıdır.
```

## `sistem`

```jus
kullan sistem

yaz("Argümanlar:", sistem.argümanlar)
yaz("Platform:", sistem.platform)
yaz(sistem.ortam("OLMAYAN_BIR_DEGISKEN_XYZ"))

eğer uzunluk(sistem.argümanlar) == 0:
    yaz("Argüman verilmedi, çıkılıyor.")
    sistem.çık(3)

yaz("Merhaba, " + sistem.argümanlar[0] + "!")
```

Programı argümansız çalıştırınca (`jus ornek.jus`):

```
Argümanlar: []
Platform: windows
boş
Argüman verilmedi, çıkılıyor.
```

Dosya adından sonra argüman verince (`jus ornek.jus Ayşe Ali`):

```
Argümanlar: ["Ayşe", "Ali"]
Platform: windows
boş
Merhaba, Ayşe!
```

| Üye | Açıklama |
|-----|----------|
| `argümanlar` | Komut satırında dosya adından sonra verilen argümanların listesi |
| `platform` | `"windows"`, `"linux"` ya da `"macos"` |
| `ortam(ad)` | Ortam değişkeninin değeri; tanımlı değilse `boş` |
| `çık(kod)` | Programı verilen çıkış koduyla sonlandırır |
| `hata_yaz(...)` | `yaz` gibi çalışır, ancak standart hata çıktısına yazar |
| `betik` | Çalıştırılan dosyanın yolu |
| `betik_klasörü` | Çalıştırılan dosyanın bulunduğu klasör |

`argümanlar`, `platform`, `betik` ve `betik_klasörü`'nün parantezsiz yazıldığına dikkat edin: bunlar fonksiyon değil, değerdir. `ortam`, `çık` ve `hata_yaz` ise fonksiyondur. Platform çıktısı sizin bilgisayarınıza göre değişir. İlk çalıştırmada `sistem.çık(3)` programı 3 çıkış koduyla bitirdi.

Argümanlar ve dosya adları Türkçe harf içerebilir; `jus ornek.jus Şükrü Çağlar` ya da `çıktı_ığ.txt` gibi bir dosya adı sorunsuz çalışır.

### Hata çıktısı ve betiğin konumu

Programlar iki ayrı çıktı kanalı kullanır: normal çıktı (`yaz`) ve hata çıktısı. Terminalde ikisi de ekranda görünür, ama birbirinden ayrılabilirler: örneğin çıktıyı bir dosyaya yönlendirirken uyarıların dosyaya karışmaması istenir. `sistem.hata_yaz` uyarı ve kullanım iletilerini hata çıktısına yazar.

`sistem.betik_klasörü`, çalışan programın bulunduğu klasördür. Programın yanındaki bir dosyayı, programı hangi klasörden çalıştırırsanız çalıştırın bulmak için yolu bununla kurarsınız:

```jus
kullan sistem
kullan dosya

yaz("Betik:", sistem.betik)
yaz("Klasör:", sistem.betik_klasörü)

# betiğin yanındaki dosyayı, nereden çalıştırılırsa çalıştırılsın bul
değişken yol = sistem.betik_klasörü + "/ayar.txt"
dosya.yaz(yol, "tema=koyu")
yaz(dosya.oku(yol))
dosya.sil(yol)

sistem.hata_yaz("Uyarı: ayar dosyası eski")
yaz("Normal çıktı")
```

`jus ornek.jus > cikti.txt` ile çalıştırırsanız `cikti.txt` dosyasına yalnızca normal çıktı gider; `Uyarı: ...` satırı ekranda kalır:

```
Betik: ornek.jus
Klasör: .
tema=koyu
Normal çıktı
```

Ekranda ayrıca şu satır görünür:

```
Uyarı: ayar dosyası eski
```

`Betik` ve `Klasör` değerleri programı nasıl çağırdığınıza bağlıdır: `jus ornek.jus` dediğimiz için `ornek.jus` ve `.` (bulunulan klasör) çıktı. Tam yolla çağırırsanız tam yol görürsünüz.

## Kendi modülünüzü yazmak

Her `.jus` dosyası bir modüldür. Bir dosyanın üst düzeyinde tanımlanan tüm değişken ve fonksiyonlar, o modülün üyeleri olur.

`kullan ad` yazdığınızda JUS önce standart kütüphanede `ad` adlı modülü arar. Bulamazsa, `kullan` satırının bulunduğu dosyanın klasöründe `ad.jus` dosyasını arar. Alt klasördeki bir dosya için yol tırnak içinde yazılır, `/` ile ayrılır ve `.jus` uzantısı yazılmaz; değişken adı yolun son parçasıdır.

Şu klasör yapısını kuralım:

```
proje/
    ana.jus
    araclar.jus
    geo/
        geometri.jus
```

`araclar.jus`:

```jus
yaz("araçlar modülü yükleniyor")

değişken sürüm = "1.0"

fonksiyon selamla(ad):
    dön "Merhaba, " + ad + "!"

fonksiyon ortalama(liste):
    değişken toplam = 0
    her n içinde liste:
        toplam += n
    dön toplam / uzunluk(liste)
```

`geo/geometri.jus`:

```jus
kullan matematik

fonksiyon daireAlanı(r):
    dön matematik.pi * r * r
```

`ana.jus`:

```jus
kullan araclar
kullan araclar olarak a2
kullan "geo/geometri"

yaz(araclar.selamla("Ayşe"))
yaz(araclar.sürüm)
yaz(a2.ortalama([70, 80, 90]))
yaz(geometri.daireAlanı(1))

# aynı ad başka dosyada ayrı bir değişkendir
değişken sürüm = "ana programın sürümü"
yaz(sürüm)
yaz(araclar.sürüm)
```

`proje` klasöründeyken `jus ana.jus` çalıştırılırsa:

```
araçlar modülü yükleniyor
Merhaba, Ayşe!
1.0
80
3.1415926535898
ana programın sürümü
1.0
```

Gözlemler:

- `araclar` iki kez (`kullan araclar` ve `kullan araclar olarak a2`) istendi ama "modül yükleniyor" iletisi yalnızca bir kez yazıldı. Modülün üst düzey kodu bir kez çalışır.
- `a2`, `araclar` ile aynı modüldür; yalnızca farklı bir ad aldı.
- `kullan "geo/geometri"` ile alt klasördeki dosya yüklendi ve değişken adı `geometri` oldu. `geometri.jus` kendi içinde `kullan matematik` yazdı; bu, `ana.jus`'u etkilemez.
- Her dosyanın genel değişkenleri kendisine aittir. `ana.jus`'ta tanımladığımız `sürüm`, `araclar.sürüm`'ü değiştirmedi.
- Yerleşik fonksiyonlar (`yaz`, `uzunluk`, ...) her dosyadan görülür.

## Sık yapılan hatalar

**Olmayan modülü istemek.**

```jus
kullan olmayanmodül
yaz("hiç çalışmaz")
```

```
ornek.jus:1: çalışma zamanı hatası: 'olmayanmodül' modülü yüklenemedi: 'olmayanmodül.jus' için dosya açılamadı. Kurulu paketlerde de yok ('jus_paketleri/olmayanmodül/olmayanmodül.jus').
```

Hata iletisinin sonundaki `jus_paketleri` ifadesi, JUS'un modülü kurulu paketlerde de aradığını gösterir; paketleri 15. bölümde anlatıyoruz.

Modül adını ve dosyanın konumunu denetleyin.

**`kullan` yazmadan modül üyesi kullanmak.**

```jus
yaz(matematik.pi)
```

```
ornek.jus:1: çalışma zamanı hatası: 'matematik' adında bir değişken ya da fonksiyon tanımlı değil.
```

**Modülde olmayan üyeyi istemek.**

```jus
kullan matematik
yaz(matematik.kübü(3))
```

```
ornek.jus:2: çalışma zamanı hatası: 'matematik' modülünde 'kübü' adında bir üye yok.
```

**Modül üyesine dışarıdan atama yapmak.**

```jus
kullan araclar
araclar.sürüm = "2.0"
```

```
araçlar modülü yükleniyor
ornek.jus:2: çalışma zamanı hatası: Yalnızca nesnelerin alanlarına değer atanabilir; modül verildi.
```

Bir modülün üyelerini yalnızca okuyabilirsiniz. Değiştirmek istiyorsanız modülde bunu yapan bir fonksiyon yazın.

## Alıştırmalar

1. `matematik` modülünü kullanarak iki nokta arasındaki uzaklığı veren `mesafe(x1, y1, x2, y2)` fonksiyonunu yazın. `(0, 0)`-`(3, 4)` ve `(1, 1)`-`(4, 5)` noktalarını deneyin.
2. `rastgele` modülüyle iki zar atın ve toplamın 2 ile 12 arasında olduğunu denetleyin.
3. `dosya` modülüyle `[12, 7, 30]` listesindeki sayıları bir dosyaya satır satır yazın, sonra dosyayı geri okuyup toplamlarını hesaplayın ve dosyayı silin.
4. `geometri2.jus` adında kendi modülünüzü yazın: `daireAlanı(r)` ve `kareAlanı(kenar)` fonksiyonları olsun. Başka bir dosyadan kullanın.
5. `sistem.argümanlar` ile komut satırından ad ve soyad alıp selamlayan bir program yazın. Argüman eksikse bir kullanım iletisini `sistem.hata_yaz` ile yazıp 1 koduyla çıksın.
6. `dosya.klasör_oluştur` ile `deneme_alistirma` klasörünü açın, içine `bir.txt` ve `iki.txt` dosyalarını yazın, `dosya.listele` ile adları sıralı yazdırın, sonra hepsini silin.

## Çözümler

**1.**

```jus
kullan matematik

fonksiyon mesafe(x1, y1, x2, y2):
    dön matematik.karekök(matematik.üs(x2 - x1, 2) + matematik.üs(y2 - y1, 2))

yaz(mesafe(0, 0, 3, 4))
yaz(mesafe(1, 1, 4, 5))
```

```
5
5
```

**2.**

```jus
kullan rastgele

değişken toplam = rastgele.tam(1, 6) + rastgele.tam(1, 6)
yaz(toplam >= 2 ve toplam <= 12)
```

```
doğru
```

**3.**

```jus
kullan dosya

değişken sayılar = [12, 7, 30]
değişken satırlar = []
her n içinde sayılar:
    ekle(satırlar, metin(n))
dosya.yaz("sayilar_deneme.txt", birleştir(satırlar, "\n") + "\n")

değişken toplam = 0
her satır içinde dosya.satırlar("sayilar_deneme.txt"):
    toplam += sayı(satır)
yaz("Toplam:", toplam)
dosya.sil("sayilar_deneme.txt")
```

```
Toplam: 49
```

**4.** `geometri2.jus`:

```jus
kullan matematik

fonksiyon daireAlanı(r):
    dön matematik.pi * r * r

fonksiyon kareAlanı(kenar):
    dön kenar * kenar
```

Aynı klasördeki `kullanici.jus`:

```jus
kullan geometri2

yaz(geometri2.kareAlanı(6))
yaz(yuvarla(geometri2.daireAlanı(2)))
```

`jus kullanici.jus` çıktısı:

```
36
13
```

**5.**

```jus
kullan sistem

eğer uzunluk(sistem.argümanlar) < 2:
    sistem.hata_yaz("Kullanım: jus ornek.jus <ad> <soyad>")
    sistem.çık(1)

yaz("Merhaba, " + sistem.argümanlar[0] + " " + sistem.argümanlar[1] + "!")
```

Argümansız çalıştırınca:

```
Kullanım: jus ornek.jus <ad> <soyad>
```

(çıkış kodu 1; ileti hata çıktısına gider). `jus ornek.jus Ayşe Yılmaz` ile:

```
Merhaba, Ayşe Yılmaz!
```

**6.**

```jus
kullan dosya

dosya.klasör_oluştur("deneme_alistirma")
dosya.yaz("deneme_alistirma/bir.txt", "1")
dosya.yaz("deneme_alistirma/iki.txt", "2")

değişken adlar = sırala(dosya.listele("deneme_alistirma"))
yaz(adlar)

her ad içinde adlar:
    dosya.sil("deneme_alistirma/" + ad)
dosya.klasör_sil("deneme_alistirma")
yaz(dosya.var_mı("deneme_alistirma/bir.txt"))
```

```
["bir.txt", "iki.txt"]
yanlış
```

---

Önceki: [09 - Hatalar](09-hatalar.md) | [İçindekiler](README.md) | Sonraki: [11 - Sınıflar](11-siniflar.md)
