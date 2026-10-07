# 15 - Test, biçim ve paket

Önceki: [14 - Ağ ve HTTP](14-ag-ve-http.md) | [İçindekiler](README.md)

Programlarınız büyüdükçe üç soru önem kazanır: Doğru çalıştığını nasıl bilirim? Kodum düzenli mi? Başkasının yazdığı kodu nasıl kullanırım? Bu bölümde JUS'un üç aracı bu sorulara cevap verir:

- `jus test`: programınızı sınayan **testleri** çalıştırır.
- `jus bicimle`: kodunuzu standart bir **biçime** getirir.
- `jus paket`: başkalarının yazdığı kodu **paket** olarak kurar.

Üçü de `jus` komutunun alt komutlarıdır; programlarınızı çalıştırdığınız `jus dosya.jus` komutundan ayrıdır.

## Doğrulama: `doğrula` ve `eşit_olmalı`

Test yazmanın temeli, bir değerin beklediğiniz gibi olup olmadığını denetlemektir. İki yerleşik fonksiyon bunun içindir:

| Fonksiyon | Açıklama |
|-----------|----------|
| `doğrula(koşul)` | Koşul `yanlış` ise hata verir |
| `doğrula(koşul, ileti)` | Aynı; hata iletisine `ileti` eklenir |
| `eşit_olmalı(bulunan, beklenen)` | İki değer eşit değilse ikisini de gösteren bir hata verir |

Koşul doğruysa ya da değerler eşitse hiçbir şey olmaz; program sessizce devam eder. Aksi hâlde hata oluşur. Bu hata, dokuzuncu bölümdeki diğer hatalar gibi `dene` / `yakala` ile yakalanabilir:

```jus
dene:
    doğrula(1 > 2)
yakala h:
    yaz(h)
dene:
    doğrula(1 > 2, "bir ikiden büyük değil")
yakala h:
    yaz(h)
dene:
    eşit_olmalı([1, 2], [1, 3])
yakala h:
    yaz(h)
dene:
    eşit_olmalı("elma", "armut")
yakala h:
    yaz(h)
eşit_olmalı({"a": [1]}, {"a": [1]})
yaz("eşitler")
```

```
Doğrulama başarısız.
Doğrulama başarısız: bir ikiden büyük değil
Beklenen [1, 3], bulunan [1, 2].
Beklenen "armut", bulunan "elma".
eşitler
```

`eşit_olmalı(bulunan, beklenen)` karşılaştırmayı `==` gibi yapar: metinler, listeler ve sözlükler içerikleri aynıysa eşittir. Hata iletisi önce beklenen, sonra bulunan değeri gösterir. İlk argüman programınızın ürettiği değer, ikincisi sizin beklediğiniz değerdir.

Bu fonksiyonlar yalnızca testlerde değil, bir fonksiyonun girdilerini denetlemek için de işe yarar: `doğrula(n >= 0, "n negatif olamaz")`.

## Test dosyaları ve `jus test`

Testleri ayrı dosyalara yazarsınız. Kurallar şunlardır:

- Test dosyasının adı `_test.jus` ile biter: `hesap_test.jus`.
- Dosyadaki, adı `test_` ile başlayan ve parametresi olmayan her fonksiyon bir testtir.
- Bir test, hata vermeden biterse **geçti**, hata verirse **kaldı**. Bir testin kalması diğerlerinin çalışmasını engellemez.

Sınayacağımız `hesap.jus` modülü:

```jus
fonksiyon topla(a, b):
    dön a + b

fonksiyon böl(a, b):
    eğer b == 0:
        fırlat "Sıfıra bölünemez."
    dön a / b

fonksiyon ortalama(liste):
    değişken toplam = 0
    her n içinde liste:
        toplam += n
    dön toplam / uzunluk(liste)
```

Aynı klasörde `hesap_test.jus`:

```jus
kullan hesap

fonksiyon test_toplama():
    eşit_olmalı(hesap.topla(2, 3), 5)

fonksiyon test_bölme():
    eşit_olmalı(hesap.böl(10, 4), 2.5)

fonksiyon test_sıfıra_bölme_hata_verir():
    değişken hata_verdi = yanlış
    dene:
        hesap.böl(1, 0)
    yakala:
        hata_verdi = doğru
    doğrula(hata_verdi, "böl(1, 0) hata vermeliydi")

fonksiyon test_ortalama():
    eşit_olmalı(hesap.ortalama([2, 4, 6]), 4)
```

