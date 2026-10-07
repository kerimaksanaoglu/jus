// Programı sayfadan ayrı bir iş parçacığında çalıştırır; böylece sonsuz
// döngüye giren bir program sayfayı kilitlemez ve durdurulabilir.
importScripts('jus.js');

onmessage = async (event) => {
  const lines = [];
  // Çıktıyı tek tek değil, kısa aralıklarla toplu gönder; çok satır yazan programlar sayfayı yormasın.
  let scheduled = false;
  const flush = () => {
    scheduled = false;
    if (lines.length > 0) postMessage({ type: 'output', lines: lines.splice(0) });
  };
  const add = (kind) => (text) => {
    lines.push({ kind, text });
    if (!scheduled) {
      scheduled = true;
      setTimeout(flush, 50);
    }
  };

  let code = 0;
  try {
    const jus = await createJus({ print: add('out'), printErr: add('err') });
    jus.FS.writeFile('/program.jus', event.data.source);
    code = jus.callMain(['/program.jus']);
  } catch (error) {
    if (error && typeof error.status === 'number') {
      code = error.status;
    } else {
      lines.push({ kind: 'err', text: 'Yorumlayıcı beklenmedik biçimde durdu: ' + error });
      code = -1;
    }
  }
  flush();
  postMessage({ type: 'done', code });
};
