# JUS Dil Tanımı

Sürüm 1.3.0

Bu belge JUS'un sözdizimini ve davranışını tanımlar. Yorumlayıcı bu belgeye
uymak zorundadır; ikisi arasındaki her fark bir hatadır. Burada yazmayan bir
özellik dilde yoktur.

1.0 sürümünden itibaren bu belgede tanımlanan davranış kararlıdır: 1.x
sürümleri yeni özellikler ekleyebilir, ancak bu belgeye uygun yazılmış bir
programın davranışını değiştirmez. Hata iletilerinin metni bu güvencenin
dışındadır; iyileştirilebilir.

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

Aşağıdaki 25 kelime ayrılmıştır ve ad olarak kullanılamaz:

```
boş      bu       değil    değilse   değişken   dene     devam    doğru   dön
eğer     fırlat   fonksiyon geç      her        içinde   iken     kır     kullan
olarak   sınıf    üst      ve        veya       yakala   yanlış
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

### 2.5A Biçimli metinler

Önünde `f` harfi bulunan ve hemen ardından çift tırnakla başlayan metin
**biçimli metindir**: `f"Merhaba, {ad}! Yaşın {yaş + 1}."`. `f` ile açılış
tırnağı arasında boşluk bulunamaz.

```jus
değişken ad = "Ayşe"
yaz(f"Merhaba, {ad}! {{süslü}} {uzunluk(ad) * 2}")
```

```
Merhaba, Ayşe! {süslü} 8
```

- Biçimli metin, düz metin parçalarından ve `{` ile `}` arasına yazılan
  ifadelerden oluşur. Düz parçalarda 2.5'teki kaçış dizileri geçerlidir.
- İfadeler metindeki sıralarıyla, soldan sağa hesaplanır. Her sonuç `metin()`
  ile aynı biçimde metne çevrilir (6.2): metinler tırnaksız, listeler ve
  sözlükler yazıldıkları gibi görünür. Parçalar art arda birleştirilir; sonuç
  `metin` türündedir.
- Süslü parantezin kendisi `{{` ve `}}` ile yazılır.
- `{ }` içindeki ifade atama olamaz (dilbilgisinde `veya_ifadesi`). İfadenin
  içinde çift tırnak kullanılamaz: çift tırnak metni bitirir. Tırnaklı bir
  değer gerekiyorsa önce bir değişkene alınır.
- Biçim belirteci yoktur: `{x:.2f}` sözdizimi hatasıdır. Biçimlendirme için
  `biçimle`, `sola_doldur` gibi fonksiyonlar `{ }` içinde çağrılır:
  `f"{biçimle(fiyat, 2)} TL"`.
- Biçimli metindeki hatalar derleme zamanında bildirilir (7.1): kapatılmamış
  `{`, tek başına `}` ve boş `{}`. Bu üç hata biçimli metnin başlangıcını
  gösterir. İfadenin kendi içindeki bir sözdizimi hatası ise sıradan bir
  sözdizimi hatası gibi, kendi satır ve sütunuyla bildirilir.

### 2.6 Satırlar ve girinti

- Her deyim bir satırda yazılır ve satır sonunda biter.
- `( )`, `[ ]` ve `{ }` içindeki satır sonları yok sayılır; uzun ifadeler,
  listeler ve sözlükler bu yolla birden çok satıra bölünebilir.
- Bloklar girintiyle belirlenir. Bloğu başlatan satır `:` ile biter; izleyen
  satırlar bir düzey içeriden yazılır. Girinti önceki düzeye döndüğünde blok
  biter.
- Girinti miktarı serbesttir, ancak aynı bloktaki satırlar aynı girintide
  olmalıdır. Önerilen girinti 4 boşluktur.
- Girinti boşlukla ya da sekmeyle yapılabilir; aynı dosyada ikisi birlikte
  kullanılamaz.
- Boş satırlar ve yalnızca yorum içeren satırlar girintiyi etkilemez.
- Her blokta en az bir deyim bulunmalıdır. Boş bırakılmak istenen bloğa hiçbir
  şey yapmayan `geç` deyimi yazılır.

## 3. Değerler ve türler

| Tür         | Örnekler              | Açıklama                               |
|-------------|-----------------------|----------------------------------------|
| `sayı`      | `42`, `3.14`          | 64 bit kayan noktalı sayı              |
| `metin`     | `"merhaba"`           | değiştirilemez UTF-8 karakter dizisi   |
| `mantıksal` | `doğru`, `yanlış`     |                                        |
| `boş`       | `boş`                 | değer yokluğu                          |
| `liste`     | `[1, 2, 3]`           | sıralı, değiştirilebilir öğe dizisi    |
| `sözlük`    | `{"ad": "Ayşe"}`      | anahtar-değer eşlemesi                 |
| `fonksiyon` |                       | kullanıcı tanımlı ya da yerleşik       |
| `sınıf`     |                       | `sınıf` ile tanımlanır                 |
| sınıf adı   |                       | bir sınıfın nesnesi; türü sınıfın adıdır |
| `modül`     |                       | `kullan` ile yüklenir                  |

JUS dinamik tiplidir: değişkenlerin değil, değerlerin türü vardır. Türler
arasında örtük dönüşüm yapılmaz; dönüşüm için `metin()` ve `sayı()` kullanılır.
Biçimli metin (2.5A) bu kuralın dışında değildir: dönüşümü `f"..."` yazımı
açıkça ister.

Tam sayı değerli sayılar ondalık kısım olmadan yazılır (`4`); diğerleri en çok
14 anlamlı basamakla yazılır (`0.33333333333333`). Tam sayılar 2^53'e
(9007199254740992) kadar kesin olarak tutulur; bundan büyük sayılar yaklaşık
değerlidir ve üslü biçimde yazılır (`1e+20`).

### 3.1 Listeler

```jus
değişken notlar = [85, 92, 78]
yaz(notlar[0])        # 85
yaz(notlar[-1])       # 78 (sondan birinci)
notlar[1] = 95
yaz(notlar[0:2])      # [85, 95]
```

- Öğeler her türden olabilir; bir liste farklı türleri bir arada tutabilir.
- Dizinler 0'dan başlar. Negatif dizin sondan sayar. Sınır dışı dizin
  çalışma zamanı hatasıdır.
- `liste[baş:son]` dilimi `baş` dizininden `son` dizinine kadar (son hariç)
  yeni bir liste verir. Uçlardan biri yazılmazsa listenin başı ya da sonu
  alınır. Dilim sınırları listenin dışına taşarsa hata olmaz, kırpılır.
- `+` iki listeyi birleştirip yeni bir liste verir.
- Listeler başvuruyla taşınır: bir listeyi başka bir değişkene atamak ya da
  fonksiyona vermek kopyalamaz; iki ad aynı listeyi gösterir.
- Son öğeden sonra virgül konabilir.

### 3.2 Sözlükler

```jus
değişken kişi = {"ad": "Ayşe", "yaş": 30}
yaz(kişi["ad"])       # Ayşe
kişi["şehir"] = "Ankara"
```

- Anahtarlar metin, sayı ya da mantıksal olabilir; değerler her türden
  olabilir. `1` ile `"1"` farklı anahtarlardır.
- Olmayan bir anahtarı okumak çalışma zamanı hatasıdır. Anahtarın varlığı
  `içinde` ile denetlenir; `al(sözlük, anahtar, varsayılan)` hata vermeden okur.
- Var olmayan bir anahtara atama yeni girdi ekler.
- Sözlükler girdileri eklenme sırasıyla tutar ve başvuruyla taşınır.

### 3.3 Metinlerde dizin ve dilim

Metinler de dizinlenebilir ve dilimlenebilir. Birim bayt değil karakterdir:
`"çay"[0]` sonucu `"ç"` olur. Metinler değiştirilemez; `metin[0] = "x"` hatadır.

## 4. İfadeler

### 4.1 İşleçler

Öncelik sırası, yüksekten düşüğe:

| Öncelik | İşleçler              | Açıklama                  | Birleşme |
|---------|-----------------------|---------------------------|----------|
| 1       | `f(...)` `x[i]` `m.ad` | çağrı, dizinleme, üye erişimi | soldan |
| 2       | `-x`                  | sayısal olumsuzlama       | sağdan   |
| 3       | `*` `/` `%`           | çarpma, bölme, kalan      | soldan   |
| 4       | `+` `-`               | toplama, çıkarma          | soldan   |
| 5       | `<` `<=` `>` `>=` `içinde` | karşılaştırma, üyelik | soldan   |
| 6       | `==` `!=`             | eşitlik                   | soldan   |
| 7       | `değil x`             | mantıksal olumsuzlama     | sağdan   |
| 8       | `ve`                  | mantıksal ve              | soldan   |
| 9       | `veya`                | mantıksal veya            | soldan   |
| 10      | `=` `+=` `-=` `*=` `/=` | atama                   | sağdan   |

Parantez önceliği değiştirir: `(2 + 3) * 4`.

### 4.2 Aritmetik

- `+`, `-`, `*`, `/`, `%` iki sayı ister. `+` ayrıca iki metni ya da iki
  listeyi birleştirir.
- Bir sayı ile bir metni `+` ile birleştirmek hatadır; önce `metin()` ile
  dönüştürülür: `"yaş: " + metin(25)`. Biçimli metin aynı işi tek yazımda
  yapar: `f"yaş: {25}"` (2.5A).
- `/` her zaman ondalıklı bölme yapar: `15 / 4` sonucu `3.75` olur.
- `%` kalanı verir; sonuç bölenin işaretini taşır: `-7 % 3` sonucu `2` olur.
- Sıfıra bölme ve sıfıra göre kalan çalışma zamanı hatasıdır.

### 4.3 Karşılaştırma ve eşitlik

- `<`, `<=`, `>`, `>=` iki sayıyı ya da iki metni karşılaştırır. Metinler Türk
  alfabesi sırasına göre karşılaştırılır: `"çay" < "dağ"`, `"ırmak" < "iz"`.
- `==` ve `!=` her türle kullanılabilir. Türleri farklı iki değer hiçbir zaman
  eşit değildir: `"12" == 12` sonucu `yanlış` olur.
- Metinler, listeler ve sözlükler içerikleri aynıysa eşittir.
- `öğe içinde kap` üyeliği denetler: listede öğeyi, metinde alt metni,
  sözlükte anahtarı arar.

### 4.4 Mantıksal işleçler

- `ve`, `veya`, `değil` yalnızca mantıksal değerlerle kullanılır; başka türde
  bir değer çalışma zamanı hatasıdır.
- `ve` ile `veya` kısa devre yapar: sonuç sol taraftan belliyse sağ taraf
  hesaplanmaz.

### 4.5 Atama

`ad = ifade` var olan bir değişkene yeni değer atar. Tanımlanmamış bir ada
atama hatadır. Atama bir ifadedir ve atanan değeri verir.

`kap[dizin] = ifade` bir liste öğesini ya da sözlük girdisini değiştirir.

`+=`, `-=`, `*=`, `/=` bileşik atamalardır: `x += 1`, `x = x + 1` ile aynıdır.
Dizinlerle de kullanılabilir: `sayım["a"] += 1`.

### 4.6 Fonksiyon çağrısı

`f(a, b)` biçimindedir. Argümanlar soldan sağa, parametrelere sırayla bağlanır.

- Varsayılan değeri olmayan her parametre için argüman verilmelidir.
- Fonksiyonun `*` parametresi (5.4) yoksa parametre sayısından fazla argüman
  verilemez.
- Varsayılan değeri olup argüman verilmeyen parametreler için varsayılan ifade
  çağrıda hesaplanır (5.4).
- `*` parametresi varsa, adlı parametrelere bağlanmayan argümanlar yeni bir
  liste olarak o parametreye verilir.
- Bu kurallara uymayan çağrı çalışma zamanı hatasıdır. İleti kabul edilen
  aralığı bildirir: `'f' fonksiyonu 2 argüman bekliyor, 3 verildi.` (varsayılansız
  ve `*`'sız fonksiyon), `'f' fonksiyonu en az 1, en çok 2 argüman bekliyor, 3
  verildi.` (varsayılanlı), `'g' fonksiyonu en az 1 argüman bekliyor, 0
  verildi.` (`*` parametreli).
- Çağrıda liste açma (`f(*liste)`) yoktur.

Bir çağrıda en çok 255 argüman bulunabilir.

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
- İç blok, dış kapsamdaki bir adı yeniden tanımlayarak gölgeleyebilir. Bir blok
  ya da fonksiyon içinde aynı adı iki kez tanımlamak hatadır. Üst düzeyde aynı
  adı yeniden tanımlamak hata değildir; yeni tanım öncekinin yerini alır.

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

```jus
her öğe içinde kap:
    ...
