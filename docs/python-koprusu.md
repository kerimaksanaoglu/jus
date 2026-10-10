# JUS'tan Python'a

[JUS rehberi](rehber/README.md) | [Dil tanımı](dil-tanimi.md)

Bu belge, JUS'u öğrenmiş birinin Python'a geçişini kolaylaştırmak için yazıldı. İki dili yan yana koyar: aynı işi yapan JUS ve Python kodunu, ortak çıktıyı ve aradaki farkları gösterir. Hangi dilin daha iyi olduğunu tartışmaz; ikisi de kendi amacına göre tasarlanmış, birbirine çok benzeyen ama ayrıntılarda ayrılan dillerdir.

## 1. Kimin için, nasıl okunur

Bu belge, JUS rehberindeki konuları (değişkenler, kararlar, döngüler, fonksiyonlar, listeler, sözlükler, hatalar, modüller, sınıflar) bilen ve Python'a geçmek isteyenler içindir. Python bilmeniz gerekmez; ama programlamayı sıfırdan anlatmaz.

Ana fikir şudur: **kavramlar aynı, kelimeler değişiyor.** `eğer` ile `if`, `her ... içinde` ile `for ... in`, `fonksiyon` ile `def` aynı fikirleri anlatır. Girinti ile blok kurma, dinamik tür, başvuruyla taşınan listeler ve sözlükler, sınıflar, `dene`/`yakala` ile `try`/`except` ikisinde de vardır. Öğreneceklerinizin çoğu yeni kelimelerdir. Gerçekten dikkat isteyen yerler, kelime değil davranış farklarıdır; bunlar 4. bölümde toplandı.

Nasıl okunur:

- Hızlıca bakmak için 2. bölümdeki çeviri tablolarına gidin.
- Konu konu ilerlemek için 3. bölümü okuyun. Her örnekte önce JUS, sonra Python kodu, ardından ortak çıktı gelir. Çıktı farklıysa iki çıktı da gösterilir.
- Python'da bir hata aldığınızda 5. bölümdeki hata iletileri sözlüğüne bakın.
- Her şeyi bir arada görmek için 6. bölümdeki tam programı okuyun.

Örneklerin hepsi çalıştırıldı: JUS örnekleri JUS 1.0.0 ile, Python örnekleri Python 3.11 ile. Gösterilen çıktılar ve hata iletileri gerçektir; yalnızca dosya adları `ornek.jus` ve `ornek.py` olarak sadeleştirildi. Python'un hata iletileri sürümler arasında küçük farklar gösterebilir; JUS'un iletileri de [dil tanımına](dil-tanimi.md) göre iyileştirilebilir. İletilerin anlamına bakın, birebir metnine değil.

```sh
jus ornek.jus
python ornek.py
```

Bazı kurulumlarda `python` yerine `python3` ya da `py` gerekebilir. Girdi alan örneklerde girdi programa boruyla verildi; bu yüzden terminalde yazdığınız harfler çıktıda görünmez. Etkileşimli kip için `jus` ya da `python` yazmanız yeterlidir (çıkmak için JUS'ta `çıkış`, Python'da `exit()`).

## 2. Hızlı çeviri tablosu

### Anahtar kelimeler ve temel deyimler

| JUS | Python | Not |
|-----|--------|-----|
| `değişken x = 1` | `x = 1` | Python'da `değişken` yok; atama değişkeni tanımlar |
| `yaz(a, b)` | `print(a, b)` | |
| `oku("soru")` | `input("soru")` | |
| `# yorum` | `# yorum` | aynı |
| `eğer` / `değilse eğer` / `değilse` | `if` / `elif` / `else` | |
| `iken koşul:` | `while koşul:` | |
| `her x içinde kap:` | `for x in kap:` | |
| `aralık(5)` | `range(5)` | JUS liste verir, Python `range` nesnesi |
| `kır` / `devam` / `geç` | `break` / `continue` / `pass` | |
| `fonksiyon f(a):` | `def f(a):` | |
| `fonksiyon f(a, b = 2):` | `def f(a, b=2):` | varsayılan parametre; JUS'ta varsayılan ifade her çağrıda yeniden hesaplanır (4.8A) |
| `fonksiyon f(*kalan):` | `def f(*args):` | JUS'ta `kalan` liste, Python'da demettir; çağrıda `f(*liste)` JUS'ta yoktur |
| `f"Merhaba, {ad}"` | `f"Merhaba, {ad}"` | aynı; JUS'ta `{x:.2f}` gibi biçim belirteci yoktur |
| `dön x` | `return x` | |
| `doğru` / `yanlış` / `boş` | `True` / `False` / `None` | Python'da baş harf büyük |
| `ve` / `veya` / `değil` | `and` / `or` / `not` | Python'da her değerle çalışır (4.1) |
| `x içinde kap` | `x in kap` | `değil x içinde kap` ile `x not in kap` |
| `x == boş` | `x is None` | JUS'ta `is` yok |
| `dene:` / `yakala h:` / `fırlat x` | `try:` / `except Hata as h:` / `raise Hata(x)` | |
| `kullan m` / `kullan m olarak a` | `import m` / `import m as a` | |
| `sınıf A(B):` | `class A(B):` | |
| `fonksiyon kur(x):` | `def __init__(self, x):` | |
| `bu.ad` | `self.ad` | Python'da `self` her yönteme açıkça yazılır |
| `üst.kur(x)` | `super().__init__(x)` | |
| `doğrula(k, ileti)` | `assert k, ileti` | |

### Yerleşik fonksiyonlar ve işleçler

| JUS | Python | Not |
|-----|--------|-----|
| `metin(x)` | `str(x)` | |
| `sayı("3.5")` | `float("3.5")`, `int("7")` | Python'da tam ve ondalıklı sayı ayrı türlerdir |
| `uzunluk(k)` | `len(k)` | |
| `tür(x)` | `type(x)` | adlar aşağıdaki tabloda |
| `örneği_mi(x, S)` | `isinstance(x, S)` | |
| `mutlak(x)` | `abs(x)` | |
| `yuvarla(x)`, `yuvarla(x, n)` | `round(x)`, `round(x, n)` | yarım değerlerde sonuç farklıdır (4.3) |
| `biçimle(x, n)` | `f"{x:.{n}f}"` | JUS'ta biçim belirteci yoktur; `f"{biçimle(x, n)}"` yazılır |
| `taban(x)`, `tavan(x)` | `math.floor(x)`, `math.ceil(x)` | |
| `karekök(x)` | `math.sqrt(x)` | |
| `taban(a / b)` | `a // b` | tam bölme |
| `matematik.üs(a, b)` | `a ** b` | JUS'ta `**` ve `//` yok |
| `+ - * / %` ve `+=` `-=` `*=` `/=` | aynı | `/` ikisinde de ondalıklı bölme (4.3) |
| `== != < <= > >=` | aynı | Python'da `1 < x < 10` yazılabilir (4.9) |
| liste işlevleri (`ekle`, `sırala` ...) | liste yöntemleri | 3.7'deki tablo |
| metin işlevleri (`büyük_harf`, `böl` ...) | metin yöntemleri | 3.8'deki tablo |

`tür(x)` JUS'ta Türkçe bir ad verir; Python'daki karşılığı `type(x).__name__` ifadesidir:

| JUS `tür(x)` | Python `type(x).__name__` |
|--------------|---------------------------|
| `sayı` | `int` (tam sayı), `float` (ondalıklı) |
| `metin`, `mantıksal`, `boş` | `str`, `bool`, `NoneType` |
| `liste`, `sözlük` | `list`, `dict` |
| `fonksiyon` | `function` (yerleşikler için `builtin_function_or_method`) |
| sınıf adı (ör. `Köpek`) | `Köpek` |

## 3. Konu konu karşılaştırma

### 3.1 Yazdırma ve yorumlar

```jus
# Yorum satırı
yaz("Toplam:", 3 + 4)  # satır sonu yorumu
yaz("a", "b", 3)
```

```python
# Yorum satırı
print("Toplam:", 3 + 4)  # satır sonu yorumu
print("a", "b", 3)
```

```
Toplam: 7
a b 3
```

İkisi de argümanların arasına boşluk koyar ve satırı bitirir. Python'un `print(a, b, sep="-", end="")` gibi ayarları JUS'ta yoktur.

Değerlerin ekrandaki **görünümü** de değişir, değerler aynı kalır: `doğru`, `yanlış`, `boş` Python'da `True`, `False`, `None` olarak yazılır; Python listedeki ve sözlükteki metinleri tek tırnakla (`['a']`), JUS çift tırnakla (`["a"]`) gösterir; sayıların görünümü 4.3'te anlatılır. İki programın çıktılarını karşılaştırırken görünüm farkını gerçek fark sanmayın.

### 3.2 Değişkenler, sayılar ve metinler

```jus
değişken ad = "Ayşe"
değişken yaş = 25
yaş += 1
değişken fiyat = 12.5
yaz(ad + " " + metin(yaş))
yaz("Toplam: " + biçimle(fiyat * 3, 2) + " TL")
yaz(sayı("40") + 2, 7 % 3, uzunluk("çalışkan"))
değişken a = 1
değişken b = 2
değişken geçici = a
a = b
b = geçici
yaz(a, b)
```

```python
ad = "Ayşe"
yaş = 25
yaş += 1
fiyat = 12.5
print(ad + " " + str(yaş))
print(f"Toplam: {fiyat * 3:.2f} TL")
print(int("40") + 2, 7 % 3, len("çalışkan"))
a = 1
b = 2
a, b = b, a
print(a, b)
```

