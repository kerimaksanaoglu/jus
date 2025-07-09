# 🔒 JUS Güvenlik Politikası

JUS Programming Language'ın güvenliğini ciddiye alıyoruz. Bu dokümanda güvenlik sorunlarını nasıl bildireceğinizi bulabilirsiniz.

## 🛡️ Desteklenen Sürümler

Aşağıdaki JUS sürümleri güvenlik güncellemeleri almaktadır:

| Sürüm | Destek Durumu |
| ------ | ------------- |
| 1.0.x  | ✅ Tam Destek |
| 0.x    | ❌ Destek Yok |

## 🚨 Güvenlik Açığı Bildirimi

### Güvenlik Açığı Nedir?

Aşağıdaki durumlar güvenlik açığı olarak kabul edilir:

- **Bellek güvenliği**: Buffer overflow, memory leak, use-after-free
- **Code injection**: Zararlı kod enjeksiyonu
- **Denial of Service**: Hizmet engelleme saldırıları
- **Privilege escalation**: Yetki yükseltme
- **Information disclosure**: Bilgi sızıntısı

### Güvenlik Açığı Bildirme Süreci

#### 1. 🔐 Güvenli Bildirimi
**Güvenlik açıklarını PUBLIC olarak bildirmeyin!**

Güvenlik açıklarını güvenli bir şekilde bildirmek için:

