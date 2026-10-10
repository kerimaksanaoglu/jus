# 01 - İlk program

[İçindekiler](README.md) | Sonraki: [02 - Değişkenler ve değerler](02-degiskenler-ve-degerler.md)

Bir program, bilgisayara verilen adım adım yazılı bir talimat listesidir. Bilgisayar bu talimatları yukarıdan aşağıya, sırayla uygular. Bu bölümde en basit talimatı öğreneceksiniz: ekrana bir şey yazdırmak.

## Merhaba, dünya

Geleneksel ilk program, ekrana bir selam yazdırır. `merhaba.jus` adlı bir dosya açın ve şunu yazın:

```jus
yaz("Merhaba, dünya!")
```

Terminalde çalıştırın:

```sh
jus merhaba.jus
```

```
Merhaba, dünya!
```

Bu tek satırda üç şey var:

- `yaz` bir **fonksiyondur**. Fonksiyon, belirli bir işi yapan ve bir adı olan hazır talimattır. `yaz` fonksiyonunun işi, kendisine verilenleri ekrana yazmaktır.
- Parantezler `( )`, fonksiyonu **çağırdığınızı** gösterir. Çağırmak, fonksiyonun işini yapmasını istemek demektir.
- `"Merhaba, dünya!"` bir **metindir**. Metin, çift tırnak içine yazılan harf, rakam ve işaret dizisidir. Parantezlerin içine yazılan şeye **argüman** denir; yani fonksiyona verdiğiniz değerdir.

## Birden çok değer yazmak

`yaz` istediğiniz kadar argüman alabilir. Argümanları virgülle ayırırsınız. `yaz` bunları aralarına birer boşluk koyarak aynı satıra yazar ve satırı bitirir.

```jus
yaz("Ali")
yaz("Ben", "JUS", "öğreniyorum")
yaz(2 + 3)
yaz("Toplam:", 2 + 3)
yaz(1, "iki", doğru, boş)
```

```
Ali
Ben JUS öğreniyorum
5
Toplam: 5
1 iki doğru boş
```

Üçüncü satıra dikkat edin: `yaz(2 + 3)` ekrana `2 + 3` değil `5` yazar. Çünkü `2 + 3` bir **ifadedir**; ifade, hesaplanınca bir değer veren yazımdır. `yaz` önce ifadeyi hesaplar, sonra sonucu yazar.

Son satırda `doğru` ve `boş` göründü. Bunlar özel değerlerdir; ikinci bölümde tanışacağız.

Sayıların çevresine tırnak koymadığımıza dikkat edin. `"5"` bir metindir, `5` ise bir sayıdır. Aradaki farkı ikinci bölümde göreceksiniz.

## Metinlerde özel karakterler

Bir metin tek satırda yazılır. Metnin içine satır sonu ya da çift tırnak koymak istiyorsanız ters bölü `\` ile başlayan **kaçış dizileri** kullanırsınız:

| Yazım | Anlamı |
|-------|--------|
| `\n` | satır sonu |
| `\t` | sekme (bir tuşluk geniş boşluk) |
| `\r` | satır başı |
| `\"` | çift tırnak |
| `\\` | ters bölü |

```jus
yaz("Birinci satır\nİkinci satır")
yaz("Ad:\tAyşe")
yaz("Dedi ki: \"Merhaba\"")
yaz("Ters bölü: \\")
```

```
Birinci satır
İkinci satır
Ad:	Ayşe
Dedi ki: "Merhaba"
Ters bölü: \
```

Bu tabloda olmayan bir kaçış dizisi yazarsanız sözdizimi hatası alırsınız. Örneğin `yaz("a\xb")` şu iletiyi verir:

```
ornek.jus:1:5: sözdizimi hatası: Geçersiz kaçış dizisi. Kullanılabilenler: \n \t \r \" \\
    yaz("a\xb")
        ^
```

## Yorumlar

Programa kendiniz ya da başkaları için not yazmak isteyebilirsiniz. `#` karakterinden satır sonuna kadar olan her şey **yorumdur**. JUS yorumları tamamen yok sayar.

```jus
# Bu satır tamamen yorumdur, çalıştırılmaz.
yaz("Birinci")  # Bu da satır sonu yorumudur.
# yaz("Bu satır çalışmaz")
yaz("İkinci")
```

```
Birinci
İkinci
```

Yorumlar iki işe yarar: kodun ne yaptığını açıklarsınız ya da bir satırı silmeden geçici olarak devre dışı bırakırsınız (üçüncü satırdaki gibi).

## Programın çalışma düzeni

- Her **deyim** (programın bir talimatı) bir satıra yazılır ve satır sonunda biter. Noktalı virgül gerekmez.
- Satırlar yukarıdan aşağıya sırayla çalışır.
- Boş satırlar programı etkilemez. Okunabilirlik için bol bol kullanın.
- JUS büyük ve küçük harfi ayırır. `yaz` ile `Yaz` farklı adlardır.

## Hata iletilerini okumak

Herkes hata yapar. Önemli olan hata iletisini okuyup anlamaktır. JUS iki tür hata bildirir.

### Sözdizimi hataları

