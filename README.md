# JUS

JUS, Türkçe sözdizimli, genel amaçlı bir betik dilidir. Kaynak kod bayt koduna
derlenir ve bir sanal makinede çalışır. Yorumlayıcı C99 ile yazılmıştır ve dış
bağımlılığı yoktur.

Geçerli sürüm: **0.1.0** (geliştirme aşamasında; dil 1.0'a kadar değişebilir).

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
jus program.jus    # dosyayı çalıştırır
jus                # etkileşimli kip
jus --surum
jus --yardim
```

Kaynak dosyalar UTF-8 olmalıdır.

Çıkış kodları: `0` başarılı, `65` sözdizimi hatası, `70` çalışma zamanı hatası,
`66` dosya okunamadı, `64` hatalı kullanım.

## 0.1.0 sürümünde neler var

- Değerler: sayı, metin, mantıksal (`doğru`/`yanlış`), `boş`
- Değişkenler ve blok kapsamı (`değişken`)
- Aritmetik, karşılaştırma ve mantıksal işleçler (`ve`, `veya`, `değil`)
- Girintiyle belirlenen bloklar
- `eğer` / `değilse eğer` / `değilse`
- `iken` döngüsü, `kır`, `devam`
- Fonksiyonlar, özyineleme, kapanımlar (`fonksiyon`, `dön`)
- Yerleşik fonksiyonlar: `yaz`, `oku`, `metin`, `sayı`, `uzunluk`, `tür`, `saat`,
  `karekök`, `mutlak`, `taban`, `tavan`, `yuvarla`
- Satır ve sütun gösteren Türkçe hata iletileri
- Otomatik bellek yönetimi (çöp toplayıcı)

Henüz olmayanlar (liste, sözlük, modüller, sınıflar ve diğerleri) için
[yol haritasına](docs/yol-haritasi.md) bakın.

## Belgeler

- [Dil tanımı](docs/dil-tanimi.md): sözdizimi ve davranışın tam tanımı
- [Yol haritası](docs/yol-haritasi.md): planlanan sürümler ve 1.0 ölçütleri
- [Değişiklik günlüğü](CHANGELOG.md)
- [Örnekler](examples/)

## Depo düzeni

```
src/        yorumlayıcının kaynak kodu
tests/      test paketi (beklentiler .jus dosyalarının içinde yazılıdır)
examples/   örnek programlar
docs/       belgeler
```

## Lisans

MIT. Ayrıntılar için [LICENSE](LICENSE) dosyasına bakın.
