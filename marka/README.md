# JUS marka dosyaları

| Dosya | Kullanım |
|-------|----------|
| `logo.svg`, `logo.png` | Açık zemin üzerinde logo |
| `logo-acik.svg`, `logo-acik.png` | Koyu zemin üzerinde logo |
| `simge.svg`, `simge.png` | Kare simge: uygulama simgesi, eklenti simgesi, site simgesi |

Vektör (`.svg`) dosyalar her boyutta nettir; mümkün olan her yerde onlar
kullanılmalıdır.

## Biçim

Logo hazır bir yazı tipine dayanmaz. J, U ve S harfleri tek kalınlıkta, yuvarlak
uçlu çizgilerden kurulmuştur; harflerin ardından, yazılmayı bekleyen bir satırı
çağrıştıran köşeli bir imleç gelir.

Biçimi tanımlayan sayılar `tools/logo_uret.py` dosyasındadır. Buradaki dosyalar
o betikle üretilir; elle düzenlenmemelidir:

```sh
python tools/logo_uret.py
```

## Renkler

| Ad | Değer | Kullanım |
|----|-------|----------|
| Mürekkep | `#16202e` | Açık zeminde yazı |
| Kâğıt | `#ffffff` | Koyu zeminde yazı |
| Vurgu | `#2f6fed` | İmleç, simge zemini, bağlantılar |
| Açık vurgu | `#a9c6ff` | Vurgu zemini üzerindeki imleç |

## Kullanım kuralları

- Logonun çevresinde en az bir harf çizgisi kalınlığı kadar boşluk bırakın.
- Logoyu eğmeyin, oranlarını bozmayın, harflerin rengini değiştirmeyin.
- Açık zeminde `logo`, koyu zeminde `logo-acik` kullanın; renkli ya da desenli
  zeminlerde kare simgeyi tercih edin.
- 32 pikselden küçük boyutlarda harfler okunmaz; o boyutlarda yalnızca simgeyi
  kullanın.
