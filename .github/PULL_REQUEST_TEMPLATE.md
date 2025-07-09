# 🚀 Pull Request: JUS'a Katkı

## 📋 Değişiklik Özeti / Summary of Changes

Bu PR'da yapılan değişikliklerin kısa açıklaması:

## 🎯 Değişiklik Türü / Type of Change

- [ ] 🐛 Bug fix (geriye uyumlu hata düzeltmesi)
- [ ] ✨ New feature (geriye uyumlu yeni özellik)
- [ ] 💥 Breaking change (geriye uyumlu olmayan değişiklik)
- [ ] 📚 Documentation update (dokümantasyon güncelleme)
- [ ] 🧪 Tests (test ekleme/güncelleme)
- [ ] 🔧 Code refactoring (fonksiyonalite değişikliği olmayan kod iyileştirme)
- [ ] 🎨 Style changes (code formatting, eksik noktalı virgül, vb.)
- [ ] 🇹🇷 Turkish language features (Türkçe dil özellikler)
- [ ] 🏗️ Build system changes (build scripts, dependencies, vb.)

## 🔗 İlgili Issue / Related Issue

- Fixes #(issue number)
- Closes #(issue number)
- Related to #(issue number)

## 📝 Detaylı Açıklama / Detailed Description

### Yapılan Değişiklikler
1. **Değişiklik 1**: Açıklama
2. **Değişiklik 2**: Açıklama
3. **Değişiklik 3**: Açıklama

### Neden Bu Değişiklik Gerekli?
- Problem açıklaması
- Çözüm yaklaşımı
- Fayda analizi

## 💻 Kod Değişiklikleri / Code Changes

### Değiştirilen Dosyalar
- `src/lexer.c` - Lexer iyileştirmeleri
- `src/parser.c` - Parser bug fix
- `src/interpreter.c` - Yeni özellik implementasyonu
- `src/turkiye_features.c` - Türkiye özel özellikler

### Yeni Eklenen Dosyalar
- `examples/yeni_ornek.jus` - Yeni özellik örneği
- `tests/test_yeni_ozellik.c` - Yeni özellik testleri

### Silinen Dosyalar
- `old_file.c` - Artık kullanılmayan dosya

## 🧪 Test Durumu / Testing Status

### Mevcut Testler
- [ ] ✅ Tüm mevcut testler geçiyor
- [ ] ⚠️ Bazı testler başarısız (açıklama gerekli)
- [ ] ❌ Testler henüz çalıştırılmadı

### Yeni Testler
- [ ] ✅ Yeni özellik için testler eklendi
- [ ] ⚠️ Testler kısmi olarak eklendi
- [ ] ❌ Testler henüz eklenmedi

### Test Komutları
```bash
# Derleme testi
make clean && make

# Temel testler
make test

# Özel testler
./jus examples/yeni_ornek.jus
```

## 🔧 Teknik Detaylar / Technical Details

### Mimari Değişiklikleri
- AST node türü eklendi: `AST_YENI_OZELLIK`
- Yeni token türü: `TOK_YENI_ANAHTAR`
- Interpreter'da yeni visitor fonksiyonu

### Performans Etkisi
- [ ] 🚀 Performans artışı
- [ ] 📊 Performans etkisi yok
- [ ] 🐌 Performans kaybı (gerekçe: ...)

### Bellek Kullanımı
- [ ] 📉 Bellek kullanımı azaldı
- [ ] 📊 Bellek kullanımı aynı
- [ ] 📈 Bellek kullanımı arttı (gerekçe: ...)

## 🇹🇷 Türkçe Dil Katkıları / Turkish Language Contributions

### Yeni Türkçe Anahtar Kelimeler
- `yeni_kelime` - Açıklama
- `başka_kelime` - Açıklama

### Türkiye Özel Özellikler
- `yeni_turkiye_fonksiyonu()` - Açıklama
- Güncellenen mevcut fonksiyonlar

### Hata Mesajları
- [ ] Yeni hata mesajları Türkçe
- [ ] Mevcut hata mesajları güncellendi
- [ ] İngilizce hata mesajları Türkçe'ye çevrildi

## 📚 Dokümantasyon / Documentation

### Güncellenen Dokümantasyon
- [ ] README.md
- [ ] CONTRIBUTING.md
- [ ] Kod yorumları
- [ ] Örnek dosyalar
- [ ] Grammar.bnf

### Yeni Dokümantasyon
- [ ] Yeni özellik dokümantasyonu
- [ ] Kullanım örnekleri
- [ ] Tutorial güncelleme

## 🔍 Code Review Checklist

