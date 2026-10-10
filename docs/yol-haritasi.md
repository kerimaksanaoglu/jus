# Yol Haritası

## Hedef

JUS: Türkçe sözdizimli, genel amaçlı, öğrenmesi kolay bir betik dili. Tek
dosyalık bir çalıştırıcıyla her platformda çalışır ve gerçek program yazmaya
yeter.

Öncelikli kullanıcılar sırasıyla: programlamayı öğrenenler ve öğretenler,
ardından küçük otomasyon betikleri yazanlar. JUS bir varış noktası değil, bir
köprüdür: kavramları ana dilinde öğrenen biri, Python gibi yaygın dillere
geçtiğinde yalnızca kelimeleri değiştirir.

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

## 1.x sürümleri

1.x sürümleri var olan programları bozmaz; yalnızca ekleme yapar.

| Sürüm | Tema | İçerik | Durum |
|-------|------|--------|-------|
| 1.0 | Kararlı dil | Dil tanımının dondurulması | Yayımlandı |
| 1.1 | Erişim | Belge sitesi, "JUS'tan Python'a" rehberi, Scoop ve winget paketleri, VS Code eklenti paketi, logo | Yayımlandı |
| 1.2 | Sınıf içi kullanım | Deneme alanında paylaşılabilir bağlantı, hata iletilerinde "şunu mu demek istediniz?" önerileri, öğretmen kiti | Planlandı |
| 1.3 | Dil rahatlığı | Varsayılan ve değişken sayıda parametre, biçimli metin (`f"..."`), etkileşimli kipte satır düzenleme, geçmiş ve tamamlama | Planlandı |
| 1.4 | Ağ ve veri | Şifreli bağlantılar (`https`), bit işleçleri, bayt dizileri (`baytlar`) | Planlandı |
| 1.5 | Düzenleyici desteği | Dil sunucusu (`jus lsp`): VS Code'da hata gösterimi, tamamlama, tanıma gitme, biçimlendirme | Planlandı |

## 2.0

2.0, 1.x'te yapılamayan çünkü var olan programların davranışını
değiştirebilecek üç değişikliği bir arada yapar. Her biri, 1.x programlarını
2.0'a taşımayı anlatan bir geçiş rehberiyle gelir.

| Konu | İçerik | Durum |
|------|--------|-------|
| Tam sayı türü | `tam sayı` ve `ondalık sayı` ayrı türlerdir. Noktasız yazılan sayılar 64 bit tam sayıdır ve bu aralıkta kesindir; `//` tam bölme işlecidir. `tür(3)` sonucu `"tam sayı"` olur. | Planlandı |
| Yapılı hatalar | Dilin ürettiği hatalar `Hata` sınıfının nesneleridir: `ileti`, `tür`, `satır`, `dosya` alanları vardır; `metin(hata)` iletiyi verir. Hata türleri (`TürHatası`, `DizinHatası`, `AnahtarHatası` ...) `Hata`'dan türer; programlar kendi hata sınıflarını tanımlayabilir. | Planlandı |
| Tür bildirimleri | İsteğe bağlı: `değişken yaş: tam sayı = 30`, `fonksiyon topla(a: sayı, b: sayı) -> sayı:`. Bildirimler çalışma zamanında denetlenir; uymayan değer `TürHatası` verir. | Planlandı |

2.0'da 1.x'teki hiçbir sözdizimi kaldırılmaz. Davranışı değişen yerler yalnızca
yukarıdakilerdir; 1.x programlarının büyük çoğunluğu değiştirilmeden çalışır.

## 1.0 ölçütleri

1.0 sürümü aşağıdaki ölçütlerin tümü sağlandığında yayımlandı.

| # | Ölçüt | Durum |
|---|-------|-------|
| 1 | **Yazılı dil tanımı.** Sözdizimi ve davranış belgede tanımlıdır; yorumlayıcı belgeye uyar. | Sağlandı: [dil tanımı](dil-tanimi.md). |
| 2 | **Doğruluk.** Her özelliğin testi vardır; testler Linux, macOS ve Windows'ta otomatik çalışır. Hatalı girdide çökme ve bellek sızıntısı yoktur. | Sağlandı: test paketi üç platformda ve bellek denetimiyle (ASan, UBSan, zorlanmış çöp toplayıcı) çalışıyor; rastgele girdiyle sınamada bulunan iki hata düzeltildi. |
| 3 | **Hız.** Standart ölçümlerde CPython ile aynı mertebededir. | Sağlandı: 11 ölçümün geometrik ortalaması CPython 3.11'in 0,92 katı ([ayrıntı](https://github.com/kerimaksanaoglu/jus/blob/main/bench/README.md)). Metin ve sözlük ağırlıklı işlerde CPython 1,7-3 kat hızlı. |
| 4 | **Yeterlilik.** JUS ile yazılmış üç gerçek program vardır. | Sağlandı: [yapılacaklar listesi](https://github.com/kerimaksanaoglu/jus/tree/main/examples/programlar/yapilacaklar) (komut satırı aracı), [not raporu](https://github.com/kerimaksanaoglu/jus/tree/main/examples/programlar/not_raporu) (JSON işleme), [not sunucusu](https://github.com/kerimaksanaoglu/jus/tree/main/examples/programlar/not_sunucusu) (HTTP sunucusu). |
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
| 1.1 | Belge sitesi, Python köprüsü rehberi, Scoop ve winget paketleri, logo |

Ayrıntılar [değişiklik günlüğündedir](https://github.com/kerimaksanaoglu/jus/blob/main/CHANGELOG.md).

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

## 2.0 sonrası için düşünülenler

Aşağıdakiler birer taahhüt değil, adaydır. Hangilerinin yapılacağını JUS'u
kullananların ihtiyaçları belirleyecektir.

- Çizim ve basit oyun modülü (kaplumbağa grafikleri, tuval)
- Paketlerde sürüm ve bağımlılık yönetimi
- Aynı anda birden çok bağlantıya hizmet verebilen ağ işlemleri
- Hata ayıklayıcı ve diğer düzenleyiciler için destek
