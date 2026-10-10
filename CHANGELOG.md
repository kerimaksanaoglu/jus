# Değişiklik Günlüğü

Bu dosya [Keep a Changelog](https://keepachangelog.com/tr/1.1.0/) biçimini
izler. Sürüm numaraları [anlamsal sürümlemeye](https://semver.org/lang/tr/)
uyar.

## [Yayımlanmadı]

### Eklendi

- Varsayılan parametre değerleri: `fonksiyon selamla(ad, selam = "Merhaba"):`.
  Varsayılan ifade argüman verilmediğinde her çağrıda yeniden hesaplanır
  (`liste = []` her çağrıda yeni liste verir) ve kendinden önceki parametreleri
  görebilir (`fonksiyon kayıt(ad, etiket = ad + "-1"):`). Yöntemlerde (`kur`
  dahil) ve iç fonksiyonlarda da çalışır. Varsayılanı olan parametreden sonra
  varsayılanı olmayan parametre derleme hatasıdır. Argüman sayısı iletisi
  aralığı söyler: `'f' fonksiyonu en az 1, en çok 2 argüman bekliyor, 3 verildi.`
- Değişken sayıda parametre: `fonksiyon topla(*sayılar):`. Adlı parametrelerden
  sonra kalan argümanlar yeni bir liste olarak gelir, hiç kalmazsa boş liste.
  `*` parametresi sonuncu olmalıdır ve `fonksiyon f(a, b = 2, *kalan)` gibi
  varsayılanlı parametrelerle birlikte kullanılabilir. Biçimlendirici `*kalan`
  yazımını korur. Çağrıda liste açma (`f(*liste)`) yoktur.
- Biçimli metin: `f"Merhaba, {ad}! Yaşın {yaş + 1}."`. `{ }` içindeki ifadeler
  hesaplanıp `metin()` ile aynı biçimde metne çevrilir; süslü parantez için
  `{{` ve `}}` yazılır. Biçim belirteci (`{x:.2f}`) yoktur; `{biçimle(x, 2)}`
  yazılır. Kapatılmamış `{`, tek `}` ve boş `{}` derleme hatasıdır.
- Etkileşimli kipte satır düzenleme: ok tuşları, `Home` / `End`, Türkçe harfleri
  tek tuşla silen `Backspace`; yukarı / aşağı okla geçmiş (ev klasöründeki
  `.jus_gecmis` dosyasında, en çok 1000 giriş); `Tab` ile anahtar kelime,
  yerleşik fonksiyon, tanımlı ad ve modül adı tamamlama. Girdi yönlendirilmişse
  düz okuma sürer.

## [1.2.0] - 2026-10-10

### Eklendi

- Hata iletilerinde "şunu mu demek istediniz?" önerileri: tanımsız ad,
  nesne üyesi, modül üyesi ve sözlük anahtarı için en yakın bilinen ad;
  Türkçe harf kullanılmadan yazılmış adlar (`sayac` → `sayaç`, `eger` → `eğer`)
  ve başka dillerden gelen adlar (`print` → `yaz`, `l.append(x)` → `ekle(l, x)`)
  tanınır; parantezsiz fonksiyon çağrısı için ipucu
- Öğretmen kiti: 10 haftalık ders planı, cevap anahtarı, değerlendirme ve dönem
  projesi (`docs/ogretmen`), belge sitesinde
- Deneme alanında "Paylaş" düğmesi: program bağlantının içine yazılır, sunucuya
  gönderilmez; son düzenlenen program tarayıcıda saklanır

## [1.1.0] - 2026-10-10

### Eklendi

- Belge sitesi: rehber, dil tanımı ve diğer belgeler aranabilir biçimde
  <https://kerimaksanaoglu.github.io/jus/belgeler/> adresinde
- "JUS'tan Python'a" rehberi: iki dilin yan yana karşılaştırması, farklı
  davranan yerler ve Python hata iletileri sözlüğü
- Kurulum sayfası; Windows'ta Scoop ile kurulum (`scoop install jus`)
- winget paket tanımları (`paketleme/winget`)
- Sürüm numarası içermeyen indirme adresleri
  (`.../releases/latest/download/jus-windows-x64.zip` gibi)
- VS Code eklentisinin kurulabilir paketi (`jus-vscode.vsix`) sürüm dosyaları
  arasında; eklenti simgesi
- JUS logosu ve marka dosyaları (`marka/`)

### Değişti

- Yol haritası 1.1 - 1.5 sürümlerinin ve 2.0'ın planını içeriyor

## [1.0.0] - 2026-10-07

İlk kararlı sürüm. Dil tanımı donduruldu: 1.x sürümleri yeni özellikler
ekleyebilir, ancak dil tanımına uygun yazılmış programların davranışını
değiştirmez.

### Eklendi

- `dosya.klasör_mü`
- Başlangıç rehberine JSON, ağ ve HTTP, test, biçimlendirme ve paket bölümleri
- 1.0 ölçütlerinin durumunu gösteren yol haritası
- Linux, macOS ve Windows için hazır paketler; deneme alanının yayımlanması

### Değişti

- Uzun metinleri döngüde `+=` ile büyütmek yaklaşık 5 kat hızlandı
- `yuvarla(x, basamak)` ve `biçimle`, tam ortadaki değerleri `yuvarla(x)` gibi
  sıfırdan uzağa yuvarlar
- `dosya.var_mı` klasörler için de `doğru` verir
- `json.yaz`, sayıları geri okunduğunda aynı değeri veren en kısa yazımla yazar

### Düzeltildi

- Windows'ta kullanımdaki bir port ikinci kez dinlenebiliyordu
- `http.sun` işleyicisi metin ya da sözlük dışında bir değer döndürdüğünde
  istemciye yanıt gitmiyordu

## [0.6.0]

### Eklendi

- Tarayıcıda deneme alanı: yorumlayıcının WebAssembly derlemesi (`playground/`)
- Biçimlendirici: `jus bicimle [--denetle] dosya...`
- Paket yöneticisi: `jus paket kur / listele / kaldır`; `kullan`, modülleri
  `jus_paketleri/` klasöründe de arar
- Üçüncü örnek program: HTTP not sunucusu (`examples/programlar/not_sunucusu`)

- Test çalıştırıcı: `jus test [yol]`; `doğrula` ve `eşit_olmalı` yerleşik fonksiyonları
- Rastgele girdiyle sınama aracı (`tools/fuzz`)
- `sırala(liste, anahtar)`: anahtar fonksiyonuyla kararlı sıralama
- `eşle`, `süz`, `biçimle`, `sola_doldur`, `sağa_doldur`, `tekrarla`
- `yuvarla(x, basamak)`
- `dosya.listele`, `dosya.klasör_oluştur`, `dosya.klasör_sil`
- `sistem.hata_yaz`, `sistem.betik`, `sistem.betik_klasörü`
- Başlangıç rehberi (`docs/rehber`) ve iki örnek program (`examples/programlar`)

### Değişti

- Windows'ta çıktıda satır sonu artık `\n` (önceden `\r\n`)
- `zaman.şimdi()` saniyenin kesirlerini de verir

### Düzeltildi

- Windows'ta Türkçe harf içeren komut satırı argümanları ve dosya yolları bozuluyordu
- Sınıf gövdesinde `fonksiyon` olmayan bir satır derleyiciyi sonsuz döngüye sokuyordu
- Çok sayıda sabit içeren dosyalarda derleme aşırı yavaştı
- Girinti uyuşmazlığı hatasının ardından gereksiz ikinci bir hata bildiriliyordu
- `1e5` gibi desteklenmeyen sayı yazımları yanıltıcı bir hata iletisi veriyordu

## [0.5.0]

### Eklendi

- `ağ` modülü: TCP istemcisi ve sunucusu
- `http` modülü: HTTP/1.1 istemcisi ve sunucusu (yalnızca şifresiz bağlantı)
- `geç` deyimi: boş bırakılacak bloklar için
- JUS ile yazılıp yorumlayıcıya gömülen standart kütüphane modülleri (`lib/`)
- Hız ölçüm paketi (`bench/`)
- `json` modülü: `çöz`, `yaz`
- `tr` modülü: `kimlik_no_geçerli_mi`, `iban_geçerli_mi`, `telefon_geçerli_mi`,
  `para`, `plaka_ili`, `iller`
- VS Code eklentisi: sözdizimi renklendirme (`editors/vscode`)

### Düzeltildi

- CRLF satır sonlu dosyalarda sözdizimi hatasının sütunu bir fazla gösteriliyordu
- 10^15 ile 2^53 arasındaki tam sayılar üslü biçimde yazılıyordu

## [0.4.0]

### Eklendi

- Sınıflar: `sınıf Ad:`, yöntemler, `kur` ile kurulum, `bu`
- Kalıtım: `sınıf Alt(Üst):`, `üst.yöntem(...)`
- Nesne alanları: `nesne.alan`, `nesne.alan = değer`, bileşik atama
- `örneği_mi(değer, sınıf)` yerleşik fonksiyonu
- `tür(nesne)` sınıfın adını verir

## [0.3.0]

### Eklendi

- Hata yakalama: `dene` / `yakala`; `fırlat` ile hata oluşturma. Dilin kendi
  hataları da yakalanabilir
- Modül sistemi: `kullan ad`, `kullan "yol/ad"`, `olarak` ile takma ad,
  `modül.üye` erişimi
- Standart kütüphane modülleri: `matematik`, `rastgele`, `zaman`, `dosya`,
  `sistem`
- Komut satırı argümanları: `jus program.jus a b c` (`sistem.argümanlar`)
- Başka dosyadaki çağrılar için hata izinde dosya adı

### Değişti

- Her dosyanın genel değişkenleri artık kendisine aittir

## [0.2.0]

### Eklendi

- Liste: `[1, 2, 3]`, dizinleme, negatif dizin, dilimleme, `+` ile birleştirme
- Sözlük: `{"ad": "Ayşe"}`; metin, sayı ve mantıksal anahtarlar; eklenme
  sırasını korur
- `her öğe içinde kap:` döngüsü (liste, metin, sözlük)
- `içinde` üyelik işleci
- Bileşik atama: `+=`, `-=`, `*=`, `/=`
- Metinlerde dizinleme ve dilimleme (karakter cinsinden)
- Metinlerin `<`, `>` ile Türk alfabesine göre karşılaştırılması
- Liste ve sözlüklerin içerikleriyle eşitlik karşılaştırması
- Yerleşik fonksiyonlar: `aralık`, `ekle`, `araya_ekle`, `çıkar`, `sil`,
  `sırala`, `ters`, `anahtarlar`, `değerler`, `al`, `bul`, `böl`, `birleştir`,
  `kırp`, `değiştir`, `büyük_harf`, `küçük_harf`, `başlar_mı`, `biter_mi`
- Bellek denetimi betiği (`tests/bellek-denetimi.sh`)

### Değişti

- `uzunluk` artık liste ve sözlükleri de kabul eder
- `( )` yanında `[ ]` ve `{ }` içinde de satır sonları yok sayılır

## [0.1.0]

Yorumlayıcı baştan yazıldı. Önceki sürümle kaynak düzeyinde uyumlu değildir.

### Eklendi

- Bayt kodu derleyicisi ve sanal makine
- Çöp toplayıcı ile otomatik bellek yönetimi
- Girintiyle belirlenen bloklar
- Değerler: sayı, metin, mantıksal, boş, fonksiyon
- `değişken` ile tanımlama, blok kapsamı
- İşleçler: `+ - * / %`, `== != < <= > >=`, `ve`, `veya`, `değil`
- `eğer` / `değilse eğer` / `değilse`
- `iken` döngüsü, `kır`, `devam`
- `fonksiyon`, `dön`, özyineleme, kapanımlar
- Yerleşik fonksiyonlar: `yaz`, `oku`, `metin`, `sayı`, `uzunluk`, `tür`,
  `saat`, `karekök`, `mutlak`, `taban`, `tavan`, `yuvarla`
- Metinlerde kaçış dizileri: `\n \t \r \" \\`
- Satır ve sütun gösteren sözdizimi hata iletileri
- Çağrı zincirini gösteren çalışma zamanı hata iletileri
- Etkileşimli kip
- Test paketi ve sürekli tümleştirme
- Dil tanımı ve yol haritası belgeleri

### Değişti

- `yaz` artık bir deyim değil, fonksiyondur: `yaz("merhaba")`
- Yorumlar `//` yerine `#` ile başlar
- Değişkenler kullanılmadan önce `değişken` ile tanımlanmalıdır
- `dönüş` yerine `dön`, `boşluk` yerine `boş`, `!` yerine `değil` kullanılır
- Türler arasında örtük dönüşüm yoktur; sayı ile metin `+` ile birleştirilemez
- Koşullar mantıksal değer olmalıdır
- Sürüm numarası 0.1.0 olarak yeniden başlatıldı

### Kaldırıldı

- Kullanılmayan ayrılmış kelimeler (anahtar kelime sayısı 14'e indi)
- `tekrar ... kez` döngüsü
- Dilin içine gömülü `turkiye_*`, `tc_kimlik_gecerli`, `tl_formatla`
  fonksiyonları (ileride ayrı bir modül olarak sunulacak)