```
Ayşe 26
Toplam: 37.50 TL
42 1 8
2 1
```

Python'da `değişken` gibi bir tanım anahtar kelimesi yoktur: bir adı ilk kez atadığınız yer onu tanımlar, sonraki atamalar değeri değiştirir. JUS'taki `değişken x` (başlangıç değeri `boş`) Python'da `x = None` olur. İkisi de dinamik tiplidir: değişkenin değil değerin türü vardır. Python'da `a, b = b, a` gibi çoklu atama vardır; JUS'ta yer değiştirmek için geçici değişken gerekir.

Metinlere değer yerleştirmenin yaygın Python yolu `f"..."` metinleridir: süslü parantez içine ifade yazarsınız, `:.2f` iki ondalık basamak ister. JUS'ta da aynı yazım vardır (`f"Merhaba, {ad}!"`; süslü parantezin kendisi için `{{` ve `}}`), ancak üç fark bulunur. Birincisi, JUS'ta biçim belirteci (`:.2f`, `:>8`) yoktur; sayıyı biçimlemek için `{}` içinde `biçimle()` ya da `sola_doldur()` çağrılır: `f"{biçimle(fiyat * 3, 2)} TL"`. İkincisi, ifadenin içinde çift tırnak kullanılamaz (Python 3.12'den önce de böyleydi); tırnaklı değer önce bir değişkene alınır. Üçüncüsü, `{x=}` gibi yardımcı yazımlar yoktur. Yukarıdaki JUS örneği `+`, `metin()` ve `biçimle()` ile yazıldı; aynı çıktı f-metinleriyle de elde edilebilirdi. Python'da tek tırnak (`'ab'`) ve üç tırnaklı çok satırlı metinler de vardır; JUS yalnızca çift tırnak ve tek satır kullanır. `\n`, `\t`, `\"`, `\\` kaçış dizileri aynıdır. İkisinde de metin karakter dizisidir ve değiştirilemez. Python Türkçe harfli adlara izin verir, ancak birçok proje ASCII adlar kullanır (`yas`, `ogrenci`).

### 3.3 Girdi alma

Girdi: `Ayşe`, `20`

```jus
değişken ad = oku("Adınız: ")
değişken yaş = sayı(oku("Yaşınız: "))
yaz("Merhaba,", ad)
yaz("Gelecek yıl", yaş + 1, "yaşında olacaksınız.")
```

```python
ad = input("Adınız: ")
yaş = int(input("Yaşınız: "))
print("Merhaba,", ad)
print("Gelecek yıl", yaş + 1, "yaşında olacaksınız.")
```

```
Adınız: Yaşınız: Merhaba, Ayşe
Gelecek yıl 21 yaşında olacaksınız.
```

Girdi her iki dilde de metindir; sayıya `sayı()` ile `int()` (ya da `float()`) çevirir. Girdi boruyla verildiği için iki istem yan yana görünüyor; terminalde her istemin yanında sizin yazdığınız satır olur. Bir fark: girdi bittiğinde (kullanıcı girdiyi kapatınca ya da dosya boşsa) `oku()` `boş` döndürür, `input()` ise `EOFError` hatası verir.

### 3.4 Koşullar

```jus
değişken puan = 72
eğer puan >= 85:
    yaz("pekiyi")
değilse eğer puan >= 60:
    yaz("orta")
değilse:
    yaz("zayıf")
eğer puan > 50 ve değil (puan == 100):
    yaz("aralıkta")
eğer "al" içinde "kalem":
    yaz("kalem içinde al var")
```

```python
puan = 72
if puan >= 85:
    print("pekiyi")
elif puan >= 60:
    print("orta")
else:
    print("zayıf")
if puan > 50 and not (puan == 100):
    print("aralıkta")
if "al" in "kalem":
    print("kalem içinde al var")
```

```
orta
aralıkta
kalem içinde al var
```

Koşulun sonundaki `:` ve girinti ile blok kurma aynıdır. `değilse eğer` Python'da tek kelime `elif` olur. JUS'ta koşul mantıksal olmak zorundadır; Python'da her değer koşul olabilir (4.1).

### 3.5 Döngüler

```jus
değişken toplam = 0
her i içinde aralık(1, 6):
    eğer i == 4:
        devam
    toplam += i
yaz("Toplam:", toplam)

değişken n = 3
iken n > 0:
    n -= 1
    eğer n == 1:
        kır
yaz(n)

değişken meyveler = ["elma", "armut"]
her i içinde aralık(uzunluk(meyveler)):
    yaz(i, meyveler[i])
```

```python
toplam = 0
for i in range(1, 6):
    if i == 4:
        continue
    toplam += i
print("Toplam:", toplam)

n = 3
while n > 0:
    n -= 1
    if n == 1:
        break
print(n)

meyveler = ["elma", "armut"]
for i, meyve in enumerate(meyveler):
    print(i, meyve)
```

```
Toplam: 11
1
0 elma
1 armut
```

`aralık(baş, son, adım)` ile `range(baş, son, adım)` aynı sayıları üretir (son hariç). Hem sırayı hem öğeyi isteyen döngülerde Python'da `enumerate` kullanılır; JUS'ta dizin üzerinden `meyveler[i]` yazılır. Metin üzerinde `her h içinde "çay":` ile `for h in "çay":` karakterleri tek tek verir; sözlük üzerinde gezinme 3.9'dadır. Python'da döngülere `else` bölümü eklenebilir (`for ... else`); JUS'ta yoktur.

### 3.6 Fonksiyonlar

```jus
fonksiyon alan(en, boy):
    dön en * boy

fonksiyon faktöriyel(n):
    eğer n <= 1:
        dön 1
    dön n * faktöriyel(n - 1)

fonksiyon kare(x):
    dön x * x

yaz(alan(3, 4), faktöriyel(5))
yaz(eşle([1, 2, 3], kare))
```

```python
def alan(en, boy):
    return en * boy

def faktöriyel(n):
    if n <= 1:
        return 1
    return n * faktöriyel(n - 1)

def kare(x):
    return x * x

print(alan(3, 4), faktöriyel(5))
print(list(map(kare, [1, 2, 3])))
```

```
12 120
[1, 4, 9]
```

İkisinde de fonksiyonlar birer değerdir: değişkene atanır, argüman olarak verilir, döndürülür. Parametreler de büyük ölçüde aynıdır: varsayılan değerli (`def f(a, b=2)`) ve sayısı değişen (`*args`) parametreler iki dilde de vardır.

```jus
fonksiyon selamla(ad, selam = "Merhaba"):
    dön f"{selam}, {ad}!"

fonksiyon topla(*sayılar):
    değişken t = 0
    her s içinde sayılar:
        t += s
    dön t

yaz(selamla("Ayşe"), selamla("Ali", "Günaydın"))
yaz(topla(), topla(1, 2, 3))
```

```
Merhaba, Ayşe! Günaydın, Ali!
0 6
```

Python karşılığı:

```python
def selamla(ad, selam="Merhaba"):
    return f"{selam}, {ad}!"

def topla(*sayılar):
    t = 0
    for s in sayılar:
        t += s
    return t

print(selamla("Ayşe"), selamla("Ali", "Günaydın"))
print(topla(), topla(1, 2, 3))
```

Farklar şunlardır:

- JUS'ta `*sayılar` bir **liste** olur, Python'da bir demet (`tuple`). Argüman kalmazsa JUS'ta `[]`, Python'da `()` gelir.
- Çağrı tarafında liste açma JUS'ta yoktur: Python'da `topla(*[1, 2, 3])` yazılabilir, JUS'ta `topla(*liste)` sözdizimi hatasıdır. Listeyi tek argüman olarak alan sıradan bir parametre kullanılır.
- Varsayılan ifade JUS'ta her çağrıda yeniden hesaplanır, Python'da `def` satırında bir kez (4.8A).
- Anahtar sözcükle verilen argümanlar (`f(b=1, a=3)`), `**kwargs` ve yalnızca anahtar sözcükle verilebilen parametreler JUS'ta yoktur; argümanlar sırayla verilir.
- İki dilde de varsayılanı olan parametreden sonra varsayılanı olmayan parametre yazılamaz (Python: `SyntaxError: non-default argument follows default argument`). JUS'ta ayrıca `*` parametresi sonuncu olmalıdır.

JUS'ta adsız fonksiyon (Python'daki `lambda x: x * x`) yoktur; fonksiyona önce bir ad verilir. Değer döndürmeyen fonksiyon JUS'ta `boş`, Python'da `None` verir.

### 3.7 Listeler

```jus
değişken sayılar = [5, 3, 8]
ekle(sayılar, 1)
araya_ekle(sayılar, 0, 9)
yaz(sayılar, sayılar[-1], sayılar[1:3])
yaz(çıkar(sayılar), sil(sayılar, 0))
yaz(sırala(sayılar), sayılar)
yaz(uzunluk(sayılar), bul(sayılar, 8), sayılar + [7])
eğer 8 içinde sayılar:
    yaz("8 var")
değişken kopya = sayılar[:]
ekle(kopya, 4)
yaz(sayılar, kopya)
```

```python
sayılar = [5, 3, 8]
sayılar.append(1)
sayılar.insert(0, 9)
print(sayılar, sayılar[-1], sayılar[1:3])
print(sayılar.pop(), sayılar.pop(0))
print(sorted(sayılar), sayılar)
print(len(sayılar), sayılar.index(8), sayılar + [7])
if 8 in sayılar:
    print("8 var")
kopya = sayılar[:]
kopya.append(4)
print(sayılar, kopya)
```

