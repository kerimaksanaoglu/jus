# Hafta 1 - İlk program: `yaz` ve hata iletisi

[Öğretmen kiti](README.md) | Sonraki: [Hafta 2 - Değişkenler](hafta-02.md)

İlgili rehber bölümü: [01 - İlk program](../rehber/01-ilk-program.md)

Bu hafta öğrenciler programın ne olduğunu bilgisayarsız bir oyunla yaşar, sonra ilk JUS programlarını yazar. Haftanın asıl hedefi `yaz` fonksiyonundan çok, hata iletisini korkmadan okumaktır. Bu alışkanlığı ilk derste kazandırırsanız sonraki haftalarda size gelen "çalışmıyor" sorularının çoğu öğrencinin kendi kendine çözebileceği sorulara dönüşür.

## Kazanımlar

Hafta sonunda öğrenci:

1. Programı "bilgisayara verilen, sırası önemli, adım adım yazılı talimat listesi" olarak kendi cümleleriyle anlatabilir ve günlük hayattan bir örnek verebilir.
2. `yaz` ile metin, sayı ve işlem sonucu yazdıran bir programı deneme alanında yazıp çalıştırabilir.
3. `#` ile yorum satırı yazabilir; bir satırı yorum haline getirerek devre dışı bırakabilir.
4. Bir hata iletisinden dosya adını, satır numarasını ve hatanın türünü (sözdizimi ya da çalışma zamanı) okuyabilir.
5. Kapatılmamış parantez, kapatılmamış tırnak ve yanlış harf büyüklüğü gibi basit hataları bulup düzeltebilir.

## Ön hazırlık

- Öğrenci başına ya da ikişer kişiye bir bilgisayar, tablet veya telefon; tarayıcıda <https://kerimaksanaoglu.github.io/jus/> adresi. Dersten önce sayfanın açıldığından ve "Çalıştır" düğmesiyle (Ctrl+Enter kısayolu da var) bir örneğin çalıştığından emin olun.
- Öğretmen ekranı yansıtılabilmeli. Tahtaya yazı yazabilmelisiniz; canlı kodlama sırasında kodu tahtaya da yazmak yavaşlatır ama öğrencilerin kendi ekranlarında aynısını yazmasına zaman tanır.
- Bilgisayarsız etkinlik için: öğrenci başına bir A4 kâğıt, kalem, ızgara çizmek için cetvel.
- Deneme alanının yazılan programı sonraki derse saklayıp saklamadığını dersten önce deneyin; saklamıyorsa öğrencilerden önemli programları deftere ya da bir metin dosyasına kopyalamalarını isteyin.
- Bu hafta girdi (`oku`) kullanılmaz; her şey tarayıcıda çalışır.

## Ders akışı

Toplam 80 dakika (2 ders saati). Ders saatleri ayrı günlere denk geliyorsa 40. dakikada bölün; "yeni kavram" ile "canlı kodlama 1" birinci ders saatine sığar.

| Süre | Evre | Ne yapılır |
|------|------|------------|
| 0-5 | Karşılama | Dersin işleyişi: her derste bir şey yazacağız, hata yapmak serbest. JUS'un anahtar kelimelerinin Türkçe olduğunu söyleyin; ilk kelime `yaz`. |
| 5-20 | Isınma (bilgisayarsız) | "Robot kardeş" etkinliği (aşağıda). |
| 20-30 | Yeni kavram | Program, talimat, sıra. Fonksiyon, parantez, argüman, metin kavramlarını `yaz("Merhaba")` üzerinde tahtada gösterin. |
| 30-40 | Canlı kodlama 1 | Adım 1-4: ilk program, sıra, çok argüman, işlem. Öğrenciler her adımı kendi ekranlarında yazar. |
| 40-50 | Canlı kodlama 2 | Adım 5-8: `\n`, tırnak, yorum, iki tür hata. Hata iletisini birlikte yüksek sesle okuyun. |
| 50-70 | Öğrenci uygulaması | Eşli çalışma: Alıştırma 1.1-1.5. Hızlı bitirenler 1.6 ve 1.7'ye geçer. |
| 70-76 | Hata avı paylaşımı | İki üç çift, bulduğu hatayı ve iletiyi sınıfa okur. |
| 76-80 | Kapanış | Çıkış bileti (3 soru). Ev çalışmasını duyurun. |

