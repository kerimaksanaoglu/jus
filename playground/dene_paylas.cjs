// paylas.js sınaması. Çalıştırma: node playground/dene_paylas.cjs
'use strict';
const zlib = require('node:zlib');
const { programiKodla, programiCoz } = require('./paylas.js');

let basarisiz = 0;
let sayac = 0;

function denetle(kosul, ad) {
  sayac++;
  if (kosul) {
    console.log('ok   ' + ad);
  } else {
    basarisiz++;
    console.log('HATA ' + ad);
  }
}

function base64url(bayt) {
  return Buffer.from(bayt).toString('base64url');
}

async function gidisDonus(ad, metin) {
  const kod = await programiKodla(metin);
  const geri = await programiCoz(kod);
  denetle(geri === metin, 'gidiş dönüş: ' + ad + ' (' + kod.length + ' karakter)');
  denetle(/^[kz]:[A-Za-z0-9_-]*$/.test(kod), 'yalnızca URL-güvenli karakter: ' + ad);
  return kod;
}

// Tekrarlanmayan, sıkıştırılması zor metin üretmek için sabit tohumlu üreteç.
function sahteRastgele(tohum) {
  let x = tohum >>> 0;
  return () => {
    x = (Math.imul(x, 1664525) + 1013904223) >>> 0;
    return x / 4294967296;
  };
}

async function main() {
  const kisa = 'yaz("Merhaba, dünya! ığüşöçİĞÜŞÖÇ")';
  const cokSatirli = [
    'fonksiyon faktöriyel(n):',
    '    eğer n <= 1:',
    '        dön 1',
    '    dön n * faktöriyel(n - 1)',
    '',
    'her n içinde [1, 5, 10, 20]:',
    '    yaz(metin(n) + "! =", faktöriyel(n))',
    '',
  ].join('\n');
  const yirmiKb = cokSatirli.repeat(Math.ceil(20480 / cokSatirli.length)).slice(0, 20480);
  const rast = sahteRastgele(42);
  let karisik = '';
  while (karisik.length < 20480) {
    karisik += String.fromCharCode(0x20 + Math.floor(rast() * 0x5f0));
  }
  karisik = karisik.slice(0, 20480);

  await gidisDonus('kısa Türkçe metin', kisa);
  await gidisDonus('çok satırlı program', cokSatirli);
  await gidisDonus('boş metin', '');
  const kodYirmi = await gidisDonus('20 KB tekrarlı metin', yirmiKb);
  denetle(kodYirmi.startsWith('z:'), '20 KB tekrarlı metin sıkıştırılır (z:)');
  denetle(kodYirmi.length < 2000, '20 KB tekrarlı metin küçülür');
  await gidisDonus('20 KB karışık metin', karisik);
  await gidisDonus('emoji ve CRLF', 'a\r\nb 😀 ç\n');
  await gidisDonus('baştaki BOM korunur', '﻿yaz(1)');
  await gidisDonus('tek karakter', 'a');
  await gidisDonus('sondaki boşluklar', 'yaz(1)   \n\n\n');

  // Kodlayıcının çıktısı standart araçlarca da okunabilmeli.
  const z = await programiKodla(yirmiKb);
  const acilmis = zlib.inflateRawSync(Buffer.from(z.slice(2), 'base64url')).toString('utf8');
  denetle(acilmis === yirmiKb, 'z: çıktısı zlib.inflateRawSync ile açılır');
  const k = await programiKodla('a');
  denetle(k === 'k:' + Buffer.from('a').toString('base64url'), 'k: çıktısı standart base64url ile aynı');

  // Çözme her iki öneki de kabul eder (kodlayıcıdan bağımsız üretilmiş girdiler).
  const kBaytlar = Buffer.from(kisa, 'utf8');
  denetle((await programiCoz('k:' + base64url(kBaytlar))) === kisa, 'k: öneki çözülür');
  denetle((await programiCoz('k:')) === '', 'k: boş yük boş metin verir');
  const zBaytlar = zlib.deflateRawSync(Buffer.from(cokSatirli, 'utf8'));
  denetle((await programiCoz('z:' + base64url(zBaytlar))) === cokSatirli, 'z: öneki çözülür');

  // Bozuk girdiler hata fırlatmadan null vermeli.
  const bozuklar = [
    ['boş parça', ''],
    ['yalnızca önek harfi', 'z'],
    ['bilinmeyen önek', 'x:AAAA'],
    ['önek ayracı yok', 'kAAAA'],
    ['büyük harfli önek', 'K:AAAA'],
    ['k: geçersiz base64 (standart karakterler)', 'k:ab+/'],
    ['k: geçersiz base64 (dolgu)', 'k:YQ=='],
    ['k: geçersiz base64 (uzunluk)', 'k:A'],
    ['k: geçersiz base64 (simge)', 'k:***'],
    ['k: geçersiz UTF-8', 'k:_w'],
    ['z: geçersiz base64', 'z:%%%%'],
    ['z: geçerli base64, deflate değil', 'z:AAAAAAAA'],
    ['z: kesilmiş deflate', 'z:' + base64url(zBaytlar.subarray(0, Math.floor(zBaytlar.length / 2)))],
    ['z: boş yük', 'z:'],
    ['ASCII dışı karakter', 'k:ğğğğ'],
    ['null', null],
    ['tanımsız', undefined],
    ['sayı', 12345],
  ];
  for (const [ad, girdi] of bozuklar) {
    let sonuc;
    try {
      sonuc = await programiCoz(girdi);
    } catch (hata) {
      sonuc = 'FIRLATTI: ' + hata;
    }
    denetle(sonuc === null, 'bozuk girdi null verir: ' + ad);
  }

  // Şişirme saldırısı: küçük bağlantı, çok büyük açılmış veri.
  const sisirilmis = zlib.deflateRawSync(Buffer.alloc(8 * 1024 * 1024, 97));
  denetle((await programiCoz('z:' + base64url(sisirilmis))) === null, 'aşırı büyük açılmış veri reddedilir');

  console.log('\n' + (sayac - basarisiz) + '/' + sayac + ' denetim geçti');
  if (basarisiz > 0) process.exit(1);
}

main().catch((hata) => {
  console.error('Sınama beklenmedik biçimde durdu:', hata);
  process.exit(1);
});
