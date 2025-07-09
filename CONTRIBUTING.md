# 🤝 JUS'a Katkıda Bulunma Rehberi

JUS Programming Language topluluğuna hoş geldiniz! Bu rehber, JUS'un geliştirilmesine nasıl katkıda bulunabileceğinizi açıklar.

## 🌟 Katkı Felsefemiz

> **"Birlikte güçlüyüz, birlikte Türkiye'nin teknoloji geleceğini inşa ediyoruz!"**

JUS, Türkiye'nin milli programlama dili olarak, topluluk katkılarıyla büyüyecek ve gelişecektir. Her katkı, Türkiye'nin teknoloji bağımsızlığına bir adım daha yaklaşmamıza yardımcı oluyor.

## 🎯 Katkı Türleri

### 1. 🐛 **Hata Bildirimi (Bug Reports)**
- Program çökmeleri
- Beklenmedik davranışlar
- Performans sorunları
- Türkçe karakter sorunları

### 2. ✨ **Yeni Özellik Önerileri**
- Yeni Türkçe anahtar kelimeler
- Türkiye özel fonksiyonlar
- Dil geliştirmeleri
- Kullanıcı deneyimi iyileştirmeleri

### 3. 📝 **Dokümantasyon**
- README geliştirmeleri
- Kod yorumları
- Kullanım örnekleri
- Türkçe çeviriler

### 4. 🧪 **Test Katkıları**
- Yeni test senaryoları
- Performans testleri
- Edge case testleri
- Türkiye özel özellik testleri

### 5. 💻 **Kod Katkıları**
- Yeni özellikler
- Hata düzeltmeleri
- Performans optimizasyonları
- Kod temizleme

## 🚀 Başlangıç Adımları

### 1. Geliştirme Ortamını Hazırlama

```bash
# Repository'yi fork edin ve klonlayın
git clone https://github.com/KULLANICI-ADINIZ/jus.git
cd jus

# Upstream repository'yi ekleyin
git remote add upstream https://github.com/kerimaksanaoglu/jus.git

# Projeyi derleyin
make

# Testleri çalıştırın
make test
```

### 2. Geliştirme Dalı Oluşturma

```bash
# Ana daldan güncel kodu alın
git checkout main
git pull upstream main

# Yeni özellik dalı oluşturun
git checkout -b feature/yeni-ozellik-adi

# Veya hata düzeltmesi dalı
git checkout -b fix/hata-aciklamasi
```

## 📋 Katkı Süreci

### 1. **Issue Oluşturma veya Seçme**
- Yeni bir özellik veya hata için issue oluşturun
- Mevcut issue'lardan birini seçin
- Issue'da ne yapacağınızı belirtin

### 2. **Kod Geliştirme**
- Kod standartlarımıza uyun
- Türkçe yorum satırları ekleyin
- Testler yazın
- Bellek yönetimini ihmal etmeyin

### 3. **Test Etme**
```bash
# Derleyin
make clean && make

# Testleri çalıştırın
make test

# Örnek dosyaları test edin
./jus examples/merhaba.jus
./jus examples/turkiye_test.jus
```

### 4. **Commit ve Push**
```bash
# Değişiklikleri stage edin
git add .

# Açıklayıcı commit mesajı yazın
git commit -m "feat: Yeni Türkçe anahtar kelime 'döngü' eklendi"

# Dalınızı push edin
git push origin feature/yeni-ozellik-adi
```

### 5. **Pull Request Oluşturma**
- GitHub'da Pull Request oluşturun
- Açıklayıcı başlık ve açıklama yazın
- Yapılan değişiklikleri detaylandırın
- Test sonuçlarını paylaşın

## 🎨 Kod Standartları

### C Kod Standartları
```c
// ✅ Doğru: Türkçe yorum satırları
// Türkiye şehir listesi kontrolü
int turkiye_sehir_mi(const char* sehir) {
    if (!sehir) return 0;
    
    // Şehir listesinde ara
    for (int i = 0; i < SEHIR_SAYISI; i++) {
        if (strcmp(sehir, TURKIYE_SEHIRLER[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// ❌ Yanlış: İngilizce yorum
// Check if city is in Turkey
```

### Değişken Adlandırma
```c
// ✅ Doğru: Açıklayıcı Türkçe isimler
char* turkiye_sehir_listesi[] = {...};
int sehir_sayisi = 81;
int tc_kimlik_gecerli(const char* tc_no);

// ❌ Yanlış: Kısaltmalar ve belirsiz isimler
char* list[] = {...};
int num = 81;
int check_tc(const char* tc);
```