```
[9, 5, 3, 8, 1] 1 [5, 3]
1 9
[3, 5, 8] [5, 3, 8]
3 2 [5, 3, 8, 7]
8 var
[5, 3, 8] [5, 3, 8, 4]
```

JUS'ta liste işlemleri fonksiyondur (`ekle(liste, x)`); Python'da listenin **yöntemidir** (`liste.append(x)`): nokta ile çağrılır. Listelerin başvuruyla taşındığı (`k = l` kopyalamaz) ve `l[:]` ile kopyalandığı iki dilde de geçerlidir. Eşleme tablosu:

| JUS | Python | Not |
|-----|--------|-----|
| `ekle(l, x)` | `l.append(x)` | |
| `araya_ekle(l, i, x)` | `l.insert(i, x)` | |
| `çıkar(l)` | `l.pop()` | son öğeyi çıkarır ve döndürür |
| `sil(l, i)` | `l.pop(i)` ya da `del l[i]` | |
| `sırala(l)` | `sorted(l)` | yeni liste verir; `l.sort()` listeyi yerinde sıralar, `None` döndürür |
| `sırala(l, anahtar)` | `sorted(l, key=anahtar)` | |
| `ters(l)` | `l[::-1]` ya da `list(reversed(l))` | |
| `eşle(l, f)` | `[f(x) for x in l]` ya da `list(map(f, l))` | köşeli parantezli biçime liste kapsamı denir |
| `süz(l, f)` | `[x for x in l if f(x)]` ya da `list(filter(f, l))` | |
| `bul(l, x)` | `l.index(x)` | öğe yoksa JUS `-1`, Python `ValueError` verir |
| `uzunluk(l)`, `x içinde l` | `len(l)`, `x in l` | |
| `l + m`, `l[a:b]`, `l[-1]` | aynı | Python'da `l[a:b:adım]` da vardır |
| `matematik.toplam(l)` | `sum(l)` | `en_büyük` / `en_küçük` için `max(l)` / `min(l)` |

### 3.8 Metin işlemleri

```jus
değişken t = kırp("  Merhaba Dünya  ")
yaz("[" + t + "]")
yaz(uzunluk(t), t[0], t[-1], t[0:7])
yaz(bul(t, "Dünya"), bul(t, "Ay"), değiştir(t, "a", "e"))
yaz(birleştir(böl(t, " "), "-"), tekrarla("=", 5))
yaz(büyük_harf("kalem"), küçük_harf("KALEM"))
yaz(sola_doldur("7", 3, "0") + "|" + sağa_doldur("ab", 4) + "|")
eğer başlar_mı(t, "Mer") ve biter_mi(t, "ya"):
    yaz("Mer ile başlar, ya ile biter")
```

```python
t = "  Merhaba Dünya  ".strip()
print("[" + t + "]")
print(len(t), t[0], t[-1], t[0:7])
print(t.find("Dünya"), t.find("Ay"), t.replace("a", "e"))
print("-".join(t.split(" ")), "=" * 5)
print("kalem".upper(), "KALEM".lower())
print("7".rjust(3, "0") + "|" + "ab".ljust(4) + "|")
if t.startswith("Mer") and t.endswith("ya"):
    print("Mer ile başlar, ya ile biter")
```

```
[Merhaba Dünya]
13 M a Merhaba
8 -1 Merhebe Dünye
Merhaba-Dünya =====
KALEM kalem
007|ab  |
Mer ile başlar, ya ile biter
```

| JUS | Python | Not |
|-----|--------|-----|
| `kırp(m)` | `m.strip()` | |
| `büyük_harf(m)`, `küçük_harf(m)` | `m.upper()`, `m.lower()` | Türkçe harflerde sonuç farklıdır (4.6) |
| `bul(m, a)` | `m.find(a)` | yoksa ikisi de `-1` |
| `değiştir(m, eski, yeni)` | `m.replace(eski, yeni)` | |
| `böl(m, ayraç)` | `m.split(ayraç)` | `böl(m, "")` karakterlere böler; Python'da karşılığı `list(m)`, çünkü `m.split("")` hata verir |
| `birleştir(l, ayraç)` | `ayraç.join(l)` | JUS öğeleri metne çevirir; Python öğelerin metin olmasını ister (`map(str, l)`) |
| `tekrarla(m, n)` | `m * n` | JUS'ta `"ab" * 3` hatadır |
| `sola_doldur(m, w, d)`, `sağa_doldur(m, w, d)` | `m.rjust(w, d)`, `m.ljust(w, d)` | |
| `başlar_mı(m, ön)`, `biter_mi(m, son)` | `m.startswith(ön)`, `m.endswith(son)` | |
| `ters(m)` | `m[::-1]` | |
| `uzunluk(m)`, `a içinde m`, `m[a:b]` | `len(m)`, `a in m`, `m[a:b]` | |

Python'daki `m.split()` (ayraçsız) art arda gelen boşlukları tek ayraç sayar; JUS'ta karşılığı yoktur: `böl("  a  b ", " ")` boş parçalar üretir, tıpkı Python'daki `"  a  b ".split(" ")` gibi.

### 3.9 Sözlükler

```jus
değişken kişi = {"ad": "Ayşe", "yaş": 30}
kişi["şehir"] = "Ankara"
yaz(kişi["ad"], uzunluk(kişi), al(kişi, "boy", 0))
eğer "yaş" içinde kişi:
    yaz("yaş var")
her anahtar içinde kişi:
    yaz(anahtar, kişi[anahtar])
yaz(sil(kişi, "yaş"), birleştir(anahtarlar(kişi), ", "))
```

```python
kişi = {"ad": "Ayşe", "yaş": 30}
kişi["şehir"] = "Ankara"
print(kişi["ad"], len(kişi), kişi.get("boy", 0))
if "yaş" in kişi:
    print("yaş var")
for anahtar, değer in kişi.items():
    print(anahtar, değer)
print(kişi.pop("yaş"), ", ".join(kişi.keys()))
```

```
Ayşe 3 0
yaş var
ad Ayşe
yaş 30
şehir Ankara
30 ad, şehir
```

| JUS | Python | Not |
|-----|--------|-----|
| `d[k]`, `d[k] = v` | aynı | anahtar yoksa okuma ikisinde de hata verir |
| `al(d, k, varsayılan)` | `d.get(k, varsayılan)` | Python'da varsayılan yazılmazsa `None`; JUS'ta üçüncü argüman zorunlu |
| `k içinde d` | `k in d` | anahtarı arar |
| `sil(d, k)` | `d.pop(k)` | |
| `anahtarlar(d)`, `değerler(d)` | `list(d.keys())`, `list(d.values())` | |
| `her k içinde d:` | `for k in d:` | anahtarları ekleme sırasıyla gezer; anahtar ve değer birlikte için `d.items()` |

JUS'ta anahtar metin, sayı ya da mantıksal olabilir; Python'da demet gibi değiştirilemeyen her değer anahtar olabilir. Sayma işinde `d[k] += 1` anahtar yoksa ikisinde de hatadır (4.7); Python'da `d.get(k, 0) + 1` ya da `collections.Counter` kullanılır.

### 3.10 Hata yakalama

```jus
fonksiyon böl(a, b):
    eğer b == 0:
        fırlat "Sıfıra bölünemez."
    dön a / b

dene:
    yaz(böl(10, 4))
    yaz(böl(1, 0))
    yaz("buraya gelinmez")
yakala hata:
    yaz("Hata:", hata)
yaz("devam")
```

```python
def böl(a, b):
    if b == 0:
        raise ValueError("Sıfıra bölünemez.")
    return a / b

try:
    print(böl(10, 4))
    print(böl(1, 0))
    print("buraya gelinmez")
except ValueError as hata:
    print("Hata:", hata)
print("devam")
```

```
2.5
Hata: Sıfıra bölünemez.
devam
```

Asıl fark hatanın **ne olduğundadır**. JUS'ta `fırlat` her değeri fırlatabilir, dilin kendi hataları birer metindir ve `yakala` her hatayı yakalar. Python'da hatalar **türü olan nesnelerdir** (`ValueError`, `ZeroDivisionError`, `KeyError` ...) ve `except` hangi türü yakalayacağını söyler. Örneğin `1 / 0` işlemini yakalayınca `h` değişkeni JUS'ta `Sıfıra bölünemez.` metnidir; Python'da `except ZeroDivisionError as h:` ile yakalanan `h` yazdırıldığında `division by zero` görünür (hata iletileri 5. bölümde). JUS'taki adsız `yakala:` her hatayı yakalar; Python'da karşılığı `except Exception:` yazımıdır. Çıplak `except:` çıkış isteklerini (Ctrl+C gibi) de yakaladığı için önerilmez. Python'da `try` bloğuna `else` (hata olmadıysa çalışır) ve `finally` (her durumda çalışır) bölümleri eklenebilir; JUS'ta ikisi de yoktur. Hata türleri için Python'un [yerleşik hata listesine](https://docs.python.org/3/builtins/exceptions.html) bakın.

### 3.11 Modüller ve standart kütüphane

```jus
kullan matematik
kullan matematik olarak m
yaz(yuvarla(matematik.karekök(2), 4), taban(m.sin(1) * 100))
yaz(matematik.üs(2, 10), matematik.en_büyük(3, 9, 4))
```

```python
import math
import math as m
print(round(math.sqrt(2), 4), math.floor(m.sin(1) * 100))
print(2 ** 10, max(3, 9, 4))
```

