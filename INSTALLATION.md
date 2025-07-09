# 🛠️ JUS Kurulum Rehberi

Bu rehber, JUS Programming Language'ın farklı işletim sistemlerinde kurulumu için detaylı adımları içerir.

## 📋 Sistem Gereksinimleri / System Requirements

### Minimum Gereksinimler
- **RAM**: 512 MB
- **Disk Alanı**: 50 MB
- **İşletim Sistemi**: Windows 10+, Ubuntu 18.04+, macOS 10.15+

### Önerilen Gereksinimler
- **RAM**: 2 GB
- **Disk Alanı**: 200 MB
- **İşletim Sistemi**: Windows 11, Ubuntu 22.04+, macOS 12.0+

### Gerekli Yazılımlar
- **C Derleyicisi**: GCC 7.0+ veya Clang 8.0+
- **Make**: Make 4.0+
- **Git**: Git 2.20+

---

## 🖥️ Windows Kurulumu

### Yöntem 1: MinGW-w64 ile Kurulum (Önerilen)

#### 1. MinGW-w64 Kurulumu
```cmd
# Scoop ile (önerilen)
scoop install mingw

# Chocolatey ile
choco install mingw

# Manuel kurulum
# https://www.mingw-w64.org/downloads/ adresinden indirin
```

#### 2. JUS Kaynak Kodunu İndirme
```cmd
# Repository klonlama
git clone https://github.com/kerimaksanaoglu/jus.git
cd jus

# Veya ZIP dosyası indirme
# https://github.com/kerimaksanaoglu/jus/archive/main.zip
```

#### 3. Derleme
```cmd
# Derleme
mingw32-make

# Veya
make

# Test
echo yaz "Merhaba JUS!" > test.jus
.\jus.exe test.jus
```

### Yöntem 2: Visual Studio ile Kurulum

#### 1. Visual Studio Community Kurulumu
- Visual Studio Community 2022 indirin
- C++ geliştirme araçlarını seçin
- Windows SDK'sını dahil edin

#### 2. Developer Command Prompt Açma
```cmd
# Start Menu → Visual Studio 2022 → Developer Command Prompt
```

#### 3. Derleme
```cmd
# JUS dizinine gidin
cd C:\path\to\jus

# Derleme
cl /I include src\*.c /Fe:jus.exe

# Test
echo yaz "Merhaba JUS!" > test.jus
jus.exe test.jus
```

### Yöntem 3: WSL ile Kurulum

#### 1. WSL Kurulumu
```powershell
# PowerShell (Admin olarak)
wsl --install -d Ubuntu-22.04
```

#### 2. Ubuntu'da JUS Kurulumu
```bash
# Ubuntu terminali açın
sudo apt update
sudo apt install gcc make git

# JUS kurulumu
git clone https://github.com/kerimaksanaoglu/jus.git
cd jus
make
```

---

## 🐧 Linux Kurulumu

### Ubuntu/Debian
```bash
# Sistem güncelleme
sudo apt update && sudo apt upgrade

# Gerekli paketler
sudo apt install gcc make git build-essential

# JUS kurulumu
git clone https://github.com/kerimaksanaoglu/jus.git
cd jus
make

# Test
echo 'yaz "Merhaba JUS!"' > test.jus
./jus test.jus
```

### CentOS/RHEL/Fedora
```bash
# CentOS/RHEL
sudo yum groupinstall "Development Tools"
sudo yum install gcc make git

# Fedora
sudo dnf groupinstall "Development Tools"
sudo dnf install gcc make git

# JUS kurulumu
git clone https://github.com/kerimaksanaoglu/jus.git
cd jus
make

# Test
echo 'yaz "Merhaba JUS!"' > test.jus
./jus test.jus
```

### Arch Linux
```bash
# Gerekli paketler
sudo pacman -S gcc make git base-devel

# JUS kurulumu
git clone https://github.com/kerimaksanaoglu/jus.git
cd jus
make

# Test
echo 'yaz "Merhaba JUS!"' > test.jus
./jus test.jus
```

---

## 🍎 macOS Kurulumu

### Yöntem 1: Homebrew ile Kurulum (Önerilen)

#### 1. Homebrew Kurulumu
```bash
# Homebrew kurulumu
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

#### 2. Gerekli Araçlar
```bash
# Development tools
brew install gcc make git

# JUS kurulumu
git clone https://github.com/kerimaksanaoglu/jus.git
cd jus
make

# Test
echo 'yaz "Merhaba JUS!"' > test.jus
./jus test.jus
```

### Yöntem 2: Xcode Command Line Tools

#### 1. Xcode Tools Kurulumu
```bash
# Command Line Tools kurulumu
xcode-select --install
```

#### 2. JUS Kurulumu
```bash
# JUS kurulumu
git clone https://github.com/kerimaksanaoglu/jus.git
cd jus
make