## Canlı kodlama betiği

Her adımda önce öğrencilere sorun, tahmin almadan çalıştırmayın. Kod bloklarının altındaki çıktılar gerçek çıktılardır.

### Adım 1 - İlk program

Tahtaya yazın:

```jus
yaz("Merhaba, dünya!")
```

```
Merhaba, dünya!
```

Söyleyin: "`yaz` bir fonksiyon, yani adı olan bir iş. Parantez 'bu işi şimdi yap' demek. Tırnak içindeki şey metin."

Sorun: "Tırnakları silersek ne olur? Parantezi silersek?" Tahminleri aldıktan sonra denetin; sonuçları aşağıdaki "Öğrenciler nerede takılır" bölümünde bulacaksınız.

### Adım 2 - Sıra önemlidir

```jus
yaz("Birinci")
yaz("İkinci")
yaz("Üçüncü")
```

```
Birinci
İkinci
Üçüncü
```

Sorun: "İkinci satırı en üste taşırsak çıktı nasıl değişir?" Öğrenciler satırları taşıyıp dener. Sonuç: bilgisayar satırları yukarıdan aşağıya, sırayla uygular; robot etkinliğiyle aynı fikir.

### Adım 3 - Birden çok değer, sayı ve işlem

```jus
yaz("Ben", "JUS", "öğreniyorum")
yaz(2 + 3)
yaz("2 + 3")
yaz("Toplam:", 2 + 3)
```

```
Ben JUS öğreniyorum
5
2 + 3
Toplam: 5
```

Sorun: "Üçüncü satır ekrana ne yazar? İkinci satırdan farkı ne?" Beklenen yanıt: tırnak içindeki her şey olduğu gibi yazılır; tırnaksız `2 + 3` önce hesaplanır. Argümanların arasına boşluk koyulduğuna dikkat çektirin.

### Adım 4 - Dört işlem

```jus
yaz(7 + 3)
yaz(7 - 3)
yaz(7 * 3)
yaz(15 / 4)
yaz(2 + 3 * 4)
yaz((2 + 3) * 4)
```

```
10
4
21
3.75
14
20
```

Söyleyin: "Çarpma işareti `*`, bölme işareti `/`. Çarpma ve bölme önce yapılır; sırayı parantezle değiştiririz. Okulda gördüğünüz işlem önceliğiyle aynı." `15 / 4` sonucu `3.75` çıkar; ondalık ayırıcı virgül değil noktadır.

### Adım 5 - Satır sonu ve tırnak

Metnin içine satır sonu ve çift tırnak koymak için ters bölü kullanılır.

```jus
yaz("Birinci satır\nİkinci satır")
yaz("Dedi ki: \"Merhaba\"")
yaz("Yol: C:\\okul")
```

```
Birinci satır
İkinci satır
Dedi ki: "Merhaba"
Yol: C:\okul
```

Söyleyin: "`\n` iki karakter gibi görünür ama bilgisayar için tek bir 'yeni satıra geç' işaretidir. `\"` metni bitirmeden tırnak yazdırır. Ters bölünün kendisini yazdırmak için ikisini yan yana yazarız."

### Adım 6 - Yorum

```jus
# Bu satır yalnızca bir nottur.
yaz("Birinci")  # bu da satır sonu notu
# yaz("Bu satır çalışmaz")
yaz("İkinci")
```

```
Birinci
İkinci
```

Sorun: "Üçüncü satırı silmeden çalışmaz hale getirdik. Bunu ne zaman yapmak işe yarar?" Beklenen yanıt: bir şeyi denemek, hatalı satırı geçici olarak kapatmak.

### Adım 7 - İlk hata: sözdizimi hatası

Bilerek parantezi kapatmayın. Öğrencilere önce hatanın kaçıncı satırda olduğunu sorun.

```jus
yaz("Merhaba"
yaz("Dünya")
```

```
ornek.jus:2:1: sözdizimi hatası: Argümanlardan sonra ')' bekleniyor.
    yaz("Dünya")
    ^
```

Birlikte okuyun: "`ornek.jus` dosyası, 2. satır, 1. sütun. Sözdizimi hatası. JUS argümanlardan sonra `)` bekliyordu."

