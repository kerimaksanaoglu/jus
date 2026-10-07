#!/usr/bin/env python3
"""JUS biçimlendiricisi doğrulama betiği.

Kullanım: python tools/bicimlendirici/dogrula.py

Depodaki tüm .jus dosyalarında şu denetimleri yapar:
  a. kararlılık      biçimle(biçimle(x)) == biçimle(x)
  b. belirteç korunumu  biçimlendirmeden önce ve sonra belirteç dizisi aynı
  c. davranış korunumu  tests/ dosyalarının stdout ve çıkış kodu aynı
  d. yorum korunumu  yorum içerikleri sırayla aynı
Ayrıca ornekler/ altındaki girdi/beklenen çiftlerini ve hatalı girdileri sınar.
Herhangi bir denetim kalırsa sıfırdan farklı kodla çıkar.
Geçici dosyalar sistemin geçici klasörüne yazılır; depodaki dosyalara dokunulmaz.
"""

import os
import shutil
import subprocess
import sys
import tempfile

KOK = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
BICIM = os.path.join(KOK, "tools", "bicimlendirici")
ZAMAN_ASIMI = 30

sayac = {}  # denetim adı -> [geçen, kalan]
kalanlar = []


def kaydet(denetim, tamam, ayrinti=""):
    s = sayac.setdefault(denetim, [0, 0])
    s[0 if tamam else 1] += 1
    if not tamam:
        kalanlar.append("KALDI [%s] %s" % (denetim, ayrinti))


def derle(gecici):
    exe = os.path.join(gecici, "bicim_dene.exe" if os.name == "nt" else "bicim_dene")
    komut = ["gcc", "-std=c99", "-Wall", "-Wextra", "-pedantic", "-I" + os.path.join(KOK, "src"),
             os.path.join(BICIM, "dene.c"), os.path.join(KOK, "src", "format.c"),
             os.path.join(KOK, "src", "scanner.c"), "-o", exe]
    r = subprocess.run(komut, capture_output=True, text=True)
    if r.returncode != 0 or r.stderr.strip():
        print("Derleme sorunu:\n" + r.stderr)
        if r.returncode != 0:
            sys.exit(2)
    return exe


def biçimle_dosya(exe, yol):
    """(çıktı baytları, çıkış kodu, stderr) döndürür."""
    r = subprocess.run([exe, yol], capture_output=True, timeout=ZAMAN_ASIMI)
    return r.stdout, r.returncode, r.stderr.decode("utf-8", "replace").strip()


def biçimle_bayt(exe, veri, gecici):
    yol = os.path.join(gecici, "_ara.jus")
    with open(yol, "wb") as f:
        f.write(veri)
    return biçimle_dosya(exe, yol)


def belirtecler(exe, yol):
    r = subprocess.run([exe, "--belirtec", yol], capture_output=True, timeout=ZAMAN_ASIMI)
    return r.stdout


def yorumlar(veri):
    """Kaynaktaki yorum içeriklerini sırayla verir ('#' ve ardındaki boşluklar hariç)."""
    metin = veri.decode("utf-8", "replace")
    sonuc = []
    for satir in metin.replace("\r\n", "\n").split("\n"):
        i = 0
        metin_icinde = False
        while i < len(satir):
            c = satir[i]
            if metin_icinde:
                if c == "\\":
                    i += 1
                elif c == '"':
                    metin_icinde = False
            elif c == '"':
                metin_icinde = True
            elif c == "#":
                sonuc.append(satir[i + 1:].strip())
                break
            i += 1
    return sonuc


def jus_dosyalari():
    bulunan = []
    for klasor in ("tests", "examples", "bench", "lib"):
        for dizin, _, adlar in os.walk(os.path.join(KOK, klasor)):
            for ad in adlar:
                if ad.endswith(".jus"):
                    bulunan.append(os.path.join(dizin, ad))
    return sorted(bulunan)


def goreli(yol):
    return os.path.relpath(yol, KOK).replace("\\", "/")


def calistir(jus, kok, dosya, girdi):
    try:
        r = subprocess.run([jus, goreli_kok(kok, dosya)], cwd=kok, input=girdi,
                           capture_output=True, timeout=ZAMAN_ASIMI)
    except subprocess.TimeoutExpired:
        return b"<zaman asimi>", -999
    return r.stdout.replace(b"\r\n", b"\n"), r.returncode


def goreli_kok(kok, dosya):
    return os.path.relpath(dosya, kok).replace("\\", "/")


def girdi_satirlari(veri):
    satirlar = []
    for s in veri.decode("utf-8", "replace").replace("\r\n", "\n").split("\n"):
        if "# girdi: " in s:
            satirlar.append(s.split("# girdi: ", 1)[1])
    return ("\n".join(satirlar) + "\n").encode("utf-8") if satirlar else b""


