# Deneme alanı

JUS programlarını kurulum yapmadan, tarayıcıda çalıştıran sayfa. Yorumlayıcı
WebAssembly'ye derlenir; programlar tümüyle tarayıcıda çalışır.

## Derleme

[Emscripten](https://emscripten.org) kurulu olmalıdır.

```sh
bash playground/derle.sh      # playground/jus.js ve playground/jus.wasm üretir
node playground/dene.cjs      # derlemeyi Node.js altında sınar
```

Üretilen `jus.js` ve `jus.wasm` depoya eklenmez. Sürekli tümleştirme her
değişiklikte bu derlemeyi yapar, sınar ve sayfayı `deneme-alani` adıyla
indirilebilir bir paket olarak sunar.

## Çalıştırma

Sayfa dosya sisteminden doğrudan açılamaz (tarayıcılar buna izin vermez);
klasörü birlikte gelen küçük sunucuyla sunun:

```sh
python playground/sun.py 8000
```

Ardından tarayıcıda `http://127.0.0.1:8000` adresini açın.

`python -m http.server` yerine bu betiği kullanın: Python'un hazır sunucusu
bazı Windows kurulumlarında `.js` dosyalarını yanlış içerik türüyle gönderir ve
tarayıcı yorumlayıcıyı yüklemeyi reddeder.

## Sınırlar

- `oku`, `dosya`, `ağ`, `http` ve `sistem.ortam` tarayıcıda kullanılamaz.
- Program ayrı bir iş parçacığında çalışır; sonsuz döngüye giren program
  "Durdur" düğmesiyle sonlandırılabilir.
- Node.js sınaması yorumlayıcının WebAssembly derlemesini doğrular. Sayfanın
  kendisi (düğmeler, çıktı alanı) otomatik olarak sınanmaz; Chrome'da elle
  denenmiştir.
