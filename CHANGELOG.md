# Değişiklik Günlüğü

Bu dosya [Keep a Changelog](https://keepachangelog.com/tr/1.1.0/) biçimini
izler. Sürüm numaraları [anlamsal sürümlemeye](https://semver.org/lang/tr/)
uyar.

## [Yayımlanmadı]

### Eklendi

- `json` modülü: `çöz`, `yaz`
- `tr` modülü: `kimlik_no_geçerli_mi`, `iban_geçerli_mi`, `telefon_geçerli_mi`,
  `para`, `plaka_ili`, `iller`
- VS Code eklentisi: sözdizimi renklendirme (`editors/vscode`)

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
