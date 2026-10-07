# 08 - Sözlükler

Önceki: [07 - Metinler](07-metinler.md) | [İçindekiler](README.md) | Sonraki: [09 - Hatalar](09-hatalar.md)

Liste öğelere sıra numarasıyla ulaşır: "üçüncü öğe". Ama bazen bir bilgiye adıyla ulaşmak istersiniz: "kişinin yaşı", "elmanın fiyatı". Bunun için **sözlük** kullanılır. Sözlük, **anahtar** ve **değer** çiftlerinden oluşur. Anahtar, değeri bulmanızı sağlayan etikettir.

## Sözlük oluşturmak ve kullanmak

Sözlük süslü parantez içinde yazılır. Her çift `anahtar: değer` biçimindedir; çiftler virgülle ayrılır.

```jus
değişken kişi = {"ad": "Ayşe", "yaş": 30}
yaz(kişi)
yaz(kişi["ad"])
yaz(kişi["yaş"])

kişi["yaş"] = 31
kişi["şehir"] = "Ankara"
yaz(kişi)
yaz(uzunluk(kişi))

kişi["yaş"] += 1
yaz(kişi["yaş"])

değişken boşSözlük = {}
yaz(boşSözlük)
yaz(uzunluk(boşSözlük))

değişken telefon = {
    "Ali": "555 111",
    "Veli": "555 222",
}
yaz(telefon["Veli"])

değişken karışıkAnahtar = {1: "bir", "1": "metin bir", doğru: "evet"}
yaz(karışıkAnahtar[1])
yaz(karışıkAnahtar["1"])
yaz(karışıkAnahtar[doğru])
```

```
{"ad": "Ayşe", "yaş": 30}
Ayşe
30
{"ad": "Ayşe", "yaş": 31, "şehir": "Ankara"}
3
32
{}
0
555 222
bir
metin bir
evet
```

Neler öğrendik:

- `sözlük["anahtar"]` değeri okur.
- `sözlük["anahtar"] = değer` mevcut bir girdiyi değiştirir. Var olmayan bir anahtara atarsanız **yeni girdi eklenir**.
- `+=` gibi bileşik atamalar sözlük girdilerinde de çalışır.
- `uzunluk(sözlük)` girdi sayısını verir.
- Köşeli ya da süslü parantez içindeki satır sonları yok sayılır; sözlüğü birden çok satıra yazabilirsiniz (`telefon` örneği). Son çiftten sonra virgül konabilir.
- Anahtarlar **metin, sayı ya da mantıksal** olabilir. Değerler her türden olabilir. `1` ile `"1"` farklı anahtarlardır.
- Sözlükler girdileri eklenme sırasıyla tutar.

## Anahtar var mı? `içinde`, `al`

Olmayan bir anahtarı okumak hatadır. Önce `içinde` ile bakabilir ya da `al(sözlük, anahtar, varsayılan)` ile hata vermeden okuyabilirsiniz. `al`, anahtar yoksa verdiğiniz varsayılan değeri döndürür.

```jus
değişken fiyat = {"elma": 12, "armut": 15, "kiraz": 40}

yaz("elma" içinde fiyat)
yaz("muz" içinde fiyat)

yaz(al(fiyat, "elma", 0))
yaz(al(fiyat, "muz", 0))
yaz(al(fiyat, "muz", "fiyat yok"))

yaz(anahtarlar(fiyat))
yaz(değerler(fiyat))

değişken silinen = sil(fiyat, "armut")
yaz("Silinen değer:", silinen)
yaz(fiyat)
```

```
doğru
yanlış
12
0
fiyat yok
["elma", "armut", "kiraz"]
[12, 15, 40]
Silinen değer: 15
{"elma": 12, "kiraz": 40}
```

- `anahtar içinde sözlük` anahtarın varlığını denetler.
- `anahtarlar(sözlük)` ve `değerler(sözlük)` birer liste verir.
- `sil(sözlük, anahtar)` girdiyi siler ve değerini döndürür.

## Sözlüğü gezmek

`her` döngüsü sözlükte **anahtarları** dolaşır. Değere ihtiyacınız varsa `sözlük[anahtar]` ile ulaşırsınız.

