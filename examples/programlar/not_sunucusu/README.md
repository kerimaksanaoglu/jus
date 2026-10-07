# Not sunucusu

Notları bellekte tutan küçük bir HTTP sunucusu. `http` ve `json` modüllerinin
birlikte kullanımını ve `jus test` ile uçtan uca sınamayı gösterir.

## Çalıştırma

```sh
jus sunucu.jus 8080
```

Sunucu yalnızca bu bilgisayardan (127.0.0.1) erişilebilir ve istekleri sırayla
işler. Notlar kalıcı değildir; sunucu durunca silinir.

| İstek                 | Sonuç                                              |
|-----------------------|----------------------------------------------------|
| `GET /`               | Tanıtım sayfası (HTML)                             |
| `GET /notlar`         | Tüm notlar (JSON); `?ara=metin` ile süzülebilir    |
| `POST /notlar`        | Gövdedeki metni yeni not olarak ekler              |
| `GET /notlar/3`       | 3 numaralı not                                     |
| `DELETE /notlar/3`    | 3 numaralı notu siler                              |
| `POST /kapat`         | Sunucuyu durdurur                                  |

Başka bir terminalden denemek için:

```sh
curl -d "Süt al" http://127.0.0.1:8080/notlar
curl http://127.0.0.1:8080/notlar
curl -X POST http://127.0.0.1:8080/kapat
```

## Sınama

```sh
bash dene.sh ../../../jus
```

`dene.sh` sunucuyu arka planda başlatır, `istemci_test.jus` içindeki testleri
`jus test` ile çalıştırır ve sunucunun düzgün kapandığını denetler.

```
istemci_test.jus
  geçti  test_1_ana_sayfa
  geçti  test_2_not_ekleme
  geçti  test_3_listeleme_ve_arama
  geçti  test_4_tek_not_ve_silme
  geçti  test_5_hatalı_istekler
  geçti  test_9_kapatma

1 dosya, 6 test: 6 geçti, 0 kaldı.
Sonuç: GEÇTİ
```
