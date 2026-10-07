# 13 - JSON ve veri

Önceki: [12 - Küçük projeler](12-kucuk-projeler.md) | [İçindekiler](README.md) | Sonraki: [14 - Ağ ve HTTP](14-ag-ve-http.md)

Programlar çoğu zaman veriyi kendi içinde tutmakla kalmaz; dosyaya kaydeder, başka bir programa gönderir ya da başka bir programdan alır. Bunun için verinin metin olarak yazılabilmesi gerekir. Yapılacaklar projesinde (12. bölüm) kendi satır biçimimizi uydurmuştuk: `0|Ekmek al`. İç içe sözlükler ve listeler için böyle bir biçim tasarlamak zahmetlidir. Bunun yerine herkesin bildiği bir biçim kullanılır: **JSON**.

JSON, veriyi metin olarak yazan bir biçimdir. Neredeyse tüm programlama dilleri okuyup yazabilir. Örnek bir JSON metni:

```
{"ad": "Ayşe", "yaş": 29, "öğrenci": true, "adres": null, "notlar": [90, 85.5]}
```

JSON ile JUS değerleri arasındaki karşılıklar şöyledir:

| JSON | JUS |
|------|-----|
| nesne `{...}` | sözlük |
| dizi `[...]` | liste |
| metin `"..."` | metin |
| sayı | sayı |
| `true`, `false` | `doğru`, `yanlış` |
| `null` | `boş` |

JSON'u `json` modülü işler. Bu modül standart kütüphanededir; `kullan json` yeter.

## `json.çöz`: metinden değere

`json.çöz(metin)` bir JSON metnini JUS değerine çevirir.

JSON metinleri çift tırnak içerdiği için JUS metni olarak yazarken her tırnağın önüne `\` koymanız gerekir (bunu ikinci bölümde gördünüz). Gerçek programlarda JSON metni genellikle bir dosyadan ya da ağdan gelir; elle yazmak yalnızca deneme içindir.

```jus
kullan json

değişken metni = "{\"ad\": \"Ayşe\", \"yaş\": 29, \"öğrenci\": true, \"adres\": null, \"notlar\": [90, 85.5, 70], \"iletişim\": {\"e_posta\": \"ayse@ornek.com\"}}"
değişken kişi = json.çöz(metni)
yaz(kişi)
yaz(tür(kişi))
yaz(kişi["ad"], kişi["yaş"] + 1)
yaz(kişi["öğrenci"])
yaz(kişi["adres"] == boş)
yaz(kişi["notlar"][1])
yaz(kişi["iletişim"]["e_posta"])

yaz(json.çöz("[1, 2, 3]"))
yaz(json.çöz("42"))
yaz(json.çöz("\"merhaba\""))
yaz(json.çöz("true"))
yaz(json.çöz("null"))
```

```
{"ad": "Ayşe", "yaş": 29, "öğrenci": doğru, "adres": boş, "notlar": [90, 85.5, 70], "iletişim": {"e_posta": "ayse@ornek.com"}}
sözlük
Ayşe 30
doğru
doğru
85.5
ayse@ornek.com
[1, 2, 3]
42
merhaba
doğru
boş
```

Sonuç sıradan bir sözlüktür; sekizinci bölümdeki her şey onun için geçerlidir. `true` değeri `doğru`, `null` değeri `boş` olarak geldi. JSON'un tek bir sayı, metin ya da `true` gibi basit bir değer de olabileceğini son dört satırda görüyorsunuz.

## `json.yaz`: değerden metne

`json.yaz(değer)` bir değeri tek satırlık JSON metnine çevirir. Çevrilebilen değerler: `boş`, mantıksal değerler, sayılar, metinler, listeler ve anahtarları metin olan sözlükler.

```jus
kullan json