```

Kabın öğelerini sırayla dolaşır: listede öğeleri, metinde karakterleri,
sözlükte anahtarları. Döngü değişkeni yalnızca döngünün içinde geçerlidir.
Belirli sayıda yineleme için `aralık` kullanılır: `her i içinde aralık(5):`.

- `kır` en içteki döngüyü bitirir.
- `devam` en içteki döngünün sonraki turuna geçer.

İkisi de yalnızca döngü içinde kullanılabilir.

### 5.4 Fonksiyon tanımı

```jus
fonksiyon ad(parametre1, parametre2 = varsayılan, *kalan):
    ...
    dön ifade
```

- Parametre listesi sırayla şunlardan oluşur: varsayılan değeri olmayan
  (zorunlu) parametreler, varsayılan değeri olan parametreler ve en çok bir `*`
  parametresi.
- `parametre = ifade` parametreye varsayılan değer verir. Çağrıda o parametre
  için argüman verilmezse ifade, fonksiyonun gövdesi başlamadan önce
  **her çağrıda yeniden** hesaplanır; yani `liste = []` varsayılanı her
  çağrıda yeni bir boş liste verir. Argüman verilirse ifade hesaplanmaz.
  Açıkça verilen `boş`, varsayılanı devreye sokmaz.
- Varsayılan ifade, kendinden önceki parametreleri ve çevredeki kapsamın
  adlarını kullanabilir; kendisini ve sonraki parametreleri kullanamaz.
- Varsayılan değeri olan bir parametreden sonra varsayılan değeri olmayan
  parametre gelemez (sözdizimi hatası: `Varsayılan değeri olan parametreden
  sonra varsayılan değeri olmayan parametre gelemez.`).
- Son parametre `*ad` biçiminde yazılırsa değişken sayıda argüman alır:
  adlı parametrelere bağlanmayan argümanlar yeni bir liste olarak `ad`'a
  verilir; hiç argüman kalmazsa boş listedir. `*` ile işaretlenen parametre
  sonuncu olmalıdır (sözdizimi hatası: `'*' ile işaretlenen parametre sonuncu
  olmalıdır.`) ve varsayılan değer alamaz. Varsayılanlı parametrelerle birlikte
  kullanılabilir: `fonksiyon f(a, b = 2, *kalan)`.
- Varsayılan değerler ve `*` parametresi yöntemlerde (`kur` dahil, 5.8) ve iç
  fonksiyonlarda da aynı kurallarla geçerlidir.
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

### 5.6 Hata yakalama

```jus
dene:
    değişken sonuç = böl(a, b)
