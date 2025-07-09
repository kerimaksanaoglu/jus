# 📝 JUS Sürüm Geçmişi

JUS Programming Language'ın gelişim sürecini ve yapılan değişiklikleri takip edebilirsiniz.

## [1.0.0] - 2025-01-09

### 🎉 İlk Kararlı Sürüm

Bu sürüm, JUS Programming Language'ın ilk kararlı sürümüdür. Türkiye'nin milli programlama dili olarak temel özelliklerini içerir.

### ✨ Yeni Özellikler

#### 🗣️ Türkçe Dil Desteği
- **Türkçe anahtar kelimeler**: `eğer`, `değilse`, `yaz`, `ve`, `veya`, `doğru`, `yanlış`
- **UTF-8 karakter desteği**: Türkçe karakterler tam destek
- **Türkçe hata mesajları**: Kullanıcı dostu hata bildirimleri

#### 🇹🇷 Türkiye Özel Özellikler
- **TC Kimlik doğrulama**: `tc_kimlik_gecerli()` fonksiyonu
- **Şehir kontrol sistemi**: `turkiye_sehir_mi()` ve `turkiye_baskent_mi()`
- **Türk Lirası formatı**: `tl_formatla()` fonksiyonu
- **Türkiye saati**: `turkiye_saati()` fonksiyonu (UTC+3)
- **Telefon doğrulama**: `telefon_gecerli()` fonksiyonu
- **Coğrafi bilgiler**: `turkiye_bilgi()` fonksiyonu

#### 💻 Teknik Altyapı
- **Lexical Analyzer**: UTF-8 destekli sözcüksel analiz
- **Recursive Descent Parser**: Güçlü sözdizimi analizi
- **Tree-walking Interpreter**: Verimli kod yürütme
- **Modüler mimari**: Genişletilebilir kod yapısı
- **Bellek yönetimi**: Güvenli malloc/free sistemi

#### 📚 Programlama Özellikleri
- **Değişken tanımlama**: Dinamik tip sistemi
- **Koşullu ifadeler**: `eğer`/`değilse` yapıları
- **Matematik operatörleri**: `+`, `-`, `*`, `/`, `%`
- **Karşılaştırma operatörleri**: `==`, `!=`, `<`, `>`, `<=`, `>=`
- **Mantıksal operatörler**: `ve`, `veya`, `!`
- **String concatenation**: String birleştirme desteği
- **Yazdırma sistemi**: `yaz` komutu

### 🏗️ Proje Yapısı

#### Kaynak Kod Organizasyonu
```
src/
├── main.c              # Ana program dosyası
├── lexer.c/.h          # Sözcüksel analiz modülü
├── parser.c/.h         # Sözdizimi analiz modülü
├── interpreter.c/.h    # Yorumlayıcı modülü
└── turkiye_features.c/.h # Türkiye özel özellikler
```

#### Geliştirme Araçları
- **Makefile**: Çapraz platform derleme desteği
- **Grammar BNF**: Formal dil grameri tanımı
- **Örnek programlar**: Kapsamlı kullanım örnekleri

### 📊 İstatistikler

- **Toplam kod satırı**: ~2,000+ satır
- **Türkçe anahtar kelime**: 15+
- **Türkiye özel fonksiyon**: 8
- **Desteklenen platform**: Windows, Linux, macOS
- **Test dosyası**: 6 örnek program

### 🧪 Test Edilen Özellikler

#### ✅ Tam Destek
- Türkçe anahtar kelimeler
- Matematik operatörleri
- Karşılaştırma operatörleri
- Mantıksal operatörleri
- String concatenation
- Koşullu ifadeler
- Değişken tanımlama ve atama

#### ✅ Türkiye Özel Özellikler
- TC kimlik doğrulama algoritması
- 81 il şehir kontrol sistemi
- Türk Lirası formatı (₺)
- Türkiye coğrafi bilgiler
- Türkiye saat dilimi desteği
- Telefon numarası doğrulama