Test dosyası sınadığı modülü sıradan bir program gibi `kullan` ile yükler. Son testin yaptığını not edin: bir fonksiyonun **hata vermesini** sınamak istiyorsanız hatayı `dene` / `yakala` ile yakalar, yakalandığını `doğrula` ile denetlersiniz.

Testleri çalıştırmak için dosyaların bulunduğu klasörde:

```sh
jus test
```

```
./hesap_test.jus
  geçti  test_bölme
  geçti  test_ortalama
  geçti  test_sıfıra_bölme_hata_verir
  geçti  test_toplama

1 dosya, 4 test: 4 geçti, 0 kaldı.
```

Gözlemler:

- `jus test`, bulunduğu klasörde ve alt klasörlerinde `_test.jus` ile biten tüm dosyaları arar. Bir klasör yolu yazarsanız (`jus test testler`) orada arar.
- Her dosyanın önce üst düzey kodu çalışır, sonra testler **ad sırasıyla** çağrılır. Testlerin birbirine bağlı olmaması gerekir.
- Tüm testler geçerse çıkış kodu 0 olur; en az biri kalırsa 1. Böylece otomatik araçlar sonucu anlayabilir.

### Kalan test

`hesap.jus` içinde bir hata yapalım: `topla` fonksiyonunda `+` yerine yanlışlıkla `-` yazdık. Ayrıca bir test daha ekledik:

```jus
fonksiyon test_boş_liste_ortalaması():
    eşit_olmalı(hesap.ortalama([]), 0)
```

```sh
jus test
```

```
./hesap_test.jus
  KALDI  test_boş_liste_ortalaması
         ./hesap.jus:13: Sıfıra bölünemez.
  geçti  test_bölme
  geçti  test_ortalama
  geçti  test_sıfıra_bölme_hata_verir
  KALDI  test_toplama
         ./hesap_test.jus:4: Beklenen 5, bulunan -1.

1 dosya, 5 test: 3 geçti, 2 kaldı.
```

Kalan her testin altında nedeni yazılır:

- `test_toplama`, `eşit_olmalı`'dan gelen iletiyle kaldı: `5` bekliyorduk, `-1` bulduk. Satır numarası, hatayı veren test satırını gösterir.
- `test_boş_liste_ortalaması` ise `hesap.jus`'un 13. satırındaki bir çalışma zamanı hatası yüzünden kaldı: boş listenin ortalaması `0 / 0` olur. Bu, testin bulduğu gerçek bir eksiktir.

Diğer üç test, kalanlardan etkilenmeden çalıştı ve geçti. Hataları düzeltelim: `topla` yeniden `a + b` olsun ve `ortalama` boş listede 0 döndürsün:

```jus
fonksiyon ortalama(liste):
    eğer uzunluk(liste) == 0:
        dön 0
    değişken toplam = 0
    her n içinde liste:
        toplam += n
    dön toplam / uzunluk(liste)
```

```
./hesap_test.jus
  geçti  test_boş_liste_ortalaması
  geçti  test_bölme
  geçti  test_ortalama
  geçti  test_sıfıra_bölme_hata_verir
  geçti  test_toplama

1 dosya, 5 test: 5 geçti, 0 kaldı.
```

Kod değiştirdikçe testleri yeniden çalıştırmak, bir değişikliğin başka bir yeri bozup bozmadığını hemen gösterir.

## Biçimlendirici: `jus bicimle`

Kodun görünüşü de önemlidir: girinti, boşluklar, işleçlerin etrafındaki aralıklar. Bunları elle düzenlemek yerine `jus bicimle` komutu dosyaları **standart biçime** getirir. Standart biçimin kuralları şunlardır:

- Her blok düzeyi 4 boşluk girintilidir.
- İkili işleçlerin iki yanında birer boşluk, virgülden sonra bir boşluk bulunur; parantezlerin iç kenarında boşluk bulunmaz.
- Satır sonu yorumundan önce iki boşluk, `#` işaretinden sonra bir boşluk bulunur.
- Art arda en çok iki boş satır bulunur; satır sonlarında boşluk kalmaz; dosya tek bir satır sonuyla biter.

Biçimlendirici satırları bölmez ya da birleştirmez; metinlerin ve yorumların içeriğine dokunmaz. Programın davranışı değişmez.

