# JUS Öğretmen Kiti

Bu kit, programlamaya ilk kez başlayan ortaokul ve lise öğrencilerine JUS ile programlama öğretecek bir öğretmenin doğrudan kullanabileceği 10 haftalık bir ders planıdır. Her hafta için dakika dakika ders akışı, tahtada adım adım yazılacak canlı kodlama betiği, bilgisayarsız bir etkinlik, kolaydan zora alıştırmalar, öğrencilerin takıldığı noktalar, çıkış bileti ve ev çalışması vardır. Kitin geri kalanı cevap anahtarını, dönem projesini ve iki sınav örneğini içerir.

JUS, anahtar kelimeleri Türkçe olan bir programlama dilidir (`eğer`, `iken`, `fonksiyon`, `yaz` gibi). Dilin tam ve bağlayıcı tanımı [dil tanımında](../dil-tanimi.md), öğrenciye dönük başlangıç rehberi [rehberde](../rehber/README.md) bulunur. Bu kit rehberin ilk dokuz bölümünü (ve on ikinci bölümündeki sayı tahmin oyununu) haftalara böler; öğrenciler rehberi ders dışında kaynak olarak kullanabilir.

## Kimler için?

- **Hedef kitle:** Programlamaya ilk kez başlayan 11-17 yaş öğrencileri. Daha önce kod yazmış öğrenciler ilk iki haftayı hızla geçebilir; bu öğrenciler için her haftanın "zor" alıştırmaları ve proje seçenekleri vardır.
- **Öğretmen:** Programlamayı önceden bilmeniz şart değildir ama kitteki her canlı kodlama adımını dersten önce kendi ekranınızda bir kez çalıştırmanızı öneririz. Bu, hem akışa alışmanızı hem de deneme alanının sizin cihazınızda çalıştığını görmenizi sağlar.

## Süre varsayımı ve uyarlama

Kit, **10 hafta, haftada 2 ders saati** (bir ders saati 40 dakika, haftada toplam 80 dakika) için yazılmıştır. Ders saatleri aynı gün art arda geliyorsa hafta planı doğrudan uygulanır; ayrı günlere denk geliyorsa her hafta dosyasındaki ders akışı tablosunda 40. dakikayı bölme noktası olarak kullanın.

| Durum | Öneri |
|-------|-------|
| Haftada 1 ders saati | Her hafta dosyasını iki haftaya bölün: birinci hafta ısınma, yeni kavram, canlı kodlama 1 ve bilgisayarsız etkinlik; ikinci hafta canlı kodlama 2, öğrenci uygulaması ve kapanış. Dönem 20 hafta sürer. |
| 6-8 hafta | Hafta 7'yi (metin) ve Hafta 8'i (sözlük) birleştirin: yalnızca `böl`, `birleştir`, `büyük_harf` ve sözlükle saymayı alın. Hafta 9'da yalnızca birinci ders saati (hata yakalama) kalsın, oyunu proje olarak verin. Dönem projesi yerine Proje 1, 2 ya da 8'in asgari sürümünü tek ders saatinde yaptırın. |
| 14 hafta | Hafta 3-9 konularının her birine iki hafta verin; ikinci haftayı alıştırma ve "zor" sorulara ayırın. Rehberin 10. (modüller) ve 11. (sınıflar) bölümlerini ileri düzey öğrenciler için ek konu olarak ekleyin. Projeyi iki aşamaya bölün. |
| Kalabalık sınıf | Eşli çalışmayı artırın; sunumları "galeri yürüyüşü" biçiminde yaptırın ([Hafta 10](hafta-10.md)). |
| Bilgisayar sayısı sınırlı | Canlı kodlama betiğini yalnızca öğretmen bilgisayarından yansıtın; öğrenciler kâğıtta "izleme tablosu" ile çalışsın, bilgisayara sıra ile geçsin. Her haftanın bilgisayarsız etkinliği bu durumda ana etkinlik olur. |

## Gereken ortam

**Tarayıcı yeter.** Kurulum gerektirmeyen çalışma ortamı olarak tarayıcıdaki deneme alanı kullanılır: <https://kerimaksanaoglu.github.io/jus/>. Programlar öğrencinin tarayıcısında çalışır; masaüstü, dizüstü, tablet ve telefondan açılabilir (telefonda uzun kod yazmak zordur; yalnızca küçük denemeler için önerilir).