```
1.4142 84
1024 9
```

`kullan x` ile `import x`, `kullan x olarak y` ile `import x as y` aynıdır; üyelere `modül.üye` ile erişilir. Python'da `from math import sqrt` ile üyeyi doğrudan almak da mümkündür; JUS'ta yoktur. JUS'ta `kullan "araçlar/geometri"` alt klasördeki dosyayı yükler; Python'da dosyalar da modüldür ve `import araçlar.geometri` ya da `from araçlar import geometri` yazılır. Standart kütüphane eşlemesi:

| JUS modülü | Python karşılığı | Üyeler |
|------------|------------------|--------|
| `matematik` | `math` | `pi`, `e`, `sin`, `cos`, `tan`, `log10` aynı; `üs(a, b)` yerine `a ** b`; `karekök` yerine `math.sqrt`; `ln` yerine `math.log`; `en_büyük`, `en_küçük`, `toplam` yerine yerleşik `max`, `min`, `sum` |
| `rastgele` | `random` | `sayı()` yerine `random.random()`; `tam(a, b)` yerine `random.randint(a, b)` (ikisinde de uçlar dahil); `seç` yerine `random.choice`; `karıştır` yerine `random.shuffle`; `tohum` yerine `random.seed` |
| `zaman` | `time`, `datetime` | `şimdi()` yerine `time.time()`; `bekle(s)` yerine `time.sleep(s)`; `tarih()` yerine `datetime.datetime.now()` (alanlar `.year`, `.month`, `.day`, `.hour`, `.minute`, `.second`, `.isoweekday()`) |
| `dosya` | `open`, `os`, `os.path` | 3.13'te |
| `sistem` | `sys`, `os` | `argümanlar` yerine `sys.argv[1:]`; `platform` yerine `sys.platform`; `ortam(ad)` yerine `os.environ.get(ad)`; `çık(k)` yerine `sys.exit(k)`; `hata_yaz(...)` yerine `print(..., file=sys.stderr)`; `betik` yerine `sys.argv[0]` |
| `json` | `json` | 3.14'te |
| `ağ`, `http` | `socket`, `urllib.request`, `http.server` | |
| `tr` | yok | Python'un standart kütüphanesinde karşılığı yoktur |

`sistem.platform` değerleri `"windows"`, `"linux"`, `"macos"`; Python'un `sys.platform` değerleri `"win32"`, `"linux"`, `"darwin"` biçimindedir. JUS'un `jus test`, `jus bicimle` ve `jus paket` komutlarının Python'daki karşılıkları sırasıyla `unittest` modülü (ya da `pytest` gibi üçüncü taraf araçlar), biçimlendirici araçlar ve `pip` komutudur. Rastgele sayılarda dikkat: `tohum(42)` her iki dilde de dizisini sabitler, ama iki dilin üreteçleri farklıdır; aynı tohumla farklı sayılar çıkar.

### 3.12 Sınıflar

```jus
sınıf Hesap:
    fonksiyon kur(sahip, bakiye):
        bu.sahip = sahip
        bu.bakiye = bakiye

    fonksiyon yatır(tutar):
        bu.bakiye += tutar

    fonksiyon özet():
        dön bu.sahip + ": " + metin(bu.bakiye) + " TL"

sınıf Vadeli(Hesap):
    fonksiyon kur(sahip, bakiye, faiz):
        üst.kur(sahip, bakiye)
        bu.faiz = faiz

    fonksiyon özet():
        dön üst.özet() + " (faiz %" + metin(bu.faiz) + ")"

değişken h = Vadeli("Ayşe", 100, 5)
h.yatır(25)
yaz(h.özet(), tür(h))
eğer örneği_mi(h, Hesap):
    yaz("Vadeli bir Hesap'tır")
```

```python
class Hesap:
    def __init__(self, sahip, bakiye):
        self.sahip = sahip
        self.bakiye = bakiye

    def yatır(self, tutar):
        self.bakiye += tutar

    def özet(self):
        return self.sahip + ": " + str(self.bakiye) + " TL"

class Vadeli(Hesap):
    def __init__(self, sahip, bakiye, faiz):
        super().__init__(sahip, bakiye)
        self.faiz = faiz

    def özet(self):
        return super().özet() + " (faiz %" + str(self.faiz) + ")"

h = Vadeli("Ayşe", 100, 5)
h.yatır(25)
print(h.özet(), type(h).__name__)
if isinstance(h, Hesap):
    print("Vadeli bir Hesap'tır")
```

```
Ayşe: 125 TL (faiz %5) Vadeli
Vadeli bir Hesap'tır
```

- `fonksiyon kur(...)` Python'da `def __init__(self, ...)` olur.
- JUS'ta `bu` yöntemin içinde hazırdır, parametre listesine yazılmaz. Python'da nesne her yöntemin **ilk parametresi** olarak açıkça yazılır (geleneksel adı `self`) ve alanlara `self.ad` ile erişilir. `self` yazmayı unutmak sık yapılan bir hatadır (5. bölüm).
- `üst.kur(...)` yerine `super().__init__(...)`, `üst.özet()` yerine `super().özet()` yazılır.
- Python'da bir sınıf birden çok sınıftan türeyebilir (`class C(A, B)`); JUS'ta tek üst sınıf vardır.
- Python'da nesneyi `print` ile yazdırınca görünen metni `__str__` yöntemi belirler; JUS'ta böyle özel yöntemler yoktur (`yaz(nesne)` yalnızca `<Vadeli nesnesi>` yazar). Python'da da özelleştirme yoksa iki nesne yalnızca aynı nesneyse eşittir.

### 3.13 Dosya okuma ve yazma

```jus
kullan dosya
dosya.yaz("not.txt", "ilk satır\nikinci satır\n")
dosya.ekle("not.txt", "üçüncü\n")
her satır içinde dosya.satırlar("not.txt"):
    yaz(satır)
eğer dosya.var_mı("not.txt"):
    dosya.sil("not.txt")
    yaz("silindi")
```

```python
import os

with open("not.txt", "w", encoding="utf-8") as f:
    f.write("ilk satır\nikinci satır\n")
with open("not.txt", "a", encoding="utf-8") as f:
    f.write("üçüncü\n")
with open("not.txt", encoding="utf-8") as f:
    for satır in f.read().splitlines():
        print(satır)
if os.path.exists("not.txt"):
    os.remove("not.txt")
    print("silindi")
```

```
ilk satır
ikinci satır
üçüncü
silindi
```

| JUS | Python |
|-----|--------|
| `dosya.oku(yol)` | `open(yol, encoding="utf-8").read()` |
| `dosya.satırlar(yol)` | `open(yol, encoding="utf-8").read().splitlines()` |
| `dosya.yaz(yol, içerik)` | `open(yol, "w", encoding="utf-8").write(içerik)` |
| `dosya.ekle(yol, içerik)` | `open(yol, "a", encoding="utf-8").write(içerik)` |
| `dosya.var_mı(yol)`, `dosya.klasör_mü(yol)` | `os.path.exists(yol)`, `os.path.isdir(yol)` |
| `dosya.sil(yol)` | `os.remove(yol)` |
| `dosya.listele(yol)` | `os.listdir(yol)` |
| `dosya.klasör_oluştur(yol)`, `dosya.klasör_sil(yol)` | `os.mkdir(yol)`, `os.rmdir(yol)` |

- Python'da dosya önce **açılır** (`open`), sonra okunur ya da yazılır; `with` bloğu iş bitince dosyayı kapatır. JUS tek çağrıyla açıp kapatır. Tablodaki tek satırlık biçimler kısa denemeler içindir; asıl kullanımda `with open(...) as f:` yazılır.
- JUS dosyaları her zaman UTF-8 okur ve yazar. Python'un `open` işlevinde `encoding` yazmazsanız kodlama sisteminizin ayarına bağlıdır ve Windows'ta çoğu kurulumda UTF-8 değildir. Türkçe harfler için `encoding="utf-8"` yazmayı alışkanlık edinin.
- Python'un `pathlib` modülü yolları nesne olarak ele alır ve yeni kodda sık kullanılır (`Path("not.txt").read_text(encoding="utf-8")`).

### 3.14 JSON

```jus
kullan json
değişken veri = json.çöz("{\"ad\": \"Ayşe\", \"notlar\": [80, 90]}")
ekle(veri["notlar"], 100)
yaz(veri["ad"], veri["notlar"][2])
yaz(json.yaz(veri))
yaz(json.yaz({"ad": veri["ad"]}, 2))
```

```python
import json

veri = json.loads('{"ad": "Ayşe", "notlar": [80, 90]}')
veri["notlar"].append(100)
print(veri["ad"], veri["notlar"][2])
print(json.dumps(veri, ensure_ascii=False, separators=(",", ":")))
print(json.dumps({"ad": veri["ad"]}, indent=2, ensure_ascii=False))
```

```
Ayşe 100
{"ad":"Ayşe","notlar":[80,90,100]}
{
  "ad": "Ayşe"
}
```

`json.çöz` ile `json.loads` ("load string"), `json.yaz` ile `json.dumps` ("dump string") karşılıklıdır; dosyayla çalışan `json.load(f)` ve `json.dump(veri, f)` de vardır. İki fark: JUS `json.yaz` varsayılan olarak boşluksuz tek satır üretir, Python `json.dumps` virgül ve iki noktadan sonra boşluk koyar; ayrıca Python varsayılan olarak ASCII dışı harfleri `\u011f` biçiminde kaçırır, Türkçe harfleri olduğu gibi yazmak için `ensure_ascii=False` gerekir. JSON'daki `null`, `true`, `false` JUS'ta `boş`, `doğru`, `yanlış`; Python'da `None`, `True`, `False` olur.

