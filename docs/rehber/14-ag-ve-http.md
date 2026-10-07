# 14 - Ağ ve HTTP

Önceki: [13 - JSON ve veri](13-json-ve-veri.md) | [İçindekiler](README.md) | Sonraki: [15 - Test, biçim ve paket](15-test-bicim-paket.md)

Web siteleri, mobil uygulamalar ve birçok program birbiriyle **HTTP** ile konuşur. HTTP'de iki taraf vardır: **istemci** bir istek gönderir, **sunucu** bu isteğe yanıt verir. Tarayıcınız bir sayfayı açtığında istemcidir; sayfayı veren bilgisayar sunucudur.

JUS'un `http` modülü hem istemci hem sunucu olarak çalışabilir. Bu bölümde ikisini de kullanacaksınız.

Başlamadan önce üç not:

- `http` modülü yalnızca şifresiz `http://` adreslerini destekler. `https://` adresleri hata verir (bölüm sonunda gösterilmiştir).
- Bu bölümdeki bütün örnekler kendi bilgisayarınızla konuşur: adres her yerde `127.0.0.1`'dir; bu, "bu bilgisayar" demektir. Dış internete hiçbir istek gönderilmez.
- Sunucu örnekleri bir terminalde çalışır ve orada bekler. İstemciyi **ikinci bir terminalde** çalıştırmanız gerekir. İkisi aynı bilgisayarda olduğu için sorun olmaz.

## En küçük sunucu

`http.sun(port, işleyici)` bir sunucu başlatır. **Port**, bilgisayarınızdaki bir "kapı numarasıdır"; aynı anda her port yalnızca bir program tarafından kullanılabilir. Biz 8085'i kullanacağız. **İşleyici**, sizin yazdığınız bir fonksiyondur: sunucu her istek geldiğinde onu çağırır, işleyicinin döndürdüğü değeri yanıt olarak gönderir.

`sunucu1.jus`:

```jus
kullan http

fonksiyon işle(istek):
    yaz("İstek geldi:", istek["yöntem"], istek["yol"])
    dön "Merhaba!"

yaz("Sunucu hazır: http://127.0.0.1:8085")
http.sun(8085, işle)
```

Birinci terminalde çalıştırın:

```sh
jus sunucu1.jus
```

```
Sunucu hazır: http://127.0.0.1:8085
```

Program burada bekler; `http.sun` istek gelmesini bekleyen bir döngüdür ve kendiliğinden bitmez. Durdurmak için `Ctrl+C` kullanın. İşleyici bir metin döndürürse bu metin yanıtın gövdesi olur ve durum kodu 200 ("tamam") sayılır.

## HTTP istemcisi: `http.getir`

İkinci terminalde `istemci1.jus` dosyasını yazın:

```jus
kullan http

değişken yanıt = http.getir("http://127.0.0.1:8085/")
yaz(yanıt)
yaz(yanıt["durum"])
yaz(yanıt["gövde"])
yaz(yanıt["başlıklar"]["content-type"])
```

```sh
jus istemci1.jus
```

```
{"durum": 200, "başlıklar": {"content-type": "text/plain; charset=utf-8", "content-length": "8", "connection": "close"}, "gövde": "Merhaba!"}
200
Merhaba!
text/plain; charset=utf-8
```

`http.getir(adres)` bir GET isteği gönderir ve **yanıt sözlüğünü** döndürür. Sözlüğün üç anahtarı vardır:

| Anahtar | Değer |
|---------|-------|
| `durum` | Durum kodu (sayı): 200 tamam, 404 bulunamadı, 500 sunucu hatası gibi |
| `başlıklar` | Yanıt başlıkları (sözlük); adlar küçük harflidir |
| `gövde` | Yanıtın içeriği (metin) |

Birinci terminale bakarsanız sunucunun isteği gördüğünü fark edersiniz:

```
İstek geldi: GET /
```

Bu örneklerle işiniz bitince birinci terminalde `Ctrl+C` ile sunucuyu durdurun.

İstek göndermenin üç yolu vardır:

| Fonksiyon | Açıklama |
|-----------|----------|
| `http.getir(adres)` | GET isteği: bir şeyi okumak için |
| `http.gönder(adres, gövde)` | POST isteği: sunucuya bir şey göndermek için |
| `http.iste(yöntem, adres, başlıklar, gövde)` | Herhangi bir yöntemle istek; `başlıklar` bir sözlüktür |

## Yol, sorgu ve durum kodu

