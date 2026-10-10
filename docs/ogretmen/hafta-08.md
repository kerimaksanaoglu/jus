# Hafta 8 - Sözlükler

[Öğretmen kiti](README.md) | Önceki: [Hafta 7](hafta-07.md) | Sonraki: [Hafta 9 - Hata yakalama ve oyun](hafta-09.md)

İlgili rehber bölümü: [08 - Sözlükler](../rehber/08-sozlukler.md)

Liste öğeleri numarayla bulur; sözlük **anahtarla** bulur. Rehberdeki ya da telefondaki kişi listesini düşünün: "3. kayıt" yerine "Ayşe" deriz. Öğrenciler bu haftaya listeleri iyi bilerek geldiyse sözlükler hızlı öğrenilir. Haftanın pratik kazancı: saymak. Kelime sayma, harf sayma ve anket sonuçları sözlükle yazılınca çok kısalır.

## Kazanımlar

Hafta sonunda öğrenci:

1. Sözlük oluşturabilir; `s["anahtar"]` ile değer okuyup değiştirebilir, yeni girdi ekleyebilir.
2. `anahtar içinde sözlük` ve `al(sözlük, anahtar, varsayılan)` ile anahtarın varlığını hata almadan denetleyebilir.
3. `her anahtar içinde sözlük` ile sözlüğü gezebilir; `anahtarlar`, `değerler`, `uzunluk` ve `sil` kullanabilir.
4. Sözlükle saymayı (harf ve kelime sıklığı) uygulayabilir.
5. Sözlüklerden oluşan bir listeyi `sırala(liste, anahtar)` ile istenen alana göre sıralayabilir.

## Ön hazırlık

- Tarayıcıda deneme alanı: <https://kerimaksanaoglu.github.io/jus/>.
- Etkinlik için: her öğrenciye iki parçalı bir kart (üstte anahtar, altta değer; ör. ad ve en sevdiği ders). Tahtada bir anket tablosu için boş alan.
- Sınav 2'ye (Hafta 9'dan sonra) iki hafta kaldığını duyurun.

## Ders akışı

| Süre | Evre | Ne yapılır |
|------|------|------------|
| 0-6 | Isınma | Hafta 7'den: `böl` ile bir cümleyi kelimelere ayırıp sayın. "Hangi kelime kaç kez geçiyor?" sorusunu açık bırakın. |
| 6-12 | Yeni kavram | Sözlük = anahtar-değer çiftleri. Liste: numara -> değer. Sözlük: anahtar -> değer. |
| 12-26 | Canlı kodlama 1 | Adım 1-3: oluşturma, okuma, ekleme, denetleme, gezme. |
| 26-38 | Etkinlik | Sınıf anketi ve anahtar-değer kartları (bilgisayarsız). |
| 38-50 | Canlı kodlama 2 | Adım 4-6: sayma, iç içe yapı, sözlük listesini sıralama. |
| 50-72 | Öğrenci uygulaması | Alıştırma 8.1-8.6, eşli. Hızlılar 8.7-8.9'a geçer. |
| 72-76 | Paylaşım | Bir çift 8.9'daki anagram fikrini anlatır. |
| 76-80 | Kapanış | Çıkış bileti. |

## Canlı kodlama betiği

### Adım 1 - Sözlük oluşturmak ve okumak

```jus
değişken kişi = {"ad": "Ayşe", "yaş": 13}
yaz(kişi["ad"])
yaz(kişi["yaş"])
kişi["yaş"] = 14
kişi["şehir"] = "Ankara"
yaz(kişi)
yaz(uzunluk(kişi))
```

```
Ayşe
13
{"ad": "Ayşe", "yaş": 14, "şehir": "Ankara"}
3
```

Söyleyin: "Süslü parantez içinde `anahtar: değer` çiftleri. Okurken köşeli parantez içine **anahtarı** yazarız. Var olan anahtara atarsak değer değişir, olmayan anahtara atarsak yeni girdi eklenir."

### Adım 2 - Anahtar var mı?

```jus
değişken kişi = {"ad": "Ayşe", "yaş": 13}
yaz("yaş" içinde kişi)
yaz("şehir" içinde kişi)
yaz(al(kişi, "şehir", "bilinmiyor"))
yaz(al(kişi, "ad", "bilinmiyor"))
```

```
doğru
yanlış
bilinmiyor
Ayşe
```

Söyleyin: "Olmayan bir anahtarı doğrudan okumak hatadır (aşağıda göreceğiz). Önce `içinde` ile sorabilir ya da `al` ile 'yoksa şunu ver' diyebiliriz."

### Adım 3 - Sözlüğü gezmek