Dağınık yazılmış bir `karisik.jus` dosyası:

```jus
değişken x=5
fonksiyon topla(a,b):
  dön a+b   
eğer x>3:
  yaz( "büyük",topla(x,1) )   #yorum
değilse:
  yaz("küçük")



yaz([1,2,3])
```

Önce yalnızca denetleyelim. `--denetle` dosyaları **değiştirmez**; biçimi standart olmayan dosyaların adını yazar ve 1 koduyla çıkar:

```sh
jus bicimle --denetle karisik.jus
```

```
biçimlendirilmeli: karisik.jus
```

Şimdi biçimlendirelim:

```sh
jus bicimle karisik.jus
```

```
biçimlendirildi: karisik.jus
```

Dosya artık şöyledir:

```jus
değişken x = 5
fonksiyon topla(a, b):
    dön a + b
eğer x > 3:
    yaz("büyük", topla(x, 1))  # yorum
değilse:
    yaz("küçük")


yaz([1, 2, 3])
```

Girintiler 4 boşluğa çıktı, işleçlerin ve virgüllerin çevresine boşluk geldi, parantez içindeki fazla boşluklar ve satır sonundaki boşluklar silindi, yorum `  # yorum` biçimine geldi. Üç boş satır ikiye indi. Program ise aynı çıktıyı verir (`büyük 6` ve `[1, 2, 3]`).

Biçimlendirdikten sonra `--denetle` komutu artık bir şey yazmaz ve 0 koduyla çıkar. Bu, "projedeki bütün dosyalar düzenli mi?" sorusunun cevabını otomatik araçlara verir:

```sh
jus bicimle --denetle hesap.jus hesap_test.jus
```

Birden çok dosya verebilirsiniz; biçimi standart olmayanların adları listelenir.

Sözcük düzeyinde hata içeren dosyalar (örneğin kapanmamış bir metin) biçimlendirilemez; komut nedenini yazar ve 65 koduyla çıkar:

```
kirik.jus:1: biçimlendirilemedi: Metin kapatılmamış; kapanış tırnağı (") eksik.
```

## Paketler: `jus paket`

Başkasının yazdığı kodu kullanmak için **paket** vardır. JUS paketleri **git depolarıdır**. Bir paket kurmak, depoyu bilgisayarınıza indirmek demektir. Bunun için bilgisayarınızda `git` kurulu olmalıdır.

| Komut | Açıklama |
|-------|----------|
| `jus paket kur <git-adresi> [ad]` | Depoyu `jus_paketleri/<ad>/` altına indirir. Ad verilmezse adresin son parçası kullanılır; baştaki `jus-` atılır |
| `jus paket listele` | Kurulu paketleri listeler |
| `jus paket kaldır <ad>` | Paketi siler |

Paketler, komutu çalıştırdığınız klasördeki `jus_paketleri/` klasörüne inen ve programınızın bulunduğu klasörden aranan sıradan klasörlerdir.

### Paket kurmak ve kullanmak

Diyelim ki biri `selam` adında küçük bir paket yazdı ve `https://github.com/kullanici/jus-selam` adresinde yayımladı. (Bu adres örnektir; gerçek bir depoya işaret etmez.) Paketin içinde, paketle aynı adı taşıyan `selam.jus` dosyası var:

```jus
fonksiyon selamla(ad):
    dön "Merhaba, " + ad + "!"

fonksiyon veda(ad):
    dön "Güle güle, " + ad + "."
```

Projenizin klasöründe kurun:

```sh
jus paket kur https://github.com/kullanici/jus-selam
```

```
'selam' paketi jus_paketleri/selam klasörüne kuruldu.
Kullanmak için: kullan selam
```

Ad olarak adresin son parçası olan `jus-selam` kullanıldı, ama baştaki `jus-` atıldığı için paketin adı `selam` oldu. Kurulu paketlere bakalım:

```sh
jus paket listele
```

```
selam
```

Şimdi paketi, kendi modülünüz gibi `kullan` ile kullanabilirsiniz. `ana.jus`:

```jus
kullan selam

yaz(selam.selamla("Ayşe"))
yaz(selam.veda("Ayşe"))
```

```
Merhaba, Ayşe!
Güle güle, Ayşe.
```

