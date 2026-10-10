# Hafta 5 - Fonksiyonlar

[Öğretmen kiti](README.md) | Önceki: [Hafta 4](hafta-04.md) | Sonraki: [Hafta 6 - Listeler](hafta-06.md)

İlgili rehber bölümü: [05 - Fonksiyonlar](../rehber/05-fonksiyonlar.md)

Öğrenciler `yaz`, `tür`, `aralık` gibi hazır fonksiyonları zaten kullanıyor. Bu hafta kendi fonksiyonlarını yazarlar. Haftanın iki ana fikri: (1) fonksiyon, kodu bir ada bağlayıp tekrar tekrar kullanmaktır; (2) `dön` ile `yaz` aynı şey değildir: `yaz` ekrana gösterir, `dön` değeri çağırana geri verir. İkincisi bu haftanın en sık yanlış anlaşılan noktasıdır.

Bu hafta sonunda [değerlendirme.md](degerlendirme.md) içindeki 1. sınavı uygulayabilirsiniz (Hafta 1-5 konuları).

## Kazanımlar

Hafta sonunda öğrenci:

1. `fonksiyon` ile bir ya da birkaç parametreli fonksiyon tanımlayıp farklı argümanlarla çağırabilir.
2. `dön` ile değer döndürmeyi, `yaz` ile ekrana yazdırmayı birbirinden ayırt edip bir örnekle açıklayabilir.
3. Bir fonksiyonun içinde tanımlanan değişkenin fonksiyonun dışında görünmediğini gösterebilir.
4. Birden çok yerde tekrar eden bir kod parçasını fonksiyona çevirebilir.
5. Döngü ve karar içeren bir fonksiyonu (asal mı, ebob) yazabilir.

## Ön hazırlık

- Tarayıcıda deneme alanı: <https://kerimaksanaoglu.github.io/jus/>.
- Etkinlik için: küçük kâğıt kartlar (öğrenci başına iki üç tane) ve "makine" kartları (aşağıda).
- Bu haftanın sonunda sınav yapacaksanız sınav kâğıtlarını hazırlayın.
- Hafta 4'te yazılan `asal mı` ve `toplam` programlarını ısınmada geri çağırın; fonksiyonla yeniden yazılacak.

## Ders akışı

| Süre | Evre | Ne yapılır |
|------|------|------------|
| 0-8 | Isınma | Aynı 3 satırlık "selamlama" kodunu tahtada üç kez yazın (üç farklı isim için). "Dördüncü kişi için yine mi yazacağız?" |
| 8-14 | Yeni kavram | Fonksiyon = ada bağlanmış, tekrar kullanılabilir talimat paketi. Tanım ve çağrı ayrımı. Parametre (tanımda), argüman (çağrıda). |
| 14-28 | Canlı kodlama 1 | Adım 1-4: `selamla`, `kare`, `dön`, `yaz` ile `dön` farkı. |
| 28-40 | Etkinlik | "Fonksiyon makinesi" (bilgisayarsız). |
| 40-50 | Canlı kodlama 2 | Adım 5-7: karar döndüren fonksiyon, kapsam, (isteğe bağlı) özyineleme. |
| 50-72 | Öğrenci uygulaması | Alıştırma 5.1-5.6, eşli. Hızlılar 5.7-5.9'a geçer. |
| 72-76 | Paylaşım | Bir çift `ebob` ya da `asalMı` çözümünü anlatır. |
| 76-80 | Kapanış | Çıkış bileti. Sınav duyurusu. |

## Canlı kodlama betiği

### Adım 1 - Tekrardan fonksiyona

Önce tekrar eden kodu yazın:

```jus
yaz("Merhaba, Ayşe!")
yaz("Bugün hava güzel.")
yaz("Merhaba, Mehmet!")
yaz("Bugün hava güzel.")
```

```
Merhaba, Ayşe!
Bugün hava güzel.
Merhaba, Mehmet!
Bugün hava güzel.
```

Sorun: "Neyi iki kere yazdım? Üçüncü bir kişi eklesek?" Ardından fonksiyona dönüştürün:

```jus
fonksiyon selamla(ad):
    yaz("Merhaba, " + ad + "!")
    yaz("Bugün hava güzel.")

selamla("Ayşe")
selamla("Mehmet")
selamla("Can")
```

```
Merhaba, Ayşe!
Bugün hava güzel.
Merhaba, Mehmet!
Bugün hava güzel.
Merhaba, Can!
Bugün hava güzel.
```

Söyleyin: "`fonksiyon` ile bir iş tanımlıyoruz. `ad` bir **parametre**: işin dışarıdan istediği bilgi. `selamla("Ayşe")` satırı işi **çağırır**; `"Ayşe"` bu çağrıdaki **argümandır**. Tanım tek başına hiçbir şey yazdırmaz; çağırınca çalışır."

