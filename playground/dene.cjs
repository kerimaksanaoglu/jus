// WebAssembly derlemesinin Node.js altında sınaması.
// Kullanım: node playground/dene.cjs   (önce: bash playground/derle.sh)
const createJus = require('./jus.js');

async function run(source) {
  const output = [];
  const errors = [];
  const jus = await createJus({
    print: (line) => output.push(line),
    printErr: (line) => errors.push(line),
  });
  jus.FS.writeFile('/program.jus', source);
  let code;
  try {
    code = jus.callMain(['/program.jus']);
  } catch (error) {
    // Emscripten, exit() çağrısını ExitStatus istisnasıyla bildirir.
    if (error && typeof error.status === 'number') code = error.status;
    else throw error;
  }
  return { code, output: output.join('\n'), errors: errors.join('\n') };
}

const cases = [
  {
    name: 'temel program',
    source: [
      'kullan json',
      'sınıf Sayaç:',
      '    fonksiyon kur():',
      '        bu.değer = 0',
      '    fonksiyon artır():',
      '        bu.değer += 1',
      '        dön bu',
      'değişken s = Sayaç()',
      'her i içinde aralık(5):',
      '    s.artır()',
      'yaz("Merhaba, dünya!", s.değer, büyük_harf("ılık"))',
      'yaz(json.yaz({"liste": sırala(["çay", "armut", "ıhlamur"])}))',
    ].join('\n'),
    code: 0,
    output: 'Merhaba, dünya! 5 ILIK\n{"liste":["armut","çay","ıhlamur"]}',
  },
  {
    name: 'sözdizimi hatası',
    source: 'değişken x =\n',
    code: 65,
    errorIncludes: 'sözdizimi hatası: İfade bekleniyor.',
  },
  {
    name: 'çalışma zamanı hatası',
    source: 'yaz("önce")\nyaz(1 / 0)\n',
    code: 70,
    output: 'önce',
    errorIncludes: 'Sıfıra bölünemez.',
  },
];

(async () => {
  let failed = 0;
  for (const test of cases) {
    const result = await run(test.source);
    const problems = [];
    if (result.code !== test.code) problems.push(`çıkış kodu ${result.code}, beklenen ${test.code}`);
    if (test.output !== undefined && result.output !== test.output) {
      problems.push(`çıktı farklı:\n${result.output}`);
    }
    if (test.errorIncludes && !result.errors.includes(test.errorIncludes)) {
      problems.push(`hata çıktısında bulunamadı: ${test.errorIncludes}\n${result.errors}`);
    }
    if (problems.length > 0) {
      failed++;
      console.log(`KALDI  ${test.name}: ${problems.join('; ')}`);
    } else {
      console.log(`geçti  ${test.name}`);
    }
  }
  process.exit(failed === 0 ? 0 : 1);
})();