def hata_sinamalari(exe, gecici):
    durumlar = [
        ("kapanmamış metin", 'x = "abc\n', 1, "Metin kapatılmamış"),
        ("kapanmamış metin (3. satır)", 'a = 1\nb = 2\nc = "x\n', 3, "Metin kapatılmamış"),
        ("geçersiz karakter", "x = 1\ny = @\n", 2, "Beklenmeyen karakter"),
        ("tek ünlem", "x = !y\n", 1, "'!'"),
        ("eşleşmeyen girinti", "eğer x:\n        a = 1\n    b = 2\n", 3, "eşleşmiyor"),
        ("boşluk ve sekme karışık", "eğer x:\n \ta = 1\n", 2, "boşluk ve sekme"),
        ("dosyada iki girinti türü", "eğer x:\n    a = 1\neğer y:\n\tb = 2\n", 4, "Girinti dosya boyunca"),
        ("üslü sayı", "x = 1e5\n", 1, "Geçersiz sayı"),
    ]
    for ad, kaynak, satir, parca in durumlar:
        cikti, kod, hata = biçimle_bayt(exe, kaynak.encode("utf-8"), gecici)
        beklenen_basi = "satır %d:" % satir
        tamam = kod == 2 and cikti == b"" and hata.startswith(beklenen_basi) and parca in hata
        kaydet("hata", tamam, "%s: kod=%s hata=%r" % (ad, kod, hata))


def main():
    if hasattr(sys.stdout, "reconfigure"):
        sys.stdout.reconfigure(encoding="utf-8")
    gecici = tempfile.mkdtemp(prefix="jus_bicim_")
    try:
        exe = derle(gecici)
        dosyalar = jus_dosyalari()
        print("%d adet .jus dosyası bulundu" % len(dosyalar))

        biçimli = {}  # dosya -> biçimlenmiş bayt
        muaf = []
        for yol in dosyalar:
            cikti, kod, hata = biçimle_dosya(exe, yol)
            ad = goreli(yol)
            if kod != 0:
                muaf.append("%s (%s)" % (ad, hata))
                continue
            biçimli[yol] = cikti

        # a, b, d
        for yol, cikti in biçimli.items():
            ad = goreli(yol)
            ikinci, kod, _ = biçimle_bayt(exe, cikti, gecici)
            kaydet("a kararlılık", kod == 0 and ikinci == cikti, ad)

            yeni = os.path.join(gecici, "_yeni.jus")
            with open(yeni, "wb") as f:
                f.write(cikti)
            kaydet("b belirteç", belirtecler(exe, yol) == belirtecler(exe, yeni), ad)

            with open(yol, "rb") as f:
                ozgun = f.read()
            kaydet("d yorum", yorumlar(ozgun) == yorumlar(cikti), ad)

        # c: davranış korunumu
        jus_kopya = os.path.join(gecici, "jus_bicim" + (".exe" if os.name == "nt" else ""))
        shutil.copyfile(os.path.join(KOK, "jus.exe" if os.name == "nt" else "jus"), jus_kopya)
        os.chmod(jus_kopya, 0o755)
        a_kok = os.path.join(gecici, "ozgun")
        b_kok = os.path.join(gecici, "bicimli")
        for kok in (a_kok, b_kok):
            for klasor in ("tests", "lib", "examples", "bench"):
                if os.path.isdir(os.path.join(KOK, klasor)):
                    shutil.copytree(os.path.join(KOK, klasor), os.path.join(kok, klasor))
        for yol, cikti in biçimli.items():
            with open(os.path.join(b_kok, goreli(yol)), "wb") as f:
                f.write(cikti)
        testler = [y for y in biçimli
                   if goreli(y).startswith("tests/")
                   and not any(p.startswith("_") for p in goreli(y).split("/")[1:-1])]
        for yol in testler:
            ad = goreli(yol)
            with open(yol, "rb") as f:
                girdi = girdi_satirlari(f.read())
            ozgun = calistir(jus_kopya, a_kok, os.path.join(a_kok, ad), girdi)
            yeni = calistir(jus_kopya, b_kok, os.path.join(b_kok, ad), girdi)
            kaydet("c davranış", ozgun == yeni, "%s: kod %s/%s" % (ad, ozgun[1], yeni[1]))

        # örnekler
        ornek_dizin = os.path.join(BICIM, "ornekler")
        adlar = sorted(a[:-len(".girdi.jus")] for a in os.listdir(ornek_dizin) if a.endswith(".girdi.jus"))
        for ad in adlar:
            with open(os.path.join(ornek_dizin, ad + ".girdi.jus"), "rb") as f:
                girdi = f.read()
            with open(os.path.join(ornek_dizin, ad + ".beklenen.jus"), "rb") as f:
                beklenen = f.read()
            cikti, kod, hata = biçimle_bayt(exe, girdi, gecici)
            kaydet("ornek", kod == 0 and cikti == beklenen, ad + " " + hata)
            ikinci, kod, _ = biçimle_bayt(exe, beklenen, gecici)
            kaydet("a kararlılık", kod == 0 and ikinci == beklenen, "ornek " + ad)
        print("%d örnek çifti" % len(adlar))

        hata_sinamalari(exe, gecici)
    finally:
        shutil.rmtree(gecici, ignore_errors=True)

    print()
    for ad in sorted(sayac):
        print("%-14s geçen %4d  kalan %4d" % (ad, sayac[ad][0], sayac[ad][1]))
    print("\nMuaf (sözcük hatası, biçimlendirilmedi): %d" % len(muaf))
    for m in muaf:
        print("  " + m)
    for k in kalanlar[:40]:
        print(k)
    kalan = sum(s[1] for s in sayac.values())
    print("\n" + ("TÜM DENETİMLER GEÇTİ" if kalan == 0 else "%d denetim kaldı" % kalan))
    return 1 if kalan else 0


if __name__ == "__main__":
    sys.exit(main())