```jus
değişken fiyat = {"elma": 12, "armut": 15, "kiraz": 40}

her ad içinde fiyat:
    yaz(ad, "->", fiyat[ad])

değişken toplam = 0
her f içinde değerler(fiyat):
    toplam += f
yaz("Toplam:", toplam)

her ad içinde sırala(anahtarlar(fiyat)):
    yaz(ad)

# değerleri değiştirirken anahtarların listesi üzerinde gezmek
her ad içinde anahtarlar(fiyat):
    fiyat[ad] = fiyat[ad] * 2
yaz(fiyat)
```

```
elma -> 12
armut -> 15
kiraz -> 40
Toplam: 67
armut
elma
kiraz
{"elma": 24, "armut": 30, "kiraz": 80}
```

Girdiler eklenme sırasıyla gelir. Alfabetik sırayla gezmek için `sırala(anahtarlar(...))` kullanın.

## Örnek: kelime sayma

Sözlüğün en klasik kullanımı saymaktır. Anahtar kelime, değer kaç kez geçtiğidir.

```jus
değişken metin1 = "Bir elma bir armut ve bir elma daha. Elma güzel!"

# noktalama işaretlerini temizle, küçük harfe çevir
değişken temiz = küçük_harf(metin1)
temiz = değiştir(temiz, ".", "")
temiz = değiştir(temiz, "!", "")

değişken sayım = {}
her kelime içinde böl(temiz, " "):
    eğer kelime içinde sayım:
        sayım[kelime] += 1
    değilse:
        sayım[kelime] = 1

her kelime içinde sırala(anahtarlar(sayım)):
    yaz(kelime + ":", sayım[kelime])

# al ile daha kısa yazım
değişken sayım2 = {}
her kelime içinde böl(temiz, " "):
    sayım2[kelime] = al(sayım2, kelime, 0) + 1
yaz(sayım2 == sayım)
```

```
armut: 1
bir: 3
daha: 1
elma: 3
güzel: 1
ve: 1
doğru
```

Mantık şöyle: Kelimeyi ilk kez görüyorsak sayısı 1 olur; daha önce gördüysek sayısı bir artar. `al(sayım2, kelime, 0) + 1` bu iki durumu tek satırda yazar: kelime yoksa `0` varsayılır. Son satır iki sözlüğün eşit olduğunu doğrular; sözlükler içerikleri aynıysa eşittir.

## İç içe yapılar

Sözlüğün değeri bir liste ya da sözlük olabilir. Listenin öğesi de sözlük olabilir. Bu, gerçek dünyadaki bilgileri tarif etmek için yeterlidir.

```jus
değişken öğrenciler = [
    {"ad": "Ayşe", "not": 90},
    {"ad": "Mehmet", "not": 75},
    {"ad": "Zeynep", "not": 85},
]

her ö içinde öğrenciler:
    yaz(ö["ad"] + " -> " + metin(ö["not"]))

değişken okul = {"ad": "Atatürk Lisesi", "sınıflar": ["9A", "9B"]}
ekle(okul["sınıflar"], "10A")
yaz(okul)
yaz(okul["sınıflar"][0])
```

```
Ayşe -> 90
Mehmet -> 75
Zeynep -> 85
{"ad": "Atatürk Lisesi", "sınıflar": ["9A", "9B", "10A"]}
9A
```

## Sözlük listesini sıralamak

Sözlüklerden oluşan bir listeyi, sözlüklerin belirli bir anahtarına göre sıralamak çok yaygın bir iştir. `sırala(liste, anahtar)` bunun içindir (altıncı bölümde tanıttık): anahtar fonksiyonu her sözlüğü alır ve sıralama ölçütünü, yani bir sayıyı ya da metni döndürür. Düz `sırala(liste)` sözlükleri karşılaştıramaz; anahtar fonksiyonu şarttır.