### Adım 2 - Değer döndürmek: `dön`

```jus
fonksiyon kare(x):
    dön x * x

yaz(kare(7))
yaz(kare(1.5))
yaz(kare(3) + kare(4))
değişken alan = kare(12)
yaz("Alan:", alan)
```

```
49
2.25
25
Alan: 144
```

Söyleyin: "`dön` hesabın sonucunu çağırana geri verir; `kare(3)` ifadesi çağrıldığı yerde `9` olur. Bu yüzden `kare(3) + kare(4)` yazabildik."

### Adım 3 - `yaz` ile `dön` aynı şey değil

Aynı işi iki biçimde yazın:

```jus
fonksiyon topla_yaz(a, b):
    yaz(a + b)

fonksiyon topla_dön(a, b):
    dön a + b

değişken s1 = topla_yaz(2, 3)
değişken s2 = topla_dön(2, 3)
yaz("s1 =", s1)
yaz("s2 =", s2)
yaz(topla_dön(2, 3) * 10)
```

```
5
s1 = boş
s2 = 5
50
```

Beklenen yorum: `topla_yaz` ekrana `5` yazdı ama değişkene hiçbir şey vermedi; `s1` `boş` kaldı. `topla_dön` ise sonucu değişkene verdi, hesapta kullanılabildi. Kural: bir fonksiyonun sonucu başka bir yerde kullanılacaksa `dön`; yalnızca göstermek içinse `yaz`.

### Adım 4 - Karar döndüren fonksiyon

```jus
fonksiyon çiftMi(n):
    dön n % 2 == 0

yaz(çiftMi(4))
yaz(çiftMi(7))

eğer çiftMi(10):
    yaz("10 çifttir.")
```

```
doğru
yanlış
10 çifttir.
```

Söyleyin: "`n % 2 == 0` zaten `doğru` ya da `yanlış` olduğu için doğrudan döndürdük. `eğer` koşulu olarak da kullanabiliyoruz." (Fonksiyon adında Türkçe harf ve büyük harf kullanılabilir; ama okunur kalmasına özen gösterin.)

### Adım 5 - Birden çok parametre ve birden çok `dön`

```jus
fonksiyon notHarfi(puan):
    eğer puan >= 85:
        dön "Pekiyi"
    değilse eğer puan >= 70:
        dön "İyi"
    değilse eğer puan >= 45:
        dön "Geçer"
    dön "Zayıf"

yaz(notHarfi(90))
yaz(notHarfi(72))
yaz(notHarfi(50))
yaz(notHarfi(30))
```

```
Pekiyi
İyi
Geçer
Zayıf
```

Söyleyin: "`dön` fonksiyonu **hemen bitirir**. İlk `dön` çalışınca altındaki satırlara bakılmaz. Bu yüzden son `dön` için `değilse` yazmaya gerek kalmadı." Sorun: "Hafta 3'te aynı iş için `yaz` kullanıyorduk. Şimdi fark ne?" (Sonucu başka yerde kullanabiliriz.)

### Adım 6 - Kapsam: yerel ve genel

```jus
değişken genel = "ben geneldim"

fonksiyon göster():
    değişken yerel = "ben yerelim"
    yaz(genel)
    yaz(yerel)

göster()
yaz(genel)
```

```
ben geneldim
ben yerelim
ben geneldim
```

Söyleyin: "Fonksiyonun içinden dışarıdaki `genel`i okuyabildik. Ama içeride tanımlanan `yerel` yalnızca orada yaşar." Sonra `göster()` çağrısından sonra `yaz(yerel)` yazıp çalıştırın; alınan hata aşağıda "Öğrenciler nerede takılır" bölümünün 5. maddesindedir. Genel değişkenleri fonksiyondan değiştirmekten kaçınmayı öğütleyin: fonksiyona bilgiyi parametreyle verin, sonucu `dön` ile alın.

### Adım 7 - İsteğe bağlı: kendini çağıran fonksiyon

```jus
fonksiyon faktöriyel(n):
    eğer n <= 1:
        dön 1
    dön n * faktöriyel(n - 1)

yaz(faktöriyel(5))
yaz(faktöriyel(10))
```

```
120
3628800
```

Söyleyin: "`faktöriyel(5)`, `5 * faktöriyel(4)` demek; o da `4 * faktöriyel(3)`... `faktöriyel(1)` doğrudan 1 verir ve zincir geri çözülür. `n <= 1` olmasaydı sonsuza kadar kendini çağırırdı." Bu adım yalnızca ilgili öğrenciler içindir; sınavda sorulmaz.

## Sınıf içi etkinlik: Fonksiyon makinesi (bilgisayarsız, 12 dk)

Amaç: Fonksiyonu "girdi alıp çıktı veren kapalı kutu" olarak yaşamak; `dön` ile `yaz` farkını sezdirmek.

