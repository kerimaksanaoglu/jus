# Değişiklik Günlüğü

Bu dosya [Keep a Changelog](https://keepachangelog.com/tr/1.1.0/) biçimini
izler. Sürüm numaraları [anlamsal sürümlemeye](https://semver.org/lang/tr/)
uyar.

## [Yayımlanmadı]

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
