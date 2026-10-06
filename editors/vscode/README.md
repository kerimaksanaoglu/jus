# JUS için VS Code eklentisi

JUS programlama dili (`.jus` dosyaları) için sözdizimi renklendirme ve temel
dil yapılandırması sağlar. Eklenti yalnızca bildirim dosyalarından oluşur;
çalıştırılabilir kod ve bağımlılık içermez.

## Kapsam

- Yorumlar (`#`), metinler ve kaçış dizileri (geçersiz kaçışlar ayrıca işaretlenir), sayılar
- Anahtar kelimeler: denetim akışı, bildirimler, mantıksal işleçler, sabitler,
  `bu` / `üst` ve `kullan` / `olarak`
- Fonksiyon, sınıf ve üst sınıf adları; fonksiyon çağrıları ve yerleşik fonksiyonlar
- İşleçler
- Satır yorumu, parantez çiftleri, otomatik kapanan çiftler ve `:` ile biten
  satırdan sonra girinti artışı

Dilin tanımı için `docs/dil-tanimi.md` dosyasına bakınız.

## Yerelde kurulum

Yöntem 1: Bu klasörü VS Code eklenti dizinine kopyalayın.

- Windows: `%USERPROFILE%\.vscode\extensions\jus`
- Linux / macOS: `~/.vscode/extensions/jus`

Ardından VS Code'u yeniden başlatın.

Yöntem 2: `vsce` ile paketleyip kurun.

```
npm install -g @vscode/vsce
cd editors/vscode
vsce package
code --install-extension jus-0.4.0.vsix
```

`vsce package` bir depo adresi ve lisans dosyası bulunmadığına dair uyarı
verebilir; yerel kurulum için bu uyarılar atlanabilir (`--allow-missing-repository`).