### 🔧 Desteklenen Platformlar

- **Windows**: MinGW-w64 ile test edildi
- **Linux**: GCC ile test edildi
- **macOS**: Clang ile test edildi

### 📖 Dokümantasyon

- **README.md**: Kapsamlı kullanım kılavuzu
- **CONTRIBUTING.md**: Katkıda bulunma rehberi
- **grammar.bnf**: Formal dil grameri
- **Inline kod yorumları**: Türkçe açıklamalar

### 🎯 Bilinen Sınırlamalar

#### 🚧 Gelecek Sürümde Gelecek
- Döngü yapıları (`döngü`, `iken`, `her`)
- Fonksiyon tanımlama sistemi
- Dizi (array) desteği
- Dosya I/O işlemleri
- Nesne tabanlı programlama

#### 🐛 Bilinen Hatalar
- Yok (şu an için)

### 🔮 Gelecek Planları

#### v1.1.0 - "Ankara" Sürümü (Planlanan)
- [ ] Döngü yapıları
- [ ] Kullanıcı tanımlı fonksiyonlar
- [ ] Dizi desteği
- [ ] Gelişmiş hata mesajları

#### v1.2.0 - "İstanbul" Sürümü (Planlanan)
- [ ] Dosya I/O
- [ ] Modül sistemi
- [ ] Exception handling
- [ ] Standart kütüphane

#### v2.0.0 - "Anadolu" Sürümü (Planlanan)
- [ ] Nesne tabanlı programlama
- [ ] Generics desteği
- [ ] Concurrent programming
- [ ] JIT compiler

---

## 🔗 Sürüm Numaralandırma

JUS, [Semantic Versioning](https://semver.org/) standardını kullanır:

- **MAJOR**: Uyumsuz API değişiklikleri
- **MINOR**: Geriye uyumlu yeni özellikler
- **PATCH**: Geriye uyumlu hata düzeltmeleri

### Sürüm Adlandırma Konvansiyonu

Her sürüm Türkiye'nin önemli şehirlerinden adını alır:

- **v1.0.0**: "Başlangıç" - İlk kararlı sürüm
- **v1.1.0**: "Ankara" - Döngü yapıları
- **v1.2.0**: "İstanbul" - Dosya I/O
- **v2.0.0**: "Anadolu" - Nesne tabanlı programlama

---

## 📞 Sürüm Destek Politikası

### Desteklenen Sürümler

| Sürüm | Durum | Destek Başlangıcı | Destek Bitişi |
|-------|-------|------------------|---------------|
| 1.0.x | ✅ LTS | 2025-01-09 | 2026-01-09 |

### Destek Türleri

- **LTS (Long Term Support)**: 12 ay güvenlik güncellemeleri
- **Current**: 6 ay aktif geliştirme
- **Beta**: Geliştirme aşamasında

---

## 🏆 Katkıda Bulunanlar

### v1.0.0 Katkıda Bulunanlar

- **Kerim Aksanoğlu** [@kerimaksanaoglu] - Proje kurucusu ve ana geliştirici
- **JUSBil Ekibi** - Teknik destek ve test

### Teşekkürler

Bu sürümü mümkün kılan herkese teşekkürler:

- Türkiye yazılım geliştirici topluluğu
- Beta test yapan gönüllüler
- Geri bildirim sağlayan kullanıcılar

---

## 🇹🇷 Milli Vizyon

JUS, Türkiye'nin teknoloji bağımsızlığına katkıda bulunmak için geliştirilmiştir. Her sürüm, bu vizyonu gerçekleştirmek için atılmış bir adımdır.

> **"Ne mutlu JUS ile kod yazan!"**

---

*Bu changelog [Keep a Changelog](https://keepachangelog.com/en/1.0.0/) formatını takip eder.*

**Made with ❤️ for Turkey's technological independence**
