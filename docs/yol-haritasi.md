# Yol Haritası

## Hedef

JUS: Türkçe sözdizimli, genel amaçlı, öğrenmesi kolay bir betik dili. Tek
dosyalık bir çalıştırıcıyla her platformda çalışır ve gerçek program yazmaya
yeter.

Öncelikli kullanıcılar sırasıyla: programlamayı öğrenenler ve öğretenler,
ardından küçük otomasyon betikleri yazanlar. JUS bir varış noktası değil, bir
köprüdür: kavramları ana dilinde öğrenen biri, Python gibi yaygın dillere
geçtiğinde yalnızca kelimeleri değiştirir.

## İlkeler

1. **Çalışmayan özellik belgelenmez.** Belgelerde yazan her şeyin testi vardır.
2. **Az ve kesin anahtar kelime.** Yalnızca gerçekten kullanılan kelimeler
   ayrılır.
3. **Çekirdek tarafsızdır.** Ülkeye ya da alana özgü işlevler dilin içinde
   değil, kütüphanelerde yer alır.
4. **Hata iletileri ürünün parçasıdır.** Her ileti Türkçedir, konumu gösterir
   ve mümkünse çözümü söyler.
5. **Sürümleme kurallıdır.** Sürüm numaraları anlamsal sürümlemeye uyar; her
   sürümün değişiklikleri kaydedilir. 1.0'dan sonra dil davranışını bozan
   değişiklik yalnızca ana sürümde yapılır.

## 1.x sürümleri

1.x sürümleri var olan programları bozmaz; yalnızca ekleme yapar.