yakala hata:
    yaz("Hesaplanamadı:", hata)
```

- `dene` bloğundaki bir deyim hata verirse bloğun kalanı atlanır ve `yakala`
  bloğu çalışır. Hata yoksa `yakala` bloğu çalışmaz.
- `yakala`'dan sonra yazılan ad, hata değerini tutan yerel değişkendir. Ad
  yazılmayabilir: `yakala:`.
- Dilin kendi ürettiği hatalarda (sıfıra bölme, tanımsız ad, tür uyuşmazlığı,
  yerleşik fonksiyon hataları ...) hata değeri, hatayı anlatan bir metindir.
- `fırlat ifade` bir hata oluşturur. Fırlatılan değer her türden olabilir ve
  `yakala` bloğuna olduğu gibi ulaşır.
- Hata, onu yakalayan bir `dene` bloğu bulunana kadar çağıran fonksiyonlara
  doğru ilerler. Hiçbir blok yakalamazsa program 7.2'de anlatıldığı gibi durur.
- Sözdizimi hataları yakalanamaz; program hiç başlamaz.

### 5.7 Modüller

```jus
kullan matematik
kullan matematik olarak m
kullan "araçlar/geometri"

yaz(matematik.pi, m.üs(2, 10), geometri.alan(3))
```

- `kullan ad` bir modülü yükler ve onu `ad` değişkenine bağlar. `olarak` ile
  değişkene başka bir ad verilebilir.
- Önce standart kütüphanede `ad` adında bir modül aranır. Yoksa, `kullan`
  deyiminin bulunduğu dosyanın klasöründe `ad.jus` dosyası aranır.
- Alt klasördeki bir dosya için yol tırnak içinde, `/` ile ve `.jus` uzantısı
  olmadan yazılır. Değişken adı yolun son parçasıdır.
- Bir modülün üst düzeyinde tanımlanan tüm değişken ve fonksiyonlar o modülün
  üyeleridir ve `modül.üye` biçiminde okunur. Modülün üyelerine dışarıdan atama
  yapılamaz.
- Her dosyanın genel değişkenleri kendisine aittir; iki dosyadaki aynı adlı
  değişkenler birbirini etkilemez. Yerleşik fonksiyonlar her dosyadan görülür.
- Modül ne standart kütüphanede ne de dosyanın klasöründe bulunursa, kurulu
  paketlerde aranır (bkz. 8C): `kullan ad` için `jus_paketleri/ad/ad.jus`,
  `kullan "ad/modül"` için `jus_paketleri/ad/modül.jus`.
- Bir modül, kaç kez `kullan` ile istenirse istensin yalnızca bir kez yüklenir
  ve üst düzey kodu bir kez çalışır.
- `kullan` bir deyimdir; fonksiyon ya da blok içinde de yazılabilir.

### 5.8 Sınıflar

```jus
sınıf Hayvan:
    fonksiyon kur(ad):
        bu.ad = ad

    fonksiyon tanıt():
        dön bu.ad + ": " + bu.ses()

    fonksiyon ses():
        dön "..."