Sözdizimi, dilin yazım kurallarıdır. Kurala uymayan bir satır yazarsanız program **hiç başlamaz**; JUS önce dosyanın tamamını denetler. Hata iletisi dosya adını, satır numarasını ve sütun numarasını gösterir.

```jus
yaz("Merhaba"
yaz("Dünya")
```

```
ornek.jus:2:1: sözdizimi hatası: Argümanlardan sonra ')' bekleniyor.
    yaz("Dünya")
    ^
```

İleti şöyle okunur: `ornek.jus` dosyasının 2. satırının 1. sütununda bir sorun var; JUS argümanlardan sonra `)` bekliyordu. Altındaki satır sorunlu satırı, `^` işareti ise sorunun yerini gösterir.

Gerçek hata aslında 1. satırdadır: parantezi kapatmayı unuttuk. JUS bunu ancak 2. satıra gelince fark edebildi. Bir hata iletisindeki satır numarası sorunun bittiği yeri gösterir; bazen gerçek hata bir önceki satırdadır. Hata görürseniz hem o satıra hem bir üstüne bakın.

Çıkış kodu 65'tir.

### Çalışma zamanı hataları

Program yazım kurallarına uyuyor ama çalışırken yapamayacağı bir şey istediyse **çalışma zamanı hatası** oluşur. Bu durumda hatadan önceki satırlar çalışmıştır.

```jus
yaz("Başlıyoruz")
yaz(mesaj)
```

```
Başlıyoruz
ornek.jus:2: çalışma zamanı hatası: 'mesaj' adında bir değişken ya da fonksiyon tanımlı değil.
```

Önce `Başlıyoruz` yazıldı, sonra 2. satırda program durdu. İleti `mesaj` adında bir şey tanımlanmadığını söylüyor. Çalışma zamanı hatasının çıkış kodu 70'tir.

İki türü şöyle ayırt edebilirsiniz: Sözdizimi hatasında hiçbir çıktı görmezsiniz; çalışma zamanı hatasında hataya kadar olan çıktıyı görürsünüz.

## Sık yapılan hatalar

**Kapatılmamış parantez.** Yukarıdaki örnekte gördük. Her açılan `(` için bir `)` olmalıdır.

**Kapatılmamış tırnak.** Metnin sonunda kapanış tırnağını unutmayın:

```jus
yaz("Merhaba)
```

```
ornek.jus:1:5: sözdizimi hatası: Metin kapatılmamış; kapanış tırnağı (") eksik.
    yaz("Merhaba)
        ^
```

**Büyük harf kullanmak.** Fonksiyon adlarını tam yazdığınız gibi yazmalısınız:

```jus
Yaz("Merhaba")
```

```
ornek.jus:1: çalışma zamanı hatası: 'Yaz' adında bir değişken ya da fonksiyon tanımlı değil. 'yaz' mı demek istediniz?
```

**Girintiyi bozmak.** Kural gereği bir bloğun içindeki satırlar aynı hizada olmalıdır. Girinti konusunu üçüncü bölümde işleyeceğiz; şimdilik şu kadarını bilin: girintisiz bir satırı birdenbire girintili yazamazsınız.

## Alıştırmalar

1. `merhaba.jus` programını kendi adınızı yazdıracak şekilde değiştirin.
2. Tek bir `yaz` çağrısıyla ekrana üç satır yazdırın. (İpucu: `\n`.)
3. Bir program yazın: iki mesaj yazsın, aralarında da bir yorum satırı olsun. Bir `yaz` satırını yorum haline getirip çalıştırın; ne değişti?
4. Aşağıdaki programı çalıştırınca hata alırsınız. Hatayı bulun ve düzeltin:
   ```jus
   yaz("Merhaba"
   ```
5. `7 * 8` işleminin sonucunu `7 çarpı 8 = 56` biçiminde ekrana yazdırın. Sonucu kendiniz yazmayın, JUS'a hesaplatın.

## Çözümler

**1.**

```jus
yaz("Merhaba, ben Ayşe.")
```

```
Merhaba, ben Ayşe.
```

**2.**

```jus
yaz("Birinci satır\nİkinci satır\nÜçüncü satır")
```

```
Birinci satır
İkinci satır
Üçüncü satır
```

**3.**

```jus
# Bu program iki mesaj yazar.
yaz("Birinci mesaj")
# yaz("Bu mesaj görünmez")
yaz("İkinci mesaj")  # bu da bir yorum
```

```
Birinci mesaj
İkinci mesaj
```

Yorum haline getirilen satır çalışmaz; ekrana yazılmaz.

**4.** Hata iletisi şuydu:

```
ornek.jus:2:1: sözdizimi hatası: Argümanlardan sonra ')' bekleniyor.
    
    ^
```

Kapanış parantezi eksiktir. Düzeltilmiş hali:

```jus
yaz("Merhaba")
```

```
Merhaba
```

**5.**

```jus
yaz("7 çarpı 8 =", 7 * 8)
```

```
7 çarpı 8 = 56
```

---

[İçindekiler](README.md) | Sonraki: [02 - Değişkenler ve değerler](02-degiskenler-ve-degerler.md)
