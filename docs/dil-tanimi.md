# JUS Dil Tanımı

Sürüm 0.1.0

Bu belge JUS'un sözdizimini ve davranışını tanımlar. Yorumlayıcı bu belgeye
uymak zorundadır; ikisi arasındaki her fark bir hatadır. Burada yazmayan bir
özellik dilde yoktur.

## 1. Kaynak dosyalar

- Kaynak dosyalar UTF-8 ile kodlanır. Dosya başındaki UTF-8 BOM yok sayılır.
- Dosya uzantısı `.jus` olur.
- Satır sonu `LF` ya da `CRLF` olabilir.

## 2. Sözcük yapısı

### 2.1 Yorumlar

`#` karakterinden satır sonuna kadar olan kısım yorumdur.

```jus
# tam satır yorum
yaz("merhaba")  # satır sonu yorumu
```

### 2.2 Adlar

Bir ad harf ya da alt çizgi (`_`) ile başlar; harf, rakam ve alt çizgi ile
sürer. Türkçe harfler dahil tüm Unicode harfleri kullanılabilir. Büyük ve küçük
harf ayrıdır: `yaş` ile `Yaş` farklı adlardır.

### 2.3 Anahtar kelimeler

Aşağıdaki 14 kelime ayrılmıştır ve ad olarak kullanılamaz:

```
boş       değil     değilse   değişken   devam   doğru   dön
eğer      fonksiyon iken      kır        ve      veya    yanlış
```

### 2.4 Sayılar

Sayılar onluk tabanda yazılır; ondalık ayırıcı noktadır: `42`, `3.14`.
Noktanın iki yanında da rakam bulunmalıdır.

### 2.5 Metinler

Metinler çift tırnak içinde yazılır ve tek satırda biter. Kaçış dizileri:

| Dizi | Anlamı        |
|------|---------------|
| `\n` | satır sonu    |
| `\t` | sekme         |
| `\r` | satır başı    |
| `\"` | çift tırnak   |
| `\\` | ters bölü     |

Başka bir kaçış dizisi sözdizimi hatasıdır.

### 2.6 Satırlar ve girinti

- Her deyim bir satırda yazılır ve satır sonunda biter.
- Parantez içindeki satır sonları yok sayılır; uzun ifadeler bu yolla birden
  çok satıra bölünebilir.
- Bloklar girintiyle belirlenir. Bloğu başlatan satır `:` ile biter; izleyen
  satırlar bir düzey içeriden yazılır. Girinti önceki düzeye döndüğünde blok
  biter.
- Girinti miktarı serbesttir, ancak aynı bloktaki satırlar aynı girintide
  olmalıdır. Önerilen girinti 4 boşluktur.
- Girinti boşlukla ya da sekmeyle yapılabilir; aynı dosyada ikisi birlikte
  kullanılamaz.
- Boş satırlar ve yalnızca yorum içeren satırlar girintiyi etkilemez.
- Her blokta en az bir deyim bulunmalıdır.

## 3. Değerler ve türler

| Tür         | Örnekler              | Açıklama                               |
|-------------|-----------------------|----------------------------------------|
| `sayı`      | `42`, `3.14`          | 64 bit kayan noktalı sayı              |
| `metin`     | `"merhaba"`           | değiştirilemez UTF-8 karakter dizisi   |
| `mantıksal` | `doğru`, `yanlış`     |                                        |
| `boş`       | `boş`                 | değer yokluğu                          |
| `fonksiyon` |                       | kullanıcı tanımlı ya da yerleşik       |

JUS dinamik tiplidir: değişkenlerin değil, değerlerin türü vardır. Türler
arasında örtük dönüşüm yapılmaz; dönüşüm için `metin()` ve `sayı()` kullanılır.

Tam sayı değerli sayılar ondalık kısım olmadan yazılır (`4`); diğerleri en çok
14 anlamlı basamakla yazılır (`0.33333333333333`).

## 4. İfadeler

### 4.1 İşleçler

Öncelik sırası, yüksekten düşüğe:

| Öncelik | İşleçler              | Açıklama                  | Birleşme |
|---------|-----------------------|---------------------------|----------|
| 1       | `f(...)`              | fonksiyon çağrısı         | soldan   |
| 2       | `-x`                  | sayısal olumsuzlama       | sağdan   |
| 3       | `*` `/` `%`           | çarpma, bölme, kalan      | soldan   |
| 4       | `+` `-`               | toplama, çıkarma          | soldan   |
| 5       | `<` `<=` `>` `>=`     | karşılaştırma             | soldan   |
| 6       | `==` `!=`             | eşitlik                   | soldan   |
| 7       | `değil x`             | mantıksal olumsuzlama     | sağdan   |
| 8       | `ve`                  | mantıksal ve              | soldan   |
| 9       | `veya`                | mantıksal veya            | soldan   |
| 10      | `=`                   | atama                     | sağdan   |