## 4. Dikkat: farklı davranan yerler

Bu bölüm, Python'a geçen JUS bilenlerin en çok yanıldığı yerleri toplar. Çoğunda JUS hata verir, Python sessizce devam eder; yani Python'da daha çok şeyi kendiniz denetlemeniz gerekir.

### 4.1 Koşulda mantıksal olmayan değer

JUS'ta `eğer`, `iken`, `ve`, `veya`, `değil` yalnızca `doğru` ya da `yanlış` ister. Python'da her değerin bir **doğruluk değeri** vardır ve koşulda doğrudan kullanılabilir.

```jus
değişken liste = []
eğer liste:
    yaz("dolu")
değilse:
    yaz("boş")
```

```python
liste = []
if liste:
    print("dolu")
else:
    print("boş")
```

JUS çıktısı:

```
ornek.jus:2: çalışma zamanı hatası: Koşul mantıksal bir değer (doğru/yanlış) olmalı; liste verildi.
```

Python çıktısı:

```
boş
```

Python'da `False`, `None`, `0`, `0.0`, boş metin `""`, boş liste `[]` ve boş sözlük `{}` **yanlış** sayılır; diğer her şey doğrudur. Python kodunda `if liste:` gördüğünüzde "liste boş değilse" diye okuyun. JUS'ta karşılığını açıkça yazarsınız:

| Python | JUS |
|--------|-----|
| `if liste:` / `if not liste:` | `eğer uzunluk(liste) > 0:` / `eğer uzunluk(liste) == 0:` |
| `if ad:` / `if sayı:` | `eğer ad != "":` / `eğer sayı != 0:` |
| `if x is None:` | `eğer x == boş:` |
| `ad = ad or "isimsiz"` | `eğer ad == "":` bloğunda `ad = "isimsiz"` |

Python'da `ve`/`veya` karşılığı `and`/`or` da işleneni döndürür; bu yüzden `ad or "isimsiz"` kalıbı yaygındır. JUS'ta `ad veya "isimsiz"` yazarsanız `'veya' mantıksal değerler ister; metin verildi.` hatası alırsınız.

### 4.2 Sayı ile metni toplama

İkisi de sayıyla metni toplamayı reddeder; Python bunu da hata sayar.

```jus
yaz("yaş: " + 25)
```

```python
print("yaş: " + 25)
```

JUS çıktısı:

```
ornek.jus:1: çalışma zamanı hatası: '+' işleci iki sayı, iki metin ya da iki liste ister; metin ve sayı verildi. Dönüştürmek için metin() ya da sayı() kullanılabilir.
```

Python çıktısı:

```
TypeError: can only concatenate str (not "int") to str
```

Çözüm ikisinde de dönüşümdür: `"yaş: " + metin(25)` / `"yaş: " + str(25)` ya da iki dilde de `f"yaş: {25}"`. JUS iletisi türleri Türkçe söyler ve çözümü önerir; Python iletisi türleri İngilizce adlarıyla (`str`, `int`) verir. Sayıyı sola yazarsanız Python iletisi değişir: `5 + "a"` için `TypeError: unsupported operand type(s) for +: 'int' and 'str'`.

Metni sayıyla **çarpmak** ise Python'da hata değil, metni tekrarlamaktır: `"5" * 2` sonucu `55` olur. JUS'ta aynı ifade `'*' işleci iki sayı ister; metin ve sayı verildi.` hatasıdır; metni tekrarlamak için `tekrarla("5", 2)` yazılır. `"5"` yerine `5` beklerken bu sessiz sonuç bir hatanın habercisi olabilir.

### 4.3 Bölme, sayı türleri ve yuvarlama

JUS'ta tek bir `sayı` türü vardır ve `/` her zaman ondalıklı bölme yapar. Python'da `/` da her zaman ondalıklı sonuç verir, ama Python tam sayı (`int`) ile ondalıklı sayıyı (`float`) ayırır; `//` ise tam bölmedir.

```jus
yaz(7 / 2, taban(7 / 2), taban(-7 / 2))
yaz(7 % 3, -7 % 3)
yaz(6 / 3, 0.1 + 0.2)
yaz(0.1 + 0.2 == 0.3)
```

```python
print(7 / 2, 7 // 2, -7 // 2)
print(7 % 3, -7 % 3)
print(6 / 3, 0.1 + 0.2)
print(0.1 + 0.2 == 0.3)
```

JUS çıktısı:

```
3.5 3 -4
1 2
2 0.3
yanlış
```

Python çıktısı:

```
3.5 3 -4
1 2
2.0 0.30000000000000004
False
```

İlk iki satır aynı, ayrılan yer sayıların **yazılışıdır**. JUS sayıları en çok 14 anlamlı basamakla yazar ve ondalık kısmı sıfır olan sayıyı `2` diye gösterir; Python tam sayı ile ondalıklı sayıyı ayırır (`2` ile `2.0`) ve tüm basamakları yazar. Altındaki değer aynı kayan noktalı sayıdır: son satırda iki dilde de karşılaştırma yanlıştır.

Python'da bu ayrım **işe yarar**: `6 / 3` bir `float`'tır ve tam sayı bekleyen yerlerde kullanılamaz. `l[4 / 2]` JUS'ta (dizin tam sayı değerliyse) çalışır ve `l[2]` öğesini verir; Python'da `TypeError: list indices must be integers or slices, not float` hatası verir. Dizin ve `range` için `//` (ya da `int()`) kullanın: `l[4 // 2]`. `//` negatif sayılarda da aşağı yuvarlar, tıpkı `taban` gibi.

Büyük sayılar ve yuvarlama da ayrılır. JUS sayıları 64 bit kayan noktalıdır ve 2^53'e kadar kesindir; Python'un tam sayıları sınırsız büyüklüktedir. JUS'ta `yuvarla` ve `biçimle` yarım değerleri sıfırdan uzağa yuvarlar; Python 3'ün `round` işlevi yarım değerleri en yakın **çift** sayıya yuvarlar ve `2.675` ikili düzende tam temsil edilemediği için aslında `2.67499999...` değerindedir.

```jus
kullan matematik
yaz(matematik.üs(2, 100), 9007199254740993)
yaz(yuvarla(2.5), yuvarla(0.5), yuvarla(-1.5))
yaz(biçimle(0.125, 2), yuvarla(2.675, 2))
```

```python
print(2 ** 100, 9007199254740993)
print(round(2.5), round(0.5), round(-1.5))
print(f"{0.125:.2f}", round(2.675, 2))
```

JUS çıktısı:

```
1.2676506002282e+30 9007199254740992
3 1 -2
0.13 2.68
```

Python çıktısı:

```
1267650600228229401496703205376 9007199254740993
2 0 -2
0.12 2.67
```

Para gibi kesin sonuç isteyen işlerde Python'da `decimal` modülü kullanılır.

### 4.4 Tanımsız ada atama

JUS'ta `değişken` ile tanımlanmamış bir ada atamak hatadır. Python'da atama, ad yoksa yeni değişken oluşturur; yazım hatası sessizce geçer.

```jus
değişken toplam = 0
toplm = toplam + 1
yaz(toplam)
```

```python
toplam = 0
toplm = toplam + 1
print(toplam)
```

JUS çıktısı:

```
ornek.jus:2: çalışma zamanı hatası: 'toplm' adında bir değişken tanımlı değil. Yeni değişken için 'değişken toplm = ...' yazın.
```

Python çıktısı:

```
0
```

Python program "çalışır" ama `toplam` hiç artmamıştır. Bu tür hataları `pyflakes` ya da `pylint` gibi statik denetleyiciler yakalar; Python öğrenirken editörünüze bir tanesini eklemeniz değerlidir.

### 4.5 Blok kapsamı, `global` ve `nonlocal`

JUS'ta `eğer`, döngü ve `dene` blokları kendi kapsamlarını kurar: blokta tanımlanan değişken dışarıda yoktur. Python'da yalnızca **fonksiyon** (ve sınıf, modül) kapsam kurar; blok kurmaz. Döngü değişkeni de Python'da döngüden sonra yaşar.

```jus
eğer doğru:
    değişken x = 5
yaz(x)
```

```python
if True:
    x = 5
print(x)
```

JUS çıktısı:

```
ornek.jus:3: çalışma zamanı hatası: 'x' adında bir değişken ya da fonksiyon tanımlı değil.
```

Python çıktısı:

```
5
```

JUS iç blokta aynı adı yeniden tanımlayıp dış değişkeni **gölgeleyebilir** (iç blok bitince dış değişken eski değerine döner: `10`, iç blokta `20` yazdırılır); Python'da aynı ada atama dış değişkeni değiştirir ve blok sonrasında da `20` görünür.

**Genel değişkeni fonksiyonda değiştirmek.** JUS'ta fonksiyon, genel (ya da dış fonksiyondaki) bir değişkene doğrudan atayabilir. Python'da bir ada fonksiyonun içinde atarsanız o ad fonksiyonda **yerel** sayılır; genel değişkeni değiştirmek için `global` yazılır:

```jus
değişken sayaç = 0

fonksiyon artır():
    sayaç += 1

artır()
artır()
yaz(sayaç)
```

```python
sayaç = 0

def artır():
    sayaç += 1

artır()
artır()
print(sayaç)
```

JUS çıktısı:

```
2
```

Python çıktısı:

```
UnboundLocalError: cannot access local variable 'sayaç' where it is not associated with a value
```

Düzeltilmiş Python:

```python
sayaç = 0

def artır():
    global sayaç
    sayaç += 1

artır()
artır()
print(sayaç)
```

```
2
```

İç içe fonksiyonlarda, dış fonksiyonun değişkenini (kapanım) değiştirmek için `nonlocal` yazılır; JUS'ta iç fonksiyon dış değişkene doğrudan atar. Genel değişkeni yalnızca **okumak** için `global` gerekmez; listenin ya da sözlüğün içeriğini değiştirmek (`liste.append(x)`) de atama olmadığı için `global` istemez. `global` yalnızca adın kendisine yeni değer atarken gerekir.

### 4.6 Türkçe harfler: büyük/küçük harf ve sıralama

JUS Türk alfabesini bilir (`i` harfinin büyüğü `İ`, `ı` harfinin büyüğü `I`; sıralama `ç`, `ğ`, `ı`, `ö`, `ş`, `ü` harflerinin yerine göre). Python'un `upper`, `lower` ve `sorted` işlevleri Türkçe kurallarını bilmez: büyük/küçük harf dönüşümü dile bağımsız Unicode kurallarını, sıralama ise harflerin kod numaralarını kullanır.

```jus
yaz(büyük_harf("ılık"), büyük_harf("istanbul"))
yaz(küçük_harf("IŞIK"), küçük_harf("İZMİR"))
yaz(birleştir(sırala(["çay", "ağaç", "zeytin", "ılık", "iz", "şeker", "sarı"]), " "))
```

```python
print("ılık".upper(), "istanbul".upper())
print("IŞIK".lower(), "İZMİR".lower())
print(" ".join(sorted(["çay", "ağaç", "zeytin", "ılık", "iz", "şeker", "sarı"])))
```

JUS çıktısı:

```
ILIK İSTANBUL
ışık izmir
ağaç çay ılık iz sarı şeker zeytin
```

Python çıktısı:

```
ILIK ISTANBUL
işik i̇zmi̇r
ağaç iz sarı zeytin çay ılık şeker
```

Bu Python'ın hatası değil, kuralların dile bağımsız olmasının sonucudur. `"İZMİR".lower()` her `İ` için iki karakterlik `i̇` (`i` ve birleştirici nokta) verir; bu yüzden sonuç `izmir` metnine **eşit değildir**. Sıralamada `ç`, `ş`, `ı` gibi harfler `z`'den sonra, kod numaralarına göre dizilir.

Python'da Türkçe sonuç gerekiyorsa küçük bir yardımcı yazılır. Aşağıdaki örnek, yalnızca 29 harfli Türk alfabesindeki küçük harfli kelimeler için çalışır:

```python
ALFABE = "abcçdefgğhıijklmnoöprsştuüvyz"

def tr_büyük(m):
    return m.replace("i", "İ").replace("ı", "I").upper()

def tr_anahtar(kelime):
    return [ALFABE.index(h) for h in kelime]

print(tr_büyük("ılık"), tr_büyük("istanbul"))
kelimeler = ["çay", "ağaç", "zeytin", "ılık", "iz", "şeker", "sarı"]
print(" ".join(sorted(kelimeler, key=tr_anahtar)))
```

```
ILIK İSTANBUL
ağaç çay ılık iz sarı şeker zeytin
```

Python'un standart kütüphanesindeki [`locale` modülü](https://docs.python.org/3/library/locale.html) de sıralamayı işletim sisteminin yerel ayarına göre yapabilir; ancak sonuç sistemde `tr_TR` ayarının kurulu olmasına bağlıdır. Kodunuzun her bilgisayarda aynı çalışması gerekiyorsa yukarıdaki gibi kendi anahtarınızı yazmak daha güvenlidir.

### 4.7 Dizin dışı erişim ve olmayan sözlük anahtarı

İkisinde de hata verir; ileti biçimleri farklıdır (örnekler 5. bölümde çalıştırılmıştır):

| Durum | JUS iletisi | Python iletisi |
|-------|-------------|----------------|
| `[1, 2, 3][5]` | `Dizin sınırların dışında: uzunluk 3, istenen dizin 5.` | `IndexError: list index out of range` |
| `{"a": 1}["b"]` | `Sözlükte "b" anahtarı yok.` | `KeyError: 'b'` |

JUS iletisi uzunluğu, istenen dizini ve anahtarı açıkça yazar; Python iletisi daha kısadır ve `KeyError` yalnızca eksik anahtarı gösterir. Metinde `"abc"[3]` da ikisinde hatadır (Python: `IndexError: string index out of range`). `d["b"] += 1` anahtar yoksa ikisinde de hatadır; önce anahtarı `içinde` / `in` ile denetleyin ya da `al` / `get` kullanın.

### 4.8 Aynı çalışanlar

Her şey farklı değil. Aşağıdaki işlemler iki dilde aynı sonucu verir: negatif dizin, sınır dışına taşan dilim (kırpılır, hata vermez), sözlükte ekleme sırasıyla gezinme, metinlerde karakter bazlı dizin ve uzunluk.

```jus
değişken d = {"a": 1, "b": 2}
değişken l = [10, 20, 30, 40]
yaz(l[-1], l[-2], l[1:3], l[2:99], l[5:])
her k içinde d:
    yaz(k, d[k])
yaz(uzunluk(d), "çay"[0], "çay"[-1], "çay"[1:])
```

```python
d = {"a": 1, "b": 2}
l = [10, 20, 30, 40]
print(l[-1], l[-2], l[1:3], l[2:99], l[5:])
for k in d:
    print(k, d[k])
print(len(d), "çay"[0], "çay"[-1], "çay"[1:])
```

```
40 30 [20, 30] [30, 40] []
a 1
b 2
2 ç y ay
```

Python'da sözlük üzerinde `for k in d` yalnızca anahtarları verir (JUS'taki gibi); değerler için `d.values()`, ikisi için `d.items()` gerekir. Python 3.7'den beri sözlükler de ekleme sırasını korur.

### 4.8A Varsayılan değer her çağrıda yeniden hesaplanır

Varsayılan değerli parametre iki dilde de aynı yazılır, ama varsayılan ifadenin **ne zaman** hesaplandığı farklıdır. Python ifadeyi `def` satırı çalışırken bir kez hesaplar ve aynı nesneyi bütün çağrılarda kullanır; JUS her çağrıda, gövde başlamadan önce yeniden hesaplar. Fark, varsayılan değer liste ya da sözlük gibi değiştirilebilir bir nesne olduğunda görünür.

```jus
fonksiyon ekle_öğe(öğe, liste = []):
    ekle(liste, öğe)
    dön liste

yaz(ekle_öğe(1))
yaz(ekle_öğe(2))
yaz(ekle_öğe(3, [10]))
```

```
[1]
[2]
[10, 3]
```

Aynı program Python'da:

```python
def ekle_öğe(öğe, liste=[]):
    liste.append(öğe)
    return liste

print(ekle_öğe(1))
print(ekle_öğe(2))
print(ekle_öğe(3, [10]))
```

Python çıktısı:

```
[1]
[1, 2]
[10, 3]
```

Python'da `liste=[]` bir kez oluşturulur ve ikinci çağrıda birinci çağrının eklediği öğe hâlâ oradadır. Python'da bu yüzden `liste=None` yazıp gövdede `if liste is None: liste = []` ile yeni liste oluşturmak alışkanlıktır; JUS'ta buna gerek yoktur, `liste = []` her çağrıda yeni bir liste verir. Aynı kural yan etkili ifadelere de uygulanır: varsayılan bir fonksiyon çağrısıysa JUS'ta her çağrıda çalışır, Python'da yalnızca tanım anında bir kez.