Sorun: "Hatayı yapan gerçekten 2. satır mı?" Hayır; unutulan parantez 1. satırdadır. JUS eksikliği ancak ikinci satıra gelince fark eder. Kural: iletideki satıra ve bir üstüne bakın. Ayrıca ekranda hiçbir şey yazılmadığına dikkat çektirin: **sözdizimi hatasında program hiç başlamaz.**

### Adım 8 - İkinci hata: çalışma zamanı hatası

```jus
yaz("Başlıyoruz")
yaz(mesaj)
yaz("Bitti")
```

```
Başlıyoruz
ornek.jus:2: çalışma zamanı hatası: 'mesaj' adında bir değişken ya da fonksiyon tanımlı değil.
```

Sorun: "Bu sefer `Başlıyoruz` yazıldı. Neden? `Bitti` neden yazılmadı?" Beklenen yanıt: yazım kuralları doğruydu, program başladı; ikinci satırda `mesaj` adında bir şey bulamayınca durdu. **Çalışma zamanı hatasında hataya kadar olan çıktıyı görürsünüz.**

Tahtaya iki sütunlu bir tablo çizin ve öğrencilerle doldurun:

| | Sözdizimi hatası | Çalışma zamanı hatası |
|---|---|---|
| Program başladı mı? | Hayır | Evet |
| Ekranda çıktı var mı? | Yok | Hataya kadar olanlar |
| İletide sütun numarası | Var (`2:1`) | Yok (yalnızca satır) |

## Sınıf içi etkinlik: Robot kardeş (bilgisayarsız, 15 dk)

Amaç: Talimatın kesin, sıralı ve eksiksiz olması gerektiğini yaşayarak görmek; "hata ayıklama" fikrini tanıtmak.

Hazırlık: Her çift bir A4 kâğıda 6x6 kareli bir ızgara çizer. Sol alt köşe "başlangıç"tır. Bir öğrenci **programcı**, diğeri **robot** olur.

Robotun anlayabildiği tek komutlar:

- `ileri N` (N kare ileri git; çizgi çizer)
- `sağa dön`
- `sola dön`
- `kalemi kaldır` / `kalemi indir`

Adımlar:

1. Programcı, ızgaraya robotun çizmesini istediği basit bir şekli (L harfi, kare, basamak) gizlice çizer.
2. Programcı yalnızca yukarıdaki komutları kullanarak numaralı bir talimat listesi yazar. Şekli robota göstermez, sözle de anlatmaz.
3. Robot, listeyi **harfi harfine** uygular. Yorum yapmaz, eksik yeri tahmin etmez. Başlangıçta robot sağa bakar.
4. Karşılaştırın: çizilen şekil istenene benziyor mu? Benzemiyorsa talimat listesinde hata var: hangi numaralı satırda?
5. Roller değişir.

Tartışma soruları (3 dk):

- Talimatların sırasını değiştirirsek ne olur? (Bkz. canlı kodlama Adım 2.)
- Robot "bir şey eksik" dediğinde programı hangi satıra kadar uyguladı? (Sözdizimi hatasına karşı çalışma zamanı hatası: hata öncesinde yapılanlar kâğıtta kalır.)
- Bilgisayar neden "tahmin etmiyor"?

Gözlem: Başlangıçta öğrenciler "bir kare çiz" gibi yüksek düzeyli komutlar yazmak ister. Sorun değil; sonra "robotun bildiği komutlarla" yazmaları gerekir. Bu, programlamanın temel işidir: büyük işi, makinenin bildiği küçük adımlara bölmek.

## Alıştırmalar