değişken veri = {"ad": "Ayşe", "yaş": 29, "öğrenci": doğru, "adres": boş, "notlar": [90, 85.5, 70], "boşluk": {}, "liste": []}
yaz(json.yaz(veri))
yaz(json.yaz([1, "iki", [3]]))
yaz(json.yaz("tırnak \" ve satır\nsonu ve Türkçe: çğış"))
yaz(json.yaz(boş))
yaz(json.yaz(3.5))
```

```
{"ad":"Ayşe","yaş":29,"öğrenci":true,"adres":null,"notlar":[90,85.5,70],"boşluk":{},"liste":[]}
[1,"iki",[3]]
"tırnak \" ve satır\nsonu ve Türkçe: çğış"
null
3.5
```

Gözlemler:

- Çıktı `true`, `null` gibi JSON yazımını kullanır; `doğru` ya da `boş` yazılmaz.
- Metin içindeki tırnak ve satır sonu `\"` ve `\n` olarak kaçışlanır. Türkçe harfler olduğu gibi kalır.
- Çıktı bilerek sıkıdır: boşluk içermez. Bu, dosya ve ağ için uygundur ama gözle okumak zordur.

### Girintili yazım

`json.yaz(değer, girinti)` aynı veriyi okunaklı biçimde yazar. `girinti`, her iç düzey için kullanılacak boşluk sayısıdır.

```jus
kullan json

değişken veri = {"ad": "Ayşe", "yaş": 29, "öğrenci": doğru, "adres": boş, "notlar": [90, 85.5, 70], "boşluk": {}, "liste": []}
yaz(json.yaz(veri, 2))
```

```
{
  "ad": "Ayşe",
  "yaş": 29,
  "öğrenci": true,
  "adres": null,
  "notlar": [
    90,
    85.5,
    70
  ],
  "boşluk": {},
  "liste": []
}
```

Anlamı aynıdır; yalnızca görünüşü farklıdır. İnsanın okuyacağı dosyalar (ayar dosyaları gibi) için girintili yazım, makinenin okuyacağı iletiler için sıkı yazım uygundur.

## JSON ile dosyaya kaydetmek ve okumak

`json.yaz` ile `dosya.yaz`'ı, `dosya.oku` ile `json.çöz`'ü art arda kullanırsanız her türlü veriyi dosyaya saklayabilirsiniz. Aşağıdaki program bir ayar dosyasını okur; dosya yoksa varsayılan ayarları kullanır. Sonra ayarları değiştirip kaydeder ve geri okuyup aynı olduklarını denetler.

```jus
kullan json
kullan dosya

değişken DOSYA = "ayarlar_deneme.json"

fonksiyon ayarlarıYükle():
    eğer değil dosya.var_mı(DOSYA):
        dön {"tema": "açık", "yazı_boyutu": 12}
    dön json.çöz(dosya.oku(DOSYA))

fonksiyon ayarlarıKaydet(ayarlar):
    dosya.yaz(DOSYA, json.yaz(ayarlar, 2) + "\n")

değişken ayarlar = ayarlarıYükle()
yaz("İlk:", ayarlar)

ayarlar["tema"] = "koyu"
ayarlar["son_dosyalar"] = ["not.txt", "çizim.png"]
ayarlarıKaydet(ayarlar)

yaz(dosya.oku(DOSYA))

değişken yeni = ayarlarıYükle()
yaz("Sonra:", yeni)
yaz(yeni == ayarlar)
dosya.sil(DOSYA)
```

```
İlk: {"tema": "açık", "yazı_boyutu": 12}
{
  "tema": "koyu",
  "yazı_boyutu": 12,
  "son_dosyalar": [
    "not.txt",
    "çizim.png"
  ]
}

Sonra: {"tema": "koyu", "yazı_boyutu": 12, "son_dosyalar": ["not.txt", "çizim.png"]}
doğru
```

Dosya içeriğinden sonraki boş satır, dosyanın sonuna eklediğimiz `"\n"` yüzünden çıkar. Sözlükler içerik olarak eşit olduğunda `==` ile `doğru` verir (sekizinci bölüm); bu yüzden kaydedip geri okunan veri özgün veriye eşittir.