```jus
değişken öğrenciler = [
    {"ad": "Ayşe", "not": 90},
    {"ad": "Mehmet", "not": 75},
    {"ad": "Zeynep", "not": 85},
    {"ad": "Can", "not": 75},
]

fonksiyon nota_göre(ö):
    dön ö["not"]

fonksiyon notun_eksisi(ö):
    dön -ö["not"]

fonksiyon ada_göre(ö):
    dön ö["ad"]

fonksiyon geçti_mi(ö):
    dön ö["not"] >= 80

yaz("Nota göre, küçükten büyüğe:")
her ö içinde sırala(öğrenciler, nota_göre):
    yaz(sağa_doldur(ö["ad"], 8) + sola_doldur(metin(ö["not"]), 4))

yaz("Nota göre, büyükten küçüğe:")
her ö içinde sırala(öğrenciler, notun_eksisi):
    yaz(sağa_doldur(ö["ad"], 8) + sola_doldur(metin(ö["not"]), 4))

yaz(eşle(sırala(öğrenciler, ada_göre), ada_göre))
yaz(eşle(süz(öğrenciler, geçti_mi), ada_göre))
```

```
Nota göre, küçükten büyüğe:
Mehmet    75
Can       75
Zeynep    85
Ayşe      90
Nota göre, büyükten küçüğe:
Ayşe      90
Zeynep    85
Mehmet    75
Can       75
["Ayşe", "Can", "Mehmet", "Zeynep"]
["Ayşe", "Zeynep"]
```

Gözlemler:

- Anahtar bir sayı olduğunda büyükten küçüğe sıralamak için sayının eksisini döndürmek yeterlidir. Metin anahtarlarda bu iş görmez; onlar için `ters(sırala(...))` kullanabilirsiniz.
- Mehmet ve Can'ın notu eşittir; iki sıralamada da özgün sıralarını (önce Mehmet, sonra Can) korudular.
- Metin anahtarlar Türk alfabesine göre sıralanır.
- `eşle` ile sıralı listeden yalnızca adları, `süz` ile yalnızca koşulu sağlayan kayıtları alabilirsiniz.

Aynı yöntem bir sözlüğün anahtarlarını değerlerine göre sıralamak için de kullanılır:

```jus
değişken sayım = {"elma": 3, "armut": 5, "muz": 1}

fonksiyon sayıya_göre(ad):
    dön -sayım[ad]

yaz(sırala(anahtarlar(sayım), sayıya_göre))
```

```
["armut", "elma", "muz"]
```

## Sık yapılan hatalar

**Olmayan anahtarı okumak.**

```jus
değişken kişi = {"ad": "Ayşe"}
yaz(kişi["yaş"])
```

```
ornek.jus:2: çalışma zamanı hatası: Sözlükte "yaş" anahtarı yok.
```

Çözüm: önce `"yaş" içinde kişi` ile bakın ya da `al(kişi, "yaş", varsayılan)` kullanın.

**Olmayan anahtara `+=` uygulamak.** `sayım["a"] += 1` aslında `sayım["a"] = sayım["a"] + 1`'dir; sağ taraf anahtarı okur. Önce `sayım["a"] = 0` yapın ya da kelime sayma örneğindeki gibi denetleyin.

```jus
değişken sayım = {}
sayım["a"] += 1
yaz(sayım)
```

```
ornek.jus:2: çalışma zamanı hatası: Sözlükte "a" anahtarı yok.
```

**Anahtar türünü şaşırmak.** Liste gibi türler anahtar olamaz:

```jus
değişken s = {}
s[[1, 2]] = "liste anahtarı"
yaz(s)
```

```
ornek.jus:2: çalışma zamanı hatası: Sözlük anahtarı metin, sayı ya da mantıksal olmalı; liste verildi.
```

Ayrıca sözlüğe sıra numarasıyla ulaşılmaz. `s[0]`, "0 numaralı öğe" değil `0` anahtarını arar:

```jus
değişken s = {"a": 1}
yaz(s[0])
```

```
ornek.jus:2: çalışma zamanı hatası: Sözlükte 0 anahtarı yok.
```

**Anahtar fonksiyonu vermeden sözlük sıralamak.**

```jus
yaz(sırala([{"a": 1}, {"a": 2}]))
```