- **E-posta**: security@jusbil.com
- **GitHub Security Advisory**: [Create advisory](https://github.com/kerimaksanaoglu/jus/security/advisories/new)
- **Şifreli iletişim**: PGP key aşağıda

#### 2. 📋 Bildirimi İçeriği

Lütfen bildiriminizde şunları ekleyin:

```
Güvenlik Açığı Raporu:

1. Açığın Türü: [Buffer overflow, injection, vb.]
2. Etkilenen Sürüm: [1.0.0, 1.0.1, vb.]
3. Etkilenen Dosya/Fonksiyon: [src/lexer.c, parser.c, vb.]
4. Açığın Açıklaması: [Detaylı açıklama]
5. Yeniden Üretme Adımları: [Adım adım nasıl tetiklenir]
6. Etki Değerlendirmesi: [Düşük/Orta/Yüksek/Kritik]
7. Çözüm Önerisi: [Varsa öneriniz]
8. İletişim Bilgileri: [E-posta, GitHub username]
```

#### 3. 🔑 PGP Anahtarı

Şifreli iletişim için PGP anahtarımız:

```
-----BEGIN PGP PUBLIC KEY BLOCK-----
[PGP anahtarı buraya gelecek]
-----END PGP PUBLIC KEY BLOCK-----
```

## ⚡ Yanıt Süreci

### 1. 📨 İlk Yanıt (24 saat içinde)
- Bildiriminizi aldığımızı onaylayacağız
- Rapor ID'si vereceğiz
- İlk değerlendirme yapacağız

### 2. 🔍 Araştırma (72 saat içinde)
- Güvenlik açığını doğrulayacağız
- Etki analizini yapacağız
- Çözüm planı oluşturacağız

### 3. 🛠️ Düzeltme (7 gün içinde)
- Güvenlik yamağını geliştireceğiz
- Test süreçlerini tamamlayacağız
- Güvenlik güncellemesini yayınlayacağız

### 4. 📢 Açıklama (14 gün içinde)
- Güvenlik açığını public olarak duyuracağız
- Çözüm detaylarını paylaşacağız
- Katkıda bulunanları kredilendireceğiz

## 🎯 Güvenlik Öncelikleri

### Kritik Seviye
- Remote code execution
- Authentication bypass
- Privilege escalation
- Data corruption

### Yüksek Seviye
- Local code execution
- Information disclosure
- Denial of service
- Memory corruption

### Orta Seviye
- Input validation issues
- Error handling problems
- Configuration issues

### Düşük Seviye
- Information leakage
- Minor vulnerabilities
- Best practice violations

## 🔒 Güvenlik Mimarimiz

### Kod Güvenliği
- **Static analysis**: Kodun statik analizi
- **Memory safety**: Bellek güvenliği kontrolü
- **Input validation**: Giriş doğrulama
- **Error handling**: Hata yönetimi

### Geliştirme Güvenliği
- **Secure coding**: Güvenli kodlama standartları
- **Code review**: Kod inceleme süreci
- **Automated testing**: Otomatik güvenlik testleri
- **Dependency scanning**: Bağımlılık taraması

## 🛡️ Güvenlik En İyi Uygulamaları

### JUS Kullanıcıları İçin
- **Güvenilir kaynaklardan indirin**: Resmi GitHub repository
- **Güncel sürümü kullanın**: Güvenlik güncellemelerini takip edin
- **Güvenli kod yazın**: Input validation yapın
- **Şüpheli kod çalıştırmayın**: Bilinmeyen kaynaklı .jus dosyaları

### Geliştiriciler İçin
- **Bellek yönetimi**: malloc/free'yi doğru kullanın
- **Buffer overflow**: Dizi sınırlarını kontrol edin
- **Input validation**: Kullanıcı girişlerini doğrulayın
- **Error handling**: Hataları güvenli şekilde yönetin

## 📚 Güvenlik Kaynakları

### Belgeler
- [OWASP C/C++ Security](https://owasp.org/www-project-c-c-security-guide/)
- [CERT C Coding Standard](https://wiki.sei.cmu.edu/confluence/display/c/SEI+CERT+C+Coding+Standard)
- [CWE Top 25](https://cwe.mitre.org/top25/archive/2023/2023_top25_list.html)

### Araçlar
- **Valgrind**: Bellek hatası tespiti
- **AddressSanitizer**: Bellek güvenliği
- **Static analyzers**: Statik kod analizi
- **Fuzzing tools**: Otomatik test araçları

## 🏆 Güvenlik Kreditlisi

JUS'un güvenliğini artırmaya yardımcı olan araştırmacılar:

### Hall of Fame
- *İlk güvenlik araştırmacısı burada yer alacak*

### Teşekkürler
Güvenlik açığı bildiren herkese teşekkür ederiz:
- Açığı bulan kişiler kredilendirilir
- Etik disclosure yapanlar desteklenir
- Güvenlik topluluğuna katkıları takdir edilir

## 🚀 Güvenlik Güncellemeleri

### Otomatik Güvencellemeler
- **Critical**: Anında güncelleme
- **High**: 24 saat içinde
- **Medium**: 1 hafta içinde
- **Low**: Sonraki minor sürümde

### Güncelleme Bildirimleri
- **GitHub Security Advisories**: Otomatik bildirim
- **Release Notes**: Güvenlik değişiklikleri
- **Security Mailing List**: security-announce@jusbil.com

## 📞 İletişim

### Güvenlik Ekibi
- **E-posta**: security@jusbil.com
- **GitHub**: @kerimaksanaoglu
- **Acil durum**: +90 538 972 47 67

### Yanıt Süreleri
- **Kritik**: 2 saat
- **Yüksek**: 24 saat
- **Orta**: 72 saat
- **Düşük**: 1 hafta

## 🎖️ Güvenlik Ödülleri

### Bug Bounty Program
*Gelecekte başlatılacak*

### Tanıma Kriterleri
- **Orijinal keşif**: İlk defa bildirilen açıklar
- **Responsible disclosure**: Etik bildirimi
- **Proof of concept**: Çalışan örnek
- **Çözüm önerisi**: Yardımcı öneriler

---

## 🇹🇷 Milli Güvenlik

JUS, Türkiye'nin milli programlama dili olarak güvenliği en üst seviyede tutmayı hedefler. Güvenlik açıklarını bildirerek Türkiye'nin teknoloji güvenliğine katkıda bulunursunuz.

> **"Güvenli kod, güvenli gelecek!"**

---

*Bu güvenlik politikası [ISO 27001](https://www.iso.org/isoiec-27001-information-security.html) ve [NIST Cybersecurity Framework](https://www.nist.gov/cyberframework) standartlarını takip eder.*

**Made with 🔒 for Turkey's cyber security**
