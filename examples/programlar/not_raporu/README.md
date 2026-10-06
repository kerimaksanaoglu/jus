# Not raporu

Bir JSON dosyasındaki öğrenci ve ders notlarını okur, doğrular, öğrenci başına
ortalama ve harf notu ile ders başına istatistik hesaplar; sonucu ekrana metin
tablosu olarak yazar ve JSON dosyasına kaydeder.

## Dosyalar

| Dosya             | Görev                                         |
|-------------------|-----------------------------------------------|
| `not_raporu.jus`  | Betiğin kendisi                               |
| `ornek_veri.json` | 17 öğrenci, 4 ders içeren örnek girdi         |
| `dene.sh`         | Uçtan uca sınama betiği                       |

## Çalıştırma

```
jus not_raporu.jus [girdi.json] [çıktı.json]
```

- Girdi varsayılan olarak `ornek_veri.json`, çıktı `not_raporu_cikti.json`
  dosyasıdır (çalışma klasöründe aranır / oluşturulur).
- Çıkış kodları: `0` başarılı; `1` girdi ya da çıktı hatası (olmayan dosya,
  bozuk JSON, beklenen yapıda olmayan girdi, geçerli kayıt yok, yazılamayan
  çıktı); `2` hatalı kullanım; `3` rapor üretildi ama bazı kayıtlar geçersiz
  olduğu için atlandı.

### Girdi biçimi

```json
{
  "dönem": "2024-2025 Güz",
  "öğrenciler": [
    {"numara": 1001, "ad": "Ayşe Yılmaz", "notlar": {"Matematik": 85, "Fizik": 78}}
  ]
}
```

Bir öğrencinin tüm derslere notu olması gerekmez; ortalama eldeki notlarla
hesaplanır. Ders sütunları, derslerin girdide ilk görüldüğü sıradadır.

### Doğrulama kuralları

Geçersiz bir kayıt atlanır ve nedeni kayıt sırasıyla bildirilir:

- `numara` bulunmalı ve pozitif tam sayı olmalı; aynı numara ikinci kez
  kullanılamaz (ikinci kayıt atlanır).
- `ad` boş olmayan bir metin olmalı.
- `notlar` boş olmayan bir ders-not eşlemesi olmalı; her not 0 ile 100 arasında
  bir sayı olmalı (metin, mantıksal değer ve aralık dışı sayılar geçersizdir).

### Harf notu ölçeği

| Ortalama | Harf | Ortalama | Harf |
|----------|------|----------|------|
| 90 ve üstü | AA | 70 - 74,99 | CC |
| 85 - 89,99 | BA | 60 - 69,99 | DC |
| 80 - 84,99 | BB | 50 - 59,99 | DD |
| 75 - 79,99 | CB | 50'den az  | FF |

Harf notu yuvarlanmamış ortalamadan belirlenir; tabloda ortalama iki ondalıkla
gösterilir.

### Sıralama

- Ada göre: Türk alfabesi sırası (`Ilgaz` < `Ismail` < `İpek`; `Ç`, `Ö`, `Ş`,
  `Ü` kendi yerlerinde).
- Ortalamaya göre: yüksekten düşüğe; eşitlikte ada göre.

Kararlı bir birleştirmeli sıralama betiğin içinde yazılmıştır (bkz. `sırala_listeyi`).

### Çıktı JSON'u

Anahtarlar: `dönem`, `öğrenci_sayısı`, `öğrenciler_ada_göre` (numara, ad,
notlar, ortalama, harf_notu), `sıralama_ortalamaya_göre` (numara listesi),
`dersler` (ders, öğrenci_sayısı, ortalama, en_yüksek, en_düşük), `hatalar`.
Ortalamalar iki ondalığa yuvarlanmıştır.

## Örnek oturum

Aşağıdaki çıktılar gerçek çalıştırmadan alınmıştır.