`kullan selam` yazıldığında JUS modülü şu sırayla arar (onuncu bölümde ilk ikisini görmüştünüz): standart kütüphane, programın klasöründeki `selam.jus`, kurulu paketler (`jus_paketleri/selam/selam.jus`). Paketin başka modülleri varsa `kullan "selam/modül"` ile kullanılır; bu, `jus_paketleri/selam/modül.jus` dosyasını yükler.

Paketi silmek için:

```sh
jus paket kaldır selam
```

```
'selam' paketi kaldırıldı.
```

Paket kaldırıldıktan sonra `ana.jus` çalışmaz; modül artık bulunamaz.

> Not: Bu bölümdeki paket çıktıları, yerel bir klasörde oluşturduğumuz git deposundan kurularak alındı; yukarıdaki GitHub adresi yalnızca gösterim içindir. Yerel bir klasör yolu da adres olarak verilebilir (`jus paket kur ../jus-selam`); git bu durumda `--depth is ignored in local clones` biçiminde zararsız bir uyarı yazabilir.

### Paketin adı ve ana modül

Paketin ana modülü, **paketin adını taşıyan dosyadır**: `selam` paketi için `selam.jus`. Kurulum sırasında farklı bir ad verebilirsiniz:

```sh
jus paket kur https://github.com/kullanici/jus-selam merhaba
```

```
'merhaba' paketi jus_paketleri/merhaba klasörüne kuruldu.
Uyarı: pakette 'merhaba.jus' dosyası yok; 'kullan merhaba' çalışmayacak. Paketin modülleri 'kullan "merhaba/<modül>"' ile kullanılabilir.
```

Paket, `jus_paketleri/merhaba/` klasörüne indi, ama içindeki dosya `selam.jus` olduğu için `kullan merhaba` çalışmaz. Modülü yine de `kullan "merhaba/selam"` ile yükleyebilirsiniz. Kurulum sırasında verilen uyarı bunu size baştan söyler.

Aynı adlı bir paket zaten kuruluysa yeniden kurmak için önce kaldırmanız gerekir:

```
jus: 'selam' paketi zaten kurulu. Yeniden kurmak için önce kaldırın: jus paket kaldır selam
```

Paket yöneticisi **sürüm seçmez** ve paketlerin birbirine bağımlılıklarını çözmez: her paket, deponun en son hâliyle kurulur. Bir pakete bağımlı olan başka bir paketi de kendiniz kurmanız gerekir.

## Sık yapılan hatalar

**Test fonksiyonuna parametre vermek.** Yalnızca parametresiz fonksiyonlar test sayılır. `fonksiyon test_x(a):` yazarsanız o fonksiyon çalıştırılmaz ve sayıma girmez; testinizin hiç çalışmadığını fark etmeyebilirsiniz.

**Test dosyasının adını yanlış yazmak.** `hesap_testi.jus` ya da `testhesap.jus` gibi adlar bulunamaz; ad tam olarak `_test.jus` ile bitmelidir. Hiç test dosyası bulunmazsa komut bunu söyler ve 66 koduyla çıkar:

```
jus: 'bos' altında adı _test.jus ile biten dosya bulunamadı.
```

**`eşit_olmalı` argümanlarının yerini karıştırmak.** İşlev iki yönde de aynı sonucu verir, ama hata iletisi "Beklenen ... bulunan ..." dediği için ilk argümanın sizin programınızın ürettiği, ikincisinin beklediğiniz değer olması gerekir. Karıştırırsanız ileti sizi yanıltır.

**`doğrula`'ya mantıksal olmayan bir değer vermek.**

```jus
doğrula(1)
```

```
ornek.jus:1: çalışma zamanı hatası: 'doğrula' mantıksal bir değer ister; sayı verildi.
```

Çözüm: `doğrula(n > 0)` gibi bir karşılaştırma yazın.

**Birbirine bağlı testler yazmak.** Bir test, başka bir testin bıraktığı duruma güvenmemelidir; sıra ad sırasıdır ve bir test kalabilir. Her test kendi verisini kurmalıdır.

**Paketi yanlış klasörde kurmak.** Paketler komutu çalıştırdığınız klasördeki `jus_paketleri/` içine iner ve programınızın klasöründeki `jus_paketleri/` içinde aranır. Programınızdan farklı bir klasörde kurarsanız `kullan` modülü bulamaz.