sınıf Köpek(Hayvan):
    fonksiyon ses():
        dön "Hav"

değişken k = Köpek("Karabaş")
yaz(k.tanıt())        # Karabaş: Hav
```

- Bir sınıfın gövdesi yalnızca `fonksiyon` tanımlarından oluşur; bunlar sınıfın
  yöntemleridir. Her sınıfın en az bir yöntemi olmalıdır.
- Sınıf, fonksiyon gibi çağrılarak nesne oluşturulur: `Köpek("Karabaş")`.
  Sınıfın `kur` adında bir yöntemi varsa yeni nesne için çağrılır ve argümanlar
  ona verilir. `kur` değer döndüremez. `kur` yoksa sınıf argümansız çağrılır.
- Yöntemlerin içinde `bu`, yöntemin çağrıldığı nesnedir. Parametre listesine
  yazılmaz.
- Nesnenin alanları `nesne.alan` ile okunur, `nesne.alan = değer` ile atanır.
  Alanlar önceden bildirilmez; ilk atamada oluşur. Olmayan bir alanı okumak
  hatadır.
- `nesne.yöntem(...)` yöntemi çağırır. `nesne.yöntem` yazılırsa nesnesine bağlı
  bir fonksiyon değeri elde edilir.
- `sınıf Alt(Üst):` kalıtım kurar: alt sınıf üst sınıfın tüm yöntemlerini
  devralır ve aynı adla yeniden tanımlayarak değiştirebilir. Bir sınıfın tek
  bir üst sınıfı olabilir.
- `üst.yöntem(...)`, yöntemin üst sınıftaki tanımını çağırır.
- `tür(nesne)` sınıfın adını verir. `örneği_mi(değer, sınıf)`, değerin o
  sınıfın ya da ondan türeyen bir sınıfın nesnesi olup olmadığını söyler.
- İki nesne yalnızca aynı nesneyse eşittir. Nesneler başvuruyla taşınır.

## 6. Yerleşik fonksiyonlar

Yerleşik fonksiyon adları anahtar kelime değildir; genel kapsamda tanımlı
sıradan değişkenlerdir.

### 6.1 Girdi ve çıktı

| Fonksiyon        | Açıklama                                                                 |
|------------------|--------------------------------------------------------------------------|
| `yaz(...)`       | Argümanları aralarında birer boşlukla yazar ve satırı bitirir.           |
| `oku()`          | Girdiden bir satır okur, metin olarak döndürür. Girdi bittiyse `boş`.    |
| `oku(istem)`     | Önce `istem` metnini yazar, sonra okur.                                  |

### 6.2 Türler

| Fonksiyon        | Açıklama                                                                 |
|------------------|--------------------------------------------------------------------------|
| `metin(değer)`   | Değeri metne çevirir.                                                    |
| `sayı(değer)`    | Metni sayıya çevirir. Metin geçerli bir sayı değilse hata verir.         |
| `tür(değer)`     | Değerin tür adını metin olarak döndürür.                                 |
| `uzunluk(kap)`   | Metnin karakter, listenin öğe, sözlüğün girdi sayısı.                    |
| `örneği_mi(d, sınıf)` | `d`, sınıfın ya da ondan türeyen bir sınıfın nesnesiyse `doğru`.    |

### 6.2A Doğrulama

| Fonksiyon                       | Açıklama                                              |
|---------------------------------|-------------------------------------------------------|
| `doğrula(koşul)`                | Koşul `yanlış` ise hata verir.                        |
| `doğrula(koşul, ileti)`         | Aynı; hata iletisine `ileti` eklenir.                 |
| `eşit_olmalı(bulunan, beklenen)` | İki değer eşit değilse ikisini de gösteren bir hata verir. |

Bu fonksiyonlar testlerde ve bir fonksiyonun girdilerini denetlemek için
kullanılır. Verdikleri hata `dene` / `yakala` ile yakalanabilir.

### 6.3 Sayılar

| Fonksiyon                | Açıklama                                                         |
|--------------------------|------------------------------------------------------------------|
| `karekök(x)`             | Karekök. Negatif sayı hata verir.                                |
| `mutlak(x)`              | Mutlak değer.                                                    |
| `taban(x)`               | `x`'ten büyük olmayan en büyük tam sayı.                         |
| `tavan(x)`               | `x`'ten küçük olmayan en küçük tam sayı.                         |
| `yuvarla(x)`             | En yakın tam sayı; tam ortadaki değerler sıfırdan uzağa yuvarlanır. |
| `yuvarla(x, basamak)`    | Verilen sayıda ondalık basamağa yuvarlar: `yuvarla(3.14159, 2)` sonucu `3.14`. Tam ortadaki değerler burada da sıfırdan uzağa yuvarlanır. |
| `biçimle(x, basamak)`    | Sayıyı tam olarak verilen sayıda ondalık basamakla metne çevirir: `biçimle(2, 2)` sonucu `"2.00"`. Yuvarlama kuralı `yuvarla` ile aynıdır. |
| `aralık(son)`            | `0`'dan `son`'a kadar (son hariç) sayıların listesi.             |
| `aralık(baş, son)`       | `baş`'tan `son`'a kadar (son hariç) sayıların listesi.           |
| `aralık(baş, son, adım)` | Aynı, `adım` kadar artarak. Adım negatif olabilir.               |
| `saat()`                 | Programın kullandığı işlemci süresi, saniye cinsinden.           |

### 6.4 Listeler ve sözlükler

| Fonksiyon                       | Açıklama                                                  |
|---------------------------------|-----------------------------------------------------------|
| `ekle(liste, öğe)`              | Öğeyi listenin sonuna ekler.                              |
| `araya_ekle(liste, dizin, öğe)` | Öğeyi verilen dizine yerleştirir, sonrakileri kaydırır.   |
| `çıkar(liste)`                  | Son öğeyi listeden çıkarır ve döndürür.                   |
| `sil(liste, dizin)`             | Dizindeki öğeyi siler ve döndürür.                        |
| `sil(sözlük, anahtar)`          | Girdiyi siler ve değerini döndürür.                       |
| `sırala(liste)`                 | Küçükten büyüğe sıralı yeni liste. Öğelerin tümü sayı ya da tümü metin olmalıdır; metinler Türk alfabesine göre sıralanır. |
| `sırala(liste, anahtar)`        | Öğeleri `anahtar(öğe)` sonucuna göre sıralar; böylece sözlükler ve nesneler de sıralanabilir. Anahtarı eşit olan öğeler özgün sıralarını korur. |
| `eşle(liste, f)`                | Her öğe için `f(öğe)` sonucunu içeren yeni liste.         |
| `süz(liste, f)`                 | `f(öğe)` sonucu `doğru` olan öğelerden oluşan yeni liste. |
| `ters(liste)`                   | Öğeleri ters sırada yeni liste. Metin de verilebilir.     |
| `bul(liste, öğe)`               | Öğenin ilk dizini; yoksa `-1`.                            |
| `anahtarlar(sözlük)`            | Anahtarların listesi.                                     |
| `değerler(sözlük)`              | Değerlerin listesi.                                       |
| `al(sözlük, anahtar, varsayılan)` | Anahtarın değeri; anahtar yoksa `varsayılan`.           |

### 6.5 Metinler

| Fonksiyon                     | Açıklama                                                    |
|-------------------------------|-------------------------------------------------------------|
| `büyük_harf(m)`               | Büyük harfe çevirir. Türkçe kuralları uygulanır: `i` → `İ`, `ı` → `I`. |
| `küçük_harf(m)`               | Küçük harfe çevirir: `İ` → `i`, `I` → `ı`.                  |
| `kırp(m)`                     | Baştaki ve sondaki boşlukları atar.                         |
| `bul(m, aranan)`              | Alt metnin ilk geçtiği karakter dizini; yoksa `-1`.         |
| `değiştir(m, eski, yeni)`     | `eski`'nin geçtiği her yeri `yeni` ile değiştirir.          |
| `böl(m, ayraç)`               | Metni ayraçtan bölerek liste üretir. Boş ayraç karakterlere böler. |
| `birleştir(liste, ayraç)`     | Öğeleri metne çevirip aralarına `ayraç` koyarak birleştirir. |
| `tekrarla(m, adet)`           | Metni arka arkaya `adet` kez yazar: `tekrarla("-", 3)` sonucu `"---"`. |
| `sola_doldur(m, genişlik)`    | Metni sağa yaslar; `genişlik` karaktere tamamlamak için soluna boşluk ekler. Üçüncü argüman olarak başka bir dolgu karakteri verilebilir: `sola_doldur("7", 3, "0")` sonucu `"007"`. |
| `sağa_doldur(m, genişlik)`    | Metni sola yaslar; sağına dolgu ekler.                      |
| `başlar_mı(m, ön)`            | Metin `ön` ile başlıyorsa `doğru`.                          |
| `biter_mi(m, son)`            | Metin `son` ile bitiyorsa `doğru`.                          |

Harf dönüşümü ve alfabetik sıralama Türk alfabesindeki harfleri ve İngilizce
harfleri kapsar; diğer alfabelerin harfleri değiştirilmeden bırakılır.

## 6A. Standart kütüphane

Aşağıdaki modüller `kullan` ile yüklenir.

### matematik

| Üye                    | Açıklama                                              |
|------------------------|-------------------------------------------------------|
| `pi`, `e`              | Sabitler.                                             |
| `üs(taban, üs)`        | Üs alma.                                              |
| `karekök(x)`           | Karekök.                                              |
| `sin(x)` `cos(x)` `tan(x)` | Trigonometrik fonksiyonlar; açı radyan cinsindendir. |
| `ln(x)` `log10(x)`     | Doğal ve onluk logaritma.                             |
| `en_küçük(...)` `en_büyük(...)` | Verilen sayıların ya da bir sayı listesinin en küçüğü, en büyüğü. |
| `toplam(...)`          | Verilen sayıların ya da bir sayı listesinin toplamı.  |

### rastgele

| Üye                | Açıklama                                                      |
|--------------------|---------------------------------------------------------------|
| `sayı()`           | 0 ile 1 arasında (1 hariç) rastgele sayı.                     |
| `tam(alt, üst)`    | `alt` ve `üst` dahil rastgele tam sayı.                       |
| `seç(liste)`       | Listeden rastgele bir öğe.                                    |
| `karıştır(liste)`  | Listenin öğelerini yerinde karıştırır.                        |
| `tohum(sayı)`      | Üreteci sıfırlar; aynı tohum her zaman aynı diziyi üretir.    |

### zaman

| Üye              | Açıklama                                                        |
|------------------|-----------------------------------------------------------------|
| `şimdi()`        | 1 Ocak 1970'ten bu yana geçen saniye; kesirli kısmı da içerir.  |
| `bekle(saniye)`  | Programı verilen süre kadar durdurur.                           |
| `tarih()`        | Yerel tarih ve saat; `yıl`, `ay`, `gün`, `saat`, `dakika`, `saniye`, `haftanın_günü` (1: Pazartesi) anahtarlı sözlük. |

### dosya

| Üye                  | Açıklama                                                    |
|----------------------|-------------------------------------------------------------|
| `oku(yol)`           | Dosyanın tüm içeriği, metin olarak.                         |
| `satırlar(yol)`      | Dosyanın satırları, liste olarak.                           |
| `yaz(yol, içerik)`   | Dosyayı içerikle oluşturur; varsa üzerine yazar.            |
| `ekle(yol, içerik)`  | İçeriği dosyanın sonuna ekler.                              |
| `var_mı(yol)`        | Yol bir dosyayı ya da klasörü gösteriyorsa `doğru`.         |
| `klasör_mü(yol)`     | Yol bir klasörü gösteriyorsa `doğru`.                       |
| `sil(yol)`           | Dosyayı siler.                                              |
| `listele(yol)`       | Klasördeki dosya ve klasör adlarının listesi; sıra belirsizdir. |
| `klasör_oluştur(yol)` | Klasör oluşturur.                                          |
| `klasör_sil(yol)`    | Boş bir klasörü siler.                                      |

Dosyalar UTF-8 olarak okunur ve yazılır. Yollar da UTF-8'dir; Türkçe harf
içerebilir.

### sistem

| Üye           | Açıklama                                                          |
|---------------|-------------------------------------------------------------------|
| `argümanlar`  | Komut satırında dosya adından sonra verilen argümanların listesi. |
| `platform`    | `"windows"`, `"linux"` ya da `"macos"`.                           |
| `ortam(ad)`   | Ortam değişkeninin değeri; tanımlı değilse `boş`.                 |
| `çık(kod)`    | Programı verilen çıkış koduyla sonlandırır.                       |
| `hata_yaz(...)` | `yaz` gibi çalışır, ancak standart hata çıktısına yazar.        |
| `betik`       | Çalıştırılan dosyanın yolu.                                       |
| `betik_klasörü` | Çalıştırılan dosyanın bulunduğu klasör.                         |

### json

| Üye                   | Açıklama                                                  |
|-----------------------|-----------------------------------------------------------|
| `çöz(metin)`          | JSON metnini JUS değerine çevirir: nesne sözlük, dizi liste, `true`/`false` mantıksal, `null` `boş` olur. Geçersiz JSON hata verir. |
| `yaz(değer)`          | Değeri tek satırlık JSON metnine çevirir.                 |
| `yaz(değer, girinti)` | Aynı, okunaklı biçimde; `girinti` her düzey için boşluk sayısıdır. |

JSON'a çevrilebilen değerler: `boş`, mantıksal, sayı, metin, liste ve anahtarları
metin olan sözlük. Sayılar, geri okunduğunda aynı değeri veren en kısa yazımla
yazılır; bu yazım `yaz` fonksiyonunun gösterdiğinden uzun olabilir.

### ağ

TCP bağlantıları. Bağlantılar ve dinleyiciler birer sayı ile temsil edilir;
bu sayı diğer fonksiyonlara verilir.

| Üye                       | Açıklama                                              |
|---------------------------|-------------------------------------------------------|
| `bağlan(sunucu, port)`    | Sunucuya bağlanır, bağlantıyı döndürür.               |
| `dinle(port)`             | Yalnızca bu bilgisayardan (127.0.0.1) gelen bağlantıları bekleyen dinleyici açar. Port 0 verilirse boş bir port seçilir. |
| `dinle(port, adres)`      | Verilen IPv4 adresinde dinler; `"0.0.0.0"` tüm ağ arayüzleridir. Port başka bir program tarafından kullanılıyorsa hata verir. |
| `port(dinleyici)`         | Dinleyicinin kullandığı port.                         |
| `kabul_et(dinleyici)`     | Bir bağlantı gelene kadar bekler, bağlantıyı döndürür. |
| `gönder(bağlantı, metin)` | Metnin tamamını gönderir.                             |
| `satır_al(bağlantı)`      | Bir satır okur (satır sonu atılmış). Veri bittiyse `boş`. |
| `al(bağlantı, en_çok)`    | En çok verilen bayt kadar veri okur. Bağlantı kapandıysa `boş`. |
| `tam_al(bağlantı, adet)`  | Tam olarak verilen bayt kadar veri okur; bağlantı erken kapanırsa daha azını döndürür. |
| `zaman_aşımı(bağlantı, saniye)` | Okurken beklenecek en uzun süre; aşılırsa okuma hata verir. 0 sınırsızdır. |
| `kapat(tutamaç)`          | Bağlantıyı ya da dinleyiciyi kapatır.                 |
| `bayt_sayısı(metin)`      | Metnin UTF-8 olarak kapladığı bayt sayısı.            |
| `url_kodla(metin)`        | Metni adreslerde kullanılabilecek biçime çevirir (`%XX`). |
| `url_çöz(metin)`          | `%XX` dizilerini ve `+` işaretini çözer.              |

Tüm işlemler bekletir: `kabul_et` ve okuma fonksiyonları sonuç gelene kadar
programı durdurur. Aynı anda en çok 256 bağlantı açık olabilir.

### http

HTTP/1.1 istemcisi ve sunucusu. Yalnızca şifresiz (`http://`) adresler
desteklenir; `https://` adresleri hata verir.