### Hata Yönetimi
```c
// ✅ Doğru: Kapsamlı hata kontrolü
char* dosya_oku(const char* dosya_adi) {
    if (!dosya_adi) {
        fprintf(stderr, "Hata: Dosya adı boş olamaz\n");
        return NULL;
    }
    
    FILE* dosya = fopen(dosya_adi, "r");
    if (!dosya) {
        fprintf(stderr, "Hata: '%s' dosyası açılamadı\n", dosya_adi);
        return NULL;
    }
    
    // Dosya işlemleri...
    fclose(dosya);
    return icerik;
}
```

## 🧪 Test Yazma

### Yeni Özellik Testleri
```jus
// test_yeni_ozellik.jus
yaz "=== Yeni Özellik Testi ==="

// Test senaryosu 1
deger = yeni_fonksiyon("test")
eğer deger == beklenen_sonuc:
    yaz "✅ Test 1 başarılı"
değilse:
    yaz "❌ Test 1 başarısız"

// Test senaryosu 2
// ...
```

### Türkiye Özel Özellik Testleri
```jus
// test_turkiye_ozellikleri.jus
yaz "=== Türkiye Özel Özellikler Testi ==="

// TC Kimlik testleri
test_tc_gecerli = tc_kimlik_gecerli("10000000146")
eğer test_tc_gecerli == doğru:
    yaz "✅ TC Kimlik testi başarılı"
    
// Şehir testleri
test_sehir = turkiye_sehir_mi("Istanbul")
eğer test_sehir == doğru:
    yaz "✅ Şehir testi başarılı"
```

## 🎯 Öncelikli Katkı Alanları

### 1. **Türkçe Dil Geliştirmeleri**
- Yeni anahtar kelime önerileri
- Türkçe hata mesajları
- Dil grameri iyileştirmeleri

### 2. **Türkiye Özel Fonksiyonlar**
- Vergi hesaplama fonksiyonları
- Türk takvimi desteği
- Coğrafi bilgi fonksiyonları
- Kültürel özellikler

### 3. **Performans Optimizasyonları**
- Bellek kullanımı iyileştirmeleri
- Hız optimizasyonları
- Daha verimli algoritmalar

### 4. **Dokümantasyon**
- Kullanım örnekleri
- Tutorial'ler
- API dokümantasyonu
- Türkçe çeviriler

## 🏆 Katkıda Bulunanlar

JUS'a katkıda bulunan herkese teşekkürler! Sizler JUS'un gelişmesinin temel taşlarısınız.

### 🌟 Özel Teşekkürler
- **Kerim Aksanoğlu** - Proje kurucusu ve ana geliştirici
- **JUSBil Ekibi** - Teknik destek ve vizyon
- **Türkiye Yazılım Topluluğu** - Geri bildirim ve destek

## 📞 İletişim

### Katkı ile İlgili Sorularınız için:
- **GitHub Issues**: Teknik sorular ve öneriler
- **GitHub Discussions**: Genel tartışmalar
- **E-posta**: jus-contribution@jusbil.com

### Topluluk:
- **Discord**: JUS Programming Community
- **Telegram**: @JUSProgrammingLanguage
- **Twitter**: @JUSLang

## 🎖️ Katkı Seviyeleri

### 🥉 **Bronz Katkıda Bulunan**
- 1-5 başarılı PR
- Dokümantasyon katkıları
- Bug report'ları

### 🥈 **Gümüş Katkıda Bulunan**
- 6-15 başarılı PR
- Yeni özellik katkıları
- Test yazma

### 🥇 **Altın Katkıda Bulunan**
- 16+ başarılı PR
- Büyük özellik geliştirmeleri
- Mentorluk yapma

### 💎 **Elmas Katkıda Bulunan**
- Core contributor
- Proje yönetimi
- Vizyon belirleme

## 🔮 Gelecek Planları

### Yakın Gelecek (3-6 ay)
- [ ] Fonksiyon tanımlama sistemi
- [ ] Döngü yapıları
- [ ] Dosya I/O işlemleri
- [ ] Hata yönetimi geliştirme

### Uzun Vadeli (6-12 ay)
- [ ] Nesne tabanlı programlama
- [ ] Modül sistemi
- [ ] Standard library
- [ ] IDE entegrasyonu

## 📜 Davranış Kuralları

JUS topluluğunda:
- **Saygılı** davranın
- **Kapsayıcı** olun
- **Yapıcı** eleştiri yapın
- **Yardımlaşma** ruhunu benimseyin
- **Türkiye'nin teknoloji geleceği** için birlikte çalışın

---

## 🇹🇷 Sonuç

JUS, Türkiye'nin milli programlama dili olarak, sizlerin katkılarıyla büyüyecek ve gelişecektir. Her katkınız, Türkiye'nin teknoloji bağımsızlığına ve yerli yazılım ekosistemine değerli bir katkıdır.

**"Birlikte JUS ile geleceği kodluyoruz!"**

---

*Made with ❤️ for Turkey's technological independence*

**Ne mutlu JUS'a katkıda bulunana! 🇹🇷**