Alıştırmaların çözümleri [cevap anahtarında](cevap-anahtari.md#hafta-1) vardır. Öğrenciler yazarken tırnak ve parantezleri tek tek kontrol etmelidir. Aşağıdaki çıktılar, alıştırmaların cevap anahtarındaki çözümleriyle alınmıştır.

### Kolay

**Alıştırma 1.1.** Ekrana adınızı içeren bir selam yazdırın: `Merhaba, ben Ayşe.` (Kendi adınızı yazın.)

Beklenen çıktı:

```
Merhaba, ben Ayşe.
```

**Alıştırma 1.2.** Tek bir `yaz` çağrısıyla ekrana üç satır yazdırın: `Birinci satır`, `İkinci satır`, `Üçüncü satır`.

Beklenen çıktı:

```
Birinci satır
İkinci satır
Üçüncü satır
```

**Alıştırma 1.3.** `7 * 8` işleminin sonucunu `7 çarpı 8 = 56` biçiminde yazdırın. Sonucu kendiniz yazmayın, JUS'a hesaplatın.

Beklenen çıktı:

```
7 çarpı 8 = 56
```

### Orta

**Alıştırma 1.4.** Aşağıdaki programı **çalıştırmadan önce** çıktısını tahmin edin, sonra çalıştırıp karşılaştırın.

```jus
yaz("A")
# yaz("B")
yaz("C")  # yaz("D")
yaz("E", "F")
```

```
A
C
E F
```

Tahmin doğruysa kural anlaşılmıştır: `#` işaretinden satır sonuna kadar her şey yok sayılır. Yanılanlar için `yaz("C")  # yaz("D")` satırına bakın: `D` neden yazılmadı?

**Alıştırma 1.5.** Bir pazarda 3 simit 7.5 TL'den, 2 ayran 12 TL'den alındı. Toplam tutarı `Toplam: 46.5` biçiminde yazdırın. Tutarı kendiniz hesaplamayın; ifadeyi `yaz` içine yazın.

Beklenen çıktı:

```
Toplam: 46.5
```

### Zor

**Alıştırma 1.6.** Sekiz `yaz` satırıyla aşağıdaki küçük evi çizin. (İpucu: ters bölü karakterini yazdırmak için `\\` gerekir.)

Beklenen çıktı:

```
   /\
  /  \
 /    \
/______\
|      |
|  []  |
|      |
|______|
```

**Alıştırma 1.7 - Hata avı.** Aşağıdaki üç program ayrı ayrı çalıştırılınca hata verir. Her biri için: hata sözdizimi mi çalışma zamanı mı, kaçıncı satırda bildiriliyor, asıl hata neydi, nasıl düzeltilir? Programları ayrı dosyalarda (ya da ayrı denemelerde) çalıştırın.

```jus
yaz("Merhaba)
yaz("Dünya")
```

```
ornek.jus:1:5: sözdizimi hatası: Metin kapatılmamış; kapanış tırnağı (") eksik.
    yaz("Merhaba)
        ^
```

```jus
yaz("Toplam:", (2 + 3) * 4
yaz("Bitti")
```

```
ornek.jus:2:1: sözdizimi hatası: Argümanlardan sonra ')' bekleniyor.
    yaz("Bitti")
    ^
```

```jus
yaz("Hesap başlıyor")
Yaz(5 * 5)
```

```
Hesap başlıyor
ornek.jus:2: çalışma zamanı hatası: 'Yaz' adında bir değişken ya da fonksiyon tanımlı değil. 'yaz' mı demek istediniz?
```

Düzeltilmiş hâllerin çıktıları cevap anahtarındadır. Yukarıdaki iletiler öğretmen içindir; öğrencilere programları ileti olmadan verin, iletiyi kendileri üretsin.

## Öğrenciler nerede takılır

Aşağıdaki iletiler gerçek iletilerdir; dosya adı `ornek.jus` olarak sadeleştirilmiştir. Öğrencinin ekranında kendi dosya adı ya da deneme alanındaki ad görünür.

### 1. Parantezi kapatmayı unutmak

```jus
yaz("Merhaba"
yaz("Dünya")
```

```
ornek.jus:2:1: sözdizimi hatası: Argümanlardan sonra ')' bekleniyor.
    yaz("Dünya")
    ^
```

Öğrenci "ikinci satırda ne hata var, doğru yazmışım" der. Yönlendirme: "İleti ne bekliyordu? `)`. Hangi parantezin kapatılmadığını bulalım. Hata satırından başlayıp yukarı doğru bakalım." Hatayı kendisi bulana kadar çözümü söylemeyin.

### 2. Tırnağı kapatmayı unutmak

```jus
yaz("Merhaba)
yaz("Dünya")
```

```
ornek.jus:1:5: sözdizimi hatası: Metin kapatılmamış; kapanış tırnağı (") eksik.
    yaz("Merhaba)
        ^
```

Burada iletiyi çok açık bulacaksınız: "kapanış tırnağı eksik". `^` işareti metnin nerede başladığını gösterir. Yönlendirme: "`^` işaretinin altındaki karaktere bakın. Metin orada mı başlıyor? Nerede bitmeli?" Öğrencilerin çoğu sağdaki parantezin tırnak yerine geçtiğini sanır.

### 3. Büyük harf kullanmak

```jus
yaz("Başlıyor")
Yaz("Merhaba")
```

```
Başlıyor
ornek.jus:2: çalışma zamanı hatası: 'Yaz' adında bir değişken ya da fonksiyon tanımlı değil. 'yaz' mı demek istediniz?
```

JUS büyük ve küçük harfi ayırır. İlk satırın çalıştığına, hatanın çalışma zamanı hatası olduğuna dikkat çektirin. Yönlendirme: "İleti hangi adı bulamadı? `Yaz`. Bizim bildiğimiz ad ne? `yaz`." Telefon ve tabletlerde klavye cümle başında otomatik büyük harf yapabilir; ayarını kapattırın.

### 4. Başka bir dilden alışkanlık: `print`

```jus
print("Merhaba")
```

```
ornek.jus:1: çalışma zamanı hatası: 'print' adında bir değişken ya da fonksiyon tanımlı değil. JUS'ta bunun karşılığı 'yaz'.
```

İleti `print`'in "tanımlı olmadığını" söyler. JUS'ta yazdırmanın adı `yaz`'dır. Başka bir dil bilen öğrenciler için bu ilk tuzaktır: yaklaşım aynı, kelime farklı.

### 5. Kopyalanan "akıllı" tırnaklar

Metin işlemcisinden ya da bir web sayfasından kopyalanan kod, düz tırnak (`"`) yerine kıvrık tırnak (`“ ”`) içerebilir.

```jus
yaz(“Merhaba”)
```

```
ornek.jus:1: çalışma zamanı hatası: '“Merhaba”' adında bir değişken ya da fonksiyon tanımlı değil.
```

İleti yanıltıcıdır: öğrenci tırnağı doğru yazdığını düşünür ama JUS tırnak işaretini görmez; `“Merhaba”` bütününü bir ad sayar. İpucu: iletideki ad tırnaklarıyla birlikte yazılıdır. Yönlendirme: "İletide ne ad olarak geçiyor? Tırnaklar sizin yazdığınızdan farklı mı görünüyor?" Çözüm: tırnakları klavyeden yeniden yazdırın.

## Çıkış bileti

Kâğıda yazdırın (3 dk).

1. `yaz(4 + 5)` ekrana ne yazar? `yaz("4 + 5")` ne yazar?
2. Bir programda sözdizimi hatası varsa, hatadan önceki satırlar çalışır mı?
3. Şu ileti ne anlatıyor? `ornek.jus:2: çalışma zamanı hatası: 'mesaj' adında bir değişken ya da fonksiyon tanımlı değil.`

Cevaplar:

1. `9` ve `4 + 5`. (İlkinde ifade hesaplanır, ikincisinde tırnak içindeki metin olduğu gibi yazılır.)
2. Hayır. JUS önce dosyanın tamamını denetler; sözdizimi hatası varsa hiçbir satır çalışmaz.
3. `ornek.jus` dosyasının 2. satırında, program çalışırken `mesaj` adında bir şey bulunamadı. Program o satırda durdu, ondan önceki satırlar çalıştı.

## Ev çalışması (isteğe bağlı)

1. Kendinizi tanıtan beş satırlık bir program yazın (ad, sınıf, sevdiğiniz bir şey, bir sayı, bir söz). Deftere ya da dosyaya kaydedin; bir sonraki derste bir arkadaşınıza gösterin.
2. Aynı programa bilerek bir hata ekleyin (parantezi silin). Çıkan iletiyi deftere yazın ve hatanın satırını gösterin.
3. Gün içinde yaptığınız beş adımlık bir işi (ders çantası hazırlama gibi) robot komutları gibi sıralayıp yazın. Hangi adımı atlarsanız işin bozulacağını işaretleyin.
