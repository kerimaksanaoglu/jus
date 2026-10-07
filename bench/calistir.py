#!/usr/bin/env python3
"""JUS ve CPython hız ölçümü.

Kullanım: python bench/calistir.py [jus-yolu] [-n TEKRAR]

bench/ klasöründeki her ad.jus / ad.py çiftini TEKRAR kez (varsayılan 3)
çalıştırır, en iyi süreyi alır, çıktıların aynı olduğunu doğrular ve
sonuç tablosunu yazar. Süre, süreç başlatmadan bitişe kadar geçen duvar saati
süresidir (yorumlayıcı başlangıcı dahil).
"""
import argparse
import math
import os
import subprocess
import sys
import time

KLASOR = os.path.dirname(os.path.abspath(__file__))


def calistir(komut):
    t0 = time.perf_counter()
    p = subprocess.run(komut, capture_output=True, cwd=KLASOR)
    sure = time.perf_counter() - t0
    cikti = p.stdout.decode("utf-8", errors="replace").replace("\r\n", "\n").strip()
    return sure, cikti, p.returncode, p.stderr.decode("utf-8", errors="replace")


def en_iyi(komut, n):
    en = None
    cikti = None
    for _ in range(n):
        sure, c, kod, err = calistir(komut)
        if kod != 0:
            return None, "çıkış kodu %d: %s" % (kod, err.strip()[:200])
        if cikti is not None and c != cikti:
            return None, "çıktı çalıştırmalar arasında değişti"
        cikti = c
        en = sure if en is None else min(en, sure)
    return en, cikti


def main():
    if hasattr(sys.stdout, "reconfigure"):
        sys.stdout.reconfigure(encoding="utf-8")
    ap = argparse.ArgumentParser()
    ap.add_argument("jus", nargs="?", default=os.path.join(KLASOR, "..", "jus.exe"))
    ap.add_argument("-n", type=int, default=3, help="tekrar sayısı")
    a = ap.parse_args()
    jus = os.path.abspath(a.jus)

    adlar = sorted(f[:-4] for f in os.listdir(KLASOR)
                   if f.endswith(".jus") and os.path.exists(os.path.join(KLASOR, f[:-4] + ".py")))
    if not adlar:
        print("ölçüm programı bulunamadı")
        return 1

    satirlar = []
    hatalar = 0
    for ad in adlar:
        sj, cj = en_iyi([jus, ad + ".jus"], a.n)
        sp, cp = en_iyi([sys.executable, ad + ".py"], a.n)
        if sj is None or sp is None:
            print("%s: HATA: JUS=%s | Python=%s" % (ad, cj if sj is None else "ok", cp if sp is None else "ok"))
            hatalar += 1
            continue
        if cj != cp:
            print("%s: ÇIKTI FARKLI\n  JUS:    %s\n  Python: %s" % (ad, cj, cp))
            hatalar += 1
            continue
        satirlar.append((ad, sj, sp, sj / sp, cj))
        print("%s tamam (%s)" % (ad, cj[:60]), flush=True)

    print()
    print("%-10s %10s %10s %8s" % ("ad", "JUS sn", "Python sn", "oran"))
    print("-" * 42)
    for ad, sj, sp, o, _ in satirlar:
        print("%-10s %10.3f %10.3f %7.2fx" % (ad, sj, sp, o))
    if satirlar:
        g = math.exp(sum(math.log(s[3]) for s in satirlar) / len(satirlar))
        print("-" * 42)
        print("Oran = JUS süresi / Python süresi (1'den küçük: JUS daha hızlı).")
        print("Geometrik ortalama oran: %.2fx (%d ölçüm)" % (g, len(satirlar)))
    return 1 if hatalar else 0


if __name__ == "__main__":
    sys.exit(main())