İkinci fark: JUS'ta varsayılan ifade kendinden önceki parametreleri kullanabilir (`fonksiyon kayıt(ad, etiket = ad + "-1")`). Python'da aynı yazım `def` satırı çalışırken hata verir (`NameError: name 'ad' is not defined`); orada `etiket=None` yazıp gövdede `ad`'dan türetmek gerekir. Ayrıca JUS'ta açıkça verilen `boş`, varsayılanı devreye sokmaz; parametre `boş` olur (Python'da `None` verildiğinde de böyledir).

### 4.9 Python'da olup JUS'ta olmayanlar

Python, JUS'tan çok daha geniştir. JUS'ta olmayan ve Python'da sık karşılaşacağınız özellikler:

| Python özelliği | JUS'ta durum |
|-----------------|--------------|
| zincirleme karşılaştırma `1 < x < 10` | yok; `1 < x ve x < 10` yazılır. JUS `1 < x < 10` ifadesini `(1 < x) < 10` diye ayrıştırır ve `'<' işleci iki sayı ya da iki metin ister; mantıksal ve sayı verildi.` hatası verir |
| `lambda x: x * x` | yok; adlı `fonksiyon` tanımlayıp adını verirsiniz |
| liste, sözlük, küme kapsamları (`[x*x for x in l]`) | yok; `eşle`, `süz` ya da döngü |
| demet `(1, 2)` ve küme `{1, 2}` türleri | yok; yalnızca liste ve sözlük |
| f-metinlerinde biçim belirteci (`f"{x:.2f}"`, `f"{x:>8}"`) ve `f"{x=}"` | yok; `{}` içinde `biçimle()`, `sola_doldur()` çağrılır. `f"{x}"` metinleri JUS'ta vardır |
| anahtar sözcüklü argümanlar (`f(b=1)`), `**kwargs`, yalnızca anahtar sözcükle verilen parametreler | yok; argümanlar sırayla verilir. Varsayılan değerli ve `*kalan` parametreleri JUS'ta vardır |
| çağrıda liste açma (`f(*liste)`) ve sözlük açma (`f(**sözlük)`) | yok |
| `//`, `**` işleçleri | yok; `taban(a / b)`, `matematik.üs(a, b)` |
| `a, b = b, a` çoklu atama ve açma | yok |
| `with`, `try ... finally`, `try ... else`, `for ... else` | yok |
| tek tırnaklı ve üç tırnaklı çok satırlı metinler | yok; çift tırnak, tek satır |
| `is`, `is not`, `not in` | yok; `==`, `!=`, `değil x içinde k` |
| üreteçler (`yield`), dekoratörler (`@`), `async` | yok |
| çoklu kalıtım, özel yöntemler (`__str__` ...), `@property` | yok; tek üst sınıf |
| tür ipuçları (`def f(x: int) -> int`) | yok |
| `global`, `nonlocal` | gerekmez (4.5) |

Öğrenmeye değer olanlar 7. bölümde sıralanmıştır.

## 5. Python hata iletileri sözlüğü

Python bir hatada **izleme** (traceback) yazar. JUS'un çağrı zincirinin tersine, Python en eski çağrıyı üstte, hatanın asıl oluştuğu satırı **altta** gösterir ve hatanın türüyle iletisini en son satıra yazar. Önce son satırı okuyun; sonra hatanın hangi satırda olduğunu bir üstteki `File ..., line N` satırlarından bulun.

```jus
fonksiyon böl(a, b):
    dön a / b

yaz(böl(10, 0))
```

```python
def böl(a, b):
    return a / b

print(böl(10, 0))
```

JUS çıktısı:

```
ornek.jus:2: çalışma zamanı hatası: Sıfıra bölünemez.
    satır 2, 'böl' fonksiyonu
    satır 4, ana program
```

Python çıktısı:

```
Traceback (most recent call last):
  File "ornek.py", line 4, in <module>
    print(böl(10, 0))
          ^^^^^^^^^^
  File "ornek.py", line 2, in böl
    return a / b
           ~~^~~
ZeroDivisionError: division by zero
```

Python 3.11'den itibaren kodun ilgili parçasını `^` ve `~` işaretleriyle altlar. Aşağıdaki maddelerde her hata için Python'un gerçek iletisi (izlemenin son satırı), anlamı ve JUS'un aynı durumdaki gerçek iletisi verilmiştir.

**NameError**: Tanımlanmamış bir adı kullandınız (yazım hatası ya da atamadan önce kullanım).

- Python: `print(toplam)` → `NameError: name 'toplam' is not defined`
- JUS: `yaz(toplam)` → `ornek.jus:1: çalışma zamanı hatası: 'toplam' adında bir değişken ya da fonksiyon tanımlı değil.`

**TypeError (tür uyuşmazlığı)**: Bir işleç ya da işlev bu türdeki değerle çalışmaz.

- Python: `print("yaş: " + 25)` → `TypeError: can only concatenate str (not "int") to str`
- JUS: `yaz("yaş: " + 25)` → `ornek.jus:1: çalışma zamanı hatası: '+' işleci iki sayı, iki metin ya da iki liste ister; metin ve sayı verildi. Dönüştürmek için metin() ya da sayı() kullanılabilir.`

**TypeError (argüman sayısı)**: Fonksiyona yanlış sayıda argüman verdiniz. Python yöntemlerde `self`'i de sayar; bu yüzden `A.f() missing 1 required positional argument: 'x'` gibi iletilerdeki sayı JUS'tan bir fazla görünebilir. `self` yazmayı unutursanız (`def f():`) ileti `A.f() takes 0 positional arguments but 1 was given` olur; JUS'ta `bu` zaten hazır olduğu için parametre listesine yazılmaz ve bu hata oluşmaz.

- Python: `print(len())` → `TypeError: len() takes exactly one argument (0 given)`
- JUS: `yaz(uzunluk())` → `ornek.jus:1: çalışma zamanı hatası: 'uzunluk' fonksiyonu 1 argüman bekliyor, 0 verildi.`

Varsayılan değerli fonksiyonlarda iki ileti de aralığı söyler. `def f(a, b=2)` ve `fonksiyon f(a, b = 2)` için üç argüman verildiğinde:

- Python: `f() takes from 1 to 2 positional arguments but 3 were given`
- JUS: `ornek.jus:4: çalışma zamanı hatası: 'f' fonksiyonu en az 1, en çok 2 argüman bekliyor, 3 verildi.`

`*` parametresi olan fonksiyonun üst sınırı olmadığından JUS yalnızca alt sınırı söyler: `'g' fonksiyonu en az 1 argüman bekliyor, 0 verildi.` (Python: `g() missing 1 required positional argument: 'a'`).

**IndexError**: Listede ya da metinde olmayan bir dizine eriştiniz.

- Python: `print([1, 2, 3][5])` → `IndexError: list index out of range`
- JUS: `yaz([1, 2, 3][5])` → `ornek.jus:1: çalışma zamanı hatası: Dizin sınırların dışında: uzunluk 3, istenen dizin 5.`

**KeyError**: Sözlükte olmayan bir anahtarı okudunuz.

- Python: `print({"a": 1}["b"])` → `KeyError: 'b'`
- JUS: `yaz({"a": 1}["b"])` → `ornek.jus:1: çalışma zamanı hatası: Sözlükte "b" anahtarı yok.`

**ZeroDivisionError**: Sıfıra böldünüz ya da sıfıra göre kalan aldınız.

- Python: `print(1 / 0)` → `ZeroDivisionError: division by zero`
- JUS: `yaz(1 / 0)` → `ornek.jus:1: çalışma zamanı hatası: Sıfıra bölünemez.`

**ValueError**: Değerin türü doğru ama içeriği uygun değil (ör. sayıya çevrilemeyen metin).

- Python: `print(int("abc"))` → `ValueError: invalid literal for int() with base 10: 'abc'`
- JUS: `yaz(sayı("abc"))` → `ornek.jus:1: çalışma zamanı hatası: "abc" bir sayıya dönüştürülemez.`

**AttributeError**: Nesnenin böyle bir özelliği ya da yöntemi yok. JUS'tan gelenler sık sık `"abc".büyük_harf()` gibi JUS fonksiyonlarını yöntem sanarak yazar (Python'da doğrusu `"abc".upper()`).

- Python: `print("abc".büyük_harf())` → `AttributeError: 'str' object has no attribute 'büyük_harf'`
- JUS: `yaz("abc".büyük_harf())` → `ornek.jus:1: çalışma zamanı hatası: metin türündeki değerlerin 'büyük_harf' adında bir yöntemi yok.`

**ModuleNotFoundError**: İçe aktarılan modül bulunamadı (yazım hatası ya da kurulu olmayan paket).

- Python: `import olmayan_modul` → `ModuleNotFoundError: No module named 'olmayan_modul'`
- JUS: `kullan olmayan_modul` → `ornek.jus:1: çalışma zamanı hatası: 'olmayan_modul' modülü yüklenemedi: 'olmayan_modul.jus' için dosya açılamadı. Kurulu paketlerde de yok ('jus_paketleri/olmayan_modul/olmayan_modul.jus').`

**FileNotFoundError**: Açılmak istenen dosya yok ya da yol yanlış.

- Python: `open("yok.txt", encoding="utf-8")` → `FileNotFoundError: [Errno 2] No such file or directory: 'yok.txt'`
- JUS: `kullan dosya`, `dosya.oku("yok.txt")` → `ornek.jus:2: çalışma zamanı hatası: 'yok.txt' okunamadı: dosya açılamadı.`

**AssertionError**: `assert` ile beklenen koşul sağlanmadı.

- Python: `assert 1 == 2, "olmamalıydı"` → `AssertionError: olmamalıydı`
- JUS: `doğrula(1 == 2, "olmamalıydı")` → `ornek.jus:1: çalışma zamanı hatası: Doğrulama başarısız: olmamalıydı`

