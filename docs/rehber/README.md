# JUS Başlangıç Rehberi

Bu rehber, JUS programlama dilini hiç programlama bilmeden öğrenmek isteyenler için yazıldı. JUS, Türkçe sözdizimli bir programlama dilidir. Anahtar kelimeleri Türkçedir: `eğer`, `iken`, `fonksiyon`, `yaz` gibi.

Rehberi bitirdiğinizde değişkenleri, kararları, döngüleri, fonksiyonları, listeleri, sözlükleri, hata yakalamayı, modülleri ve sınıfları kullanabilir; küçük programlar yazabilirsiniz. Son üç bölüm JSON ile veri saklamayı, HTTP ile ağda konuşmayı, test yazmayı, kodu biçimlendirmeyi ve paket kullanmayı anlatır.

## Kimin için?

- Daha önce hiç kod yazmamış olanlar için.
- Başka bir dilden gelip JUS'un sözdizimine hızlıca bakmak isteyenler için de kullanılabilir. Bu durumda 01 ve 02. bölümleri hızla geçebilirsiniz.

Bu rehber dilin tam tanımı değildir. Tam ve bağlayıcı tanım için [dil tanımına](../dil-tanimi.md) bakın.

## Nasıl kullanılır?

1. Bölümleri sırayla okuyun. Her bölüm öncekinin üzerine kurulur.
2. Örnekleri okumakla yetinmeyin; kendiniz yazıp çalıştırın. Yazarken yanlış yapmak öğrenmenin parçasıdır.
3. Her bölümün sonundaki alıştırmaları çözmeden sonraki bölüme geçmeyin. Çözümler dosyanın en sonundadır; önce kendiniz deneyin.
4. Her bölümde "Sık yapılan hatalar" başlığı vardır. Hata iletilerini okumayı bu bölümlerden öğrenirsiniz.

Rehberdeki her kod örneği çalıştırılmış, gösterilen çıktılar gerçek çıktılardır. Hata iletilerinde dosya adı sadeleştirilerek `ornek.jus` yazılmıştır; sizin ekranınızda kendi dosya adınız görünür.

## İçindekiler

| Bölüm | Konu |
|-------|------|
| [01 - İlk program](01-ilk-program.md) | `yaz`, yorumlar, programı çalıştırma, hata iletisi okuma |
| [02 - Değişkenler ve değerler](02-degiskenler-ve-degerler.md) | sayı, metin, mantıksal, boş; aritmetik; `yuvarla` ve `biçimle`; `oku` ile girdi |
| [03 - Kararlar](03-kararlar.md) | karşılaştırma, `ve` / `veya` / `değil`, `eğer` |
| [04 - Döngüler](04-donguler.md) | `iken`, `her ... içinde`, `aralık`, `kır`, `devam`, `geç` |
| [05 - Fonksiyonlar](05-fonksiyonlar.md) | tanım, parametre, `dön`, kapsam, özyineleme |
| [06 - Listeler](06-listeler.md) | dizin, dilim, ekleme, silme, sıralama, `eşle`, `süz` |
| [07 - Metinler](07-metinler.md) | dilim, bölme, bulma, Türkçe harf kuralları, hizalama |
| [08 - Sözlükler](08-sozlukler.md) | anahtar-değer eşlemeleri, kelime sayma |
| [09 - Hatalar](09-hatalar.md) | `dene` / `yakala`, `fırlat` |
| [10 - Modüller](10-moduller.md) | `kullan`, standart kütüphane (`dosya` klasörleri, `sistem`), kendi modülünüz |
| [11 - Sınıflar](11-siniflar.md) | sınıf, yöntem, kalıtım |
| [12 - Küçük projeler](12-kucuk-projeler.md) | üç tam program |
| [13 - JSON ve veri](13-json-ve-veri.md) | `json.çöz`, `json.yaz`, dosyaya kaydetme, `tr` modülü |
| [14 - Ağ ve HTTP](14-ag-ve-http.md) | `http.getir`, `http.gönder`, `http.sun`, `ağ` modülü |
| [15 - Test, biçim ve paket](15-test-bicim-paket.md) | `doğrula`, `jus test`, `jus bicimle`, `jus paket` |

## Kurulum

JUS yorumlayıcısı C99 ile yazılmıştır. Çalıştırmak için önce derlemeniz gerekir. Derleme, kaynak kodu çalıştırılabilir bir programa çevirme işlemidir.

Gerekenler:

- Bir C derleyicisi (GCC ya da Clang)
- `make`

Deponun ana klasöründe şunu çalıştırın:

```sh
make
```

Bu komut, Linux ve macOS'ta `jus`, Windows'ta `jus.exe` dosyasını üretir. Windows'ta MSYS2/MinGW kullanıyorsanız `make` yerine `mingw32-make` gerekebilir.

Derlemeyi denetlemek için sürüm numarasını yazdırın:

```sh
jus --surum
```

```
JUS 1.2.0
```

Komut bulunamazsa `jus` dosyasının bulunduğu klasörde olduğunuzdan emin olun. Linux ve macOS'ta `./jus`, Windows'ta `jus.exe` yazmanız gerekebilir. Rehberin geri kalanında kısalık için `jus` yazılmıştır.

> Not: Bu rehberdeki çıktılar, derlenmiş hazır bir `jus.exe` ile alındı. Derleme adımlarını bu rehberi yazarken denemedik; sorun yaşarsanız deponun ana klasöründeki `README.md` dosyasına bakın.

### Dosyaları nereye yazacaksınız?

JUS programları düz metin dosyalarıdır. Not Defteri, VS Code gibi herhangi bir metin düzenleyiciyle yazabilirsiniz. İki kurala uyun:

- Dosyayı **UTF-8** kodlamasıyla kaydedin. Aksi halde Türkçe harfler bozulur.
- Dosya adı `.jus` ile bitsin. Örneğin `merhaba.jus`.

### İlk çalıştırma

`merhaba.jus` adlı bir dosya oluşturun ve içine şunu yazın:

```jus
yaz("Merhaba, dünya!")
```

Sonra terminalde çalıştırın:

```sh
jus merhaba.jus
```

```
Merhaba, dünya!
```

Bu çıktıyı gördüyseniz kurulum tamamdır. Programlama yolculuğunuz [birinci bölümle](01-ilk-program.md) başlıyor.

### Çıkış kodları

Bir program bittiğinde işletim sistemine bir sayı bırakır. Buna çıkış kodu denir. İlk bölümlerde bunu bilmeniz gerekmez, ama hata iletilerinde karşınıza çıkabilir:

| Kod | Anlamı |
|-----|--------|
| 0 | Program başarıyla bitti |
| 65 | Sözdizimi hatası (program hiç başlamadı) |
| 70 | Çalışma sırasında hata oluştu |
| 66 | Dosya okunamadı |
| 64 | Komut yanlış kullanıldı |

## Etkileşimli kip

Kısa denemeler için dosya yazmanız gerekmez. `jus` komutunu dosya adı vermeden çalıştırırsanız etkileşimli kip açılır. Bu kipte yazdığınız her satır hemen çalıştırılır.

Aşağıda, `>>>` istemini izleyen satırlar sizin yazdıklarınızdır; altındaki satırlar JUS'un yanıtıdır:

```
JUS 1.2.0 - çıkmak için 'çıkış' yazın.
>>> 2 + 3
5
>>> değişken x = 10
>>> x * 2
20
>>> yaz("selam")
selam
>>> fonksiyon kare(n):
...     dön n * n
... 
>>> kare(7)
49
>>> çıkış
```

Dikkat edilecek noktalar:

- Tek başına yazdığınız bir ifadenin sonucu `boş` değilse ekrana yazılır. `2 + 3` yazınca `5` görürsünüz.
- `değişken x = 10` gibi bir tanım sonuç yazmaz.
- Sonu `:` ile biten bir satır blok başlatır. Blok içindeki satırlar için istem `...` olur. Bloğu bitirmek için boş bir satır bırakın.
- Tanımladığınız değişkenler ve fonksiyonlar sonraki satırlarda da geçerlidir.
- Çıkmak için `çıkış` yazın.

Etkileşimli kip, bir şeyi hızlıca denemek için çok uygundur. Ne zaman bir fikrin çalışıp çalışmadığından emin olmasanız, önce orada deneyin.

Hata yaparsanız etkileşimli kip kapanmaz; hata iletisini yazar ve bir sonraki satırı bekler:

```
>>> yaz(yok)
<etkileşimli>:1: çalışma zamanı hatası: 'yok' adında bir değişken ya da fonksiyon tanımlı değil.
```

## Rehberde kullanılan gösterim

- ```` ```jus ```` ile işaretli bloklar JUS kodudur.
- Düz ``` ile işaretli bloklar programın ekrana yazdığı çıktıdır.
- `#` ile başlayan satırlar yorumdur; programın çalışmasını etkilemez.

Haydi başlayalım: [01 - İlk program](01-ilk-program.md)
