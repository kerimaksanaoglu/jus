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

## 1.0 ölçütleri

Aşağıdakilerin tümü sağlanmadan 1.0 sürümü yayımlanmaz.

| # | Ölçüt | Tanım |
|---|-------|-------|
| 1 | Yazılı dil tanımı | Sözdizimi ve davranış belgede tanımlıdır; yorumlayıcı belgeye uyar. |
| 2 | Doğruluk | Her özelliğin testi vardır; testler Linux, macOS ve Windows'ta otomatik çalışır. Hatalı girdide çökme ve bellek sızıntısı yoktur. |
| 3 | Hız | Standart ölçümlerde CPython ile aynı mertebededir. |
| 4 | Yeterlilik | JUS ile yazılmış üç gerçek program vardır: bir komut satırı aracı, JSON işleyen bir betik, küçük bir HTTP sunucusu. |
| 5 | Araçlar | Etkileşimli kip, biçimlendirici, VS Code eklentisi, tarayıcıda deneme alanı. |
| 6 | Belgeler | Başlangıç rehberi, dil başvurusu, standart kütüphane başvurusu. |

## Sürümler

### 0.1 Çekirdek (tamamlandı)

Sayı, metin, mantıksal ve boş değerler; değişkenler ve kapsam; işleçler;
girintili bloklar; `eğer`, `iken`, `kır`, `devam`; fonksiyonlar ve kapanımlar;
temel yerleşik fonksiyonlar; bayt kodu sanal makinesi ve çöp toplayıcı;
etkileşimli kip; test paketi.

### 0.2 Veri yapıları (tamamlandı)

Liste ve sözlük; dizinleme ve dilimleme; `her ... içinde` döngüsü ve
`aralık()`; `içinde` işleci; metin işlemleri; Türkçeye duyarlı büyük/küçük
harf dönüşümü ve sıralama; bileşik atama.

### 0.3 Hata yönetimi ve modüller (tamamlandı)

`dene` / `yakala` / `fırlat`; `kullan` ile modüller; standart kütüphanenin ilk
modülleri: matematik, rastgele, zaman, dosya, sistem.

### 0.4 Nesneler (tamamlandı)

Sınıflar, yöntemler, `kur`, `bu`, tekli kalıtım ve `üst`.

### 0.5 Standart kütüphane

- JSON, dosya sistemi, komut satırı argümanları, süreç çalıştırma
- Ağ: TCP ve HTTP istemcisi/sunucusu
- `tr` modülü: T.C. kimlik numarası, IBAN, telefon numarası doğrulama; para
  biçimlendirme

### 0.6 Araçlar

- Biçimlendirici (`jus bicimle`)
- Test çalıştırıcı (`jus test`)
- VS Code eklentisi: renklendirme, hata gösterimi, tamamlama
- Tarayıcıda deneme alanı (WebAssembly)

### 0.7 Hız ve sağlamlık

- Ölçüm paketi ve CPython ile karşılaştırma
- Sanal makine iyileştirmeleri
- Rastgele girdiyle sınama (fuzzing)

### 0.8 - 0.9 Kararlılık

- Paket yöneticisi
- Belgelerin tamamlanması
- 1.0 ölçütlerindeki üç programın yazılması
- Dil tanımının dondurulması

## Bilinen sınırlar

- Windows konsolunda `oku()` ile Türkçe karakter girişi, konsolun kod sayfası
  ayarına bağlıdır.
- Windows'ta ASCII dışı karakter içeren dosya yolları açılamayabilir.
- Etkileşimli kipte satır düzenleme ve geçmiş yoktur.
