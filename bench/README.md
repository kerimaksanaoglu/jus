# JUS hız ölçümleri

Bu klasör, JUS yorumlayıcısını CPython ile karşılaştıran küçük bir ölçüm
paketidir. Her ölçüm için aynı algoritmayı uygulayan bir `ad.jus` ve bir `ad.py`
dosyası vardır; ikisi de aynı iş yükünü çalıştırır ve aynı tek satırlık sonucu
yazar.

## Çalıştırma

```
python bench/calistir.py [jus-yolu] [-n TEKRAR]
```

- `jus-yolu` verilmezse depo kökündeki `jus.exe` kullanılır.
- `-n` her programın kaç kez çalıştırılacağını belirler (varsayılan 3); en iyi
  süre alınır.
- Betik yalnızca standart kütüphaneyi kullanır.
- Her çift için JUS ve Python çıktıları karşılaştırılır; fark varsa o ölçüm
  tabloya girmez ve betik sıfır olmayan kodla çıkar.

## Ne ölçülüyor

Süre, süreç başlatılmasından bitişine kadar geçen duvar saati süresidir.
Yorumlayıcının açılış süresi ve kaynak dosyanın ayrıştırılması da dahildir
(CPython için yaklaşık 20 ms, JUS için birkaç ms). Her program yaklaşık
0,2-0,8 saniye sürecek biçimde boyutlandırılmıştır; bu nedenle açılış süresinin
payı küçüktür ama sıfır değildir.

| Program | Ölçülen iş |
|---|---|
| `fib` | Özyinelemeli `fib(32)`: fonksiyon çağrısı, karşılaştırma, toplama |
| `dongu` | 3000x2000 iç içe döngü, `toplam += (i * j) % 7` |
| `elek` | 2.000.000 öğeli listeyle Eratosthenes eleği: dizinleme, atama, `iken` döngüsü |
| `liste` | 900.000 ekleme, 900.000 dizinleme, 900.000 öğenin sıralanması |
| `sozluk` | Sayı anahtarlı 400.000 ekleme/okuma; metin anahtarlı 300.000 ekleme/okuma |
| `metin` | `metin()` dönüşümü, `birleştir`/`böl` (300.000 parça), karakter dolaşma, büyük harf |
| `birlestir` | Döngüde `s += "ab"` ile metin büyütme (20.000 ve 5.000 tur) |
| `sinif` | 2.000.000 yöntem çağrısı (biri `üst.` çağrısıyla), 100.000 nesne oluşturma |
| `kapanim` | Yakalanmış değişkeni değiştiren iç fonksiyonun 4.000.000 çağrısı |
| `hata` | 2.000.000 turda `dene`/`yakala` (üçte biri `fırlat`), 600.000 sıfıra bölme hatası yakalama |
| `matris` | 140x140 matris çarpımı, iç içe liste |

Eşdeğerlik notları:

- Döngüler JUS'ta `her i içinde aralık(n)`, Python'da `for i in range(n)`
  biçimindedir. JUS'ta `aralık` tam bir liste üretir, Python'da `range` tembeldir;
  bu fark ölçüme dahildir.
- Sayı dönüşümleri JUS'ta `metin()`, Python'da `str()` iledir. JUS sayıları 64 bit
  kayan noktalıdır, Python tam sayıları sınırsızdır; bu yüzden tüm ara değerler
  2^53'ün altında tutulmuştur.
- Python programları bilerek düz döngülerle yazılmıştır (liste üreteci, `sum` gibi
  kestirmeler kullanılmamıştır); amaç iki yorumlayıcının aynı işi aynı adımlarla
  yapmasıdır. `sorted`, `str.join`, `str.split`, `str.upper` ise JUS'taki
  `sırala`, `birleştir`, `böl`, `büyük_harf` gibi iki tarafta da yerleşik işlevdir.
- `hata` ölçümünde Python tarafı `Exception` alt sınıfı fırlatır; JUS'ta
  fırlatılan değer sıradan bir sayıdır.

