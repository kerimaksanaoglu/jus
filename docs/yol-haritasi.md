# Yol Haritası

## Hedef

JUS: Türkçe sözdizimli, genel amaçlı, öğrenmesi kolay bir betik dili. Tek
dosyalık bir çalıştırıcıyla her platformda çalışır ve gerçek program yazmaya
yeter.

Öncelikli kullanıcılar sırasıyla: programlamayı öğrenenler ve öğretenler,
ardından küçük otomasyon betikleri yazanlar.

## İlkeler

1. **Çalışmayan özellik belgelenmez.** Belgelerde yazan her şeyin testi vardır.
2. **Az ve kesin anahtar kelime.** Yalnızca gerçekten kullanılan kelimeler
   ayrılır.
3. **Çekirdek tarafsızdır.** Ülkeye ya da alana özgü işlevler dilin içinde
   değil, kütüphanelerde yer alır.
4. **Hata iletileri ürünün parçasıdır.** Her ileti Türkçedir, konumu gösterir
   ve mümkünse çözümü söyler.
5. **Sürümleme kurallıdır.** Sürüm numaraları anlamsal sürümlemeye uyar; her
   sürümün değişiklikleri kaydedilir. 1.0'dan sonra dil davranışını bozan
   değişiklik yalnızca ana sürümde yapılır.

## 1.0 ölçütleri ve durumları

| # | Ölçüt | Durum |
|---|-------|-------|
| 1 | **Yazılı dil tanımı.** Sözdizimi ve davranış belgede tanımlıdır; yorumlayıcı belgeye uyar. | Sağlandı: [dil tanımı](dil-tanimi.md). |
| 2 | **Doğruluk.** Her özelliğin testi vardır; testler Linux, macOS ve Windows'ta otomatik çalışır. Hatalı girdide çökme ve bellek sızıntısı yoktur. | Sağlandı: test paketi üç platformda ve bellek denetimiyle (ASan, UBSan, zorlanmış çöp toplayıcı) çalışıyor; rastgele girdiyle sınamada bulunan iki hata düzeltildi. |
| 3 | **Hız.** Standart ölçümlerde CPython ile aynı mertebededir. | Sağlandı: 11 ölçümün geometrik ortalaması CPython 3.11'in 0,92 katı ([ayrıntı](../bench/README.md)). Metin ve sözlük ağırlıklı işlerde CPython 1,7-3 kat hızlı. |
| 4 | **Yeterlilik.** JUS ile yazılmış üç gerçek program vardır. | Sağlandı: [yapılacaklar listesi](../examples/programlar/yapilacaklar/) (komut satırı aracı), [not raporu](../examples/programlar/not_raporu/) (JSON işleme), [not sunucusu](../examples/programlar/not_sunucusu/) (HTTP sunucusu). |
| 5 | **Araçlar.** Etkileşimli kip, biçimlendirici, VS Code eklentisi, tarayıcıda deneme alanı. | Sağlandı. Ayrıca test çalıştırıcı ve paket yöneticisinin ilk sürümü. |
| 6 | **Belgeler.** Başlangıç rehberi, dil başvurusu, standart kütüphane başvurusu. | Sağlandı: [rehber](rehber/README.md); dil ve standart kütüphane başvurusu [dil tanımında](dil-tanimi.md). |

## Sürüm geçmişi

| Sürüm | İçerik |
|-------|--------|
| 0.1 | Çekirdek: değerler, değişkenler, işleçler, girintili bloklar, `eğer`, `iken`, fonksiyonlar ve kapanımlar, bayt kodu sanal makinesi, çöp toplayıcı, etkileşimli kip |
| 0.2 | Liste ve sözlük, dizinleme ve dilimleme, `her ... içinde`, Türkçeye duyarlı metin işlemleri |
| 0.3 | `dene` / `yakala` / `fırlat`, modüller, standart kütüphanenin ilk modülleri |
| 0.4 | Sınıflar ve kalıtım |
| 0.5 | `json`, `ağ`, `http`, `tr` modülleri; fonksiyon alan yerleşikler (`sırala`, `eşle`, `süz`); biçimlendirme fonksiyonları |
| 0.6 | Test çalıştırıcı, biçimlendirici, paket yöneticisi, deneme alanı, rastgele girdiyle sınama |
| 1.0 | Dil tanımının dondurulması |

Ayrıntılar [değişiklik günlüğündedir](../CHANGELOG.md).

## Bilinen sınırlar

- `http` modülü şifreli (`https://`) adreslere bağlanamaz.
- Ağ işlemleri bekletir; bir sunucu aynı anda tek bir isteğe hizmet verir.
- Sayılar 64 bit kayan noktalıdır; ayrı bir tam sayı türü yoktur. Tam sayılar
  2^53'e kadar kesindir.
- Fonksiyonların varsayılan parametre değerleri ve değişken sayıda parametresi
  yoktur.
- Paket yöneticisi sürüm seçmez ve bağımlılık çözmez.
- Etkileşimli kipte satır düzenleme ve geçmiş yoktur.
- Windows konsolunda `oku()` ile Türkçe karakter girişi, konsolun ayarlarına
  bağlıdır; dosyadan ya da başka bir programdan yönlendirilen girdi sorunsuzdur.
- Harf dönüşümü ve alfabetik sıralama yalnızca Türk ve İngiliz alfabelerini
  kapsar.
- Uzun bir metni döngüde `+=` ile büyütmek, parçaları listede toplayıp
  `birleştir` ile birleştirmekten yavaştır.

## 1.0 sonrası için düşünülenler

Aşağıdakiler birer taahhüt değil, adaydır. Hepsi var olan programları bozmadan
eklenebilecek niteliktedir.

- Şifreli bağlantılar (`https`)
- Varsayılan ve adlandırılmış parametreler
- Metin içine değer yerleştirme (var olan metinleri etkilemeyen yeni bir yazımla)
- Etkileşimli kipte satır düzenleme ve geçmiş
- VS Code eklentisinde hata gösterimi ve tamamlama
- Paketlerde sürüm ve bağımlılık yönetimi
- Daha hızlı metin ve sözlük işlemleri
- Aynı anda birden çok bağlantıya hizmet verebilen ağ işlemleri