# Test
echo 'yaz "Merhaba JUS!"' > test.jus
./jus test.jus
```

---

## 🚀 Hızlı Kurulum Scripti

### Linux/macOS için Otomatik Kurulum
```bash
#!/bin/bash
# JUS Otomatik Kurulum Scripti

echo "🇹🇷 JUS Programming Language Kurulumu Başlıyor..."

# Sistem tespiti
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    echo "Linux sistemi tespit edildi"
    if command -v apt &> /dev/null; then
        sudo apt update
        sudo apt install -y gcc make git
    elif command -v yum &> /dev/null; then
        sudo yum groupinstall -y "Development Tools"
        sudo yum install -y gcc make git
    elif command -v dnf &> /dev/null; then
        sudo dnf groupinstall -y "Development Tools"  
        sudo dnf install -y gcc make git
    fi
elif [[ "$OSTYPE" == "darwin"* ]]; then
    echo "macOS sistemi tespit edildi"
    if ! command -v brew &> /dev/null; then
        echo "Homebrew kuruluyor..."
        /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    fi
    brew install gcc make git
fi

# JUS kurulumu
echo "JUS indiriliyor..."
git clone https://github.com/kerimaksanaoglu/jus.git
cd jus

echo "JUS derleniyor..."
make

# Test
echo 'yaz "Merhaba JUS!"' > test.jus
echo "JUS test ediliyor..."
./jus test.jus

echo "✅ JUS başarıyla kuruldu!"
echo "Kullanım: ./jus dosya.jus"
```

### Windows için PowerShell Scripti
```powershell
# JUS Windows Kurulum Scripti
Write-Host "🇹🇷 JUS Programming Language Kurulumu Başlıyor..." -ForegroundColor Green