**JSONDecodeError** (`ValueError`'ın bir alt türü): Çözülmek istenen metin geçerli JSON değil.

- Python: `import json`, `json.loads("{bozuk")` → `json.decoder.JSONDecodeError: Expecting property name enclosed in double quotes: line 1 column 2 (char 1)`
- JUS: `kullan json`, `json.çöz("{bozuk")` → `ornek.jus:2: çalışma zamanı hatası: Geçersiz JSON (2. karakter): anahtar olarak metin bekleniyor.`

**UnboundLocalError**: Fonksiyonda atadığınız bir adı, genel değişken sanarak kullandınız. JUS'ta aynı kod hata vermez; ayrıntı ve düzeltme 4.5'tedir.

- Python: `sayaç = 0`, `def artır(): sayaç += 1`, `artır()` → `UnboundLocalError: cannot access local variable 'sayaç' where it is not associated with a value`

**SyntaxError**: Yazım hatası; program hiç başlamaz (JUS'ta da sözdizimi hataları yakalanamaz ve program hiç başlamaz). Sözdizimi hatalarında Python izleme başlığı yazmaz, hatalı satırı `^` ile işaretler. Bir `if` satırında `:` unutulursa:

```jus
eğer doğru
    yaz(1)
```

```python
if True
    print(1)
```

JUS çıktısı:

```
ornek.jus:1:11: sözdizimi hatası: Blok başlatmak için ':' bekleniyor.
    eğer doğru
              ^
```

Python çıktısı:

```
  File "ornek.py", line 1
    if True
           ^
SyntaxError: expected ':'
```

Sözdizimi hatalarının JUS iletisi üç satırdır (ileti, kaynak satır, `^` işareti); aşağıdaki maddelerde yalnızca ilk satır gösterilmiştir. Python için de yalnızca son satır gösterilmiştir.

**IndentationError** (`SyntaxError`'ın bir alt türü): Girinti hatası; `:` işaretinden sonra girintili blok yok (burada dosyanın tek satırı `if True:` ya da `eğer doğru:`).

- Python: `if True:` → `IndentationError: expected an indented block after 'if' statement on line 1`
- JUS: `eğer doğru:` → `ornek.jus:2:1: sözdizimi hatası: ':' işaretinden sonra girintili bir blok bekleniyor.`

**SyntaxError** (kapanmamış parantez): Parantez açıp kapatmayı unuttunuz.

- Python: `print("a"` → `SyntaxError: '(' was never closed`
- JUS: `yaz("a"` → `ornek.jus:2:1: sözdizimi hatası: Argümanlardan sonra ')' bekleniyor.`

Girinti konusunda Python da JUS kadar katıdır: bir blokta girinti düzeylerini karıştırmak ya da aynı dosyada boşlukla sekmeyi birlikte kullanmak hata verir. Python'da girinti için dört boşluk önerilir.

## 6. Baştan sona bir örnek

Aşağıdaki program öğrencilerin notlarını işler: bir sınıf, listeler, bir sözlük, bir fonksiyon ve hata yakalama kullanır. İki dilde aynı çıktıyı verir.

```jus
# Öğrenci notlarını işler: ortalama, harf notu ve harf dağılımı.

sınıf Öğrenci:
    fonksiyon kur(ad):
        bu.ad = ad
        bu.notlar = []

    fonksiyon not_ekle(puan):
        eğer puan < 0 veya puan > 100:
            fırlat "Geçersiz not: " + metin(puan)
        ekle(bu.notlar, puan)

    fonksiyon ortalama():
        eğer uzunluk(bu.notlar) == 0:
            fırlat bu.ad + " için not yok"
        değişken toplam = 0
        her n içinde bu.notlar:
            toplam += n
        dön toplam / uzunluk(bu.notlar)

fonksiyon harf_notu(ort):
    eğer ort >= 85:
        dön "A"
    değilse eğer ort >= 70:
        dön "B"
    değilse eğer ort >= 50:
        dön "C"
    dön "F"

değişken girişler = [
    ["Ayşe", [90, 85, 100]],
    ["Burak", [70, 65, 80]],
    ["Çağla", [40, 55]],
    ["Deniz", [60, 120]],
    ["Ece", []],
]

değişken sınıf_listesi = []
her giriş içinde girişler:
    değişken kişi = Öğrenci(giriş[0])
    dene:
        her puan içinde giriş[1]:
            kişi.not_ekle(puan)
        ekle(sınıf_listesi, kişi)
    yakala hata:
        yaz("Atlandı:", hata)

değişken dağılım = {}
her öğrenci içinde sınıf_listesi:
    dene:
        değişken ort = öğrenci.ortalama()
        değişken harf = harf_notu(ort)
        dağılım[harf] = al(dağılım, harf, 0) + 1
        yaz(sağa_doldur(öğrenci.ad, 8), biçimle(ort, 1), harf)
    yakala hata:
        yaz("Hesaplanamadı:", hata)

her harf içinde sırala(anahtarlar(dağılım)):
    yaz(harf + ":", dağılım[harf])
```

```python
# Öğrenci notlarını işler: ortalama, harf notu ve harf dağılımı.

class Öğrenci:
    def __init__(self, ad):
        self.ad = ad
        self.notlar = []

    def not_ekle(self, puan):
        if puan < 0 or puan > 100:
            raise ValueError("Geçersiz not: " + str(puan))
        self.notlar.append(puan)

    def ortalama(self):
        if len(self.notlar) == 0:
            raise ValueError(self.ad + " için not yok")
        toplam = 0
        for n in self.notlar:
            toplam += n
        return toplam / len(self.notlar)

def harf_notu(ort):
    if ort >= 85:
        return "A"
    elif ort >= 70:
        return "B"
    elif ort >= 50:
        return "C"
    return "F"

girişler = [
    ["Ayşe", [90, 85, 100]],
    ["Burak", [70, 65, 80]],
    ["Çağla", [40, 55]],
    ["Deniz", [60, 120]],
    ["Ece", []],
]

sınıf_listesi = []
for giriş in girişler:
    kişi = Öğrenci(giriş[0])
    try:
        for puan in giriş[1]:
            kişi.not_ekle(puan)
        sınıf_listesi.append(kişi)
    except ValueError as hata:
        print("Atlandı:", hata)

dağılım = {}
for öğrenci in sınıf_listesi:
    try:
        ort = öğrenci.ortalama()
        harf = harf_notu(ort)
        dağılım[harf] = dağılım.get(harf, 0) + 1
        print(öğrenci.ad.ljust(8), f"{ort:.1f}", harf)
    except ValueError as hata:
        print("Hesaplanamadı:", hata)

for harf in sorted(dağılım):
    print(harf + ":", dağılım[harf])
```

```
Atlandı: Geçersiz not: 120
Ayşe     91.7 A
Burak    71.7 B
Çağla    47.5 F
Hesaplanamadı: Ece için not yok
A: 1
B: 1
F: 1
```

Bu programda gördüğünüz çeviri kalıpları:

- `sınıf` / `kur` / `bu` yerine `class` / `__init__` / `self`; yöntemlerin ilk parametresi `self`.
- `fırlat "ileti"` yerine `raise ValueError("ileti")`; `yakala hata:` yerine `except ValueError as hata:`.
- `ekle(liste, x)` yerine `liste.append(x)`; `uzunluk(l)` yerine `len(l)`; `al(d, k, 0)` yerine `d.get(k, 0)`.
- `sağa_doldur(ad, 8)` yerine `ad.ljust(8)`; `biçimle(ort, 1)` yerine `f"{ort:.1f}"`; `sırala(anahtarlar(d))` yerine `sorted(d)`.
- `değişken` yerine düz atama; `veya` yerine `or`; `her ... içinde` yerine `for ... in`.
- `dene` bloğunda `değişken` ile tanımlanan `ort` ve `harf` JUS'ta blokla sınırlıdır; bu yüzden hepsi aynı blokta kullanıldı. Python'da bu adlar blok dışında da yaşar (4.5).

## 7. Sonraki adımlar

JUS'tan Python'a geçerken şu sırayı izlemeniz işe yarar:

1. **Python'u kurun** ([python.org/downloads](https://www.python.org/downloads/)) ve etkileşimli kipi (`python`) açın. JUS'taki `jus` kipi gibi çalışır: bir satır yazarsınız, sonucu hemen görürsünüz.
2. **Eski JUS programlarınızı çevirin.** [Küçük projeler](rehber/12-kucuk-projeler.md) bölümündeki programları Python'a taşıyın ve çıktıları karşılaştırın. Bu belgedeki 6. bölüm bu çalışmanın bir örneğidir.
3. **Resmî öğreticiyi okuyun:** [Python Tutorial](https://docs.python.org/3/tutorial/index.html). Bu belgenin atladığı konuları anlatır: [veri yapıları](https://docs.python.org/3/tutorial/datastructures.html) (demetler, kümeler, liste kapsamları), [modüller ve paketler](https://docs.python.org/3/tutorial/modules.html), [hatalar ve istisnalar](https://docs.python.org/3/tutorial/errors.html), [sınıflar](https://docs.python.org/3/tutorial/classes.html). Belgelerin kısmî bir Türkçe çevirisi de vardır ([docs.python.org/tr/3](https://docs.python.org/tr/3/)); gerektiğinde İngilizce sürüme bakın.
4. **Bu belgede kısaca değinilen konuları sırasıyla öğrenin:**
   - f-metinlerinin biçim belirteçleri (`:.2f`, `:>8`) ve metin yöntemleri ([yerleşik türler](https://docs.python.org/3/builtins/stdtypes.html))
   - anahtar sözcüklü argümanlar, `**kwargs`, `lambda`, liste kapsamları
   - demet, küme; `with` deyimi; `try` / `finally`
   - özel yöntemler (`__str__`, `__eq__`) ve `@property`
   - sanal ortam ve paketler: [`venv` öğreticisi](https://docs.python.org/3/tutorial/venv.html), [`pip` ile modül kurma](https://docs.python.org/3/installing/index.html)
   - test yazmak: [`unittest`](https://docs.python.org/3/library/unittest.html)
5. **Başvuru olarak:** [standart kütüphane](https://docs.python.org/3/library/index.html), [yerleşik fonksiyonlar](https://docs.python.org/3/builtins/functions.html), [yerleşik hatalar](https://docs.python.org/3/builtins/exceptions.html) ve [dil başvurusu](https://docs.python.org/3/reference/index.html). Kod yazım düzeni için [PEP 8](https://peps.python.org/pep-0008/), Python topluluğunun ortak stil kılavuzudur.

Bir hatayla karşılaştığınızda iletinin son satırını okuyun ve bu belgenin 4. ile 5. bölümlerinden ilgili maddeye bakın. JUS tarafında takıldığınızda [JUS rehberine](rehber/README.md) ve [dil tanımına](dil-tanimi.md) dönebilirsiniz.