Deneme alanında şunlar **çalışmaz**: `oku` (kullanıcıdan girdi), `dosya`, `ağ` ve `http` modülleri. Bu yüzden kitteki etkinlikler girdiyi program içinde değişkenle verecek biçimde tasarlanmıştır. Girdi isteyen birkaç isteğe bağlı adım, JUS'un bilgisayara kurulu olmasını gerektirir ve kitte açıkça işaretlenmiştir:

- [Hafta 2, Adım 7](hafta-02.md): `oku` ile ad ve yaş sorma.
- [Hafta 9, Adım 8 ve Alıştırma 9.8](hafta-09.md): girdili sayı tahmin oyunu.
- [Proje](proje.md) fikirlerinin genişletilmiş sürümlerinden bazıları.

Kurulum isteğe bağlıdır. Bilgisayara kurmak isterseniz [kurulum sayfasına](../kurulum.md) bakın. Bu kit, tarayıcıdaki deneme alanında çalıştırılamayan hiçbir kazanımı zorunlu kılmaz.

Not: Deneme alanının yazdığınız programı sonraki derse saklayıp saklamadığını dersten önce deneyin. Saklamıyorsa öğrencilerden programlarını bir metin dosyasına (UTF-8, `.jus` uzantılı) ya da deftere kopyalamalarını isteyin.

## Haftaların özeti

| Hafta | Konu | Rehber bölümü | Hafta sonunda öğrenci şunu yapabilir |
|-------|------|----------------|---------------------------------------|
| [1](hafta-01.md) | Program nedir, `yaz`, hata iletisi okuma | [01](../rehber/01-ilk-program.md) | `yaz` ile metin, sayı ve işlem sonucu yazdıran programı çalıştırır; sözdizimi ve çalışma zamanı hatasını ayırt edip hatanın satırını bulur. |
| [2](hafta-02.md) | Değişkenler, sayılar, metinler | [02](../rehber/02-degiskenler-ve-degerler.md) | Değişkenlerle hesap yapar, metin birleştirir, ondalık sonucu yuvarlar ve biçimler. |
| [3](hafta-03.md) | Kararlar: `eğer` / `değilse` | [03](../rehber/03-kararlar.md) | Karşılaştırma ve `ve` / `veya` / `değil` ile duruma göre farklı çıktı veren program yazar. |
| [4](hafta-04.md) | Döngüler: `iken`, `her` / `aralık` | [04](../rehber/04-donguler.md) | Toplam, sayaç, çarpım tablosu ve asal denetimi gibi döngülü programlar yazar. |
| [5](hafta-05.md) | Fonksiyonlar | [05](../rehber/05-fonksiyonlar.md) | Parametreli, değer döndüren fonksiyon yazar; `yaz` ile `dön` farkını açıklar. Sınav 1. |
| [6](hafta-06.md) | Listeler | [06](../rehber/06-listeler.md) | Listede dizin ve dilim kullanır; listeyi döngüyle işleyip toplam, en küçük ve süzme yapar. |
| [7](hafta-07.md) | Metin işlemleri | [07](../rehber/07-metinler.md) | Metni böler, arar, harf sayar; Türkçe harf kurallarıyla büyük/küçük harfe çevirir; hizalı çıktı üretir. |
| [8](hafta-08.md) | Sözlükler | [08](../rehber/08-sozlukler.md) | Sözlükle sayma ve arama yapar; sözlük listesini bir alana göre sıralar. |
| [9](hafta-09.md) | Hata yakalama ve küçük bir oyun | [09](../rehber/09-hatalar.md), [12](../rehber/12-kucuk-projeler.md) | `dene` / `yakala` / `fırlat` kullanır; `rastgele` ile kendi kendine çalışan bir oyun yazar. Sınav 2. |
| [10](hafta-10.md) | Dönem projesi ve sunum | [12](../rehber/12-kucuk-projeler.md) | Kendi seçtiği küçük bir programı tamamlar, test eder ve sınıfa anlatır. |

## Kitin diğer dosyaları

| Dosya | İçerik |
|-------|--------|
| [cevap-anahtari.md](cevap-anahtari.md) | Tüm haftaların tüm alıştırmalarının çalıştırılıp doğrulanmış çözümleri ve çıktıları. |
| [proje.md](proje.md) | Sekiz proje fikri (asgari ve genişletilmiş sürümleriyle), proje takvimi, öğrenci teslim listesi ve değerlendirme ölçeği. |
| [degerlendirme.md](degerlendirme.md) | İki sınav örneği (5. ve 9. hafta sonrası), kod okuma, hata bulma ve kod yazma soruları, cevap anahtarı ve puanlama. |