### Kod Kalitesi
- [ ] Kod okunabilir ve anlaşılır
- [ ] Değişken isimleri açıklayıcı
- [ ] Fonksiyonlar tek sorumluluğa sahip
- [ ] Kod duplikasyonu yok
- [ ] Magic number'lar sabitleştirildi

### Türkçe Standartları
- [ ] Türkçe yorum satırları eklendi
- [ ] Türkçe değişken/fonksiyon isimleri kullanıldı
- [ ] Türkçe hata mesajları yazıldı
- [ ] UTF-8 karakter desteği kontrol edildi

### Güvenlik
- [ ] Buffer overflow kontrolleri
- [ ] NULL pointer kontrolleri
- [ ] Bellek sızıntısı kontrolleri
- [ ] Input validation

### Performans
- [ ] Gereksiz döngüler yok
- [ ] Bellek kullanımı optimize
- [ ] Algoritma karmaşıklığı makul
- [ ] Profiling yapıldı (gerekiyorsa)

## 🧪 Test Senaryoları / Test Scenarios

### Temel Testler
```bash
# Test 1: Derleme
make clean && make

# Test 2: Temel çalışma
echo 'yaz "Test"' > test.jus
./jus test.jus

# Test 3: Türkiye özellikleri
./jus examples/turkiye_test.jus
```

### Özel Testler
```jus
// Test senaryosu 1
yeni_ozellik_test()

// Test senaryosu 2
hata_durumu_test()
```

### Edge Cases
- [ ] Empty input
- [ ] Large files
- [ ] Unicode characters
- [ ] Memory limits

## 🔄 Geriye Uyumluluk / Backward Compatibility

- [ ] ✅ Geriye uyumlu değişiklik
- [ ] ⚠️ Deprecation uyarısı gerekli
- [ ] 💥 Breaking change (MAJOR version bump)

### Breaking Changes Açıklaması
Eğer breaking change varsa detaylı açıklama:

## 🚀 Deployment Notları / Deployment Notes

### Derleme Gereksinimleri
- [ ] Yeni bağımlılık yok
- [ ] Compiler version gereksinimi aynı
- [ ] Yeni build step gerekmiyor

### Kurulum Değişiklikleri
- [ ] Kurulum süreci değişmedi
- [ ] Yeni dosya/dizin oluşturulacak
- [ ] Konfigürasyon güncelleme gerekli

## 📸 Ekran Görüntüleri / Screenshots

Eğer UI değişikliği varsa:

### Öncesi
![Öncesi](link-to-image)

### Sonrası
![Sonrası](link-to-image)

## 📋 Kontrol Listesi / Checklist

### Geliştirme
- [ ] Kod tamamlandı
- [ ] Testler yazıldı
- [ ] Testler geçiyor
- [ ] Dokümantasyon güncellendi
- [ ] Örnekler eklendi/güncellendi

### İnceleme
- [ ] Self-review yapıldı
- [ ] Code style kontrol edildi
- [ ] Memory leak kontrol edildi
- [ ] Performance impact değerlendirildi
- [ ] Security kontrol edildi

### Türkçe Dil
- [ ] Türkçe yorum satırları eklendi
- [ ] Türkçe hata mesajları yazıldı
- [ ] Türkçe dokümantasyon güncellendi
- [ ] Türkiye özel özellikler kontrol edildi

### GitHub
- [ ] Branch name açıklayıcı
- [ ] Commit message'ları anlamlı
- [ ] PR description detaylı
- [ ] Reviewer'lar atandı
- [ ] Labels eklendi

## 🎯 Gelecek Planları / Future Plans

Bu PR'dan sonra planladığınız işler:

- [ ] Takip edecek özellikler
- [ ] Performans optimizasyonları
- [ ] Dokümantasyon genişletme
- [ ] Test coverage artırma

## 🤝 Katkıda Bulunanlar / Contributors

### Ana Geliştirici
- @kerimaksanaoglu (Proje sahibi)

### Bu PR Katkıda Bulunanlar
- @contributor1 (Özellik geliştirme)
- @contributor2 (Test yazma)
- @contributor3 (Dokümantasyon)

### Teşekkürler
- Geri bildirim verenler
- Test yapan gönüllüler
- Dokümantasyon yardımcıları

---

## 🇹🇷 Milli Değerler / National Values

Bu PR, JUS'un Türkiye'nin milli programlama dili olma yolunda nasıl katkı sağlıyor:

- **Türkçe Dil Desteği**: 
- **Kültürel Değerler**: 
- **Eğitim Katkısı**: 
- **Teknoloji Bağımsızlığı**: 

---

> **"Ne mutlu JUS'a katkıda bulunana!"**

> **JUS ile Türkiye'nin teknoloji geleceğini birlikte inşa ediyoruz! 🇹🇷**

**Made with ❤️ for Turkey's technological independence**
