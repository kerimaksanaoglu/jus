# JUS

![JUS](logo.svg#only-light){ .jus-logo }
![JUS](logo-acik.svg#only-dark){ .jus-logo }

JUS, Türkçe sözdizimli, genel amaçlı bir betik dilidir. Programlamaya yeni
başlayanların kavramları ana dillerinde öğrenebilmesi için tasarlanmıştır:
anahtar kelimeler de, hata iletileri de Türkçedir.

```jus
fonksiyon selamla(ad):
    dön "Merhaba, " + ad + "!"

her ad içinde ["Ayşe", "Mehmet", "Zeynep"]:
    yaz(selamla(ad))
```

```
Merhaba, Ayşe!
Merhaba, Mehmet!
Merhaba, Zeynep!
```

## Nereden başlamalı

| Amacınız | Gidilecek yer |
|----------|---------------|
| Hemen denemek | [Tarayıcıdaki deneme alanı](https://kerimaksanaoglu.github.io/jus/); kurulum gerekmez |
| Bilgisayara kurmak | [Kurulum](kurulum.md) |
| Programlamayı sıfırdan öğrenmek | [Başlangıç rehberi](rehber/README.md) |
| Python'a geçmek ya da Python'dan gelmek | [JUS'tan Python'a](python-koprusu.md) |
| Bir özelliğin tam tanımına bakmak | [Dil tanımı](dil-tanimi.md) |

## JUS'u ayıran özellikler

- **Türkçe ve anlaşılır hata iletileri.** Hata iletisi dosyayı, satırı ve
  sütunu gösterir; neyin yanlış olduğunu Türkçe söyler.
- **Kurulumsuz çalışma.** Programlar tarayıcıda çalıştırılabilir.
- **Türkçeye duyarlı metin işlemleri.** `büyük_harf("ılık")` sonucu `"ILIK"`
  olur; alfabetik sıralamada `ç`, `ğ`, `ı`, `ö`, `ş`, `ü` doğru yerlerindedir.
- **Gerçek bir dil.** Fonksiyonlar, listeler, sözlükler, sınıflar, modüller,
  hata yakalama, JSON ve HTTP desteği vardır.

JUS bir varış noktası değil, bir köprüdür: döngüyü, fonksiyonu, listeyi JUS ile
kavrayan biri Python gibi yaygın dillere geçtiğinde yalnızca kelimeler değişir.

## Kaynak kod

JUS açık kaynaklıdır ve MIT lisansı ile dağıtılır. Kaynak kod, sürümler ve hata
bildirimi için: [github.com/kerimaksanaoglu/jus](https://github.com/kerimaksanaoglu/jus)
