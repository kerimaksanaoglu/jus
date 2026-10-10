<p align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="marka/logo-acik.svg">
    <img src="marka/logo.svg" alt="JUS" width="240">
  </picture>
</p>

# JUS

JUS, Türkçe sözdizimli, genel amaçlı bir betik dilidir. Kaynak kod bayt koduna
derlenir ve bir sanal makinede çalışır. Yorumlayıcı C99 ile yazılmıştır ve dış
bağımlılığı yoktur.

Geçerli sürüm: **1.1.0**. Dil tanımı kararlıdır; 1.x sürümleri var olan
programların davranışını değiştirmez.

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

## Denemek ve kurmak

- **Tarayıcıda:** [deneme alanı](https://kerimaksanaoglu.github.io/jus/) kurulum
  gerektirmez.
- **Windows (Scoop):**
  `scoop bucket add jus https://github.com/kerimaksanaoglu/jus` ve ardından
  `scoop install jus`
- **Hazır paket:** [sürümler sayfasından](https://github.com/kerimaksanaoglu/jus/releases/latest)
  işletim sisteminize uygun paketi indirin, açın ve içindeki `jus` dosyasını
  çalıştırın. Ayrıntılı adımlar [kurulum sayfasındadır](https://kerimaksanaoglu.github.io/jus/belgeler/kurulum/).
- **Kaynaktan:** aşağıdaki gibi derleyin.

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
jus test [klasör]  # adı _test.jus ile biten dosyalardaki testleri çalıştırır
jus bicimle dosya.jus          # dosyayı standart biçime getirir
jus paket kur <git-adresi>     # paket kurar
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
  `dosya`, `sistem`, `json`, `ağ`, `http`, `tr`
- 30'dan fazla yerleşik fonksiyon
- Satır ve sütun gösteren Türkçe hata iletileri; yazım yanlışlarında "şunu mu
  demek istediniz?" önerileri
- Otomatik bellek yönetimi (çöp toplayıcı)

Henüz olmayanlar (şifreli ağ bağlantıları ve diğerleri) için
[yol haritasına](docs/yol-haritasi.md) bakın.

## Belgeler

Belgelerin tamamı aranabilir biçimde
[belge sitesindedir](https://kerimaksanaoglu.github.io/jus/belgeler/).

- [Başlangıç rehberi](docs/rehber/README.md): programlamaya sıfırdan başlayanlar
  için adım adım anlatım
- [Öğretmen kiti](docs/ogretmen/README.md): 10 haftalık ders planı, dönem projesi ve
  değerlendirme
- [JUS'tan Python'a](docs/python-koprusu.md): iki dilin yan yana karşılaştırması
- [Dil tanımı](docs/dil-tanimi.md): sözdizimi ve davranışın tam tanımı
- [VS Code eklentisi](editors/vscode/): sözdizimi renklendirme
- [Deneme alanı](playground/): programları tarayıcıda çalıştıran sayfa
- [Hız ölçümleri](bench/): CPython ile karşılaştırma
- [Yol haritası](docs/yol-haritasi.md): planlanan sürümler ve 1.0 ölçütleri
- [Değişiklik günlüğü](CHANGELOG.md)
- [Örnekler](examples/); daha büyük üç program:
  [yapılacaklar listesi](examples/programlar/yapilacaklar/),
  [not raporu](examples/programlar/not_raporu/) ve
  [not sunucusu](examples/programlar/not_sunucusu/)

## Depo düzeni

```
src/        yorumlayıcının kaynak kodu
lib/        JUS ile yazılmış standart kütüphane modülleri (yorumlayıcıya gömülür)
tests/      test paketi (beklentiler .jus dosyalarının içinde yazılıdır)
examples/   örnek programlar
docs/       belgeler
editors/    düzenleyici eklentileri
bench/      hız ölçüm paketi
tools/      geliştirme araçları
marka/      logo ve simge dosyaları
bucket/     Scoop paket tanımı
paketleme/  winget paket tanımları
```

## Lisans

MIT. Ayrıntılar için [LICENSE](LICENSE) dosyasına bakın.