| Üye                                   | Açıklama                                  |
|---------------------------------------|-------------------------------------------|
| `getir(adres)`                        | GET isteği gönderir, yanıtı döndürür.     |
| `gönder(adres, gövde)`                | POST isteği gönderir, yanıtı döndürür.    |
| `iste(yöntem, adres, başlıklar, gövde)` | Herhangi bir istek gönderir. `başlıklar` bir sözlüktür. |
| `sun(port, işleyici)`                 | Bu bilgisayarda bir sunucu başlatır; her istek için `işleyici(istek)` çağrılır. |
| `dinleyiciyle_sun(dinleyici, işleyici)` | Aynı, `ağ.dinle` ile açılmış bir dinleyici üzerinde. |

Yanıt, `durum` (sayı), `başlıklar` (sözlük; adlar küçük harfli) ve `gövde`
(metin) anahtarlı bir sözlüktür.

İşleyiciye verilen istek, `yöntem`, `yol`, `sorgu` (sözlük), `başlıklar`
(sözlük; adlar küçük harfli) ve `gövde` anahtarlı bir sözlüktür. İşleyici ya
bir metin (gövde; durum 200 olur) ya da `durum`, `gövde`, `başlıklar`
anahtarlarından istediklerini içeren bir sözlük döndürür. Sözlükte
`"durdur": doğru` varsa sunucu o yanıttan sonra durur. İşleyicideki
yakalanmamış hata ya da metin ve sözlük dışında bir dönüş değeri istemciye 500
yanıtı olarak gider; sunucu çalışmayı sürdürür.

