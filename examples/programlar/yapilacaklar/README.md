# Yapılacaklar listesi

Görevleri komut satırından ekleyip listeleyen, tamamlandı olarak işaretleyen,
silen ve arayan küçük bir araç. Veriler bir JSON dosyasında saklanır.

## Dosyalar

| Dosya               | Görev                                                              |
|---------------------|--------------------------------------------------------------------|
| `yapilacaklar.jus`  | Ana dosya: argümanları ayrıştırır, komutu seçer, hataları ele alır |
| `komutlar.jus`      | Alt komutlar (`ekle`, `listele`, `tamamla`, `sil`, `ara`, `yardım`) |
| `depolama.jus`      | `Depo` sınıfı: JSON dosyasını okur, yazar, görevleri yönetir       |
| `dene.sh`           | Uçtan uca sınama betiği                                            |

## Çalıştırma

```
jus yapilacaklar.jus [--dosya yol] <komut> [argümanlar]
```

| Komut               | Açıklama                                                  |
|---------------------|-----------------------------------------------------------|
| `ekle <başlık>`     | Yeni görev ekler; tırnaksız verilen sözcükler birleştirilir |
| `listele [süzgeç]`  | `tümü` (varsayılan), `bekleyen` ya da `tamamlanan`         |
| `tamamla <no>`      | Görevi tamamlandı işaretler                                |
| `sil <no>`          | Görevi siler (numara başka göreve verilmez)                |
| `ara <metin>`       | Başlığında metin geçen görevleri bulur                     |
| `yardım` / `yardim` | Kullanım iletisini yazar                                   |

Veri dosyası varsayılan olarak çalışma klasöründeki `yapilacaklar.json`
dosyasıdır; `--dosya yol` ya da `--dosya=yol` ile değiştirilir. Dosya yoksa ilk
yazmada oluşturulur.

Çıkış kodları: `0` başarılı, `1` işlem hatası (olmayan görev, bozuk veri
dosyası, yazılamayan dosya), `2` hatalı kullanım (bilinmeyen komut, eksik ya da
geçersiz argüman). Hata iletileri `Hata:` ile başlar.

### Bilinen sınırlama

Denenen Windows yorumlayıcı sürümünde komut satırı argümanlarındaki Türkçe
karakterler (`ç`, `ğ`, `ı`, `ö`, `ş`, `ü`) yorumlayıcıya bozuk ulaşıyor;
başlık ya da arama metni ASCII yazılmalıdır. Veri dosyasındaki Türkçe karakterler
etkilenmez. Aşağıdaki örnek oturum bu yüzden ASCII başlıklar kullanır.

## Veri biçimi

```json
{
  "sonraki_no": 4,
  "görevler": [
    {"no": 1, "başlık": "Sut al", "tamam": false, "tarih": "2026-10-06"}
  ]
}
```

## Örnek oturum

Aşağıdaki çıktılar gerçek çalıştırmadan alınmıştır (boş bir klasörde).

```
$ jus yapilacaklar.jus ekle Sut al
Eklendi: #1 Sut al
$ jus yapilacaklar.jus ekle Ekmek ve peynir
Eklendi: #2 Ekmek ve peynir
$ jus yapilacaklar.jus ekle Fatura ode
Eklendi: #3 Fatura ode
$ jus yapilacaklar.jus listele
  1  [ ]  Sut al  (2026-10-06)
  2  [ ]  Ekmek ve peynir  (2026-10-06)
  3  [ ]  Fatura ode  (2026-10-06)
Toplam 3 görev, 0 tamamlandı.
$ jus yapilacaklar.jus tamamla 2
Tamamlandı: #2 Ekmek ve peynir
$ jus yapilacaklar.jus listele bekleyen
  1  [ ]  Sut al  (2026-10-06)
  3  [ ]  Fatura ode  (2026-10-06)
Toplam 3 görev, 1 tamamlandı.
$ jus yapilacaklar.jus ara sut
  1  [ ]  Sut al  (2026-10-06)
$ jus yapilacaklar.jus sil 3
Silindi: #3 Fatura ode
$ jus yapilacaklar.jus tamamla 9
Hata: Görev bulunamadı: 9
(çıkış kodu: 1)
$ jus yapilacaklar.jus sil abc
Hata: Geçersiz görev numarası: 'abc'
(çıkış kodu: 2)
$ jus yapilacaklar.jus ekle
Hata: 'ekle' bir başlık ister. Örnek: ekle "Süt al"
(çıkış kodu: 2)
$ jus yapilacaklar.jus listele
  1  [ ]  Sut al  (2026-10-06)
  2  [x]  Ekmek ve peynir  (2026-10-06)
Toplam 2 görev, 1 tamamlandı.
```

## Sınama

```
bash dene.sh [jus-yolu]      # varsayılan: ./jus
```

Betik tüm komutları, hatalı kullanımları, bozuk veri dosyalarını ve Türkçe
karakterli veri dosyasını geçici bir klasörde dener; sonda `GEÇTİ` ya da
`KALDI` yazar ve buna göre `0` / `1` koduyla çıkar.
