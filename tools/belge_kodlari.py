#!/usr/bin/env python3
"""Belgelerdeki JUS kod bloklarını çalıştırıp doğrular.

Kullanım: python tools/belge_kodlari.py [--jus YOL] [--katı] DOSYA_YA_DA_KLASÖR...

Markdown dosyalarındaki her ```jus bloğu ayrı bir dosyaya yazılır ve
yorumlayıcıyla çalıştırılır (standart girdi kapalı, 10 saniye zaman aşımı).

- Çıkış kodu 0, 65 (sözdizimi hatası) ve 70 (çalışma zamanı hatası) dışında
  bir kodla biten ya da zaman aşımına uğrayan blok "çökme" sayılır; bu her
  zaman başarısızlıktır.
- Kod bloğunun hemen ardından dilsiz bir ``` bloğu geliyorsa bu, beklenen
  çıktıdır: programın standart çıktısı ve hata çıktısı (dosya yolu
  "ornek.jus" olarak sadeleştirilerek) onunla karşılaştırılır. Fark varsa
  "çıktı farkı" sayılır. Beklenen çıktıda "..." geçiyorsa yalnızca "..."
  dışındaki parçaların sırayla geçtiği denetlenir.
- Bloğun hemen üstündeki satırda "<!-- girdi: a, b -->" yorumu varsa
  programa standart girdiden bu satırlar verilir. "<!-- çalıştırma -->" varsa
  blok atlanır (parça örnekleri için).

--katı verildiğinde çıktı farkları da başarısızlık sayılır. Çıkış kodu: sorun
yoksa 0, varsa 1.
"""
import argparse
import os
import re
import subprocess
import sys
import tempfile

KOK = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def dosyalari_topla(yollar):
    for yol in yollar:
        if os.path.isdir(yol):
            for klasor, _, adlar in sorted(os.walk(yol)):
                for ad in sorted(adlar):
                    if ad.endswith(".md"):
                        yield os.path.join(klasor, ad)
        else:
            yield yol


def bloklari_cikar(metin):
    """(satır numarası, kod, beklenen çıktı ya da None, girdi satırları, atla) listesi."""
    satirlar = metin.split("\n")
    sonuc = []
    i = 0
    while i < len(satirlar):
        if satirlar[i].strip() == "```jus":
            baslangic = i + 1
            j = baslangic
            while j < len(satirlar) and satirlar[j].strip() != "```":
                j += 1
            kod = "\n".join(satirlar[baslangic:j]) + "\n"
            onceki = satirlar[i - 1] if i > 0 else ""
            girdi = None
            atla = "<!-- çalıştırma -->" in onceki
            eslesme = re.search(r"<!--\s*girdi:\s*(.*?)\s*-->", onceki)
            if eslesme:
                girdi = [p.strip() for p in eslesme.group(1).split(",")]
            # Hemen ardından gelen dilsiz blok beklenen çıktıdır.
            beklenen = None
            k = j + 1
            while k < len(satirlar) and satirlar[k].strip() == "":
                k += 1
            if k < len(satirlar) and satirlar[k].strip() == "```":
                m = k + 1
                while m < len(satirlar) and satirlar[m].strip() != "```":
                    m += 1
                beklenen = "\n".join(satirlar[k + 1:m])
                j = m
            sonuc.append((baslangic + 1, kod, beklenen, girdi, atla))
            i = j + 1
        else:
            i += 1
    return sonuc


def calistir(jus, kod, girdi, klasor):
    yol = os.path.join(klasor, "ornek.jus")
    with open(yol, "w", encoding="utf-8", newline="\n") as f:
        f.write(kod)
    stdin_metin = None if girdi is None else "\n".join(girdi) + "\n"
    try:
        p = subprocess.run(
            [jus, "ornek.jus"], cwd=klasor, capture_output=True, timeout=10,
            input=None if stdin_metin is None else stdin_metin.encode("utf-8"),
            stdin=subprocess.DEVNULL if stdin_metin is None else None,
        )
    except subprocess.TimeoutExpired:
        return None, "", ""
    cikti = p.stdout.decode("utf-8", "replace").replace("\r\n", "\n")
    hata = p.stderr.decode("utf-8", "replace").replace("\r\n", "\n")
    return p.returncode, cikti, hata


def uyusuyor(beklenen, gercek):
    beklenen = beklenen.strip()
    gercek = gercek.strip()
    if "..." not in beklenen:
        return beklenen == gercek
    konum = 0
    for parca in beklenen.split("..."):
        parca = parca.strip()
        if not parca:
            continue
        yer = gercek.find(parca, konum)
        if yer < 0:
            return False
        konum = yer + len(parca)
    return True


def main():
    ayristirici = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ayristirici.add_argument("yollar", nargs="+")
    ayristirici.add_argument("--jus", default=os.path.join(KOK, "jus.exe" if os.name == "nt" else "jus"))
    ayristirici.add_argument("--katı", "--kati", action="store_true", dest="kati")
    args = ayristirici.parse_args()
    jus = os.path.abspath(args.jus)

    toplam = 0
    kodlar = {0: 0, 65: 0, 70: 0}
    cokme = []
    farklar = []
    karsilastirilan = 0
    with tempfile.TemporaryDirectory() as klasor:
        for dosya in dosyalari_topla(args.yollar):
            with open(dosya, encoding="utf-8") as f:
                metin = f.read()
            for satir, kod, beklenen, girdi, atla in bloklari_cikar(metin):
                if atla:
                    continue
                toplam += 1
                kod_no, cikti, hata = calistir(jus, kod, girdi, klasor)
                ad = "%s:%d" % (os.path.relpath(dosya, KOK), satir)
                if kod_no is None or kod_no not in kodlar:
                    cokme.append((ad, "zaman aşımı" if kod_no is None else "çıkış kodu %d" % kod_no))
                    continue
                kodlar[kod_no] += 1
                if beklenen is not None:
                    karsilastirilan += 1
                    gercek = (cikti + hata).replace(klasor.replace("\\", "/") + "/", "").replace(klasor + "\\", "")
                    gercek = re.sub(r"(?m)^(?:[A-Za-z]:)?[^\s:]*[\\/]ornek\.jus", "ornek.jus", gercek)
                    if not uyusuyor(beklenen, gercek):
                        farklar.append((ad, beklenen.strip(), gercek.strip()))

    print("%d kod bloğu çalıştırıldı: %d başarılı, %d sözdizimi hatası, %d çalışma zamanı hatası"
          % (toplam, kodlar[0], kodlar[65], kodlar[70]))
    print("%d bloğun çıktısı belgedekiyle karşılaştırıldı, %d fark" % (karsilastirilan, len(farklar)))
    for ad, neden in cokme:
        print("ÇÖKME  %s: %s" % (ad, neden))
    for ad, beklenen, gercek in farklar:
        print("FARK   %s" % ad)
        print("  --- belgede")
        for s in beklenen.splitlines():
            print("  " + s)
        print("  --- gerçek")
        for s in gercek.splitlines():
            print("  " + s)
    basarisiz = bool(cokme) or (args.kati and bool(farklar))
    sys.exit(1 if basarisiz else 0)


if __name__ == "__main__":
    main()