```jus
kullan http

fonksiyon işle(istek):
    eğer istek["yol"] == "/":
        dön "Merhaba, " + al(istek["sorgu"], "ad", "dünya")
    dön {"durum": 404, "gövde": "Bulunamadı"}

http.sun(8080, işle)
```

Sunucu istekleri sırayla, birer birer işler. Daha alt düzey kullanım için
`istek_gönder`, `yanıtı_oku`, `isteği_oku` ve `yanıt_gönder` fonksiyonları açık
bağlantılar üzerinde çalışır.

### tr

Türkiye'ye özgü doğrulama ve biçimlendirme işlevleri.

| Üye                         | Açıklama                                            |
|-----------------------------|-----------------------------------------------------|
| `kimlik_no_geçerli_mi(no)`  | T.C. kimlik numarasının denetim basamakları doğruysa `doğru`. Metin ya da sayı alır. |
| `iban_geçerli_mi(iban)`     | Türkiye IBAN'ının biçimi ve denetim basamakları doğruysa `doğru`. Boşluklar yok sayılır. |
| `telefon_geçerli_mi(no)`    | Türkiye telefon numarası biçimine uyuyorsa `doğru`. Boşluk, parantez, tire ve `+90` kabul edilir. |
| `para(tutar)`               | Tutarı `1.234,50 ₺` biçiminde metne çevirir.        |
| `plaka_ili(kod)`            | Plaka koduna (1-81) karşılık gelen ilin adı.        |
| `iller`                     | 81 ilin plaka koduna göre sıralı listesi.           |

