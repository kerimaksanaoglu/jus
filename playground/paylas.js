// Programı bağlantıya sığacak bir metne çevirir ve geri çözer.
// Sayfadan bağımsızdır: tarayıcıda window.JUSPaylas, Node.js'te require ile kullanılır.
//
// Biçim: "<önek>:<base64url>"
//   z:  UTF-8 baytları deflate-raw ile sıkıştırılmış
//   k:  UTF-8 baytları olduğu gibi
(function () {
  'use strict';

  const ALFABE = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_';
  // Açılmış program için üst sınır; küçük bir bağlantıyla şişirilmiş veriyi engeller.
  const ACILMIS_SINIR = 1 << 20;

  const COZUM = new Int16Array(128).fill(-1);
  for (let i = 0; i < ALFABE.length; i++) COZUM[ALFABE.charCodeAt(i)] = i;

  function base64urlYaz(bayt) {
    let sonuc = '';
    let i = 0;
    for (; i + 2 < bayt.length; i += 3) {
      const n = (bayt[i] << 16) | (bayt[i + 1] << 8) | bayt[i + 2];
      sonuc += ALFABE[n >> 18] + ALFABE[(n >> 12) & 63] + ALFABE[(n >> 6) & 63] + ALFABE[n & 63];
    }
    if (bayt.length - i === 1) {
      const n = bayt[i] << 16;
      sonuc += ALFABE[n >> 18] + ALFABE[(n >> 12) & 63];
    } else if (bayt.length - i === 2) {
      const n = (bayt[i] << 16) | (bayt[i + 1] << 8);
      sonuc += ALFABE[n >> 18] + ALFABE[(n >> 12) & 63] + ALFABE[(n >> 6) & 63];
    }
    return sonuc;
  }

  // Geçersiz girdide null döndürür.
  function base64urlOku(metin) {
    const uzunluk = metin.length;
    if (uzunluk % 4 === 1) return null;
    const bayt = new Uint8Array(Math.floor((uzunluk * 3) / 4));
    let k = 0;
    let birikim = 0;
    let bit = 0;
    for (let i = 0; i < uzunluk; i++) {
      const kod = metin.charCodeAt(i);
      const deger = kod < 128 ? COZUM[kod] : -1;
      if (deger < 0) return null;
      birikim = (birikim << 6) | deger;
      bit += 6;
      if (bit >= 8) {
        bit -= 8;
        bayt[k++] = (birikim >> bit) & 255;
        birikim &= (1 << bit) - 1;
      }
    }
    return bayt;
  }

  // Baytları verilen dönüştürme akışından geçirir. Akış hata verirse reddeder.
  async function akistanGecir(girdi, akis, sinir) {
    const yazici = akis.writable.getWriter();
    // Yazma tarafındaki hata okuma tarafında da görüneceği için burada yutulur.
    yazici.write(girdi).catch(() => {});
    yazici.close().catch(() => {});
    const okuyucu = akis.readable.getReader();
    const parcalar = [];
    let toplam = 0;
    for (;;) {
      const { done, value } = await okuyucu.read();
      if (done) break;
      toplam += value.length;
      if (sinir && toplam > sinir) {
        okuyucu.cancel().catch(() => {});
        throw new Error('açılmış veri çok büyük');
      }
      parcalar.push(value);
    }
    const sonuc = new Uint8Array(toplam);
    let konum = 0;
    for (const parca of parcalar) {
      sonuc.set(parca, konum);
      konum += parca.length;
    }
    return sonuc;
  }

  async function programiKodla(metin) {
    const bayt = new TextEncoder().encode(String(metin));
    if (typeof CompressionStream === 'function') {
      try {
        const sikisik = await akistanGecir(bayt, new CompressionStream('deflate-raw'), 0);
        // Çok kısa metinlerde sıkıştırma çıktıyı büyütebilir; o zaman ham yazılır.
        if (sikisik.length < bayt.length) return 'z:' + base64urlYaz(sikisik);
      } catch (hata) {
        // Sıkıştırma kullanılamadı; sıkıştırmasız yazılır.
      }
    }
    return 'k:' + base64urlYaz(bayt);
  }

  async function programiCoz(parca) {
    try {
      if (typeof parca !== 'string' || parca.length < 2 || parca[1] !== ':') return null;
      const onek = parca[0];
      if (onek !== 'z' && onek !== 'k') return null;
      let bayt = base64urlOku(parca.slice(2));
      if (bayt === null) return null;
      if (onek === 'z') {
        if (typeof DecompressionStream !== 'function') return null;
        bayt = await akistanGecir(bayt, new DecompressionStream('deflate-raw'), ACILMIS_SINIR);
      } else if (bayt.length > ACILMIS_SINIR) {
        return null;
      }
      return new TextDecoder('utf-8', { fatal: true, ignoreBOM: true }).decode(bayt);
    } catch (hata) {
      return null;
    }
  }

  const api = { programiKodla, programiCoz };
  if (typeof module !== 'undefined' && module.exports) {
    module.exports = api;
  } else {
    globalThis.JUSPaylas = api;
  }
})();
