# Rastgele girdiyle sınama

`fuzz.py`, yorumlayıcıyı rastgele üretilmiş ve bozulmuş girdilerle çalıştırır.
Amaç, hiçbir girdinin yorumlayıcıyı çökertmediğini ya da takmadığını
doğrulamaktır.

Geçerli çıkış kodları yalnızca 0, 65, 66 ve 70'tir. Başka bir çıkış kodu ya da
zaman aşımı bulgu sayılır ve girdi `bulgular/ham/` altına kaydedilir. Program
gerçekten sonsuz döngü içeriyorsa (`iken doğru:` gibi) zaman aşımı bir hata
değildir; bulgular bu yüzden elle incelenmelidir.

## Çalıştırma

```sh
python tools/fuzz/fuzz.py --jus ./jus --adet 2000 --tohum 1
```

Aynı tohum aynı girdileri üretir. `--strateji` ile tek bir strateji seçilebilir:

- `bozma`: `tests/` altındaki geçerli dosyaları bayt ve satır düzeyinde bozar.
- `belirtec`: dilin belirteçlerinden rastgele diziler üretir.
- `yapisal`: dilbilgisine uygun, uç boyutlarda programlar üretir (derin iç içe
  yapılar, çok uzun satırlar, çok sayıda sabit ve değişken).
- `sinir`: bilinen sınır durumlarını dener (boş dosya, BOM, geçersiz UTF-8,
  sınırları aşan sayıda yerel değişken ve sabit).

## Bulunan ve düzeltilen hatalar

| Girdi | Belirti | Neden |
|-------|---------|-------|
| Sınıf gövdesinde `fonksiyon` olmayan bir satır | Derleyici sonsuz döngüye giriyordu | Hata bildirildikten sonra belirteç tüketilmiyordu |
| On binlerce farklı sabit içeren dosya | Derleme dakikalarca sürüyordu | Her sabit, tablodaki tüm sabitlerle karşılaştırılıyordu |

Bu iki durum `tests/hata/` altında kalıcı teste dönüştürüldü. Düzeltmelerden
sonra yedi farklı tohumla 17.500 girdi denendi; çökme ya da takılma bulunmadı.
Zaman aşımına uğrayan altı girdinin tümü gerçekten sonsuz döngü içeren ya da
hiç gelmeyecek ağ verisini bekleyen programlardı.