```
ornek.jus:1: çalışma zamanı hatası: 'sırala' için listenin tüm öğeleri sayı ya da tüm öğeleri metin olmalı. Başka türde öğeler için ikinci argüman olarak bir anahtar fonksiyonu verin.
```

**Anahtar fonksiyonunda olmayan bir anahtarı okumak.** Sıralanan her sözlükte o anahtar bulunmalıdır; yoksa hata, anahtar fonksiyonunun içinden gelir ve çağrı zinciri bunu gösterir:

```jus
fonksiyon yaşa_göre(k):
    dön k["yaş"]

yaz(sırala([{"ad": "Ali"}], yaşa_göre))
```

```
ornek.jus:2: çalışma zamanı hatası: Sözlükte "yaş" anahtarı yok.
    satır 2, 'yaşa_göre' fonksiyonu
    satır 4, ana program
```

## Alıştırmalar

1. `{"Ayşe": 90, "Mehmet": 70, "Zeynep": 80}` sözlüğündeki notların ortalamasını yazdırın.
2. `"mississippi"` içindeki her harfin kaç kez geçtiğini sözlükle sayıp yazdırın.
3. `{"elma": "apple", "kitap": "book", "su": "water"}` sözlüğünün anahtar ve değerlerini yer değiştirin (yeni sözlükte İngilizce kelimeler anahtar olsun).
4. Bir telefon rehberinde `"Veli"` ve `"Selim"` adlarını arayın; kayıt yoksa `kayıtlı değil` yazdırın.
5. `"kedi köpek kedi kuş köpek kedi"` metninde en çok geçen kelimeyi ve sayısını bulun.
6. `[{"ad": "Ali", "yaş": 31}, {"ad": "Ece", "yaş": 24}, {"ad": "Deniz", "yaş": 45}]` listesini yaşa göre küçükten büyüğe sıralayıp her kişiyi `Ece 24` biçiminde yazdırın.

## Çözümler

**1.**

```jus
değişken notlar = {"Ayşe": 90, "Mehmet": 70, "Zeynep": 80}
değişken toplam = 0
her ad içinde notlar:
    toplam += notlar[ad]
yaz("Ortalama:", toplam / uzunluk(notlar))
```

```
Ortalama: 80
```

**2.**

```jus
değişken sıklık = {}
her harf içinde "mississippi":
    sıklık[harf] = al(sıklık, harf, 0) + 1
yaz(sıklık)
```

```
{"m": 1, "i": 4, "s": 4, "p": 2}
```

**3.**

```jus
değişken ingilizce = {"elma": "apple", "kitap": "book", "su": "water"}
değişken ters2 = {}
her anahtar içinde ingilizce:
    ters2[ingilizce[anahtar]] = anahtar
yaz(ters2)
```

```
{"apple": "elma", "book": "kitap", "water": "su"}
```

**4.**

```jus
değişken rehber = {"Ali": "555 111", "Veli": "555 222"}
her aranan içinde ["Veli", "Selim"]:
    yaz(aranan + ":", al(rehber, aranan, "kayıtlı değil"))
```

```
Veli: 555 222
Selim: kayıtlı değil
```

**5.**

```jus
değişken metin3 = "kedi köpek kedi kuş köpek kedi"
değişken sayım = {}
her k içinde böl(metin3, " "):
    sayım[k] = al(sayım, k, 0) + 1

değişken enÇok = ""
değişken enÇokSayı = 0
her k içinde anahtarlar(sayım):
    eğer sayım[k] > enÇokSayı:
        enÇok = k
        enÇokSayı = sayım[k]
yaz(enÇok, enÇokSayı)
```

```
kedi 3
```

**6.**

```jus
değişken kişiler = [{"ad": "Ali", "yaş": 31}, {"ad": "Ece", "yaş": 24}, {"ad": "Deniz", "yaş": 45}]

fonksiyon yaşa_göre(k):
    dön k["yaş"]

her k içinde sırala(kişiler, yaşa_göre):
    yaz(k["ad"], k["yaş"])
```

```
Ece 24
Ali 31
Deniz 45
```

---

Önceki: [07 - Metinler](07-metinler.md) | [İçindekiler](README.md) | Sonraki: [09 - Hatalar](09-hatalar.md)