```jus
değişken notlar = {"Ayşe": 90, "Mehmet": 70, "Zeynep": 80}

her ad içinde notlar:
    yaz(ad, "->", notlar[ad])

yaz(anahtarlar(notlar))
yaz(değerler(notlar))

değişken toplam = 0
her n içinde değerler(notlar):
    toplam += n
yaz("Ortalama:", toplam / uzunluk(notlar))
```

```
Ayşe -> 90
Mehmet -> 70
Zeynep -> 80
["Ayşe", "Mehmet", "Zeynep"]
[90, 70, 80]
Ortalama: 80
```

Söyleyin: "`her x içinde sözlük` **anahtarları** gezer. Değere ulaşmak için `sözlük[x]` yazarız. Yalnızca değerlerle ilgileniyorsak `değerler(sözlük)` bir liste verir." Sözlükler girdileri eklenme sırasıyla tutar.

### Adım 4 - Saymak

```jus
değişken cümle = "kedi köpek kedi kuş köpek kedi"
değişken sayım = {}

her kelime içinde böl(cümle, " "):
    eğer kelime içinde sayım:
        sayım[kelime] += 1
    değilse:
        sayım[kelime] = 1

yaz(sayım)
```

```
{"kedi": 3, "köpek": 2, "kuş": 1}
```

Söyleyin: "İlk kez gördüğümüz kelime için sayısı 1; daha önce gördüysek bir artır." Ardından `al` ile kısaltın:

```jus
değişken sayım = {}
her kelime içinde böl("kedi köpek kedi kuş köpek kedi", " "):
    sayım[kelime] = al(sayım, kelime, 0) + 1
yaz(sayım)
```

```
{"kedi": 3, "köpek": 2, "kuş": 1}
```

Sorun: "`al(sayım, kelime, 0) + 1` ne yapıyor, kelime yokken?" (0 verir, 1 ekler.)

### Adım 5 - İç içe yapı

```jus
değişken öğrenciler = [
    {"ad": "Ayşe", "not": 90},
    {"ad": "Mehmet", "not": 75},
    {"ad": "Zeynep", "not": 85},
]

her ö içinde öğrenciler:
    yaz(ö["ad"] + " -> " + metin(ö["not"]))

yaz(öğrenciler[1]["ad"])
```

```
Ayşe -> 90
Mehmet -> 75
Zeynep -> 85
Mehmet
```

Söyleyin: "Bir listenin öğeleri sözlük olabilir. Liste bize 'kaçıncı öğrenci', sözlük 'o öğrencinin hangi bilgisi' sorusunu cevaplar." Uzun listeler ve sözlükler köşeli ve süslü parantezin içinde birden çok satıra bölünebilir; son öğeden sonra virgül koymak serbesttir.

### Adım 6 - Sözlük listesini sıralamak

```jus
değişken öğrenciler = [
    {"ad": "Ayşe", "not": 90},
    {"ad": "Mehmet", "not": 75},
    {"ad": "Zeynep", "not": 85},
    {"ad": "Can", "not": 75},
]

fonksiyon notunaGöre(ö):
    dön ö["not"]

fonksiyon notunEksisi(ö):
    dön -ö["not"]

yaz("Küçükten büyüğe:")
her ö içinde sırala(öğrenciler, notunaGöre):
    yaz(sağa_doldur(ö["ad"], 8) + sola_doldur(metin(ö["not"]), 4))

yaz("Büyükten küçüğe:")
her ö içinde sırala(öğrenciler, notunEksisi):
    yaz(sağa_doldur(ö["ad"], 8) + sola_doldur(metin(ö["not"]), 4))
```

```
Küçükten büyüğe:
Mehmet    75
Can       75
Zeynep    85
Ayşe      90
Büyükten küçüğe:
Ayşe      90
Zeynep    85
Mehmet    75
Can       75
```

Söyleyin: "`sırala` ikinci bir bilgi alabilir: her öğe için 'neye göre sıralayalım' diyen bir fonksiyon. Bu fonksiyonu **parantezsiz** veriyoruz (Hafta 6, Adım 8). Büyükten küçüğe için sayının eksisini döndürdük. Notu eşit olanlar (Mehmet ve Can) kendi sıralarını korudu."

## Sınıf içi etkinlik: Anket ve anahtar-değer kartları (bilgisayarsız, 12 dk)

Amaç: Sözlüğü "etiketli veri" olarak görmek; saymanın sözlük olmadan ne kadar zahmetli olduğunu yaşamak; olmayan anahtarın ne demek olduğunu öğrenmek.

**Bölüm 1 - Anket (6 dk).** Sınıfa "En sevdiğiniz ders hangisi?" diye sorun (en fazla 5-6 seçenek). Her öğrenci elini kaldırır. Bir öğrenci tahtada çetele tutar. Sonra çeteleyi şu hâle getirin:

| Anahtar (ders) | Değer (kaç kişi) |
|----------------|------------------|
| Matematik | 7 |
| Fen | 5 |
| ... | ... |

Sorun: "Bu tabloyu JUS'ta yazsak hangi türü kullanırdık?" Öğrenciler "sözlük" diyecektir; anahtar dersin adı, değer sayıdır. Tahtadaki tabloyu `{"Matematik": 7, "Fen": 5}` biçiminde yazdırın.

**Bölüm 2 - Anahtar-değer kartları (6 dk).** Her öğrenciye bir kart: üstte kendi adı (anahtar), altta en sevdiği yemek (değer). Öğrenciler sıra olur; sınıf bir "sözlük"tür.

- Bir öğrenci "kart[Ali]" der; Ali değeri söyler (okuma).
- "kart[Ece] = pilav" der; Ece değerini değiştirir (atama).
- Defterde olmayan bir ad söylenir: "kart[Selim]". Kimse cevap veremez: **olmayan anahtar**. Bilgisayar ne yapmalı? (Hata verir.) Önlem olarak "Selim var mı?" diye önce sormak (`içinde`) ya da "yoksa 'bilmiyorum' de" (`al`) fikrini bulun.
- Aynı ad iki öğrencide varsa? (Anahtar benzersizdir; ikinci atama birincinin üzerine yazar.)

## Alıştırmalar