Parantez önceliği değiştirir: `(2 + 3) * 4`.

### 4.2 Aritmetik

- `+`, `-`, `*`, `/`, `%` iki sayı ister. `+` ayrıca iki metni birleştirir.
- Bir sayı ile bir metni `+` ile birleştirmek hatadır; önce `metin()` ile
  dönüştürülür: `"yaş: " + metin(25)`.
- `/` her zaman ondalıklı bölme yapar: `15 / 4` sonucu `3.75` olur.
- `%` kalanı verir; sonuç bölenin işaretini taşır: `-7 % 3` sonucu `2` olur.
- Sıfıra bölme ve sıfıra göre kalan çalışma zamanı hatasıdır.

### 4.3 Karşılaştırma ve eşitlik

- `<`, `<=`, `>`, `>=` yalnızca sayılarla kullanılır.
- `==` ve `!=` her türle kullanılabilir. Türleri farklı iki değer hiçbir zaman
  eşit değildir: `"12" == 12` sonucu `yanlış` olur.
- Metinler içerikleri aynıysa eşittir.

### 4.4 Mantıksal işleçler

- `ve`, `veya`, `değil` yalnızca mantıksal değerlerle kullanılır; başka türde
  bir değer çalışma zamanı hatasıdır.
- `ve` ile `veya` kısa devre yapar: sonuç sol taraftan belliyse sağ taraf
  hesaplanmaz.

### 4.5 Atama

`ad = ifade` var olan bir değişkene yeni değer atar. Tanımlanmamış bir ada
atama hatadır. Atama bir ifadedir ve atanan değeri verir.

### 4.6 Fonksiyon çağrısı

`f(a, b)` biçimindedir. Argüman sayısı fonksiyonun parametre sayısına eşit
olmalıdır. Bir çağrıda en çok 255 argüman bulunabilir.

## 5. Deyimler

### 5.1 Değişken tanımı

```jus
değişken ad = ifade
değişken ad          # başlangıç değeri boş
```

- Bir değişken kullanılmadan önce tanımlanmalıdır.
- Üst düzeyde tanımlanan değişkenler geneldir ve dosyanın her yerinden
  görülür.
- Bir blok ya da fonksiyon içinde tanımlanan değişken yereldir; tanımlandığı
  satırdan bloğun sonuna kadar geçerlidir.
- İç blok, dış kapsamdaki bir adı yeniden tanımlayarak gölgeleyebilir. Aynı
  blok içinde aynı adı iki kez tanımlamak hatadır.

### 5.2 Koşul

```jus
eğer koşul:
    ...
değilse eğer başka_koşul:
    ...
değilse:
    ...
```

`değilse eğer` ve `değilse` bölümleri isteğe bağlıdır. Koşul mantıksal bir
değer olmalıdır; `eğer 1:` çalışma zamanı hatasıdır.

### 5.3 Döngü

```jus
iken koşul:
    ...
```

Koşul `doğru` olduğu sürece blok yinelenir.

- `kır` en içteki döngüyü bitirir.
- `devam` en içteki döngünün sonraki turuna geçer.

İkisi de yalnızca döngü içinde kullanılabilir.

### 5.4 Fonksiyon tanımı

```jus
fonksiyon ad(parametre1, parametre2):
    ...
    dön ifade
```

- `dön ifade` fonksiyonu bitirir ve değeri çağırana verir. Yalnız `dön` ya da
  gövdenin sonuna ulaşmak `boş` döndürür.
- `dön` yalnızca fonksiyon içinde kullanılabilir.
- Fonksiyonlar değerdir: değişkene atanabilir, argüman olarak verilebilir ve
  başka bir fonksiyondan döndürülebilir.
- Fonksiyonlar iç içe tanımlanabilir. İç fonksiyon, tanımlandığı kapsamdaki
  değişkenleri kullanabilir ve değiştirebilir; dış fonksiyon bittikten sonra
  da bu değişkenlere erişimini korur (kapanım).
- Bir fonksiyonun en çok 255 parametresi ve 256 yerel değişkeni olabilir.
- İç içe çağrı derinliği sınırlıdır; sınır aşılırsa çalışma zamanı hatası
  oluşur.

### 5.5 İfade deyimi

Tek başına yazılan bir ifade hesaplanır ve sonucu atılır. Fonksiyon çağrıları
ve atamalar bu biçimde kullanılır.

## 6. Yerleşik fonksiyonlar