# Chocolatey kontrolü
if (!(Get-Command choco -ErrorAction SilentlyContinue)) {
    Write-Host "Chocolatey kuruluyor..." -ForegroundColor Yellow
    Set-ExecutionPolicy Bypass -Scope Process -Force
    [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072
    iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
}

# MinGW kurulumu
Write-Host "MinGW kuruluyor..." -ForegroundColor Yellow
choco install mingw git -y

# JUS kurulumu
Write-Host "JUS indiriliyor..." -ForegroundColor Yellow
git clone https://github.com/kerimaksanaoglu/jus.git
cd jus

Write-Host "JUS derleniyor..." -ForegroundColor Yellow
mingw32-make

# Test
Write-Host "JUS test ediliyor..." -ForegroundColor Yellow
echo 'yaz "Merhaba JUS!"' > test.jus
.\jus.exe test.jus

Write-Host "✅ JUS başarıyla kuruldu!" -ForegroundColor Green
Write-Host "Kullanım: .\jus.exe dosya.jus" -ForegroundColor Cyan
```

---

## 📦 Paket Yöneticisi Kurulumu

### Gelecek Planları
JUS'un paket yöneticileri üzerinden kurulumu için planlar:

#### Linux
```bash
# APT (Ubuntu/Debian) - Gelecek
sudo apt install jus-lang

# YUM/DNF (CentOS/Fedora) - Gelecek  
sudo dnf install jus-lang

# Snap - Gelecek
sudo snap install jus-lang
```

#### macOS
```bash
# Homebrew - Gelecek
brew install jus-lang

# MacPorts - Gelecek
sudo port install jus-lang
```

#### Windows
```cmd
# Chocolatey - Gelecek
choco install jus-lang

# Scoop - Gelecek
scoop install jus-lang

# Winget - Gelecek
winget install jus-lang
```

---

## 🔧 Geliştirme Ortamı Kurulumu

### Visual Studio Code Extension
```json
{
    "name": "JUS Language Support",
    "version": "1.0.0",
    "description": "JUS Programming Language desteği",
    "engines": {
        "vscode": "^1.60.0"
    },
    "categories": ["Programming Languages"],
    "contributes": {
        "languages": [{
            "id": "jus",
            "aliases": ["JUS", "jus"],
            "extensions": [".jus"],
            "configuration": "./language-configuration.json"
        }]
    }
}
```

### Vim/Neovim Syntax Highlighting
```vim
" ~/.vim/syntax/jus.vim
syntax keyword jusKeyword eğer değilse yaz ve veya doğru yanlış
syntax keyword jusFunction turkiye_sehir_mi tc_kimlik_gecerli tl_formatla
syntax match jusComment "//.*$"
syntax region jusString start='"' end='"'
syntax match jusNumber '\d\+'

highlight link jusKeyword Keyword
highlight link jusFunction Function
highlight link jusComment Comment
highlight link jusString String
highlight link jusNumber Number
```

---

## 🧪 Test ve Doğrulama

### Kurulum Testi
```bash
# 1. Derleme testi
make clean && make

# 2. Temel test
echo 'yaz "JUS çalışıyor!"' > basic_test.jus
./jus basic_test.jus

# 3. Türkiye özellikler testi
echo 'yaz turkiye_sehir_mi("Istanbul")' > turkey_test.jus
./jus turkey_test.jus

# 4. Matematik testi
echo 'yaz 2 + 3 * 4' > math_test.jus
./jus math_test.jus

# 5. Koşul testi
cat > condition_test.jus << 'EOF'
a = 10
eğer a > 5:
    yaz "a büyük"
değilse:
    yaz "a küçük"
EOF
./jus condition_test.jus
```

### Performans Testi
```bash
# Büyük dosya testi
head -c 1M /dev/urandom | base64 | sed 's/^/yaz "/' | sed 's/$/"/' > large_test.jus
time ./jus large_test.jus

# Bellek testi (Linux)
valgrind --leak-check=full ./jus test.jus

# Bellek testi (macOS)
leaks --atExit -- ./jus test.jus
```

---

## 🚨 Sorun Giderme

### Yaygın Sorunlar ve Çözümleri

#### 1. "gcc: command not found"
```bash
# Ubuntu/Debian
sudo apt install gcc

# CentOS/RHEL
sudo yum install gcc

# macOS
xcode-select --install
```

#### 2. "make: command not found"
```bash
# Ubuntu/Debian
sudo apt install make

# CentOS/RHEL
sudo yum install make

# macOS
brew install make
```

#### 3. "Permission denied"
```bash
# Executable izni verme
chmod +x jus

# Sudo ile çalıştırma (gerekirse)
sudo ./jus dosya.jus
```

#### 4. "UTF-8 karakter sorunu"
```bash
# Locale ayarı
export LC_ALL=tr_TR.UTF-8
export LANG=tr_TR.UTF-8

# Terminalde UTF-8 desteği
echo $LANG
```

#### 5. Windows'ta "mingw32-make: command not found"
```cmd
# PATH'e MinGW ekleme
set PATH=%PATH%;C:\MinGW\bin

# Kalıcı olarak PATH'e ekleme
setx PATH "%PATH%;C:\MinGW\bin"
```

### Debug Modunda Çalıştırma
```bash
# Debug derlemesi
make clean && make debug

# GDB ile debug
gdb ./jus
(gdb) run test.jus
(gdb) bt
```

---

## 🔄 Güncelleme

### JUS Güncelleme
```bash
# Git ile güncelleme
cd jus
git pull origin main
make clean && make

# Yeni sürüm kontrolü
./jus --version
```

### Otomatik Güncelleme Scripti
```bash
#!/bin/bash
# JUS Güncelleme Scripti

echo "🔄 JUS güncelleniyor..."

cd jus
git fetch origin
LOCAL=$(git rev-parse HEAD)
REMOTE=$(git rev-parse @{u})

if [ $LOCAL != $REMOTE ]; then
    echo "Yeni sürüm bulundu, güncelleniyor..."
    git pull origin main
    make clean && make
    echo "✅ JUS güncellendi!"
else
    echo "JUS zaten güncel!"
fi
```

---

## 📞 Destek

### Kurulum Sorunları
- **GitHub Issues**: [github.com/kerimaksan/jus/issues](https://github.com/kerimaksanaoglu/jus/issues)
- **E-posta**: install-support@jusbil.com
- **Discord**: JUS Community Server

### Belgeler
- [README.md](README.md) - Genel bilgiler
- [CONTRIBUTING.md](CONTRIBUTING.md) - Katkı rehberi
- [examples/](examples/) - Kod örnekleri

---

## 🎯 Sonraki Adımlar

Kurulum tamamlandıktan sonra:

1. **Örnekleri İnceleyin**: `examples/` klasöründeki dosyaları çalıştırın
2. **Dokümantasyon Okuyun**: README.md dosyasını okuyun
3. **İlk Programınızı Yazın**: Basit bir "Merhaba Dünya" programı yazın
4. **Topluluğa Katılın**: GitHub Discussions'a katılın
5. **Katkıda Bulunun**: CONTRIBUTING.md rehberini okuyun

---

> **"Ne mutlu JUS kurana!"**

> **JUS ile Türkiye'nin teknoloji geleceğini birlikte inşa ediyoruz! 🇹🇷**

**Made with ❤️ for easy installation**