Gerçek bir sunucu, istenen **yola** göre farklı yanıt verir. Adres `http://127.0.0.1:8085/selam?ad=Ayşe` olsun. Burada `/selam` **yol**, `?` işaretinden sonrası **sorgu**dur (`ad=Ayşe`). İşleyiciye verilen `istek` sözlüğü bunları ayrı anahtarlarla taşır:

| Anahtar | Değer |
|---------|-------|
| `yöntem` | `"GET"`, `"POST"` gibi |
| `yol` | `"/selam"` |
| `sorgu` | `{"ad": "Ayşe"}` (sözlük; değerler metindir) |
| `başlıklar` | İstek başlıkları (sözlük; adlar küçük harflidir) |
| `gövde` | İsteğin gövdesi (metin) |

İşleyici, metin yerine bir **sözlük** döndürerek yanıtı ayrıntılı belirleyebilir. Sözlükte şu anahtarlardan istediğinizi yazarsınız:

- `durum`: durum kodu (yazmazsanız 200)
- `gövde`: yanıtın içeriği
- `başlıklar`: ek yanıt başlıkları (sözlük)
- `durdur`: `doğru` ise sunucu o yanıttan sonra kapanır

Şimdi bu bilgileri kullanan, küçük bir not sunucusu yazalım. `sunucu.jus`:

```jus
kullan http
kullan json

değişken notlar = []

fonksiyon işle(istek):
    değişken yol = istek["yol"]

    eğer yol == "/":
        dön "JUS not sunucusu"

    eğer yol == "/selam":
        dön "Merhaba, " + al(istek["sorgu"], "ad", "dünya") + "!"

    eğer yol == "/topla":
        dene:
            değişken a = sayı(istek["sorgu"]["a"])
            değişken b = sayı(istek["sorgu"]["b"])
            dön metin(a + b)
        yakala:
            dön {"durum": 400, "gövde": "a ve b sayı olarak verilmeli"}

    eğer yol == "/not":
        eğer istek["yöntem"] == "POST":
            ekle(notlar, istek["gövde"])
            dön {"durum": 201, "gövde": "Kaydedildi: " + metin(uzunluk(notlar))}
        dön json.yaz(notlar)

    eğer yol == "/bölme":
        dön 1 / 0

    eğer yol == "/durdur":
        dön {"durum": 200, "gövde": "Güle güle", "durdur": doğru}

    dön {"durum": 404, "gövde": "Bulunamadı: " + yol, "başlıklar": {"x-sunucu": "jus"}}

yaz("Sunucu hazır: http://127.0.0.1:8085")
http.sun(8085, işle)
yaz("Sunucu durdu")
```

Nasıl çalışır:

- İşleyici yola göre dallanır. Hiçbir yola uymayan istekler son satırdaki `404` yanıtını alır; bu yanıtla birlikte özel bir başlık (`x-sunucu`) da gönderilir.
- `/selam` yolunda `al(istek["sorgu"], "ad", "dünya")`, `ad` parametresi yoksa `"dünya"` değerini kullanır (sekizinci bölüm).
- `/topla` yolunda sayıya çevirme başarısız olursa `dene` / `yakala` devreye girer ve **400** ("hatalı istek") durum kodu döner.
- `/not` yolu, aynı yola gelen GET ve POST isteklerini `istek["yöntem"]` ile ayırır. POST ile gelen gövde `notlar` listesine eklenir ve **201** ("oluşturuldu") döner. GET ise listeyi JSON olarak verir (13. bölüm).
- `/bölme` yolu bilerek hata verir; aşağıda ne olduğunu göreceksiniz.
- `/durdur` yolu `"durdur": doğru` döndürür; sunucu yanıtı gönderdikten sonra kapanır ve `http.sun` çağrısından sonraki satır (`Sunucu durdu`) çalışır.

Sunucuyu birinci terminalde başlatın (`jus sunucu.jus`), ikinci terminalde istemci betiklerini çalıştırın. `istemci2.jus`:

```jus
kullan http
kullan ağ

değişken ana = "http://127.0.0.1:8085"

fonksiyon göster(yanıt):
    yaz(yanıt["durum"], yanıt["gövde"])

göster(http.getir(ana + "/"))
göster(http.getir(ana + "/selam"))
göster(http.getir(ana + "/selam?ad=Ayşe"))
göster(http.getir(ana + "/selam?ad=" + ağ.url_kodla("Şükrü Çağlar")))
göster(http.getir(ana + "/topla?a=2&b=3"))
göster(http.getir(ana + "/topla?a=2&b=üç"))
göster(http.getir(ana + "/olmayan"))
```