| Fonksiyon        | Açıklama                                                                 |
|------------------|--------------------------------------------------------------------------|
| `yaz(...)`       | Argümanları aralarında birer boşlukla yazar ve satırı bitirir.           |
| `oku()`          | Girdiden bir satır okur, metin olarak döndürür. Girdi bittiyse `boş`.    |
| `oku(istem)`     | Önce `istem` metnini yazar, sonra okur.                                  |
| `metin(değer)`   | Değeri metne çevirir.                                                    |
| `sayı(değer)`    | Metni sayıya çevirir. Metin geçerli bir sayı değilse hata verir.         |
| `uzunluk(metin)` | Metnin karakter sayısını döndürür.                                       |
| `tür(değer)`     | Değerin tür adını metin olarak döndürür.                                 |
| `saat()`         | Programın kullandığı işlemci süresini saniye cinsinden döndürür.         |
| `karekök(x)`     | Karekök. Negatif sayı hata verir.                                        |
| `mutlak(x)`      | Mutlak değer.                                                            |
| `taban(x)`       | `x`'ten büyük olmayan en büyük tam sayı.                                 |
| `tavan(x)`       | `x`'ten küçük olmayan en küçük tam sayı.                                 |
| `yuvarla(x)`     | En yakın tam sayı; tam ortadaki değerler sıfırdan uzağa yuvarlanır.      |

Yerleşik fonksiyon adları anahtar kelime değildir; genel kapsamda tanımlı
sıradan değişkenlerdir.

## 7. Hatalar

### 7.1 Sözdizimi hataları

Program çalıştırılmadan önce tümüyle denetlenir. Sözdizimi hatası varsa
programın hiçbir satırı çalışmaz. Her hata dosya adı, satır ve sütunla
bildirilir:

```
ornek.jus:3:13: sözdizimi hatası: İfade bekleniyor.
    değişken x =
                ^
```

Çıkış kodu 65 olur.

### 7.2 Çalışma zamanı hataları

Çalışma sırasında oluşan hata programı durdurur. Hata bir fonksiyonun içinde
oluştuysa çağrı zinciri de gösterilir:

```
ornek.jus:2: çalışma zamanı hatası: Sıfıra bölünemez.
    satır 2, 'böl' fonksiyonu
    satır 4, ana program
```

Çıkış kodu 70 olur.

## 8. Etkileşimli kip

`jus` argümansız çalıştırıldığında etkileşimli kip başlar.

- Her satır yazıldığı anda çalıştırılır; tanımlar sonraki satırlarda
  geçerliliğini korur.
- Tek başına yazılan bir ifadenin sonucu `boş` değilse ekrana yazılır.
- `:` ile biten satır bir blok başlatır; blok boş bir satırla bitirilir.
- `çıkış` yazmak ya da girdiyi kapatmak kipi sonlandırır.

## 9. Dilbilgisi

```
program        = { bildirim } ;
bildirim       = değişken_tanımı | fonksiyon_tanımı | deyim ;

değişken_tanımı  = "değişken" AD [ "=" ifade ] SATIR_SONU ;
fonksiyon_tanımı = "fonksiyon" AD "(" [ AD { "," AD } ] ")" blok ;

deyim          = eğer_deyimi | iken_deyimi | dön_deyimi
               | "kır" SATIR_SONU | "devam" SATIR_SONU
               | ifade SATIR_SONU ;
eğer_deyimi    = "eğer" ifade blok [ "değilse" ( eğer_deyimi | blok ) ] ;
iken_deyimi    = "iken" ifade blok ;
dön_deyimi     = "dön" [ ifade ] SATIR_SONU ;
blok           = ":" SATIR_SONU GİRİNTİ bildirim { bildirim } GİRİNTİ_SONU ;

ifade          = atama ;
atama          = AD "=" atama | veya_ifadesi ;
veya_ifadesi   = ve_ifadesi { "veya" ve_ifadesi } ;
ve_ifadesi     = değil_ifadesi { "ve" değil_ifadesi } ;
değil_ifadesi  = "değil" değil_ifadesi | eşitlik ;
eşitlik        = karşılaştırma { ( "==" | "!=" ) karşılaştırma } ;
karşılaştırma  = toplam { ( "<" | "<=" | ">" | ">=" ) toplam } ;
toplam         = çarpım { ( "+" | "-" ) çarpım } ;
çarpım         = tekli { ( "*" | "/" | "%" ) tekli } ;
tekli          = "-" tekli | çağrı ;
çağrı          = birincil { "(" [ ifade { "," ifade } ] ")" } ;
birincil       = SAYI | METİN | AD | "doğru" | "yanlış" | "boş"
               | "(" ifade ")" ;
```