Program sonunda dosyayı sildik. Denerken siliş satırını kaldırırsanız `ayarlar_deneme.json` dosyasını bir metin düzenleyicide açıp görebilirsiniz.

## Bozuk JSON: hatayı yakalamak

Dışarıdan gelen veri bozuk olabilir: dosya yarım yazılmış, bir virgül eksik ya da metin hiç JSON değildir. Geçersiz JSON `json.çöz`'de çalışma zamanı hatası verir. Bu hatayı dokuzuncu bölümdeki gibi `dene` / `yakala` ile karşılarsınız. Hata değeri, sorunun hangi karakterde olduğunu söyleyen bir metindir.

```jus
kullan json

fonksiyon güvenliÇöz(metin2):
    dene:
        dön json.çöz(metin2)
    yakala hata:
        yaz("Geçersiz JSON:", hata)
        dön boş

yaz(güvenliÇöz("{\"a\": 1}"))
yaz(güvenliÇöz("{\"a\": 1"))
yaz(güvenliÇöz("{'a': 1}"))
yaz(güvenliÇöz(""))
yaz(güvenliÇöz("[1, 2,]"))
yaz(güvenliÇöz("merhaba"))
```

```
{"a": 1}
Geçersiz JSON: Geçersiz JSON (8. karakter): ',' ya da '}' bekleniyor.
boş
Geçersiz JSON: Geçersiz JSON (2. karakter): anahtar olarak metin bekleniyor.
boş
Geçersiz JSON: Geçersiz JSON (1. karakter): değer bekleniyor.
boş
Geçersiz JSON: Geçersiz JSON (7. karakter): beklenmeyen karakter.
boş
Geçersiz JSON: Geçersiz JSON (1. karakter): beklenmeyen karakter.
boş
```

JSON, JUS'tan daha katıdır: metinler yalnızca çift tırnakla yazılır (`'a'` geçersizdir), son öğeden sonra virgül konamaz ve boş metin geçerli bir JSON değildir.

Hatayı yakalamadan bırakırsanız program durur:

```jus
kullan json
yaz(json.çöz("{bozuk"))
```

```
ornek.jus:2: çalışma zamanı hatası: Geçersiz JSON (2. karakter): anahtar olarak metin bekleniyor.
```

Dosyadan okuyacağınız her JSON için bu `dene` bloğunu yazmanız iyi bir alışkanlıktır: dosya bozulursa program çökmek yerine varsayılan değere dönebilir.

## `tr` modülü: Türkiye'ye özgü işlemler

Veriyle çalışırken sık karşılaşılan birkaç Türkiye'ye özgü iş için `tr` modülü vardır: para yazımı, T.C. kimlik numarası, IBAN ve telefon denetimi, plaka kodları.

```jus
kullan tr

yaz(tr.para(1234.5))
yaz(tr.para(1234567.891))
yaz(tr.para(0.5))
yaz(tr.para(99))
yaz(tr.para(-250.75))

yaz(tr.kimlik_no_geçerli_mi("10000000146"))
yaz(tr.kimlik_no_geçerli_mi(10000000146))
yaz(tr.kimlik_no_geçerli_mi("10000000147"))
yaz(tr.kimlik_no_geçerli_mi("123"))

yaz(tr.telefon_geçerli_mi("0532 123 45 67"))
yaz(tr.telefon_geçerli_mi("+90 (532) 123-45-67"))
yaz(tr.telefon_geçerli_mi("12345"))
yaz(tr.iban_geçerli_mi("TR33 0006 1005 1978 6457 8413 26"))
yaz(tr.iban_geçerli_mi("TR00 0000 0000 0000 0000 0000 00"))

yaz(tr.plaka_ili(6))
yaz(tr.plaka_ili(34))
yaz(uzunluk(tr.iller))
yaz(tr.iller[0], tr.iller[80])
```

```
1.234,50 ₺
1.234.567,89 ₺
0,50 ₺
99,00 ₺
-250,75 ₺
doğru
doğru
yanlış
yanlış
doğru
doğru
yanlış
doğru
yanlış
Ankara
İstanbul
81
Adana Düzce
```

