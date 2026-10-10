# Deneme alanı

JUS programlarını kurulum yapmadan, tarayıcıda çalıştıran sayfa. Yorumlayıcı
WebAssembly'ye derlenir; programlar tümüyle tarayıcıda çalışır.

Yayımlanmış hâli: <https://kerimaksanaoglu.github.io/jus/>

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

## Paylaşım bağlantısı

"Paylaş" düğmesi programı sıkıştırıp adres çubuğundaki `#program=` parçasına
yazar ve bağlantıyı panoya kopyalar. Pano kullanılamıyorsa bağlantı, elle
kopyalanabilmesi için salt okunur bir kutuda gösterilir. Program bağlantının
içinde taşınır; sunucuya hiçbir şey gönderilmez. Böyle bir bağlantı açıldığında
program düzenleyiciye yüklenir ve örnekler listesinde "(paylaşılan program)"
olarak seçilir; çözülemezse çıktı alanına not düşülür ve ilk örnek açılır.

Bağlantı yoksa son düzenlenen program tarayıcının `localStorage` alanında
(`jus.sonProgram`) saklanır ve sayfa açılırken geri yüklenir. Örnek seçmek
adresteki `#program=` parçasını temizler.

Biçim `<önek>:<base64url>` şeklindedir: `z:` UTF-8 metnin `deflate-raw` ile
sıkıştırılmış, `k:` sıkıştırılmamış hâlidir. Tarayıcıda `CompressionStream`
yoksa sıkıştırmasız yazılır; çözerken her iki önek de kabul edilir.

- `paylas.js`: kodlama ve çözme (`programiKodla`, `programiCoz`). Tarayıcıda
  `window.JUSPaylas` olarak, Node.js'te `require` ile kullanılır.
- `dene_paylas.cjs`: `paylas.js` sınaması. Emscripten gerektirmez.

```sh
node playground/dene_paylas.cjs
```

## Sınırlar

- `oku`, `dosya`, `ağ`, `http` ve `sistem.ortam` tarayıcıda kullanılamaz.
- Program ayrı bir iş parçacığında çalışır; sonsuz döngüye giren program
  "Durdur" düğmesiyle sonlandırılabilir.
- Paylaşım bağlantısı için kodlanmış parça en çok 8000 karakter olabilir; daha
  uzun programlar paylaşılamaz. Çözülürken açılmış program 1 MiB ile sınırlıdır.
- Node.js sınaması yorumlayıcının WebAssembly derlemesini doğrular. Sayfanın
  kendisi (düğmeler, çıktı alanı) otomatik olarak sınanmaz; Chrome'da elle
  denenmiştir.