```
200 JUS not sunucusu
200 Merhaba, dünya!
200 Merhaba, Ayşe!
200 Merhaba, Şükrü Çağlar!
200 5
400 a ve b sayı olarak verilmeli
404 Bulunamadı: /olmayan
```

Adreste boşluk ve `&` gibi özel karakterler bulunabilecekse değeri `ağ.url_kodla` ile kodlayın; sunucu tarafında sorgu değerleri otomatik olarak çözülür (`Şükrü Çağlar` yeniden okunabilir hâliyle geldi).

Sunucuyu kapatmadan ikinci betiği çalıştırın. `istemci3.jus`:

```jus
kullan http

değişken ana = "http://127.0.0.1:8085"

değişken yanıt = http.getir(ana + "/olmayan")
yaz(anahtarlar(yanıt))
yaz(yanıt["başlıklar"]["x-sunucu"])

yaz(http.gönder(ana + "/not", "Ekmek al")["gövde"])
yaz(http.gönder(ana + "/not", "Çiçekleri sula")["gövde"])
yaz(http.getir(ana + "/not")["gövde"])

değişken y2 = http.iste("PUT", ana + "/olmayan", {"x-deneme": "1"}, "gövde")
yaz(y2["durum"], y2["gövde"])

değişken hatalı = http.getir(ana + "/bölme")
yaz(hatalı["durum"], hatalı["gövde"])

yaz(http.getir(ana + "/durdur")["gövde"])

dene:
    http.getir(ana + "/")
yakala hata:
    yaz("Sunucuya ulaşılamadı:", hata)
```

```
["durum", "başlıklar", "gövde"]
jus
Kaydedildi: 1
Kaydedildi: 2
["Ekmek al","Çiçekleri sula"]
404 Bulunamadı: /olmayan
500 Sunucu hatası: Sıfıra bölünemez.
Güle güle
Sunucuya ulaşılamadı: '127.0.0.1' sunucusunun 8085 numaralı portuna bağlanılamadı.
```

Gözlemler:

- Sunucunun gönderdiği `x-sunucu` başlığı, istemcide `yanıt["başlıklar"]["x-sunucu"]` ile okundu.
- `http.gönder` POST gönderdi; sunucu 201 ile yanıtladı. Bu betiğin gövdeyi yazdırdığına dikkat edin: durum kodunu görmek için `yanıt["durum"]` yazdırılır.
- `http.iste` ile PUT yöntemi gönderdik ve kendi başlığımızı (`x-deneme`) ekledik.
- İşleyicideki yakalanmamış hata (`/bölme`'deki sıfıra bölme) **sunucuyu durdurmadı**. İstemci 500 ("sunucu hatası") yanıtı aldı, sunucu çalışmayı sürdürdü: sonraki istekler hâlâ yanıtlandı.
- `/durdur` isteğinden sonra sunucu kapandı. Son `http.getir` bağlanamadı ve bir hata verdi; bunu `dene` / `yakala` ile karşıladık. Sunucuya ulaşılamamak ağ programlarında her zaman olası bir durumdur.

Birinci terminalde sunucunun kendi çıktısı şöyledir:

```
Sunucu hazır: http://127.0.0.1:8085
Sunucu durdu
```

Sunucu istekleri **sırayla, birer birer** işler: bir istek bitmeden sonraki başlamaz. Küçük araçlar için yeterlidir; işleyiciniz uzun sürüyorsa diğer istekler bekler.

### İstek sözlüğünü görmek

İşleyiciye gelen `istek` sözlüğünün tamamını görmek için onu JSON olarak yanıta koyabilirsiniz. `bilgi.jus`:

```jus
kullan http
kullan json

fonksiyon işle(istek):
    dön {"gövde": json.yaz(istek, 2), "durdur": doğru}

http.sun(8085, işle)
```

`istemci4.jus`:

```jus
kullan http

değişken yanıt = http.gönder("http://127.0.0.1:8085/bilgi?ad=Ayşe&yaş=29", "örnek gövde")
yaz(yanıt["gövde"])
```

```
{
  "yöntem": "POST",
  "yol": "/bilgi",
  "sorgu": {
    "ad": "Ayşe",
    "yaş": "29"
  },
  "başlıklar": {
    "host": "127.0.0.1",
    "connection": "close",
    "content-length": "13",
    "content-type": "text/plain; charset=utf-8"
  },
  "gövde": "örnek gövde"
}
```

Sorgu değerlerinin hep **metin** olduğuna dikkat edin: `yaş` değeri `29` değil `"29"`dur. Sayı olarak kullanmak için `sayı()` ile çevirmeniz gerekir; `/topla` yolunda yaptığımız gibi. `content-length` gövdenin bayt cinsinden uzunluğudur: `örnek gövde` 11 karakterdir ama Türkçe harfler iki bayt tuttuğu için 13 bayttır.

## `https` desteklenmez

`http` modülü şifreli bağlantıları desteklemez. Bir `https://` adresi verirseniz hata alırsınız:

```jus
kullan http

dene:
    http.getir("https://127.0.0.1/")
yakala hata:
    yaz(hata)
```

```
http modülü şifreli (https) bağlantıları desteklemiyor: https://127.0.0.1/
```

Bu yüzden `http` modülü kendi bilgisayarınızda ya da güvendiğiniz bir yerel ağda araç yazmak için uygundur; şifreli bağlantı gerektiren sitelerle konuşamaz.

## `ağ` modülü: bir alt katman

`http`, bir alt katmanın üzerine kuruludur: `ağ` modülü. `ağ` modülü doğrudan **TCP bağlantıları** ile çalışır: bağlanmak (`ağ.bağlan`), bağlantı beklemek (`ağ.dinle`, `ağ.kabul_et`), metin göndermek ve okumak (`ağ.gönder`, `ağ.satır_al`) ve kapatmak (`ağ.kapat`). HTTP gibi bir kuralı kendiniz yazmak ya da başka bir protokolle konuşmak isterseniz kullanılır. Bu rehberde ayrıntıya girmiyoruz; yalnızca ne olduğunu görmek için tek programda hem sunucu hem istemci rolünü oynayan küçük bir örnek:

```jus
kullan ağ

yaz(ağ.url_kodla("Şükrü Çağlar & oğlu"))
yaz(ağ.url_çöz("%C5%9E%C3%BCkr%C3%BC+%C3%87a%C4%9Flar"))
yaz(uzunluk("Şükrü"), ağ.bayt_sayısı("Şükrü"))

değişken dinleyici = ağ.dinle(0)
değişken port = ağ.port(dinleyici)
yaz(port > 0)

değişken istemci = ağ.bağlan("127.0.0.1", port)
değişken sunucuTarafı = ağ.kabul_et(dinleyici)

ağ.gönder(istemci, "merhaba sunucu\n")
yaz("Sunucu aldı:", ağ.satır_al(sunucuTarafı))
ağ.gönder(sunucuTarafı, "merhaba istemci\n")
yaz("İstemci aldı:", ağ.satır_al(istemci))

ağ.kapat(istemci)
ağ.kapat(sunucuTarafı)
ağ.kapat(dinleyici)
```

```
%C5%9E%C3%BCkr%C3%BC%20%C3%87a%C4%9Flar%20%26%20o%C4%9Flu
Şükrü Çağlar
5 8
doğru
Sunucu aldı: merhaba sunucu
İstemci aldı: merhaba istemci
```

`ağ.dinle(0)` boş bir port seçer; hangisi seçildiğini `ağ.port` söyler. `ağ.bayt_sayısı` bir metnin bayt cinsinden uzunluğunu verir. Ağ işlemleri **bekletir**: `ağ.kabul_et` ve okuma fonksiyonları sonuç gelene kadar programı durdurur. `http` ile işiniz görülüyorsa `ağ`'a gerek kalmaz.

## Sık yapılan hatalar

**Sunucu çalışmıyorken istek göndermek.** Sunucu başlamadan ya da kapandıktan sonra istek atarsanız bağlantı kurulamaz:

```jus
kullan http

yaz(http.getir("http://127.0.0.1:8085/"))
```

```
http:150: çalışma zamanı hatası: '127.0.0.1' sunucusunun 8085 numaralı portuna bağlanılamadı.
    satır 150, 'iste' fonksiyonu
    satır 163, 'getir' fonksiyonu
    satır 3, ana program (ornek.jus)
```

Önce sunucunun ayrı bir terminalde çalıştığından emin olun; ağ kodunu `dene` / `yakala` içine alın.

**İstemci ile sunucuyu aynı terminalde çalıştırmak.** `http.sun` bekler ve program bitmez; istemciyi aynı terminalde çalıştıramazsınız. İkinci bir terminal açın.

**Adresin başına `http://` yazmamak.** `http.getir("merhaba")` bir adres olarak kabul edilmez; adres `http://` ile başlamalıdır.

**Sorgu değerini sayı sanmak.** `istek["sorgu"]["n"]` her zaman metindir; hesapta kullanmadan önce `sayı()` ile çevirin ve çevrilemeyen durumu `dene` ile karşılayın.

**Yanıtın `gövde`'sini JSON olarak kullanmak için çözmemek.** Gövde her zaman metindir. JSON gönderen bir sunucudan gelen veriyi sözlük olarak kullanmak için `json.çöz(yanıt["gövde"])` yazın (13. bölüm).

**Aynı portu iki sunucuda kullanmak.** Bir port normalde tek programa ayrılır. Önceki sunucunuz hâlâ çalışırken aynı portta yenisini başlatırsanız `8085 numaralı port dinlenemiyor; başka bir program kullanıyor olabilir.` gibi bir hata alırsınız. Eskisini durdurun ya da başka bir port seçin.

## Alıştırmalar

1. `/kare?n=7` isteğine `49` yanıtı veren, `n` sayı değilse 400 durum kodu ve `n sayı olmalı` gövdesi döndüren bir sunucu yazın. `/ters?m=merhaba` isteğine metnin tersini (`abahrem`) versin; bilinmeyen yollara 404 döndürsün; `/durdur` ile kapansın.
2. Birinci alıştırmadaki sunucuyu bir istemci betiğiyle deneyin: dört yolu sırayla isteyip her biri için `yol -> durum gövde` biçiminde yazdırın.
3. Her isteğe `İstek sayısı: N` diye yanıt veren, üçüncü istekte `durdur` ile kapanan bir sayaç sunucusu yazın. Sunucu kapanınca `Sunucu 3 istekten sonra durdu` yazdırsın.
4. Üçüncü alıştırmanın sunucusuna üç GET isteği gönderen bir istemci yazın.

## Çözümler

Her çözümde sunucuyu bir terminalde, istemciyi ikinci bir terminalde çalıştırın.

**1.** `c1.jus` (sunucu):

```jus
kullan http

fonksiyon işle(istek):
    eğer istek["yol"] == "/kare":
        dene:
            değişken n = sayı(istek["sorgu"]["n"])
            dön metin(n * n)
        yakala:
            dön {"durum": 400, "gövde": "n sayı olmalı"}
    eğer istek["yol"] == "/ters":
        dön ters(al(istek["sorgu"], "m", ""))
    eğer istek["yol"] == "/durdur":
        dön {"gövde": "kapandı", "durdur": doğru}
    dön {"durum": 404, "gövde": "yok"}

http.sun(8085, işle)
```

**2.** `c1i.jus` (istemci):

```jus
kullan http

değişken ana = "http://127.0.0.1:8085"
her yol içinde ["/kare?n=7", "/kare?n=x", "/ters?m=merhaba", "/olmayan", "/durdur"]:
    değişken y = http.getir(ana + yol)
    yaz(yol, "->", y["durum"], y["gövde"])
```

```
/kare?n=7 -> 200 49
/kare?n=x -> 400 n sayı olmalı
/ters?m=merhaba -> 200 abahrem
/olmayan -> 404 yok
/durdur -> 200 kapandı
```

**3.** `c3.jus` (sunucu):

```jus
kullan http

değişken sayaç = 0

fonksiyon işle(istek):
    sayaç += 1
    yaz("Sayaç:", sayaç)
    eğer sayaç == 3:
        dön {"gövde": "Bu son istekti: " + metin(sayaç), "durdur": doğru}
    dön "İstek sayısı: " + metin(sayaç)

http.sun(8085, işle)
yaz("Sunucu 3 istekten sonra durdu")
```

Sunucu terminalinde:

```
Sayaç: 1
Sayaç: 2
Sayaç: 3
Sunucu 3 istekten sonra durdu
```

`sayaç` bir genel değişkendir; işleyici fonksiyon onu değiştirebilir ve değeri istekler arasında korunur.

**4.** `c3i.jus` (istemci):

```jus
kullan http

her i içinde aralık(3):
    yaz(http.getir("http://127.0.0.1:8085/")["gövde"])
```

```
İstek sayısı: 1
İstek sayısı: 2
Bu son istekti: 3
```

---

Önceki: [13 - JSON ve veri](13-json-ve-veri.md) | [İçindekiler](README.md) | Sonraki: [15 - Test, biçim ve paket](15-test-bicim-paket.md)
