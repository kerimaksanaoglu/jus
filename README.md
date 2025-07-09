# 🇹🇷 JUS - Türkiye'nin Yerli ve Milli Programlama Dili

<div align="center">

[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)](#-kurulum)
[![Language](https://img.shields.io/badge/language-C99-blue.svg)](#-teknik-mimari)
[![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)](#-kurulum)
[![Version](https://img.shields.io/badge/version-1.0.0-green.svg)](#-sürüm-geçmişi)

**"Ana diliyle kod yazan, geleceği kuran!"**

*Türkiye'nin teknoloji bağımsızlığına katkıda bulunmak için geliştirilen modern programlama dili*

[🚀 Hızlı Başlangıç](#-hızlı-başlangıç) • [📚 Dokümantasyon](#-dil-referansı) • [🇹🇷 Türkiye Özellikleri](#-türkiye-özel-özellikler) • [🤝 Katkıda Bulunun](#-katkıda-bulunma)

</div>

---

## 🌟 JUS Nedir?

**JUS**, Türkiye'nin milli programlama dili vizyonuyla ortaya çıkan, ana dilde kod yazma deneyimi sunan modern bir programlama dilidir. Türkçe anahtar kelimeler ve Türkiye'ye özel özelliklerle donatılmış olan JUS, hem eğitim hem de profesyonel geliştirme alanında kullanılmak üzere tasarlanmıştır.

### 🎯 Misyonumuz

> *"Türkiye'nin genç yeteneklerinin ana dillerinde kod yazarak, teknoloji üretiminde dünya lideri olmalarını sağlamak"*

### ✨ Neden JUS?

- **🗣️ Ana Dil Avantajı**: Türkçe düşünürken Türkçe kod yazın
- **🇹🇷 Kültürel Bağlantı**: Türkiye'ye özel özellikler ve fonksiyonlar
- **🎓 Eğitim Dostu**: Öğrenme sürecini kolaylaştıran sade syntax
- **🏗️ Modern Mimari**: Gelişmiş lexer, parser ve interpreter yapısı
- **🔧 Genişletilebilir**: Modüler kod yapısı ile kolay geliştirme

---

## 🚀 Hızlı Başlangıç

### Gereksinimler

- **GCC Derleyici** (C99 desteği)
- **Make** aracı
- **İşletim Sistemi**: Windows, Linux, macOS

### Kurulum

```bash
# Repository'yi klonlayın
git clone https://github.com/kerimaksanaoglu/jus.git
cd jus

# Projeyi derleyin
make

# İlk JUS programınızı çalıştırın
./jus examples/merhaba.jus
```

### İlk Programınız

```jus
// merhaba.jus
yaz "Merhaba, JUS Dünyası!"
yaz "Türkiye'nin milli programlama dili ile tanışın"

// Değişken tanımlama
isim = "Kerim"
yaş = 25
aktif = doğru

yaz "Merhaba " + isim + ", yaşın " + yaş

// Türkiye özel özellikler
eğer turkiye_sehir_mi("Istanbul"):
    yaz "İstanbul güzel bir şehir!"
```

---

## 📚 Dil Referansı

### 🔤 Temel Sözdizimi

#### Değişken Tanımlama
```jus
// Temel değişken türleri
isim = "Ahmet Yılmaz"
yaş = 30
maaş = 15000.50
evli_mi = doğru
```

#### Kontrol Yapıları
```jus
// Koşullu ifadeler
eğer yaş >= 18:
    yaz "Reşit"
    eğer yaş >= 65:
        yaz "Emekli"
    değilse:
        yaz "Çalışan"
değilse:
    yaz "Reşit değil"
```

#### Matematik İşlemleri
```jus
a = 15
b = 4

toplam = a + b      // 19
fark = a - b        // 11
çarpım = a * b      // 60
bölüm = a / b       // 3.75
kalan = a % b       // 3
```

#### Karşılaştırma Operatörleri
```jus
// Karşılaştırma işlemleri
eğer a > b:
    yaz "a büyük"
    
eğer a == b:
    yaz "eşit"
    
eğer a != b:
    yaz "eşit değil"
```

#### Mantıksal Operatörler
```jus
// Mantıksal bağlaçlar
eğer (yaş >= 18) ve (şehir == "Istanbul"):
    yaz "İstanbul'da yaşayan yetişkin"
    
eğer (not_average >= 80) veya (özel_yetenek == doğru):
    yaz "Başarılı öğrenci"
```

---

## 🇹🇷 Türkiye Özel Özellikler

JUS'un en güçlü yanlarından biri, Türkiye'ye özel geliştirilmiş built-in fonksiyonlarıdır:

### 🏛️ Türkiye Coğrafi Bilgiler
```jus
// Türkiye hakkında genel bilgi
turkiye_bilgi()
// Çıktı: Başkent: Ankara, Yüzölçümü: 783.356 km², Nüfus: ~84 milyon...
```

### 🏙️ Şehir Kontrolü
```jus
// Şehir doğrulama
yaz turkiye_sehir_mi("İstanbul")    // doğru
yaz turkiye_sehir_mi("Paris")       // yanlış
yaz turkiye_baskent_mi("Ankara")    // doğru
yaz turkiye_baskent_mi("İzmir")     // yanlış
```

### 🆔 TC Kimlik Doğrulama
```jus
// TC Kimlik No algoritması ile doğrulama
tc_no = "12345678901"
eğer tc_kimlik_gecerli(tc_no):
    yaz "Geçerli TC Kimlik"
değilse:
    yaz "Geçersiz TC Kimlik"
```

### 📞 Telefon Numarası Doğrulama
```jus
// Türkiye telefon formatı kontrolü
telefon = "05321234567"
eğer telefon_gecerli(telefon):
    yaz "Geçerli telefon numarası"
```

### 💰 Türk Lirası Formatı
```jus
// Para birimi formatı
para = 15000.75
yaz tl_formatla(para)    // "15000.75 ₺"
```

### 🕐 Türkiye Saati
```jus
// Türkiye saat dilimi (UTC+3)
yaz turkiye_saati()      // "09/01/2025 15:30:45"
```

---

## 🏗️ Teknik Mimari

### 📊 Proje Yapısı

```
jus/
├── src/                    # Kaynak kod
│   ├── main.c             # Ana program
│   ├── lexer.c/h          # Sözcüksel analiz
│   ├── parser.c/h         # Sözdizimi analizi
│   ├── interpreter.c/h    # Yorumlayıcı
│   └── turkiye_features.c/h # Türkiye özel özellikler
├── include/
│   └── common.h           # Ortak tanımlar
├── examples/              # Örnek programlar
├── grammar.bnf           # Dil grameri
├── Makefile              # Derleme dosyası
└── README.md             # Bu dosya
```

### 🔧 Derleme Sistemi

```bash
# Normal derleme
make

# Debug modu
make debug

# Test çalıştırma
make test

# Temizlik
make clean
```

### 🧪 Desteklenen Özellikler

- ✅ **Lexical Analysis**: UTF-8 Türkçe karakter desteği
- ✅ **Syntax Analysis**: Recursive descent parser
- ✅ **Interpretation**: Tree-walking interpreter
- ✅ **Türkçe Anahtar Kelimeler**: 15+ anahtar kelime
- ✅ **Türkiye Özel Fonksiyonlar**: 8 built-in fonksiyon
- ✅ **Hata Yönetimi**: Kapsamlı hata mesajları
- ✅ **Bellek Yönetimi**: Güvenli malloc/free sistemi

---

## 🎓 Örnekler

### Örnek 1: Öğrenci Değerlendirme Sistemi
```jus
// Öğrenci bilgileri
öğrenci_adı = "Ayşe Demir"
vize_notu = 85
final_notu = 92
tc_kimlik = "12345678901"

// Not hesaplama
genel_not = (vize_notu * 0.4) + (final_notu * 0.6)

yaz "Öğrenci: " + öğrenci_adı
yaz "Genel Not: " + genel_not

// TC kimlik kontrolü
eğer tc_kimlik_gecerli(tc_kimlik):
    yaz "Kimlik doğrulandı"
    
    eğer genel_not >= 90:
        yaz "Harf Notu: AA"
    değilse:
        eğer genel_not >= 80:
            yaz "Harf Notu: BA"
        değilse:
            yaz "Harf Notu: BB"
değilse:
    yaz "Geçersiz kimlik numarası"
```

### Örnek 2: Maaş Hesaplama
```jus
// Personel bilgileri
personel_adı = "Mehmet Özkan"
brüt_maaş = 20000
çalışma_şehri = "Ankara"

// Vergi hesaplama (basitleştirilmiş)
vergi_oranı = 0.20
vergi_tutarı = brüt_maaş * vergi_oranı
net_maaş = brüt_maaş - vergi_tutarı

yaz "Personel: " + personel_adı
yaz "Çalışma Şehri: " + çalışma_şehri
yaz "Brüt Maaş: " + tl_formatla(brüt_maaş)
yaz "Vergi: " + tl_formatla(vergi_tutarı)
yaz "Net Maaş: " + tl_formatla(net_maaş)

// Şehir kontrolü
eğer turkiye_sehir_mi(çalışma_şehri):
    yaz "✓ Türkiye'de çalışıyor"
değilse:
    yaz "✗ Yurt dışında çalışıyor"
```

---

## 🔮 Gelecek Özellikler

### 🚧 Geliştirme Aşamasında
- **Döngü Yapıları**: `döngü`, `iken`, `her` anahtar kelimeleri
- **Fonksiyon Sistemi**: Kullanıcı tanımlı fonksiyonlar
- **Dizi Desteği**: Array ve list yapıları
- **Dosya I/O**: Dosya okuma/yazma işlemleri

### 📋 Gelecek Sürümlerde
- **Nesne Tabanlı Programlama**: Class ve object desteği
- **Modül Sistemi**: Import/export yapısı
- **Standard Library**: Zengin kütüphane desteği
- **IDE Entegrasyonu**: Syntax highlighting ve IntelliSense

---

## 🤝 Katkıda Bulunma

JUS'un geliştirilmesine katkıda bulunmak istiyorsanız:

1. **Repository'yi fork edin**
2. **Yeni özellik branch'i oluşturun** (`git checkout -b feature/yeni-ozellik`)
3. **Değişikliklerinizi commit edin** (`git commit -m 'Yeni özellik eklendi'`)
4. **Branch'inizi push edin** (`git push origin feature/yeni-ozellik`)
5. **Pull Request oluşturun**

### 🎯 Katkı Alanları

- **Yeni Türkçe anahtar kelimeler** önerisi
- **Türkiye özel fonksiyonlar** geliştirme
- **Hata düzeltmeleri** ve optimizasyonlar
- **Dokümantasyon** iyileştirmeleri
- **Örnek program** yazma

---

## 📄 Lisans

Bu proje **MIT Lisansı** altında dağıtılmaktadır. Detaylar için [LICENSE](LICENSE) dosyasını inceleyebilirsiniz.

---

## 👨‍💻 Geliştirici

<div align="center">

**Kerim Aksanoğlu**  
*JUSBil Kurucusu*

*Türkiye'nin teknoloji bağımsızlığına katkıda bulunmak için JUS programlama dilini geliştiren yazılım mühendisi*

---

### 🏢 JUSBil Hakkında

**JUSBil**, Türkiye'nin milli teknoloji vizyonunu destekleyen, yerli ve milli yazılım çözümleri geliştiren bir teknoloji firmasıdır. JUS programlama dili, JUSBil'in Türkiye'nin teknolojik bağımsızlığına katkıda bulunma misyonunun ilk adımıdır.

</div>

---

## 🌟 Teşekkürler

Bu projeye destek veren ve katkıda bulunan herkese teşekkür ederiz. JUS, Türkiye'nin teknoloji geleceği için atılmış önemli bir adımdır.

---

## 📞 İletişim

- **GitHub Issues**: Sorularınız ve önerileriniz için
- **E-posta**: jus-programlama-dili@jusbil.com
- **Web**: www.jusbil.com

---

<div align="center">

### 🇹🇷 Türkiye'nin Gururu JUS!

**"Ne mutlu JUS ile kod yazan!"**

*Türkiye'nin teknoloji bağımsızlığına katkıda bulunmak için geliştirilen JUS programlama dili,  
ana dilde kod yazma deneyimini Türk yazılım geliştiricilerine sunuyor.*

---

**⭐ Projeyi beğendiyseniz yıldızlamayı unutmayın!**

*Made with ❤️ in Turkey*

</div>