Her hafta dosyası aynı bölümlere sahiptir: Kazanımlar, Ön hazırlık, Ders akışı, Canlı kodlama betiği, Sınıf içi etkinlik, Alıştırmalar (kolay, orta, zor), Öğrenciler nerede takılır, Çıkış bileti ve Ev çalışması. Kazanımlar, resmî bir müfredattaki koda bağlanmamış, gözlenebilir davranış olarak yazılmıştır; kendi müfredatınızdaki kazanımlarla eşleştirmek size kalmıştır.

Belgelerdeki bütün kod örnekleri JUS 1.0.0 ile çalıştırılmış, gösterilen çıktılar ve hata iletileri gerçek çıktılardır. Hata iletilerinde dosya adı `ornek.jus` olarak sadeleştirilmiştir; öğrencinin ekranında kendi dosya adı ya da deneme alanındaki ad görünür. Hata iletilerinin metni dilin sürümleri arasında iyileştirilebilir (bkz. [dil tanımı](../dil-tanimi.md)); bu yüzden farklı bir sürümde iletiler küçük farklar gösterebilir.

## Dersin genel işleyişi için öneriler

**Aynı iskelet.** Her ders aynı akışı izlesin: ısınma (önceki hafta), yeni kavram (en çok 10 dakika), canlı kodlama, bilgisayarsız etkinlik, öğrenci uygulaması, kapanış. Öğrenciler akışı öğrenince derse hazır gelir.

**Canlı kodlama.** Kodu öğrencilerle birlikte, küçük adımlarla yazın: siz yazarken onlar da kendi ekranlarında yazar. Her adımdan önce "Ne olacak?" diye tahmin isteyin, sonra çalıştırın. Kasıtlı hata yapmaktan çekinmeyin (kitteki betikler bilerek hata verdirilen adımlar içerir); öğrencilere hatanın korkulacak bir şey olmadığını bu şekilde gösterirsiniz. Hata iletisini her zaman yüksek sesle ve beraber okuyun.

**Eşli programlama.** Öğrenci uygulamasında ikişerli çalışın: biri **sürücü** (yazan), diğeri **gezgin** (söyleyen ve kodu izleyen). Her 7-8 dakikada roller değişir. Gezgin klavyeye dokunmaz; yalnızca söyler. Bu, hem öğrencilerin birbirinden öğrenmesini sağlar hem de size gelen soru sayısını azaltır.

**Hata okuma alışkanlığı.** Öğrencilerin size gelmeden önce üç soruyu yanıtlamasını isteyin: (1) İleti ne diyor? (Yüksek sesle okusun.) (2) Hangi satır ve ne tür hata (sözdizimi mi, çalışma zamanı mı)? (3) O satırda ne yapmaya çalışıyordum? Kitin her hafta dosyasındaki "Öğrenciler nerede takılır" bölümü, o hafta en sık görülen hataları gerçek iletileriyle ve nasıl yönlendireceğinizle birlikte verir.