| Üye | Açıklama |
|-----|----------|
| `para(tutar)` | Tutarı `1.234,50 ₺` biçiminde metne çevirir |
| `kimlik_no_geçerli_mi(no)` | T.C. kimlik numarasının denetim basamakları doğruysa `doğru`; metin ya da sayı alır |
| `iban_geçerli_mi(iban)` | Türkiye IBAN'ının biçimi ve denetim basamakları doğruysa `doğru`; boşluklar yok sayılır |
| `telefon_geçerli_mi(no)` | Türkiye telefon numarası biçimine uyuyorsa `doğru`; boşluk, parantez, tire ve `+90` kabul edilir |
| `plaka_ili(kod)` | 1-81 arasındaki plaka koduna karşılık gelen il |
| `iller` | 81 ilin plaka koduna göre sıralı listesi |

Örneklerde kullanılan kimlik numarası ve IBAN, doğrulama denemelerinde yaygın kullanılan örnek değerlerdir; gerçek bir kişiye ait olduklarını varsaymayın. Bu işlevler yalnızca **biçimi ve denetim basamaklarını** doğrular: bir numaranın denetimden geçmesi, o numaranın gerçekten var olduğu anlamına gelmez.

`tr.para` bir sayı ister; sayıyı metne çevirmek için ayrıca `metin()` yazmanız gerekmez. Sonuç binlik ayracı nokta, ondalık ayracı virgül olan bir metindir; bu yüzden onunla artık aritmetik yapamazsınız. Hesabı sayılarla yapın, yalnızca ekrana yazarken `tr.para` kullanın.

## Sık yapılan hatalar

**JSON'u metin olarak değil, değer olarak vermek.** `json.çöz` metin ister:

```jus
kullan json
yaz(json.çöz(5))
```

```
ornek.jus:2: çalışma zamanı hatası: 'json.çöz' fonksiyonu metin ister; sayı verildi.
```

**Anahtarı metin olmayan sözlüğü yazmak.** JSON nesnelerinin anahtarları metindir. JUS'ta sayı anahtarı kullanılabilse de JSON'a çevrilemez:

```jus
kullan json
yaz(json.yaz({1: "a"}))
```

```
ornek.jus:2: çalışma zamanı hatası: JSON'a çevrilecek sözlüklerde anahtarlar metin olmalı; sayı bulundu.
```

Çözüm: anahtarı `metin(1)` ile metne çevirin.

**Fonksiyon gibi çevrilemeyen değerleri yazmak.**

```jus
kullan json

fonksiyon f():
    dön 1

yaz(json.yaz(f))
```

```
ornek.jus:6: çalışma zamanı hatası: fonksiyon türündeki değerler JSON'a çevrilemez.
```

JSON yalnızca veridir; fonksiyonları ve sınıf nesnelerini taşımaz. Nesnenin alanlarını bir sözlükte toplayıp onu yazabilirsiniz.

**JUS yazımını JSON sanmak.** `{'a': 1}` ya da sondaki virgül JUS'ta geçerlidir ama JSON'da değildir. Dışarıdan gelen JSON'u `json.çöz`'e vermeden önce elle değiştirmeyin; bozuk veriyi bozuk olarak yakalayın.

**`yaz` ile `json.yaz` çıktısının aynı görünmesini beklemek.** `yaz(1 / 3)` `0.33333333333333` yazar (en çok 14 anlamlı basamak); `yaz(json.yaz(1 / 3))` ise `0.3333333333333333` yazar. İkisi aynı sayıdır; `json.yaz`, veri geri okunduğunda tam olarak aynı sayı elde edilsin diye daha çok basamak yazar. Yuvarlanmış görünüm istiyorsanız veriyi yazmadan önce `yuvarla(x, basamak)` ile düzenleyin.

## Alıştırmalar