Bu işlevler yalnızca biçimi ve denetim basamaklarını doğrular; numaranın
gerçekten var olup olmadığını denetlemez.

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

Çalışma sırasında oluşan ve bir `dene` bloğunca yakalanmayan hata programı
durdurur. Hata bir fonksiyonun içinde oluştuysa çağrı zinciri de gösterilir:

```
ornek.jus:2: çalışma zamanı hatası: Sıfıra bölünemez.
    satır 2, 'böl' fonksiyonu
    satır 4, ana program
```

Çıkış kodu 70 olur.

### 7.3 Öneriler

Hata iletileri, bulunamayan ad için bilinen adlar arasından en yakınını önerir:
tanımsız bir değişken için kapsamdaki değişkenler, yerleşik fonksiyonlar ve
anahtar kelimeler; nesnede bulunamayan üye için alanlar ve yöntemler; modülde
bulunamayan üye için modülün üyeleri; sözlükte bulunamayan metin anahtarı için
sözlüğün anahtarları. Yakınlık ölçülürken Türkçe harfler ASCII karşılıklarıyla
eş sayılır; `sayac` yazılmışsa `sayaç` önerilir. Başka dillerden gelen adlar
(`print`, `len`, `append`, `if` ...) için JUS karşılığı söylenir; parantezsiz
fonksiyon çağrısı için parantez hatırlatılır.

```
ornek.jus:2: çalışma zamanı hatası: 'sayac' adında bir değişken ya da fonksiyon tanımlı değil. 'sayaç' mı demek istediniz?
```

Öneriler iletinin parçasıdır ve 1.0'daki güvencenin dışındadır; `dene` ile
yakalanan hata değeri de öneriyi içerir.

## 8. Etkileşimli kip

`jus` argümansız çalıştırıldığında etkileşimli kip başlar.

- Her satır yazıldığı anda çalıştırılır; tanımlar sonraki satırlarda
  geçerliliğini korur.
- Tek başına yazılan bir ifadenin sonucu `boş` değilse ekrana yazılır.
- `:` ile biten satır bir blok başlatır; blok boş bir satırla bitirilir.
- `çıkış` yazmak ya da girdiyi kapatmak kipi sonlandırır.

Girdi bir uçbirimse (klavye) satır düzenleme kullanılır:

- Sol ve sağ ok tuşları ile `Home` / `End` imleci taşır. `Backspace` imlecin
  solundaki karakteri siler; Türkçe harfler (`ç`, `ğ`, `ş`, `ö`, `ü` ...) tek
  tuşla silinir.
- Yukarı ve aşağı ok tuşları önceki girişler arasında gezdirir. Geçmiş,
  kullanıcının ev klasöründeki `.jus_gecmis` dosyasında saklanır ve en çok 1000
  girişi tutar.
- `Tab` yazılmakta olan adı tamamlar. Adaylar anahtar kelimeler, yerleşik
  fonksiyonlar, o ana kadar tanımlanan adlar ve yüklü modüllerdir.