## Makine ve sürüm

- İşletim sistemi: Windows 11 Enterprise (10.0.26300), Intel64 Family 6 Model 191
  işlemci
- Python: 3.11.9
- JUS: 0.6.0 (`jus --surum` çıktısı)
- Çalıştırma: `python bench/calistir.py ./jus.exe -n 3`

## Sonuçlar

Oran = JUS süresi / Python süresi. 1'den küçük değer JUS'un daha hızlı olduğunu
gösterir. Süreler saniyedir, 3 çalıştırmanın en iyisidir.

| Program | JUS sn | Python sn | Oran |
|---|---:|---:|---:|
| birlestir | 0,108 | 0,035 | 3,06x |
| dongu | 0,220 | 0,373 | 0,59x |
| elek | 0,265 | 0,370 | 0,72x |
| fib | 0,187 | 0,234 | 0,80x |
| hata | 0,200 | 0,502 | 0,40x |
| kapanim | 0,149 | 0,229 | 0,65x |
| liste | 0,256 | 0,310 | 0,83x |
| matris | 0,188 | 0,241 | 0,78x |
| metin | 0,245 | 0,138 | 1,78x |
| sinif | 0,198 | 0,269 | 0,73x |
| sozluk | 0,302 | 0,173 | 1,74x |

Oranların geometrik ortalaması: 0,92x (11 ölçüm).

## Yorum

Bunlar tek makinede, tek oturumda alınmış ölçümlerdir. Aynı makinede yapılan
önceki bir deneme çalıştırmasında Python süreleri %30'a varan farklar gösterdi
(örneğin `fib`); bu nedenle yaklaşık 0,1'lik oran farkları anlamlı sayılmamalıdır.
Tek bir Python sürümü (3.11) ve tek bir derleme ile karşılaştırılmıştır.

- JUS'un önde olduğu yerler: döngü, aritmetik, fonksiyon/yöntem çağrısı, kapanım,
  liste dizinleme/atama ve `dene`/`yakala` içeren programlarda JUS bu ölçümlerde
  CPython 3.11'den yaklaşık 1,3-2,6 kat hızlı çıktı. En büyük fark `hata`
  programında; burada Python'un istisna oluşturma maliyeti öne çıkıyor.
  `elek` ve `dongu` gibi saf döngü programlarında fark 1,6-1,8 kat.
- JUS'un geride olduğu yerler:
  - Metin ekleme (`birlestir`): `s += "ab"` her turda metnin tamamını kopyalar;
    CPython aynı kalıpta metni yerinde büyütür. Bu ölçümde JUS yaklaşık 3 kat
    yavaş. 0.6 sürümünden önce fark 17 kattı; uzun metinlerin karma değeri
    artık tüm baytlar yerine örneklenerek hesaplanıyor. Çok büyük metinler
    `birleştir` ile kurulduğunda kopyalama maliyeti de ortadan kalkar.
  - Sözlük (`sozluk`) ve metin işleme (`metin`): bu ölçümlerde JUS yaklaşık 1,6-1,8
    kat yavaş. Sözlük programında metin anahtarı üretimi (`"k" + metin(i)`) ve
    sözlük erişimi birlikte ölçülüyor; ayrı ayrı profillenmedi, bu nedenle payların
    hangisinden geldiği bilinmiyor.
- Ölçülmeyenler: bellek kullanımı, çok büyük girdilerde davranış, standart
  kütüphane modülleri, dosya girdi/çıktısı, ilk açılış süresinin ayrı ölçümü.
  Python'da C ile yazılmış kütüphaneler (örneğin `sorted` dışındaki sayısal
  paketler) bu karşılaştırmaya girmedi.
- Sonuçlar bir dilin genel olarak diğerinden hızlı olduğunu göstermez; yalnızca
  bu 11 küçük programın bu makinede nasıl davrandığını gösterir.