**Paket adı ile dosya adının uyuşmaması.** Yukarıda gördüğünüz `merhaba` örneğindeki gibi, `kullan paket` yazabilmek için pakette `paket.jus` dosyası bulunmalıdır.

## Alıştırmalar

1. `hesap.jus` içine `en_büyük(liste)` fonksiyonunu ekleyin (boş listede `boş` döndürsün). `hesap_test.jus` dosyasına iki test ekleyin: `[3, 9, 4]` listesi için `9`, boş liste için `boş`.
2. `yazi.jus` modülüne büyük harfle başlatan `baştanBüyüt(m)` fonksiyonunu yazın ve `yazi_test.jus` dosyasında `"ışıl"` ile `"istanbul"` için sonuçları (`Işıl`, `İstanbul`) sınayın.
3. İçinde dağınık girinti ve boşluk bulunan küçük bir dosya yazın. `--denetle` ile denetleyin, biçimlendirin, tekrar denetleyin.
4. `jus-arac` adında yerel bir git deposu oluşturun; içine `kare(n)` fonksiyonu olan `arac.jus` dosyasını koyup commit edin. Başka bir klasörde bu depoyu `jus paket kur` ile kurup `kullan arac` ile kullanın.

## Çözümler

**1.** `hesap.jus` dosyasına:

```jus
fonksiyon en_büyük(liste):
    eğer uzunluk(liste) == 0:
        dön boş
    değişken en = liste[0]
    her n içinde liste:
        eğer n > en:
            en = n
    dön en
```

`hesap_test.jus` dosyasına:

```jus
fonksiyon test_en_büyük():
    eşit_olmalı(hesap.en_büyük([3, 9, 4]), 9)

fonksiyon test_en_büyük_boş_liste():
    eşit_olmalı(hesap.en_büyük([]), boş)
```

```sh
jus test
```

```
./hesap_test.jus
  geçti  test_boş_liste_ortalaması
  geçti  test_bölme
  geçti  test_en_büyük
  geçti  test_en_büyük_boş_liste
  geçti  test_ortalama
  geçti  test_sıfıra_bölme_hata_verir
  geçti  test_toplama

1 dosya, 7 test: 7 geçti, 0 kaldı.
```

**2.** `yazi.jus`:

```jus
fonksiyon baştanBüyüt(m):
    dön büyük_harf(m[0]) + m[1:]
```

`yazi_test.jus`:

```jus
kullan yazi

fonksiyon test_türkçe_harf():
    eşit_olmalı(yazi.baştanBüyüt("ışıl"), "Işıl")
    eşit_olmalı(yazi.baştanBüyüt("istanbul"), "İstanbul")

fonksiyon test_tek_harf():
    eşit_olmalı(yazi.baştanBüyüt("a"), "A")
```

Sınamayı yalnızca bu dosya için görmek amacıyla ayrı bir klasörde çalıştırırsanız:

```
./yazi_test.jus
  geçti  test_tek_harf
  geçti  test_türkçe_harf

1 dosya, 2 test: 2 geçti, 0 kaldı.
```

**3.** Örneğin `alistirma.jus`:

```jus
değişken a=1
eğer a==1:
      yaz( "bir" )
```

```sh
jus bicimle --denetle alistirma.jus
jus bicimle alistirma.jus
jus bicimle --denetle alistirma.jus
```

İlk komut `biçimlendirilmeli: alistirma.jus` yazıp 1 koduyla çıkar; ikincisi `biçimlendirildi: alistirma.jus` yazar; üçüncüsü bir şey yazmaz ve 0 koduyla çıkar. Dosya şu hâle gelir:

```jus
değişken a = 1
eğer a == 1:
    yaz("bir")
```

**4.** Komutlar (`git` kurulu olmalıdır):

```sh
mkdir jus-arac
cd jus-arac
git init
```

`arac.jus` dosyasının içeriği:

```jus
fonksiyon kare(n):
    dön n * n
```

```sh
git add arac.jus
git commit -m "ilk sürüm"
cd ..
mkdir proje
cd proje
jus paket kur ../jus-arac
```

```
'arac' paketi jus_paketleri/arac klasörüne kuruldu.
Kullanmak için: kullan arac
```

`proje` klasöründeki `ana.jus`:

```jus
kullan arac

yaz(arac.kare(7))
```

```
49
```

---

Önceki: [14 - Ağ ve HTTP](14-ag-ve-http.md) | [İçindekiler](README.md)