Hazırlık: Dört öğrenci "makine" olur; her birine tarif kartı verin:

| Makine | Kartın üzerinde yazan |
|--------|-----------------------|
| `kare` | Gelen sayıyı kendisiyle çarp, sonucu söyle. |
| `ikiKat` | Gelen sayıyı 2 ile çarp, sonucu söyle. |
| `çiftMi` | Gelen sayı çiftse "doğru", değilse "yanlış" de. |
| `selamla` | Gelen isme "Merhaba, ..." de. **Hiçbir şey geri vermez, yalnızca yüksek sesle söyler.** |

Adımlar:

1. Diğer öğrenciler kâğıda bir sayı (ya da isim) yazıp bir makineye verir. Makine kartındaki işi yapar ve cevabı **kâğıda yazıp geri verir** (`dön`).
2. `selamla` makinesi cevabı kâğıda yazmaz, yalnızca söyler (`yaz`). Öğrenci "geri alacak bir şey" bulamaz. Sorun: "Bu makinenin sonucunu bir sonraki makineye verebilir miyiz?"
3. Zincirleme: "Önce `ikiKat`, sonra `kare`" sırasıyla bir sayıyı iki makineden geçirin (`kare(ikiKat(3))`). Sonucu tahtaya yazdırın: 36. Sorun: "İki makineyi tek ifadede nasıl yazarız?"
4. Sınıf makine kartlarını JUS fonksiyonuna çevirir (en az `kare` ve `çiftMi`). `selamla` için `dön` yerine `yaz` kullanılacağını bulun.

Tartışma: Fonksiyonun içini bilmeden kullanabildik mi? (Evet: `yaz` ve `tür` gibi hazır fonksiyonları da böyle kullanıyoruz.)

## Alıştırmalar

