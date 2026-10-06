# JUS

JUS, Türkçe sözdizimli, genel amaçlı bir betik dilidir. Kaynak kod bayt koduna
derlenir ve bir sanal makinede çalışır. Yorumlayıcı C99 ile yazılmıştır ve dış
bağımlılığı yoktur.

Geçerli sürüm: **0.4.0** (geliştirme aşamasında; dil 1.0'a kadar değişebilir).

```jus
fonksiyon faktöriyel(n):
    eğer n <= 1:
        dön 1
    dön n * faktöriyel(n - 1)

değişken i = 1
iken i <= 5:
    yaz(metin(i) + "! =", faktöriyel(i))
    i = i + 1
```

## Derleme

Bir C derleyicisi (GCC ya da Clang) ve `make` gerekir.

```sh
make          # jus (Windows'ta jus.exe) dosyasını üretir
make test     # test paketini çalıştırır
```

Windows'ta MSYS2/MinGW ile `mingw32-make` kullanılabilir.

## Kullanım

```sh
jus program.jus    # dosyayı çalıştırır (sonraki argümanlar programa verilir)
jus                # etkileşimli kip
jus --surum
jus --yardim
```

Kaynak dosyalar UTF-8 olmalıdır.

Çıkış kodları: `0` başarılı, `65` sözdizimi hatası, `70` çalışma zamanı hatası,
`66` dosya okunamadı, `64` hatalı kullanım.

## Dilde neler var

- Değerler: sayı, metin, mantıksal (`doğru`/`yanlış`), `boş`, liste, sözlük
- Değişkenler ve blok kapsamı (`değişken`)
- Aritmetik, karşılaştırma ve mantıksal işleçler (`ve`, `veya`, `değil`,
  `içinde`), bileşik atama (`+=` ...)
- Girintiyle belirlenen bloklar
- `eğer` / `değilse eğer` / `değilse`
- `iken` ve `her ... içinde` döngüleri, `kır`, `devam`
- Fonksiyonlar, özyineleme, kapanımlar (`fonksiyon`, `dön`)
- Dizinleme ve dilimleme: `liste[0]`, `metin[1:4]`
- Türkçeye duyarlı metin işlemleri: `büyük_harf("ılık")` sonucu `"ILIK"`,
  alfabetik sıralamada `ç`, `ğ`, `ı`, `ö`, `ş`, `ü` doğru yerlerinde
- Sınıflar, yöntemler ve kalıtım (`sınıf`, `bu`, `üst`)
- Hata yakalama: `dene` / `yakala` / `fırlat`
- Modüller (`kullan`) ve standart kütüphane: `matematik`, `rastgele`, `zaman`,
  `dosya`, `sistem`, `json`, `tr`
- 30'dan fazla yerleşik fonksiyon
- Satır ve sütun gösteren Türkçe hata iletileri
- Otomatik bellek yönetimi (çöp toplayıcı)

Henüz olmayanlar (ağ, biçimlendirici, paket yöneticisi ve diğerleri) için
[yol haritasına](docs/yol-haritasi.md) bakın.

## Belgeler

- [Dil tanımı](docs/dil-tanimi.md): sözdizimi ve davranışın tam tanımı
- [VS Code eklentisi](editors/vscode/): sözdizimi renklendirme
- [Yol haritası](docs/yol-haritasi.md): planlanan sürümler ve 1.0 ölçütleri
- [Değişiklik günlüğü](CHANGELOG.md)
- [Örnekler](examples/)

## Depo düzeni

```
src/        yorumlayıcının kaynak kodu
tests/      test paketi (beklentiler .jus dosyalarının içinde yazılıdır)
examples/   örnek programlar
docs/       belgeler
editors/    düzenleyici eklentileri
```

## Lisans

MIT. Ayrıntılar için [LICENSE](LICENSE) dosyasına bakın.