- `Ctrl+C` yazılmakta olan satırı iptal eder. Boş satırda
  `Ctrl+D` (Windows'ta `Ctrl+Z`) girdiyi kapatır ve kipten çıkar.

Girdi bir dosyadan ya da başka bir programdan yönlendirilmişse satır
düzenleme kullanılmaz; satırlar düz okunur.

## 8A. Testler

`jus test [yol]` komutu, verilen klasördeki (yazılmazsa bulunulan klasördeki)
ve alt klasörlerindeki, adı `_test.jus` ile biten dosyaları çalıştırır. Yol
olarak tek bir test dosyası da verilebilir.

Her dosya için önce dosyanın üst düzey kodu çalışır. Ardından adı `test_` ile
başlayan, parametresiz her fonksiyon ad sırasıyla çağrılır. Hata vermeden biten
fonksiyon geçmiş, hata veren kalmış sayılır; bir testin kalması diğerlerinin
çalışmasını engellemez.

```jus
# hesap_test.jus
kullan hesap

fonksiyon test_toplama():
    eşit_olmalı(hesap.topla(2, 3), 5)

fonksiyon test_sıfıra_bölme_hata_verir():
    değişken hata_verdi = yanlış
    dene:
        hesap.böl(1, 0)
    yakala:
        hata_verdi = doğru
    doğrula(hata_verdi, "böl(1, 0) hata vermeliydi")
```

```
hesap_test.jus
  geçti  test_sıfıra_bölme_hata_verir
  geçti  test_toplama

1 dosya, 2 test: 2 geçti, 0 kaldı.
```

Tüm testler geçerse çıkış kodu 0, en az biri kalırsa 1 olur.

## 8B. Biçimlendirici

`jus bicimle dosya.jus ...` komutu, verilen dosyaları standart biçime getirir
ve değişen dosyaların üzerine yazar. `jus bicimle --denetle dosya.jus ...`
dosyaları değiştirmez; biçimi standart olmayan dosya varsa adlarını yazar ve 1
koduyla çıkar.

Standart biçim:

- Her blok düzeyi 4 boşluk girintilidir.
- İkili işleçlerin iki yanında birer boşluk, virgülden sonra bir boşluk bulunur;
  parantezlerin iç kenarında boşluk bulunmaz.
- Satır sonu yorumundan önce iki boşluk, `#` işaretinden sonra bir boşluk bulunur.
- Art arda en çok iki boş satır bulunur; satır sonlarında boşluk kalmaz; dosya
  tek bir satır sonuyla biter.

Biçimlendirici satırları bölmez ya da birleştirmez; metinlerin ve yorumların
içeriğine dokunmaz. Programın davranışı değişmez. Sözcük düzeyinde hata içeren
(ör. kapanmamış metin) dosyalar biçimlendirilmez.

## 8C. Paketler

Paketler git depolarıdır ve komutun çalıştırıldığı klasördeki `jus_paketleri/`
klasörüne indirilir. Bu işlem için bilgisayarda `git` kurulu olmalıdır.

| Komut                             | Açıklama                                   |
|-----------------------------------|--------------------------------------------|
| `jus paket kur <git-adresi> [ad]` | Depoyu `jus_paketleri/<ad>/` altına indirir. Ad verilmezse adresin son parçası kullanılır; baştaki `jus-` atılır. |
| `jus paket listele`               | Kurulu paketleri listeler.                 |
| `jus paket kaldır <ad>`           | Paketi siler.                              |

Bir paketin ana modülü, paketin adını taşıyan dosyadır: `renkler` paketi için
`renkler.jus`. Program bu modülü `kullan renkler` ile, paketin diğer
modüllerini `kullan "renkler/modül"` ile kullanır. Paketler, ana programın
bulunduğu klasördeki `jus_paketleri/` klasöründe aranır.

Paket yöneticisi sürüm seçmez ve paketlerin birbirine bağımlılıklarını
çözmez; her paket deponun en son hâliyle kurulur.

## 9. Dilbilgisi

```
program        = { bildirim } ;
bildirim       = değişken_tanımı | fonksiyon_tanımı | sınıf_tanımı | deyim ;
sınıf_tanımı   = "sınıf" AD [ "(" AD ")" ] ":" SATIR_SONU
                 GİRİNTİ fonksiyon_tanımı { fonksiyon_tanımı } GİRİNTİ_SONU ;

değişken_tanımı  = "değişken" AD [ "=" ifade ] SATIR_SONU ;
fonksiyon_tanımı = "fonksiyon" AD "(" [ parametreler ] ")" blok ;
parametreler     = parametre { "," parametre } [ "," "*" AD ] | "*" AD ;
parametre        = AD [ "=" ifade ] ;

deyim          = eğer_deyimi | iken_deyimi | her_deyimi | dön_deyimi
               | dene_deyimi | fırlat_deyimi | kullan_deyimi
               | "kır" SATIR_SONU | "devam" SATIR_SONU | "geç" SATIR_SONU
               | ifade SATIR_SONU ;
dene_deyimi    = "dene" blok "yakala" [ AD ] blok ;
fırlat_deyimi  = "fırlat" ifade SATIR_SONU ;
kullan_deyimi  = "kullan" ( AD | METİN ) [ "olarak" AD ] SATIR_SONU ;
eğer_deyimi    = "eğer" ifade blok [ "değilse" ( eğer_deyimi | blok ) ] ;
iken_deyimi    = "iken" ifade blok ;
her_deyimi     = "her" AD "içinde" ifade blok ;
dön_deyimi     = "dön" [ ifade ] SATIR_SONU ;
blok           = ":" SATIR_SONU GİRİNTİ bildirim { bildirim } GİRİNTİ_SONU ;

ifade          = atama ;
atama          = hedef atama_işleci atama | veya_ifadesi ;
hedef          = AD | çağrı "[" ifade "]" | çağrı "." AD ;
atama_işleci   = "=" | "+=" | "-=" | "*=" | "/=" ;
veya_ifadesi   = ve_ifadesi { "veya" ve_ifadesi } ;
ve_ifadesi     = değil_ifadesi { "ve" değil_ifadesi } ;
değil_ifadesi  = "değil" değil_ifadesi | eşitlik ;
eşitlik        = karşılaştırma { ( "==" | "!=" ) karşılaştırma } ;
karşılaştırma  = toplam { ( "<" | "<=" | ">" | ">=" | "içinde" ) toplam } ;
toplam         = çarpım { ( "+" | "-" ) çarpım } ;
çarpım         = tekli { ( "*" | "/" | "%" ) tekli } ;
tekli          = "-" tekli | çağrı ;
çağrı          = birincil { "(" [ ifade { "," ifade } ] ")" | dizin | "." AD } ;
dizin          = "[" ifade "]" | "[" [ ifade ] ":" [ ifade ] "]" ;
birincil       = SAYI | METİN | BİÇİMLİ_METİN | AD | "doğru" | "yanlış" | "boş" | "bu"
               | "üst" "." AD | "(" ifade ")" | liste | sözlük ;
liste          = "[" [ ifade { "," ifade } [ "," ] ] "]" ;
sözlük         = "{" [ çift { "," çift } [ "," ] ] "}" ;
çift           = ifade ":" ifade ;
```

`BİÇİMLİ_METİN` bir sözcük türüdür ve 2.5A'da tanımlanır; `{ }` arasındaki
ifadeler `veya_ifadesi` kuralıyla ayrıştırılır. Varsayılan değeri olan
parametreden sonra varsayılan değeri olmayan parametre gelemeyeceği kuralı
(5.4) dilbilgisinin dışında, anlam denetiminde uygulanır.