Çözümler [cevap anahtarında](cevap-anahtari.md#hafta-5).

### Kolay

**Alıştırma 5.1.** Bir sayının karesini veren `kare` fonksiyonunu yazın. `kare(7)` ve `kare(1.5)` sonuçlarını yazdırın.

Beklenen çıktı:

```
49
2.25
```

**Alıştırma 5.2.** `selamla(ad)` fonksiyonu `Merhaba, ... !` biçiminde bir satır yazdırsın. `"Ali"` ve `"Zeynep"` için çağırın.

Beklenen çıktı:

```
Merhaba, Ali!
Merhaba, Zeynep!
```

**Alıştırma 5.3.** Üç sayının ortalamasını döndüren `ortalama(a, b, c)` fonksiyonunu yazın. `ortalama(70, 85, 100)` ve `ortalama(1, 2, 2)` sonuçlarını yazdırın.

Beklenen çıktı:

```
85
1.6666666666667
```

### Orta

**Alıştırma 5.4.** Bir sayının çift olup olmadığını `doğru`/`yanlış` olarak döndüren `çiftMi(n)` fonksiyonunu yazın. 1'den 5'e kadar her sayı için `1 tek`, `2 çift`... biçiminde yazdırın.

Beklenen çıktı:

```
1 tek
2 çift
3 tek
4 çift
5 tek
```

**Alıştırma 5.5.** Üç sayının en büyüğünü döndüren `enBüyük(a, b, c)` fonksiyonunu yazın. `enBüyük(14, 29, 8)` ve `enBüyük(5, 5, 2)` için sonucu yazdırın.

Beklenen çıktı:

```
29
5
```

**Alıştırma 5.6.** Hafta 3'ün bilet fiyatı kuralını `biletFiyatı(yaş, öğrenci)` fonksiyonuna dönüştürün (7 yaşından küçük: 0; öğrenci ya da 65 ve üstü: 50; diğerleri: 100). Şu üç yolcu için çağırın: `(5, yanlış)`, `(16, doğru)`, `(40, yanlış)`.

Beklenen çıktı:

```
0
50
100
```

### Zor

**Alıştırma 5.7.** Hafta 4'teki asal sayı denetimini `asalMı(n)` fonksiyonuna dönüştürün. Ardından 1'den 30'a kadar olan asal sayıları, aralarında birer boşlukla tek satırda yazdırın. (İpucu: bir `satır` metni oluşturup `satır = satır + metin(i) + " "` ile büyütün.)

Beklenen çıktı:

```
2 3 5 7 11 13 17 19 23 29
```

**Alıştırma 5.8 - ebob.** İki sayının en büyük ortak bölenini bulan `ebob(a, b)` fonksiyonunu yazın. Yöntem (Öklid): `b` sıfır olmadığı sürece `a`'ya `b`'yi, `b`'ye `a % b`'yi atayın; sonunda `a` sonuçtur. `ebob(48, 18)`, `ebob(17, 5)` ve `ebob(100, 75)` sonuçlarını yazdırın.

Beklenen çıktı:

```
6
1
25
```

**Alıştırma 5.9 - Özyineleme.** 1'den `n`'e kadar sayıların toplamını özyinelemeyle bulan `topla(n)` fonksiyonunu yazın (taban durum: `n == 0` ise 0). `topla(10)` ve `topla(100)` sonuçlarını yazdırın.

Beklenen çıktı:

```
55
5050
```

## Öğrenciler nerede takılır

### 1. `dön` yazmayı unutmak

```jus
fonksiyon kare(x):
    x * x

yaz(kare(4))
```

```
boş
```

Hata iletisi yoktur; sonuç `boş` gelir. `x * x` hesaplanır ama sonuç atılır. Yönlendirme: "Fonksiyondan beklediğin değer yerine `boş` görüyorsan ilk bakacağın yer `dön`." Hafta 2'den tanıdık `x * x` ifadesinin tek başına bir şey yapmadığına dikkat çektirin.

### 2. Yanlış sayıda argüman vermek

```jus
fonksiyon topla(a, b):
    dön a + b

yaz(topla(1))
```

```
ornek.jus:4: çalışma zamanı hatası: 'topla' fonksiyonu 2 argüman bekliyor, 1 verildi.
```

İleti hem beklenen hem verilen sayıyı söyler; öğrenciler genellikle kendi kendine düzeltir.

### 3. Fonksiyonu tanımdan önce çağırmak

```jus
yaz(kare(4))

fonksiyon kare(x):
    dön x * x
```

```
ornek.jus:1: çalışma zamanı hatası: 'kare' adında bir değişken ya da fonksiyon tanımlı değil.
```

Bilgisayar yukarıdan aşağıya ilerlediği için `kare(4)` çalıştığında `kare` henüz tanımlanmamıştır. İleti "tanımlı değil" der; öğrenci "ama aşağıda yazdım" diye şaşırır. Yönlendirme: robot etkinliği: "Robot bu komutu daha öğrenmemişken ona söylersen ne olur?" Çözüm: tanımlar çağrılardan önce yazılır.

### 4. Fonksiyonu tanımlayıp çağırmamak

```jus
fonksiyon selamla(ad):
    yaz("Merhaba, " + ad)

selamla
```

Çıktı yoktur ve hata iletisi de yoktur. `selamla` adı fonksiyonu gösterir ama parantez olmadan çağırmaz. Öğrenci "program çalışmadı mı" diye sorar. Yönlendirme: "Hafta 1'de parantez ne demişti? Hangi satır fonksiyonu gerçekten çağırıyor?" Karşılaştırmak için `yaz(selamla)` yazarsanız şunu görürsünüz:

```jus
fonksiyon selamla(ad):
    yaz("Merhaba, " + ad)

yaz(selamla)
```

```
<fonksiyon selamla>
```

### 5. Yerel değişkeni dışarıda kullanmak

```jus
fonksiyon hesapla():
    değişken gizli = 42

hesapla()
yaz(gizli)
```

```
ornek.jus:5: çalışma zamanı hatası: 'gizli' adında bir değişken ya da fonksiyon tanımlı değil.
```

Yönlendirme: "`gizli` hangi satırda doğuyor, hangisinde yok oluyor?" Çözüm: değeri `dön` ile dışarı vermek: `dön gizli`, çağıran taraf `değişken sonuç = hesapla()` yazar.

## Çıkış bileti

1. Aşağıdaki program ne yazar?
2. `yaz` ile `dön` arasındaki fark nedir?
3. Bir fonksiyonu neden yazarız?

Soru 1'in programı:

```jus
fonksiyon ikiKat(x):
    dön x * 2

yaz(ikiKat(ikiKat(3)))
```

```
12
```

Cevaplar:

1. `12`. İçteki çağrı `6` verir, dıştaki `12`.
2. `yaz` ekrana gösterir; fonksiyonun çağırana bir değer vermesini sağlamaz. `dön` değeri çağırana geri verir; bu değer başka hesaplarda kullanılabilir ve ekrana yazılmaz.
3. Tekrar eden işi bir kere yazıp adıyla kullanmak, kodu okunur ve değiştirmesi kolay yapmak için.

## Ev çalışması (isteğe bağlı)

1. Hafta 2'deki Celsius'tan Fahrenheit'e çeviriyi `fahrenheit(c)` fonksiyonuna dönüştürün ve 0, 25, 100 derece için çağırın.
2. Bir dairenin alanını `3.14 * r * r` ile hesaplayan `daireAlanı(r)` fonksiyonu yazın; sonucu `yuvarla(..., 2)` ile düzenleyin.
3. Yazdığınız iki fonksiyonu başka bir arkadaşınıza verin; onun fonksiyonunu çalıştırmadan yalnızca bakarak ne yaptığını açıklamasını isteyin.