Durum sütunu: "Yayımlandı" yalnızca `main`'e birleştirilip etiketi gönderilmiş
sürümler için kullanılır; "Tamamlandı, yayım bekliyor" işi bitmiş, etiketi yerel
olarak konmuş ama henüz yayımlanmamış sürümleri gösterir (bkz. "Elle yapılacak
adımlar").

| Sürüm | Tema | İçerik | Durum |
|-------|------|--------|-------|
| 1.0 | Kararlı dil | Dil tanımının dondurulması | Yayımlandı |
| 1.1 | Erişim | Belge sitesi, "JUS'tan Python'a" rehberi, Scoop ve winget paket tanımları, VS Code eklenti paketi, logo | Tamamlandı, yayım bekliyor |
| 1.2 | Sınıf içi kullanım | Deneme alanında paylaşılabilir bağlantı, hata iletilerinde "şunu mu demek istediniz?" önerileri, öğretmen kiti | Tamamlandı, yayım bekliyor |
| 1.3 | Dil rahatlığı | Varsayılan ve değişken sayıda parametre, biçimli metin (`f"..."`), etkileşimli kipte satır düzenleme, geçmiş ve tamamlama | Planlandı |
| 1.4 | Ağ ve veri | Şifreli bağlantılar (`https`), bit işleçleri, bayt dizileri (`baytlar`); ayrıntı aşağıda | Planlandı |
| 1.5 | Düzenleyici desteği ve 2.0'a hazırlık | Dil sunucusu (`jus lsp`): VS Code'da hata gösterimi, tamamlama, tanıma gitme, biçimlendirme. 2.0'a geçişi kolaylaştıran eklemeler (`//`, `tam`, `ondalık`, `sayı_mı`) ve `jus denetle --2.0` geçiş denetimi; ayrıntı aşağıda | Planlandı |

### 1.4: bit işleçleri ve bayt dizileri

Bu özellikler 2.0'daki tam sayı türünden önce geliyor. Tanımları, tam sayı türü
geldiğinde davranışları değişmeyecek biçimde yapılmıştır.

**Bit işleçleri** `&`, `|`, `^`, `~`, `<<`, `>>` (öncelik Python ile aynıdır:
kaydırma işleçleri `+`/`-` ile karşılaştırmanın arasında; `&`, `^`, `|` sırasıyla
kaydırmanın altında, karşılaştırmanın üstünde).

- İşlenenler **tam değerli** olmalıdır. Ondalık kısmı olan bir sayı (`1.5`)
  çalışma zamanı hatasıdır; kırpma yapılmaz. Gerekçe: sessiz kırpma hatayı
  gizler; `~1.5` gibi bir ifade neredeyse her zaman bir yanlışlıktır.
- 1.4'te işlenenler ±2^53 aralığında olmalıdır; aşan değer hatadır. Gerekçe:
  1.4'te sayılar ondalık gösterimlidir ve bu sınırın ötesinde kesin değildir;
  kesin olmayan bir sayı üzerinde bit işlemi anlamsızdır. 2.0'da tam sayı
  türü için sınır ±2^63 olur; ondalık türdeki tam değerler için 2^53 sınırı
  sürer. 1.4'te geçerli olan her ifade 2.0'da aynı sonucu verir.
- Hesap 64 bit ikiye tümleyen üzerinde yapılır. `~x` sonucu `-x - 1`; `>>`
  işareti korur (aritmetik kaydırma). Kaydırma miktarı 0 ile 63 arasında
  olmalıdır. `<<` sonucu izin verilen aralığı aşarsa hatadır (kırpma ya da
  sarma yapılmaz); gerekçe aynıdır.
- Sonuç 1.4'te `sayı`, 2.0'da her durumda `tam sayı` türündedir; işlenenler
  tam değerli ondalık sayı olsa da sonuç tam sayıdır, böylece dizin olarak
  kullanılabilir. Yazılı biçim iki sürümde de aynıdır.

**Bayt dizisi** (`baytlar` türü): değiştirilebilir, sabit uzunluklu olmayan
bayt dizisi. `baytlar(uzunluk)` sıfırlarla dolu, `baytlar(liste)` verilen
sayılarla dolu bir dizi oluşturur.

- Öğeler 0 ile 255 arasında tam değerlerdir. Dizin okuması bu aralıkta bir
  sayı verir (1.4'te `sayı`, 2.0'da `tam sayı`; yazılı biçimi aynıdır).
- Aralık dışı ya da tam olmayan bir değer atamak çalışma zamanı hatasıdır;
  256'ya göre kalan alınmaz. Gerekçe: bit işleçleriyle aynı.
- Dizinleme, dilimleme, `uzunluk`, `her ... içinde`, `+` ile birleştirme,
  içerikle eşitlik ve `içinde` desteklenir; metinlerle aynı kurallar geçerlidir.
- `bayt` modülü dönüşümleri sağlar: `metinden` / `metne` (UTF-8),
  `onaltılıktan` / `onaltılığa`, `base64ten` / `base64e`. `dosya.baytları_oku`,
  `dosya.baytları_yaz`, `ağ.bayt_gönder`, `ağ.bayt_al` ikili veriyle çalışır.

### 1.4: şifreli bağlantılar için TLS seçimi

| Seçenek | Çalıştırıcı boyutu | Tek dosya hedefi | Üç platformda derleme | Bakım yükü |
|---------|--------------------|------------------|-----------------------|------------|
| Gömülü TLS kütüphanesi (BearSSL ya da mbedTLS kaynakları depoya alınır) | +400-700 KB; kök sertifika demeti de gömülmeli (+200 KB) | Korunur | Kolay: salt C, dış bağımlılık yok | Yüksek: kütüphanenin güvenlik yamaları ve kök sertifika demeti her sürümde güncellenmeli; kriptografi kodu depoda taşınır |
| Platformun kendi altyapısı (Windows SChannel, macOS Secure Transport ya da Network.framework) | +0 | Korunur | Windows ve macOS'ta sistem kütüphaneleri her zaman vardır; Linux'ta karşılığı yoktur | Düşük: sertifika deposu ve yamalar işletim sistemine ait; platform başına ince bir katman |
| Sistemdeki OpenSSL (çalışma zamanında yüklenir, derlemede bağımlılık yok) | +0 | Korunur | Derleme her yerde kolaydır; ancak Windows'ta sistem OpenSSL'i yoktur, macOS'ta yalnızca Homebrew ile gelir | Düşük: tek kod yolu; sürüm farklarına (1.1 / 3.x) dikkat gerekir |

**Karar:** Windows'ta SChannel, macOS'ta Secure Transport, Linux'ta çalışma
zamanında yüklenen sistem OpenSSL'i (`libssl.so.3`, yoksa `libssl.so.1.1`).
Gerekçe: çalıştırıcı büyümez, tek dosya hedefi korunur, kök sertifikalar ve
güvenlik yamaları işletim sistemiyle gelir, derleme hiçbir platformda yeni
bağımlılık istemez. Bedeli üç ince kod yoludur; bunlar ortak bir arayüzün
(bağlan, gönder, al, kapat) arkasında durur ve aynı testle sınanır. Linux'ta
OpenSSL yoksa `https` adresleri, neyin kurulması gerektiğini söyleyen bir
hata verir; `http` çalışmayı sürdürür. Tarayıcı derlemesinde `https`, `ağ`
gibi kullanılamaz.

Kararın doğrulanması:

- **Linux ve durağan bağlama.** Sürüm iş akışı yalnızca Windows paketini
  `-static` ile derler; Linux ve macOS paketleri dinamik bağlanır (sistem C
  kütüphanesine). Dinamik bağlanan bir çalıştırıcıda `dlopen` çalışır; karar
  geçerlidir. Bu bir koşuldur ve belgelenir: Linux paketi ileride durağan
  bağlanırsa `dlopen` kullanılamaz ve Linux için gömülü kütüphane seçeneğine
  dönülmesi gerekir. Eski glibc sürümleri için `-ldl` bağlanır.
- **macOS ve Secure Transport.** Secure Transport macOS 10.15'ten (2019) bu
  yana kullanımdan kaldırılmış durumdadır, yeni özellik almaz ve **TLS 1.3
  desteklemez**; TLS 1.2 ile çalışır ve güncel macOS sürümlerinde hâlâ
  mevcuttur. Apple'ın önerdiği Network.framework TLS 1.3 verir, ancak soketi
  kendisi yönetir (bizim `ağ` modülümüzün soketlerine takılamaz), eşzamansız
  çalışır ve C'den kullanımı blok uzantısı ister. 1.4 için Secure Transport
  seçildi: `ağ` modülünün var olan soketine okuma/yazma geri çağrılarıyla
  takılır, ~250 satırdır ve bugün çalışır. Kabul edilen sınır: macOS'ta
  `https` TLS 1.2 ile konuşur; 2026'da yalnızca TLS 1.3 kabul eden sunucu
  nadirdir. Apple bu arayüzü kaldırırsa ya da TLS 1.3 zorunlu hâle gelirse
  macOS katmanı Network.framework ile yeniden yazılır; bu, "2.0 sonrası"
  listesinde adaydır. Secure Transport'un derlenip çalıştığı CI'daki macOS
  ağ testiyle her sürümde doğrulanır.

### 1.5: 2.0'a hazırlık eklemeleri

Aşağıdakiler 1.x'i bozmayan eklemelerdir ve 1.5'te 1.x anlamıyla gelir;
2.0'da aynı adlarla, tam sayı türüne göre çalışırlar. Amaç, 2.0'a geçişte
önerilen her düzeltmenin 1.5'te de çalışmasıdır.

| Ekleme | 1.5'teki anlamı | 2.0'daki anlamı |
|--------|-----------------|-----------------|
| `a // b` | Tabana yuvarlanmış bölme; sonuç `sayı`. `7 // 2` sonucu `3`, `-7 // 2` sonucu `-4`. Sıfıra bölme hatadır. | Aynı değer; iki tam sayıda `tam sayı`, aksi halde `ondalık sayı`. |
| `tam(x)` | Sıfıra doğru kesilmiş tam değer; sonuç `sayı`. Metin verilirse `sayı()` gibi çevirip keser. | Aynı değer; `tam sayı` türünde. |
| `ondalık(x)` | Değeri olduğu gibi verir (tüm sayılar zaten ondalık gösterimlidir); metin verilirse `sayı()` gibi çevirir. | `ondalık sayı` türüne çevirir. |
| `sayı_mı(x)` | `tür(x) == "sayı"` | `x` tam sayı ya da ondalık sayıysa `doğru`. |

### 1.5: `jus denetle --2.0`

2.0'da davranışı değişecek kodu satır satır raporlayan denetim komutu. Dosya ya
da klasör alır; her bulgu dosya adı, satır, açıklama ve önerilen değişiklikle
yazılır. Bulgu varsa çıkış kodu 1'dir.

Denetim yalnızca **hem 1.5'te hem 2.0'da aynı sonucu veren** düzeltmeler
önerir; böylece programlar 2.0 çıkmadan düzeltilebilir.

| Bulgu | Neden | Öneri |
|-------|-------|-------|
| İki tam sayı sabitinin `/` ile bölünmesi; `/` sonucunun doğrudan dizin, dilim sınırı, `aralık`, `tekrarla`, `sola_doldur` gibi tam sayı isteyen yerlerde kullanılması | 2.0'da `/` her zaman ondalık sayı verir ve ondalık sayı dizin olamaz | Tam bölme için `//` |
| `tür(...)` sonucunun `"sayı"` ile karşılaştırılması | 2.0'da `tür` tam sayılar için `"tam sayı"`, ondalıklar için `"ondalık sayı"` verir | `sayı_mı(x)` |
| `yakala ad:` ile yakalanan değerin metin olarak kullanılması: `+` ile birleştirme, `bul`, `başlar_mı`, `içinde`, dizinleme, `uzunluk`, `böl` | 2.0'da dilin hataları `Hata` nesnesidir; metin işlemleri `TürHatası` verir | `metin(hata)` |
| Yakalanan değerin `==` ya da `!=` ile bir metinle karşılaştırılması | **Sessiz değişiklik:** farklı türdeki değerler hiçbir zaman eşit olmadığı için `hata == "..."` 2.0'da hata vermez, her zaman `yanlış` olur. Denetim bu bulguyu ayrıca vurgular. | `metin(hata) == "..."` |
| `json.yaz(...)` çağrıları | 2.0'da tam değerli ondalık sayılar JSON'a `2.0` olarak yazılır; sayının türü satır üzerinden bilinemediği için bulgu "olası" olarak işaretlenir | Tam sayı istenen yerde `tam(x)` |
| `sayı(...)` sonucunun `tür` ile denetlenmesi | `sayı("3")` 2.0'da tam sayı verir | `sayı_mı(x)` |

Denetim sözdizimsel ve yerel bilgiyle çalışır; tür bilgisi gerektiren
durumlarda (ör. bir fonksiyonun döndürdüğü değerin bölünmesi) bulguyu
"olası" olarak işaretler. 2.0'ın her kırıcı değişikliği için denetimin onu
yakaladığını gösteren bir test vardır.

## 2.0

2.0, 1.x'te yapılamayan çünkü var olan programların davranışını
değiştirebilecek üç değişikliği bir arada yapar. 1.x'teki hiçbir sözdizimi
kaldırılmaz; 1.x programlarının büyük çoğunluğu değiştirilmeden çalışır.
Değişiklikler bir geçiş rehberiyle gelir ve `jus denetle --2.0` ile önceden
bulunabilir.

### Yapılış sırası

1. **Tam sayı türü** önce ve tek başına biter. Sanal makinenin değer
   gösterimini, yerleşik fonksiyonları, JSON'u ve biçimlendirmeyi
   değiştirdiği için diğer iki konu başlamadan test paketi, örnek programlar
   ve hız ölçümleri bu değişiklikle geçmiş olmalıdır.
2. **Yapılı hatalar** tam sayı türünün üzerine kurulur: `satır` alanı tam
   sayıdır, hata türleri sınıf hiyerarşisidir, `TaşmaHatası` tam sayı
   türünün hatasıdır.
3. **Tür bildirimleri** en son gelir: `tam sayı`, `ondalık sayı` ve `Hata`
   dahil tüm türleri adlandırabilmek için önceki ikisine gerek duyar.

### Tam sayı türü: değer gösterimi

Sanal makinedeki her değer, bir tür etiketi ve bir birleşimden (mantıksal,
ondalık sayı, nesne işaretçisi) oluşan 16 baytlık bir yapıdır; birleşim 8
bayttır. 64 bit tam sayı bu birleşime sığar: ölçüldü, yapının boyutu 16 bayt
kalır. Dolayısıyla bellek kullanımı değişmez ve aralığı daraltmak (ör. NaN
kutulama ile 48-51 bit) gerekmez.

Hız etkisi aritmetikteki ek tür ayrımından gelir (tam/ondalık/karışık). Karar
ölçümle bağlanır: tam sayı türü bittiğinde `bench/` paketi 1.5 ile
karşılaştırılır. Geometrik ortalama %5'ten fazla gerilerse önce hızlı yol
(iki tam sayı ya da iki ondalık için tek dal) iyileştirilir; yine yetmezse
aralığı daraltma seçeneği (NaN kutulama) değerlendirilir ve karar burada
güncellenir.

### Tam sayı türü

| Konu | Karar | Gerekçe |
|------|-------|---------|
| Yazım ve aralık | Noktasız sayı sabiti tam sayıdır; 64 bit işaretlidir (±9.2×10^18). Bu aralığı aşan sabit sözdizimi hatasıdır. Noktalı sabit ondalık sayıdır. | 2^53 sınırı kalkar; sınırın aşılması sessizce yaklaşık değere dönmez. |
| `tür` | `tür(3)` sonucu `"tam sayı"`, `tür(3.5)` sonucu `"ondalık sayı"`. Tür bildirimlerinde `sayı` ikisini de kabul eder; `sayı_mı(x)` ikisi için `doğru` verir. | Öğrenci iki türü görür; eski "sayı" sözü kapsayıcı ad olarak kalır. |
| `/` | İki tam sayıda da sonuç ondalık sayıdır: `6 / 3` sonucu `2.0`. | Python 3 ile aynı; 1.x'teki "`/` her zaman ondalıklı bölme yapar" kuralı korunur. |
| `//` | Tam bölme: tabana yuvarlar. İki tam sayıda tam sayı, aksi halde ondalık sayı verir. Sıfıra bölme hatadır. | Dizin ve sayma işleri için tam sayı üreten bir bölme gerekir. |
| `%` | İki tam sayıda tam sayı; işaret kuralı 1.x ile aynı (bölenin işareti). | |
| Taşma | `+`, `-`, `*`, `//` ve `üs` sonucu tam sayı aralığını aşarsa `TaşmaHatası`: "Sonuç tam sayı aralığını aşıyor; büyük sayılar için ondalık(x) ile ondalık sayıya çevirin." Sarma ya da sessiz tür değişimi yoktur. | Aralığı aşan sabit, `<<` taşması ve bildirimli değişkenlerle tutarlı: hiçbir yerde tam sayı sessizce başka bir şeye dönmez. Hata, çözümü söyler. |
| En küçük değer | `-(-9223372036854775808)` ve `mutlak(-9223372036854775808)` `TaşmaHatası` verir. | Aynı kural. |
| Üs alma | `üs(tam, tam)`: üs 0 ya da pozitifse sonuç tam sayıdır (taşarsa `TaşmaHatası`); üs negatifse sonuç ondalık sayıdır (`üs(2, -1)` sonucu `0.5`). İşlenenlerden biri ondalıksa sonuç ondalık. `üs(0, -1)` `BölmeHatası` verir. | Python ile aynı. |
| Karışık işlemler | Tam sayı ile ondalık sayının aritmetiği ondalık sayı verir. | |
| Karşılaştırma | Tam sayı ile ondalık sayı matematiksel değerleriyle karşılaştırılır: `1 == 1.0` doğrudur; `9007199254740993 == 9007199254740992.0` yanlıştır (tam sayı ondalığa çevrilerek karşılaştırılmaz). Sözlükte `1` ile `1.0` aynı anahtardır. | Python ile aynı; "aynı değer, farklı tür" tuzağı önlenir. |
| Dizin ve sayım | Dizin, dilim sınırı, `aralık`, `tekrarla` gibi yerler tam sayı ister; `2.0` gibi tam değerli ondalık kabul edilmez. Hata iletisi çözümü söyler: "Dizin tam sayı olmalı; 2.0 ondalık sayı. Tam bölme için `//` kullanın." | `liste[6 / 3]` 1.x'te çalışıyordu; 2.0'da `//` istenir. Denetim komutu bu yerleri bulur. |
| Dönüşümler | `sayı("3")` tam sayı, `sayı("3.0")` ondalık sayı verir. `tam(x)` sıfıra doğru keser, `ondalık(x)` ondalığa çevirir. `taban`, `tavan`, `yuvarla(x)` tam sayı döndürür; `yuvarla(x, basamak)` ondalık döndürür. | |
| Aralık dışı dönüşüm | `tam`, `taban`, `tavan`, `yuvarla(x)` sonucu tam sayı aralığına sığmıyorsa `TaşmaHatası`; `x` sonsuz ya da tanımsız (NaN) ise `DeğerHatası` ("sonsuz bir değer tam sayıya çevrilemez"); sayı olmayan değer `TürHatası`. | Sessiz kırpma yoktur; her durumun adı ve iletisi vardır. |
| Yazım | `yaz` ve `metin()` 1.x ile aynıdır: tam sayılar olduğu gibi, tam değerli ondalıklar ondalık kısım olmadan yazılır (`yaz(6 / 3)` çıktısı `2`). Etkileşimli kipteki değer gösterimi ve hata iletileri ise türü belli eder: `6 / 3` yazınca `2.0` görünür, dizin hatası "2.0 ondalık sayı" der. | Rehber ve testlerdeki çıktılar korunur; öğrenci türü etkileşimli kipte ve hatalarda görür. |
| JSON | Tam sayılar JSON'da tam sayı, ondalıklar her zaman ondalık yazılır (`json.yaz(2.0)` sonucu `2.0`); `json.çöz` noktasız sayıları tam sayı, noktalı sayıları ondalık okur. Böylece yaz-oku döngüsü türü korur. **Bu bir kırıcı değişikliktir:** 1.x `json.yaz(2.0)` için `2` yazıyordu. | Türü koruyan tek seçenek budur; denetim komutu `json.yaz` çağrılarını "olası" bulgu olarak raporlar. |
| Bit işleçleri | Tam sayılarda 64 bit; tam değerli ondalıklarda 1.4 kuralları; sonuç her durumda tam sayı. | 1.4 bölümüne bakın. |

### Yapılı hatalar

- Dilin ürettiği her hata, yerleşik `Hata` sınıfının bir alt sınıfının
  nesnesidir: `TürHatası`, `DeğerHatası`, `TaşmaHatası`, `DizinHatası`,
  `AnahtarHatası`, `AdHatası`, `BölmeHatası`, `DosyaHatası`, `AğHatası`,
  `SözdizimiHatası` (modül yüklerken). Nesnenin `ileti`, `tür` (sınıf adı),
  `satır`, `dosya` alanları vardır; `metin(hata)` iletiyi verir, böylece
  `yaz("Hata:", hata)` 1.x'teki gibi çalışır.
- **Türe göre yakalama:** `yakala Tür olarak ad:` yalnızca `Tür`'ün ya da
  ondan türeyen bir sınıfın nesnelerini yakalar. Bir `dene` bloğunu birden
  çok `yakala` izleyebilir; ilk uyan çalışır, hiçbiri uymazsa hata dışarıya
  ilerler. `yakala ad:` ve `yakala:` 1.x'teki gibi her değeri yakalar ve
  yalnızca en sonda yazılabilir. `olarak` zaten ayrılmış bir kelimedir; yeni
  anahtar kelime eklenmez. Python köprüsünde karşılığı `except DizinHatası
  olarak h` ↔ `except IndexError as h`, `yakala h` ↔ `except Exception as h`
  olarak gösterilir.
- Yakalanan bir hata `fırlat ad` ile yeniden fırlatıldığında nesne aynıdır;
  `satır` ve `dosya` alanları özgün yeri göstermeyi sürdürür, hata izinde
  özgün yer gösterilir.
- `fırlat` her türden değeri fırlatmayı sürdürür. Programlar `sınıf
  BenimHatam(Hata):` ile kendi hata türlerini tanımlayabilir;
  `örneği_mi(hata, DizinHatası)` ile elle ayrım da yapılabilir.
- Değişen davranış: `yakala hata:` içinde `hata`'nın metin olduğu varsayımı.
  `"..." + hata` 2.0'da `TürHatası` verir. `hata == "..."` ise dilin genel
  kuralıyla (farklı türler hiçbir zaman eşit değildir) hata vermeden `yanlış`
  olur; bu sessiz değişiklik denetim komutunda ayrıca vurgulanır.

### Tür bildirimleri

- İsteğe bağlıdır: `değişken yaş: tam sayı = 30`,
  `fonksiyon topla(a: sayı, b: sayı) -> sayı:`, sınıf alanları için
  `bu.ad: metin = ad`. Tür adları: yerleşik tür adları, `sayı` (iki sayı
  türü), sınıf adları, `liste`, `sözlük`, `fonksiyon`; `boş` ile birleşim için
  `metin | boş`.
- Denetim çalışma zamanındadır: atama ve çağrı anında değer türe uymazsa
  `TürHatası`. Bildirim yazılmamış kodda hiçbir denetim kodu üretilmez.
- Durağan (çalıştırmadan) denetim bu sürümün kapsamında değildir; dil
  sunucusu bildirimleri tamamlama ve ipucu için kullanır.

### Ön sürüm ve yayım

2.0.0 doğrudan yayımlanmaz:

1. **2.0.0-rc.1**: üç konu bitip 2.0 ölçütlerinin 1-4 ve 6. maddeleri
   sağlandığında etiketlenir. Sürüm iş akışı `-` içeren etiketleri ön sürüm
   olarak işaretler; Scoop ve winget tanımları güncellenmez, deneme alanı
   ayrı bir adreste (`/rc/`) yayımlanır.
2. **Deneme dönemi: 4 hafta.** Öğretmen kitindeki tüm ders akışları ve
   alıştırma çözümleri, üç örnek program ve rehberdeki tüm örnekler ön sürümle
   çalıştırılır; geçiş rehberi bu denemeyle doğrulanır. Dışarıdan gelen geri
   bildirimler için depo sorunları (issues) kullanılır.
3. Geri bildirime göre gereken düzeltmeler `rc.2`, `rc.3` ... olarak çıkar.
   Davranış değiştiren her düzeltme geçiş rehberine işlenir.
4. Son ön sürümden sonra en az 1 hafta davranış değişikliği olmadığında
   **2.0.0** etiketlenir.

### 2.0 ölçütleri

2.0 aşağıdaki ölçütlerin tümü sağlandığında yayımlanır.

| # | Ölçüt | Durum |
|---|-------|-------|
| 1 | **Geriye uyum.** 1.x test paketi ve üç örnek program 2.0'da olduğu gibi geçer ya da geçiş rehberindeki adımlarla geçer; gereken her değişiklik rehberde örneklenir. | Bekliyor |
| 2 | **Hız.** Hız ölçümlerinin geometrik ortalaması 1.5'e göre %5'ten fazla gerilemez. Tür bildirimlerinin maliyeti sıfırdır: bildirimli bir programdan bildirimler silindiğinde üretilen bayt kodu, programın bildirimsiz yazılmış hâliyle bayt bayt aynıdır; bu, test paketindeki bir araçla denetlenir. | Bekliyor |
| 3 | **Geçiş testleri.** Her kırıcı değişikliğin (`/` sonucunun dizin olması, `tür` sonuçları, hata nesneleri ve `==` davranışı, JSON'da ondalık yazımı) 1.x ve 2.0 davranışını yan yana gösteren bir geçiş testi vardır. | Bekliyor |
| 4 | **Denetim.** `jus denetle --2.0`, 3. ölçütteki değişikliklerin tümünü yakalar; her kırıcı değişiklik için denetimin bulguyu raporladığını gösteren bir test vardır. Denetimin önerdiği her düzeltme uygulandığında program 1.5'te ve 2.0'da aynı çıktıyı verir; bu, aynı düzeltilmiş programların iki sürümle çalıştırılıp çıktılarının karşılaştırılmasıyla sınanır. | Bekliyor |
| 5 | **Belgeler.** Dil tanımı, rehber, Python köprüsü ve öğretmen kiti 2.0'a göre güncellenir; geçiş rehberi yayımlanır. | Bekliyor |
| 6 | **Araçlar.** Biçimlendirici, dil sunucusu, deneme alanı ve VS Code eklentisi yeni sözdizimini (`//`, tür bildirimleri, türe göre `yakala`) tanır. | Bekliyor |
| 7 | **Ön sürüm.** En az bir `rc` sürümü yayımlanmış, 4 haftalık deneme dönemi tamamlanmış ve son `rc`'den sonra en az 1 hafta davranış değişikliği olmamıştır. | Bekliyor |

## 1.0 ölçütleri

1.0 sürümü aşağıdaki ölçütlerin tümü sağlandığında yayımlandı.

| # | Ölçüt | Durum |
|---|-------|-------|
| 1 | **Yazılı dil tanımı.** Sözdizimi ve davranış belgede tanımlıdır; yorumlayıcı belgeye uyar. | Sağlandı: [dil tanımı](dil-tanimi.md). |
| 2 | **Doğruluk.** Her özelliğin testi vardır; testler Linux, macOS ve Windows'ta otomatik çalışır. Hatalı girdide çökme ve bellek sızıntısı yoktur. | Sağlandı: test paketi üç platformda ve bellek denetimiyle (ASan, UBSan, zorlanmış çöp toplayıcı) çalışıyor; rastgele girdiyle sınamada bulunan iki hata düzeltildi. |
| 3 | **Hız.** Standart ölçümlerde CPython ile aynı mertebededir. | Sağlandı: 11 ölçümün geometrik ortalaması CPython 3.11'in 0,92 katı ([ayrıntı](https://github.com/kerimaksanaoglu/jus/blob/main/bench/README.md)). Metin ve sözlük ağırlıklı işlerde CPython 1,7-3 kat hızlı. |
| 4 | **Yeterlilik.** JUS ile yazılmış üç gerçek program vardır. | Sağlandı: [yapılacaklar listesi](https://github.com/kerimaksanaoglu/jus/tree/main/examples/programlar/yapilacaklar) (komut satırı aracı), [not raporu](https://github.com/kerimaksanaoglu/jus/tree/main/examples/programlar/not_raporu) (JSON işleme), [not sunucusu](https://github.com/kerimaksanaoglu/jus/tree/main/examples/programlar/not_sunucusu) (HTTP sunucusu). |
| 5 | **Araçlar.** Etkileşimli kip, biçimlendirici, VS Code eklentisi, tarayıcıda deneme alanı. | Sağlandı. Ayrıca test çalıştırıcı ve paket yöneticisinin ilk sürümü. |
| 6 | **Belgeler.** Başlangıç rehberi, dil başvurusu, standart kütüphane başvurusu. | Sağlandı: [rehber](rehber/README.md); dil ve standart kütüphane başvurusu [dil tanımında](dil-tanimi.md). |

## Sürüm geçmişi

| Sürüm | İçerik |
|-------|--------|
| 0.1 | Çekirdek: değerler, değişkenler, işleçler, girintili bloklar, `eğer`, `iken`, fonksiyonlar ve kapanımlar, bayt kodu sanal makinesi, çöp toplayıcı, etkileşimli kip |
| 0.2 | Liste ve sözlük, dizinleme ve dilimleme, `her ... içinde`, Türkçeye duyarlı metin işlemleri |
| 0.3 | `dene` / `yakala` / `fırlat`, modüller, standart kütüphanenin ilk modülleri |
| 0.4 | Sınıflar ve kalıtım |
| 0.5 | `json`, `ağ`, `http`, `tr` modülleri; fonksiyon alan yerleşikler (`sırala`, `eşle`, `süz`); biçimlendirme fonksiyonları |
| 0.6 | Test çalıştırıcı, biçimlendirici, paket yöneticisi, deneme alanı, rastgele girdiyle sınama |
| 1.0 | Dil tanımının dondurulması |
| 1.1 | Belge sitesi, Python köprüsü rehberi, Scoop ve winget paket tanımları, logo |
| 1.2 | Hata iletilerinde öneriler, öğretmen kiti, deneme alanında paylaşım bağlantısı |

Ayrıntılar [değişiklik günlüğündedir](https://github.com/kerimaksanaoglu/jus/blob/main/CHANGELOG.md).

## Bilinen sınırlar

- `http` modülü şifreli (`https://`) adreslere bağlanamaz. (1.4'te kalkar.)
- Ağ işlemleri bekletir; bir sunucu aynı anda tek bir isteğe hizmet verir.
- Sayılar 64 bit kayan noktalıdır; ayrı bir tam sayı türü yoktur. Tam sayılar
  2^53'e kadar kesindir. (2.0'da kalkar.)
- Fonksiyonların varsayılan parametre değerleri ve değişken sayıda parametresi
  yoktur. (1.3'te kalkar.)
- Paket yöneticisi sürüm seçmez ve bağımlılık çözmez.
- Etkileşimli kipte satır düzenleme ve geçmiş yoktur. (1.3'te kalkar.)
- Windows konsolunda `oku()` ile Türkçe karakter girişi, konsolun ayarlarına
  bağlıdır; dosyadan ya da başka bir programdan yönlendirilen girdi sorunsuzdur.
- Harf dönüşümü ve alfabetik sıralama yalnızca Türk ve İngiliz alfabelerini
  kapsar.
- Uzun bir metni döngüde `+=` ile büyütmek, parçaları listede toplayıp
  `birleştir` ile birleştirmekten yavaştır.

## 2.0 sonrası için düşünülenler

Aşağıdakiler birer taahhüt değil, adaydır. Hangilerinin yapılacağını JUS'u
kullananların ihtiyaçları belirleyecektir.

- Çizim ve basit oyun modülü (kaplumbağa grafikleri, tuval)
- Paketlerde sürüm ve bağımlılık yönetimi
- Aynı anda birden çok bağlantıya hizmet verebilen ağ işlemleri
- macOS'ta TLS 1.3 (Network.framework ile)
- Durağan tür denetimi (tür bildirimlerinin çalıştırmadan doğrulanması)
- Hata ayıklayıcı ve diğer düzenleyiciler için destek

## Elle yapılacak adımlar

Aşağıdaki işler proje sahibinin hesaplarını ya da onayını gerektirir;
geliştirme bunları beklemeden sürer. Her sürüm için sıra: dalı `main`'e
birleştir, etiketi gönder, sürüm iş akışının ürettiği paketleri denetle.

| Sürüm | Adım | Açıklama |
|-------|------|----------|
| Her sürüm | `main`'e birleştirme | `surum-1.x` dalındaki çekme isteğini gözden geçirip birleştirmek. CI üç platformda yeşil olmalı. |
| Her sürüm | Etiket ve yayım | `git tag vX.Y.Z && git push origin vX.Y.Z`. Etiket, sürüm iş akışını başlatır: paketler, GitHub sürüm sayfası, Scoop ve winget tanımlarının güncellenmesi otomatiktir. |
| 1.1.0 | GitHub Pages | Depo ayarlarında Pages kaynağı "GitHub Actions" olarak seçilmeli; ardından `main`'e her birleştirme deneme alanını ve belge sitesini yayımlar. |
| 1.1.0 | Scoop | Ek işlem gerekmez; depo kendisi bir Scoop kovasıdır (`bucket/jus.json`). İsteğe bağlı: `scoop bucket add jus <depo adresi>` komutunun README'de denendiğini doğrulamak. |
| 1.1.0 | winget | `paketleme/winget/<sürüm>/` tanımları, proje sahibinin GitHub hesabından `microsoft/winget-pkgs` deposuna çekme isteği olarak gönderilmeli (ilk sürümde `JUSBil.JUS` tanımlayıcısıyla yeni paket). Her yeni sürümde yinelenir. |
| 1.1.0 | VS Code Marketplace | `jusbil` yayıncısı için Azure DevOps kişisel erişim anahtarı oluşturulmalı ve `vsce publish` ile eklenti yayımlanmalı; ya da anahtar depo gizli değişkeni (`VSCE_PAT`) olarak eklenip yayım sürüm iş akışına bağlanmalı. 1.5.0 ve 2.0.0'da eklenti yeniden yayımlanır. |
| 1.4.0 | Ağ testi | CI'da gerçek bir `https` adresine bağlanan test vardır; depo ayarlarında dış ağ erişimi kısıtlanmışsa `JUS_AG_TESTI=0` ile kapatılabilir. |
| 2.0.0-rc.1 | Ön sürüm etiketi ve duyuru | `v2.0.0-rc.1` etiketi gönderilir; ön sürüm notlarıyla birlikte deneme dönemi ve geçiş rehberi duyurulur. Deneme döneminde gelen sorunlar depo sorunlarında toplanır. |
| 2.0.0 | Yayım ve geçiş duyurusu | Deneme dönemi bittikten sonra `v2.0.0` etiketi gönderilir. 2.0 kırıcı değişiklikler içerir; sürüm notlarıyla birlikte geçiş rehberinin bağlantısı belge sitesinde ve README'de öne çıkarılmalı. |