1. `[{"ad": "Ali", "puan": 7}, {"ad": "Ece", "puan": 9}, {"ad": "Can", "puan": 8}]` JSON metnini `json.çöz` ile çözün ve puanların ortalamasını yazdırın.
2. `{"ad": "Sefiller", "yazar": "Victor Hugo", "sayfa": 1488, "etiketler": ["roman", "klasik"]}` sözlüğünü dört boşluk girintiyle JSON olarak yazdırın.
3. `[70, 85, 90, 65]` listesini JSON olarak bir dosyaya kaydedin, dosyadan okuyup toplamlarını hesaplayın ve dosyayı silin.
4. `çözVeyaVarsayılan(metin, varsayılan)` fonksiyonunu yazın: metin geçerli JSON ise çözülmüş değeri, değilse `varsayılan` değeri döndürsün. Üç ayrı girdiyle deneyin.
5. `{"Defter": 24.5, "Kalem": 7, "Çanta": 1250.75}` sözlüğündeki ürünleri adı sola, fiyatı (`tr.para` ile) sağa yaslı yazdırın. Ardından `"10000000146"`, `"10000000147"` ve `"abc"` için kimlik numarası denetimi yapıp `geçerli` ya da `geçersiz` yazdırın.

## Çözümler

**1.**

```jus
kullan json

değişken metin2 = "[{\"ad\": \"Ali\", \"puan\": 7}, {\"ad\": \"Ece\", \"puan\": 9}, {\"ad\": \"Can\", \"puan\": 8}]"
değişken liste = json.çöz(metin2)
değişken toplam = 0
her k içinde liste:
    toplam += k["puan"]
yaz("Ortalama puan:", toplam / uzunluk(liste))
```

```
Ortalama puan: 8
```

**2.**

```jus
kullan json

değişken kitap = {"ad": "Sefiller", "yazar": "Victor Hugo", "sayfa": 1488, "etiketler": ["roman", "klasik"]}
yaz(json.yaz(kitap, 4))
```

```
{
    "ad": "Sefiller",
    "yazar": "Victor Hugo",
    "sayfa": 1488,
    "etiketler": [
        "roman",
        "klasik"
    ]
}
```

**3.**

```jus
kullan json
kullan dosya

değişken notlar = [70, 85, 90, 65]
dosya.yaz("notlar_deneme.json", json.yaz(notlar))
yaz(dosya.oku("notlar_deneme.json"))

değişken okunan = json.çöz(dosya.oku("notlar_deneme.json"))
değişken toplam = 0
her n içinde okunan:
    toplam += n
yaz("Toplam:", toplam)
dosya.sil("notlar_deneme.json")
```

```
[70,85,90,65]
Toplam: 310
```

**4.**

```jus
kullan json

fonksiyon çözVeyaVarsayılan(metin2, varsayılan):
    dene:
        dön json.çöz(metin2)
    yakala:
        dön varsayılan

yaz(çözVeyaVarsayılan("[1, 2]", []))
yaz(çözVeyaVarsayılan("bozuk", []))
yaz(çözVeyaVarsayılan("{\"a\": ", {"a": 0}))
```

```
[1, 2]
[]
{"a": 0}
```

**5.**

```jus
kullan tr

değişken fiyatlar = {"Defter": 24.5, "Kalem": 7, "Çanta": 1250.75}
her ad içinde anahtarlar(fiyatlar):
    yaz(sağa_doldur(ad, 8) + sola_doldur(tr.para(fiyatlar[ad]), 14))

her no içinde ["10000000146", "10000000147", "abc"]:
    eğer tr.kimlik_no_geçerli_mi(no):
        yaz(no, "geçerli")
    değilse:
        yaz(no, "geçersiz")
```

```
Defter         24,50 ₺
Kalem           7,00 ₺
Çanta       1.250,75 ₺
10000000146 geçerli
10000000147 geçersiz
abc geçersiz
```

---

Önceki: [12 - Küçük projeler](12-kucuk-projeler.md) | [İçindekiler](README.md) | Sonraki: [14 - Ağ ve HTTP](14-ag-ve-http.md)