```
$ jus not_raporu.jus ornek_veri.json rapor.json
Not raporu: 17 öğrenci, 4 ders

Ada göre (Türk alfabesi):
   No  Ad                    Matematik  Fizik  Türkçe  Tarih  Ortalama  Harf
----------------------------------------------------------------------------
 1015  Ali Can Tunç               59.5   64.5      67     62    63.25  DC
 1001  Ayşe Yılmaz                  85     78      92     88    85.75  BA
 1013  Burak Özkan                  66     70      73      -    69.67  DC
 1006  Çağla Şahin                  90     84      77     93    86.00  BA
 1016  Ebru Güneş                   81     75      79     70    76.25  CB
 1012  Elif Doğan                   77     82      90     85    83.50  BB
 1008  Gül Arslan                  100     98      95     99    98.00  AA
 1011  Hasan Hüseyin Yıldız         56     61      49     58    56.00  DD
 1005  Ilgaz Kaya                   48     52      60     45    51.25  DD
 1017  Ismail Polat                 93     87      82     86    87.00  BA
 1004  İpek Demir                   74     68      81     79    75.50  CB
 1002  Mehmet Çelik                 62     55      70     64    62.75  DC
 1007  Ömer Faruk Aydın             71     66      58     73    67.00  DC
 1014  Sevgi Aksoy                  88     93      84     91    89.00  BA
 1003  Şule Öztürk                  95     91      89     97    93.00  AA
 1009  Ünal Koç                     39     44      51     47    45.25  FF
 1010  Zeynep Kurt                  83     79      86     80    82.00  BB

Ortalamaya göre (yüksekten düşüğe):
Sıra     No  Ad                    Ortalama  Harf
-------------------------------------------------
   1   1008  Gül Arslan              98.00  AA
   2   1003  Şule Öztürk             93.00  AA
   3   1014  Sevgi Aksoy             89.00  BA
   4   1017  Ismail Polat            87.00  BA
   5   1006  Çağla Şahin             86.00  BA
   6   1001  Ayşe Yılmaz             85.75  BA
   7   1012  Elif Doğan              83.50  BB
   8   1010  Zeynep Kurt             82.00  BB
   9   1016  Ebru Güneş              76.25  CB
  10   1004  İpek Demir              75.50  CB
  11   1013  Burak Özkan             69.67  DC
  12   1007  Ömer Faruk Aydın        67.00  DC
  13   1015  Ali Can Tunç            63.25  DC
  14   1002  Mehmet Çelik            62.75  DC
  15   1011  Hasan Hüseyin Yıldız    56.00  DD
  16   1005  Ilgaz Kaya              51.25  DD
  17   1009  Ünal Koç                45.25  FF

Ders istatistikleri:
Ders       Öğrenci  Ortalama  En yüksek  En düşük
-------------------------------------------------
Matematik       17     74.56        100        39
Fizik           17     73.38         98        44
Türkçe          17     75.47         95        49
Tarih           16     76.06         99        45

Rapor yazıldı: rapor.json
```

Geçersiz kayıtlı bir girdi (çıkış kodu 3):

```
$ jus not_raporu.jus kucuk.json k_rapor.json
Geçersiz kayıtlar atlandı (2 sorun):
  - Kayıt 2: numara 1 daha önce 1. kayıtta kullanılmış
  - Kayıt 3: 'Matematik' notu geçersiz: 120 (0-100 arası sayı olmalı)

Not raporu: 1 öğrenci, 2 ders
...
Rapor yazıldı: k_rapor.json
```

Hatalı girdi dosyaları (çıkış kodu 1):

```
$ jus not_raporu.jus yok.json
Hata: Girdi dosyası bulunamadı: yok.json
$ jus not_raporu.jus b.json
Hata: Girdi dosyası geçerli JSON değil: b.json (Geçersiz JSON (2. karakter): anahtar olarak metin bekleniyor.)
```

## Sınama

```
bash dene.sh [jus-yolu]      # varsayılan: ./jus
```

Betik örnek veriyi, ada ve ortalamaya göre sıralamayı, doğrulama kurallarını,
bozuk/boş/olmayan girdi dosyalarını ve kullanım hatalarını geçici bir klasörde
dener; sonda `GEÇTİ` ya da `KALDI` yazar ve buna göre `0` / `1` koduyla çıkar.
