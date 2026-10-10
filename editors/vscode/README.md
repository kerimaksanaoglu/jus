# JUS için VS Code eklentisi

[JUS](https://github.com/kerimaksanaoglu/jus) programlama dili (`.jus` dosyaları)
için sözdizimi renklendirme ve temel dil yapılandırması sağlar.

## Özellikler

- Yorumların, metinlerin, sayıların, anahtar kelimelerin ve yerleşik
  fonksiyonların renklendirilmesi
- Fonksiyon, sınıf ve üst sınıf adlarının ayırt edilmesi
- `#` ile satır yorumu (Ctrl+/), parantez eşleştirme, otomatik kapanan çiftler
- `:` ile biten satırdan sonra otomatik girinti

## Kurulum

VS Code'un Eklentiler panelinde "JUS" diye aratıp kurun.

Marketplace'e erişemiyorsanız JUS'un
[sürümler sayfasından](https://github.com/kerimaksanaoglu/jus/releases/latest)
`jus-vscode.vsix` dosyasını indirip şu komutla kurun:

```
code --install-extension jus-vscode.vsix
```

## JUS hakkında

JUS, Türkçe sözdizimli, genel amaçlı bir betik dilidir. Dili denemek için
[tarayıcıdaki deneme alanını](https://kerimaksanaoglu.github.io/jus/), öğrenmek
için [belge sitesini](https://kerimaksanaoglu.github.io/jus/belgeler/) kullanın.

## Geliştirme

Eklenti yalnızca bildirim dosyalarından oluşur; çalıştırılabilir kod ve
bağımlılık içermez. Paketlemek için:

```
npx @vscode/vsce package
```
