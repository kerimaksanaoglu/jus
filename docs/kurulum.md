# Kurulum

JUS'u denemek için kurulum gerekmez:
[deneme alanı](https://kerimaksanaoglu.github.io/jus/) tarayıcıda çalışır.
Dosya okuyan, girdi alan ya da ağ kullanan programlar için JUS'u bilgisayarınıza
kurun.

## Windows

### Scoop ile

[Scoop](https://scoop.sh) kuruluysa:

```
scoop bucket add jus https://github.com/kerimaksanaoglu/jus
scoop install jus
```

Güncellemek için `scoop update jus` yeterlidir.

### Elle

1. [jus-windows-x64.zip](https://github.com/kerimaksanaoglu/jus/releases/latest/download/jus-windows-x64.zip)
   dosyasını indirin.
2. Zip dosyasını kalıcı bir klasöre açın, örneğin `C:\Araclar\jus`.
3. O klasörü `PATH` ortam değişkenine ekleyin: Başlat menüsünde "ortam
   değişkenleri" diye aratın, "Path" satırını düzenleyip klasörü ekleyin.
4. Yeni bir komut penceresi açıp deneyin:

```
jus --surum
```

## Linux

```sh
curl -LO https://github.com/kerimaksanaoglu/jus/releases/latest/download/jus-linux-x64.tar.gz
tar -xzf jus-linux-x64.tar.gz
sudo cp jus-*-linux-x64/jus /usr/local/bin/
jus --surum
```

Paket x86-64 işlemciler ve glibc kullanan dağıtımlar içindir. Başka sistemlerde
JUS'u kaynaktan derleyin.

## macOS

Apple Silicon işlemcili Mac'ler için:

```sh
curl -LO https://github.com/kerimaksanaoglu/jus/releases/latest/download/jus-macos-arm64.tar.gz
tar -xzf jus-macos-arm64.tar.gz
xattr -d com.apple.quarantine jus-*-macos-arm64/jus
sudo cp jus-*-macos-arm64/jus /usr/local/bin/
jus --surum
```

`xattr` komutu, macOS'un internetten indirilen dosyalara koyduğu engeli kaldırır;
dosyada böyle bir işaret yoksa komut hata verir, bu durumda sonraki adıma geçin.
Intel işlemcili Mac'lerde JUS'u kaynaktan derleyin.

## Kaynaktan derleme

Bir C derleyicisi (GCC ya da Clang) ve `make` yeterlidir:

```sh
git clone https://github.com/kerimaksanaoglu/jus
cd jus
make
make test
```

Derleme sonunda klasörde `jus` (Windows'ta `jus.exe`) dosyası oluşur.

## VS Code eklentisi

Eklenti `.jus` dosyalarında sözdizimi renklendirme sağlar.
[Sürümler sayfasından](https://github.com/kerimaksanaoglu/jus/releases/latest)
`jus-vscode.vsix` dosyasını indirip kurun:

```
code --install-extension jus-vscode.vsix
```

Ardından VS Code'u yeniden başlatın.

## İlk program

`merhaba.jus` adında bir dosya oluşturup içine şunu yazın:

```jus
yaz("Merhaba, dünya!")
```

Çalıştırın:

```
jus merhaba.jus
```

Devamı için [başlangıç rehberine](rehber/README.md) geçin.