**İzleme tablosu.** Programın satır satır ilerleyişini kâğıtta ya da tahtada bir tabloyla göstermek (Hafta 2'den itibaren) döngü ve fonksiyon haftalarında en çok işe yarayan tekniktir. Öğrenci "program ne yapar?" diye sorulduğunda çalıştırmadan önce tabloyu doldurabilmelidir.

**Bilgisayarsız etkinlikler.** Her haftanın kısa (10-15 dakikalık) bir bilgisayarsız etkinliği vardır. Bunlar zaman kaybı değildir: kavramı bedenle ve kâğıtla yaşayan öğrenci kodu daha az hata yaparak yazar. Bilgisayar sayısı sınırlıysa bu etkinlikler ana etkinlik olur.

| Hafta | Bilgisayarsız etkinlik |
|-------|-------------------------|
| 1 | Robot kardeş (talimat yazma) |
| 2 | Kutular ve bilgisayar (izleme tablosu) |
| 3 | Ayağa kalk (koşul oyunu) |
| 4 | FizzBuzz çemberi ve izleme tablosu |
| 5 | Fonksiyon makinesi |
| 6 | Dizin kartları |
| 7 | Sezar şifresi çarkı |
| 8 | Anket ve anahtar-değer kartları |
| 9 | Yakala mı, düzelt mi? ve kâğıtta tahmin oyunu |
| 10 | Akran kod gezisi |

**Farklılaştırma.** Her haftanın alıştırmaları kolaydan zora sıralıdır. Kazanımları tüm sınıfın "kolay" ve "orta" düzeyde yakalamasını bekleyin; "zor" alıştırmalar hızlı ilerleyenler içindir. Zorlanan öğrenciye, çözümün iskeletini (ilk üç satırını) vererek yardım edin.

**Ölçme ve değerlendirme.** Çıkış biletleri (her ders sonunda 3 soru) anlık geri bildirim verir. [Sınav 1 ve Sınav 2](degerlendirme.md) ara kontrol noktasıdır. [Dönem projesi](proje.md) kavramların birleşimini ölçer; rubrik tablosu bu amaçla hazırlanmıştır.

## Ayrılmış kelimeler

JUS'un 25 ayrılmış kelimesi değişken ya da fonksiyon adı olarak kullanılamaz:

```
boş      bu       değil    değilse   değişken   dene     devam    doğru   dön
eğer     fırlat   fonksiyon geç      her        içinde   iken     kır     kullan
olarak   sınıf    üst      ve        veya       yakala   yanlış
```

Öğrencilerin günlük dilde kullandığı ve ad olarak seçmek isteyeceği kelimeler arasında `sınıf`, `geç`, `dön`, `üst`, `bu`, `her`, `ve` ve `boş` vardır. Bunlardan birini ad olarak kullanan öğrencinin aldığı ileti bu kelimenin ayrılmış olduğunu söylemez ("Değişken adı bekleniyor." der); bu, kitte [Hafta 2](hafta-02.md) ve [Hafta 9](hafta-09.md) dosyalarında ayrıca ele alınmıştır. Dersin ilk haftalarında bu listeyi sınıfa asmanız önerilir.

Yerleşik fonksiyon adları (`sayı`, `metin`, `tür`, `ekle`, `sil`, `bul`, `al`, `ters`, `aralık`, ...) ayrılmış kelime değildir; ama değişken adı olarak kullanılırsa o fonksiyon çağrılamaz hâle gelir (bkz. [Hafta 2](hafta-02.md), "Öğrenciler nerede takılır", 4. madde).

## Sık gelen hata iletileri: öğretmen için kısa sözlük

Aşağıdaki iletiler kitin çeşitli yerlerinde gerçek çıktı olarak görünür. Bu tablo, ilk haftalarda öğrencilerin sorabileceği iletileri tek yerde toplar.

| İleti (kısaltılmış) | Ne demek? | Genellikle neden? |
|----------------------|-----------|-------------------|
| `'X' adında bir değişken ya da fonksiyon tanımlı değil.` | `X` adlı bir şey yok. | Yazım hatası, büyük-küçük harf farkı, `kullan` unutmak, bloğun dışında kullanmak ya da tanımdan önce çağırmak. |
| `'+' işleci iki sayı, iki metin ya da iki liste ister; metin ve sayı verildi.` | Metin ile sayı `+` ile birleşmez. | `metin()` unutulmuş. |
| `Koşul mantıksal bir değer (doğru/yanlış) olmalı; sayı verildi.` | Koşul `doğru`/`yanlış` vermeli. | `==` yerine `=`; `eğer n:` yazımı. |
| `Blok başlatmak için ':' bekleniyor.` | Satır sonunda iki nokta eksik. | `eğer`, `iken`, `fonksiyon` satırlarında iki nokta unutulmuş. |
| `':' işaretinden sonra girintili bir blok bekleniyor.` | İki noktadan sonra içeri girintili satır yok. | Girinti unutulmuş ya da blok boş bırakılmış. |
| `Beklenmeyen girinti; bu satır bir bloğun içinde değil.` | Satır gereksiz içeride. | Tutarsız girinti. |
| `Dizin sınırların dışında: uzunluk 3, istenen dizin 3.` | Listede o numarada öğe yok. | Dizin 0'dan başlar. |
| `Sözlükte "yaş" anahtarı yok.` | Sözlükte o anahtar yok. | Yazım hatası ya da önce eklenmemiş anahtar. |
| `'topla' fonksiyonu 2 argüman bekliyor, 1 verildi.` | Argüman sayısı yanlış. | Eksik ya da fazla argüman. |
| `Sıfıra bölünemez.` | Bölen sıfır. | Boş listenin ortalaması gibi uç durum. |

## JUS'un sınıfta işe yarayan özellikleri

- **Türkçe anahtar kelimeler.** `eğer`, `değilse`, `iken`, `fonksiyon`, `yaz` gibi kelimeler öğrencinin ana dilinde olduğu için sözdizimi, "dilin kendisi" yerine fikrin kendisine odaklanmayı kolaylaştırır. Değişken ve fonksiyon adlarında Türkçe harfler (`yaş`, `öğrenci`, `çiftMi`) kullanılabilir.
- **Türkçe hata iletileri.** İletiler dosya adı, satır ve (sözdizimi hatalarında) sütun gösterir, çoğu zaman düzeltmeyi de önerir ("metin() ya da sayı() kullanılabilir"). Başka dillerin İngilizce iletilerini okumakta zorlanan öğrenciler için büyük avantajdır.
- **Kurulumsuz çalışma.** Tarayıcıdaki deneme alanı sayesinde ilk derste kurulum zamanı harcanmaz.
- **Sözdizimi hatasında program hiç başlamaz.** Bu kural ("önce bütün dosya denetlenir") hata türlerini ayırt etmeyi öğretmek için kullanışlıdır.
- **Türkçe harf kuralları dilde yerleşiktir.** `büyük_harf("ışık")` sonucu `IŞIK`'tır; `sırala` ve `<` Türk alfabesine göre sıralar. Bu, metin işleme haftasında ek çaba gerektirmeden doğru sonuç verir.
- **Küçük dil.** 25 ayrılmış kelime ve az sayıda yerleşik fonksiyon, öğrencilerin tüm dili kavramasını mümkün kılar.
- **Tutarlı biçim.** Blokların girintiyle belirlenmesi ve tek bir önerilen girinti (4 boşluk), kodun okunabilir olmasını destekler.

## Dürüst sınırlar

- **Kütüphane ekosistemi yoktur.** JUS'un standart kütüphanesi küçüktür (`matematik`, `rastgele`, `zaman`, `dosya`, `sistem`, `json`, `ağ`, `http`, `tr`). Grafik, oyun motoru, veri bilimi gibi alanlarda hazır paket bulunmaz. Öğrencilerin ilgisini çeken "gerçek" uygulamalar (mobil uygulama, web sitesi, oyun) bu dille yapılamaz.
- **Endüstride yaygın bir dil değildir.** Öğrencilere JUS'ta öğrendikleri kavramların (değişken, karar, döngü, fonksiyon, liste, sözlük, hata yakalama) başka dillere aynen taşındığını söyleyin; dilin kendisini bir meslek hedefi olarak sunmayın. Kitin hedefi, JUS'u Python gibi yaygın bir dile geçmeden önce kavramları öğrenmek için kullanmaktır. Bu geçiş için [JUS'tan Python'a](../python-koprusu.md) belgesine bakabilirsiniz.
- **Tarayıcıda girdi yoktur.** Etkileşimli (kullanıcıdan girdi alan) programlar yalnızca bilgisayara kurulu JUS ile çalışır. Kitteki etkinlikler bunu dikkate alır; ama öğrencilerin "oyun" beklentisi, girdi eksikliğinden dolayı kısmen karşılanır.
- **Bazı hata iletileri yanıltıcıdır.** Özellikle ayrılmış bir kelimeyi ad olarak kullanmak, `kullan` ile yüklenmemiş bir modülü çağırmak ve yerleşik bir fonksiyonun adını değişken yapmak, iletinin asıl nedeni söylemediği durumlardır. Kitte bu durumlar "Öğrenciler nerede takılır" bölümlerinde işaretlenmiştir.
- **Kit gerçek bir sınıfta sınanmadı.** Kitteki kodlar ve çıktılar JUS 1.0.0 ile çalıştırılarak doğrulanmıştır; ama süre tahminleri, etkinlik akışları ve öğrenci hataları öğretim deneyimine ve dilin kuralına dayanarak yazılmıştır, bir sınıfla denenmemiştir. İlk uygulamada sürelerin sizin sınıfınıza göre kayabileceğini göz önünde bulundurun.