Çözümler [cevap anahtarında](cevap-anahtari.md#hafta-8).

### Kolay

**Alıştırma 8.1.** `ad`, `yaş` ve `şehir` anahtarlı bir `kişi` sözlüğü yapın (ör. Deniz, 13, Bursa). Yaşı bir artırın, `sınıf` anahtarıyla `"7-A"` ekleyin, sözlüğü yazdırın.

Beklenen çıktı:

```
{"ad": "Deniz", "yaş": 14, "şehir": "Bursa", "sınıf": "7-A"}
```

**Alıştırma 8.2.** `{"Ayşe": 90, "Mehmet": 70, "Zeynep": 80}` sözlüğündeki notların ortalamasını yazdırın.

Beklenen çıktı:

```
Ortalama: 80
```

**Alıştırma 8.3.** Bir telefon rehberi sözlüğü yapın (`"Ayşe"` ve `"Veli"` kayıtlı olsun). `"Veli"` ve `"Selim"` adlarını arayın; kayıt yoksa `kayıtlı değil` yazdırın.

Beklenen çıktı:

```
Veli: 555 444 55 66
Selim: kayıtlı değil
```

### Orta

**Alıştırma 8.4.** `"mississippi"` kelimesindeki her harfin kaç kez geçtiğini sözlükle sayın. Sonucu harfler alfabetik sırada olacak biçimde yazdırın.

Beklenen çıktı:

```
i: 4
m: 1
p: 2
s: 4
```

**Alıştırma 8.5.** `{"elma": "apple", "kitap": "book", "su": "water"}` sözlüğünün anahtar ve değerlerini yer değiştirin (İngilizce kelimeler anahtar olsun).

Beklenen çıktı:

```
{"apple": "elma", "book": "kitap", "water": "su"}
```

**Alıştırma 8.6.** `"kedi köpek kedi kuş köpek kedi"` metninde en çok geçen kelimeyi ve kaç kez geçtiğini bulun.

Beklenen çıktı:

```
En çok geçen: kedi sayısı: 3
```

### Zor

**Alıştırma 8.7.** `[{"ad": "Ali", "yaş": 31}, {"ad": "Ece", "yaş": 24}, {"ad": "Deniz", "yaş": 45}]` listesini yaşa göre küçükten büyüğe sıralayıp her kişiyi `Ece 24` biçiminde yazdırın.

Beklenen çıktı:

```
Ece 24
Ali 31
Deniz 45
```

**Alıştırma 8.8 - Sepet.** Fiyatlar bir sözlükte (`ekmek` 7.5, `süt` 32, `peynir` 125.75), sepet ise `ürün` ve `adet` anahtarlı sözlüklerden oluşan bir listede tutulsun (2 ekmek, 1 süt, 2 peynir). Toplam tutarı iki ondalık basamakla yazdırın.

Beklenen çıktı:

```
Toplam: 298.50 TL
```

**Alıştırma 8.9 - Anagram.** Aynı harflerin farklı sırayla yazılmasıyla oluşan kelimelere anagram denir. İki kelimenin harf sayımlarını iki sözlükte çıkarıp sözlükleri karşılaştırarak `anagramMı(a, b)` fonksiyonunu yazın. `"kalem"`/`"melak"`, `"kitap"`/`"patik"` ve `"ev"`/`"el"` çiftlerini deneyin. (İpucu: JUS'ta içeriği aynı olan iki sözlük `==` ile eşit sayılır.)

Beklenen çıktı:

```
doğru
doğru
yanlış
```

## Öğrenciler nerede takılır

### 1. Olmayan anahtarı okumak

```jus
değişken kişi = {"ad": "Ayşe"}
yaz(kişi["yaş"])
```

```
ornek.jus:2: çalışma zamanı hatası: Sözlükte "yaş" anahtarı yok.
```

İleti hangi anahtarın bulunmadığını söyler. Yönlendirme: "Sözlükte hangi anahtarlar var? Yazdığın anahtar bunlardan biri mi? Büyük-küçük harf ve Türkçe karakter de dahil." Çözümler: önce `"yaş" içinde kişi` ile sormak ya da `al(kişi, "yaş", 0)`.

### 2. Olmayan anahtara `+=` uygulamak

```jus
değişken sayım = {}
sayım["a"] += 1
yaz(sayım)
```

```
ornek.jus:2: çalışma zamanı hatası: Sözlükte "a" anahtarı yok.
```

`sayım["a"] += 1`, `sayım["a"] = sayım["a"] + 1` demektir; sağ taraftaki okuma anahtar yokken hata verir. Önce `sayım["a"] = 0` atamak, ya da `sayım["a"] = al(sayım, "a", 0) + 1` yazmak gerekir.

### 3. Anahtar türünü şaşırmak ya da sözlüğe sıra numarasıyla ulaşmaya çalışmak

```jus
değişken s = {"a": 1}
yaz(s[0])
```

```
ornek.jus:2: çalışma zamanı hatası: Sözlükte 0 anahtarı yok.
```

Listelerden alışılan `[0]` yazımı sözlükte "0 numaralı öğe" değil, `0` anahtarıdır. Benzer olarak `1` sayısı ile `"1"` metni farklı anahtarlardır. Yönlendirme: "Bu liste mi sözlük mü? Sözlükte öğeleri numarayla mı buluruz?"

### 4. Sözlük gezerken değeri beklemek

```jus
değişken notlar = {"Ayşe": 90, "Mehmet": 70}
değişken toplam = 0
her n içinde notlar:
    toplam += n
yaz(toplam)
```

```
ornek.jus:4: çalışma zamanı hatası: '+' işleci iki sayı, iki metin ya da iki liste ister; sayı ve metin verildi. Dönüştürmek için metin() ya da sayı() kullanılabilir.
```

`her n içinde notlar` anahtarları verir; `n` burada `"Ayşe"` ve `"Mehmet"`'tir. İleti sayı ile metnin toplanamayacağını söyler. Çözüm: `toplam += notlar[n]` ya da `her n içinde değerler(notlar)`.

### 5. Sözlük listesini anahtar fonksiyonu olmadan sıralamak

```jus
yaz(sırala([{"a": 2}, {"a": 1}]))
```

```
ornek.jus:1: çalışma zamanı hatası: 'sırala' için listenin tüm öğeleri sayı ya da tüm öğeleri metin olmalı. Başka türde öğeler için ikinci argüman olarak bir anahtar fonksiyonu verin.
```

`sırala` sözlükleri kendiliğinden karşılaştıramaz; hangi alana göre sıralanacağını söyleyen bir fonksiyon gerekir: `sırala(liste, alanaGöre)`. İleti çözümü içerir.

## Çıkış bileti

1. `k = {"a": 1}` ve `k["b"] = 2` yazıldıktan sonra `uzunluk(k)` kaçtır?
2. `k["z"]` yazarsak ne olur? Hata almadan okumanın bir yolu nedir?
3. `her x içinde sözlük:` döngüsünde `x` anahtar mı, değer mi?

Soru 1 ve 2 için program:

```jus
değişken k = {"a": 1}
k["b"] = 2
yaz(uzunluk(k))
yaz(al(k, "z", 0))
```

```
2
0
```

Cevaplar:

1. `2`.
2. Olmayan anahtar hatası olur ve program durur. `al(k, "z", varsayılan)` ya da önce `"z" içinde k` ile sormak hata vermez.
3. Anahtar.

## Ev çalışması (isteğe bağlı)

1. Sınıfta yaptığınız anketi bir sözlük olarak yazın; en çok oy alan dersi bulan bir program yazın.
2. İngilizce-Türkçe en az on kelimelik küçük bir sözlük yapın. Bir İngilizce kelime verildiğinde Türkçesini yazan, kelime yoksa `bilinmiyor` diyen bir program yazın.
3. Sevdiğiniz bir şarkı sözünde ya da paragrafta en çok geçen üç kelimeyi bulun. (İpucu: kelime sayımını yaptıktan sonra `sırala(anahtarlar(sayım), anahtarFonksiyonu)`.)
