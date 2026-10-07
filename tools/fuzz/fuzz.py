#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""JUS yorumlayicisi icin tekrarlanabilir rastgele girdi (fuzz) betigi.

Kullanim:
    python tools/fuzz/fuzz.py --jus /tmp/jus_fuzz.exe --adet 2000 --tohum 1

Yalnizca standart kutuphane kullanir. Ayni tohum ayni girdi dizisini uretir.
Beklenmeyen cikis kodu ya da zaman asimi olan girdiler bulgular/ham/ altina
kaydedilir.
"""
import argparse
import os
import random
import shutil
import subprocess
import sys
import tempfile
import threading
from concurrent.futures import ThreadPoolExecutor

KOK = os.path.dirname(os.path.abspath(__file__))
DEPO = os.path.dirname(os.path.dirname(KOK))
GECERLI = {0, 65, 66, 70}

ANAHTAR = ["boş", "bu", "değil", "değilse", "değişken", "dene", "devam", "doğru",
           "dön", "eğer", "fırlat", "fonksiyon", "her", "içinde", "iken", "kır",
           "kullan", "olarak", "sınıf", "üst", "ve", "veya", "yakala", "yanlış"]
ISLEC = ["+", "-", "*", "/", "%", "<", "<=", ">", ">=", "==", "!=", "=", "+=",
         "-=", "*=", "/=", "!", ".", ",", ":", "#", "&", "|", "@", "$", "~", ";"]
PAREN = ["(", ")", "[", "]", "{", "}"]
SAYI = ["0", "1", "42", "3.14", "0.1", "1e5", "99999999999999999999", "1.", ".5",
        "00", "0x10", "1_000", "-1", "1.2.3", "9" * 400, "0." + "0" * 300 + "1"]
METIN = ['""', '"a"', '"çay"', '"\\n"', '"\\q"', '"abc', '"\\"', '"\\\\"', '"ğüşıöç"',
         '"' + "x" * 5000 + '"', '"\\u00e7"', '"%s%d"']
AD = ["a", "b", "x", "yaz", "uzunluk", "ekle", "Ş", "_", "_x", "ğ1", "metin",
      "sayı", "aralık", "tür", "böl", "birleştir", "sırala", "sil", "çıkar", "al",
      "kur", "tanıt", "Hayvan", "f", "g", "liste", "kap", "i", "j"]
YASAK_MODUL = ("dosya", "sistem", "zaman")


def tohum_dosyalari():
    sonuc = []
    for yol, _, adlar in os.walk(os.path.join(DEPO, "tests")):
        for ad in sorted(adlar):
            if not ad.endswith(".jus"):
                continue
            with open(os.path.join(yol, ad), "rb") as f:
                veri = f.read()
            metin = veri.decode("utf-8", "replace")
            if any(("kullan " + m) in metin or ('kullan "' + m) in metin for m in YASAK_MODUL):
                continue
            if "çık(" in metin or "bekle(" in metin:
                continue
            sonuc.append(veri)
    sonuc.sort()
    return sonuc


# ---------------------------------------------------------------- (a) bozma
def bozma(r, veri):
    veri = bytearray(veri)
    for _ in range(r.randint(1, 6)):
        if not veri:
            veri = bytearray(b"yaz(1)\n")
        t = r.randrange(10)
        n = len(veri)
        if t == 0:
            del veri[r.randrange(n)]
        elif t == 1:
            veri.insert(r.randrange(n + 1), r.randrange(256))
        elif t == 2:
            veri[r.randrange(n)] = r.randrange(256)
        elif t == 3:
            i = r.randrange(n)
            del veri[i:i + r.randint(1, 40)]
        elif t == 4:
            s = veri.split(b"\n")
            if len(s) > 1:
                del s[r.randrange(len(s))]
            veri = bytearray(b"\n".join(s))
        elif t == 5:
            s = veri.split(b"\n")
            i = r.randrange(len(s))
            s[i:i] = [s[i]] * r.randint(1, 30)
            veri = bytearray(b"\n".join(s))
        elif t == 6:
            s = veri.split(b"\n")
            if len(s) > 1:
                i, j = r.randrange(len(s)), r.randrange(len(s))
                s[i], s[j] = s[j], s[i]
            veri = bytearray(b"\n".join(s))
        elif t == 7:
            s = veri.split(b"\n")
            i = r.randrange(len(s))
            m = r.randrange(4)
            if m == 0:
                s[i] = b" " * r.randint(1, 9) + s[i].lstrip()
            elif m == 1:
                s[i] = b"\t" + s[i]
            elif m == 2:
                s[i] = s[i].lstrip(b" \t")
            else:
                s[i] = b" " * r.randint(0, 200) + s[i]
            veri = bytearray(b"\n".join(s))
        elif t == 8:
            veri = veri[:r.randrange(n + 1)]
        else:
            i = r.randrange(n + 1)
            veri[i:i] = r.choice(ANAHTAR + ISLEC + PAREN + METIN).encode("utf-8")
    return bytes(veri)


# ------------------------------------------------------------ (b) belirtec
def belirtec(r):
    satirlar = []
    for _ in range(r.randint(1, 40)):
        girinti = r.choice(["", "", "    ", "        ", "\t", "  ", "   "])
        parcalar = []
        for _ in range(r.randint(0, 14)):
            k = r.randrange(7)
            if k == 0:
                parcalar.append(r.choice(ANAHTAR))
            elif k == 1:
                parcalar.append(r.choice(ISLEC))
            elif k == 2:
                parcalar.append(r.choice(PAREN))
            elif k == 3:
                parcalar.append(r.choice(SAYI))
            elif k == 4:
                parcalar.append(r.choice(METIN))
            else:
                parcalar.append(r.choice(AD))
        s = girinti + " ".join(parcalar)
        if r.random() < 0.3:
            s += ":"
        satirlar.append(s)
    son = r.choice(["\n", "\r\n", "", "\n\n"])
    return (r.choice(["\n", "\n", "\r\n"]).join(satirlar) + son).encode("utf-8")


# ------------------------------------------------------------- (c) yapisal
class Uretec:
    def __init__(self, r):
        self.r = r
        self.sayac = 0

    def yeni(self, onek):
        self.sayac += 1
        return "%s%d" % (onek, self.sayac)

    def sabit(self):
        r = self.r
        k = r.randrange(9)
        if k == 0:
            return str(r.choice([0, 1, 2, 3, 7, 255, 256, 65536, 10 ** 9]))
        if k == 1:
            return r.choice(["0.5", "3.14", "2.5", "1000000.25"])
        if k == 2:
            return '"%s"' % r.choice(["", "a", "çay", "merhaba", "x\\ny", "ığüşöç"])
        if k == 3:
            return r.choice(["doğru", "yanlış"])
        if k == 4:
            return "boş"
        if k == 5:
            return "[%s]" % ", ".join(str(r.randint(-3, 9)) for _ in range(r.randint(0, 4)))
        if k == 6:
            return '{"a": %d, "b": %d}' % (r.randint(0, 9), r.randint(0, 9))
        return str(r.randint(0, 20))

    def ifade(self, d, adlar):
        r = self.r
        if d <= 0 or r.random() < 0.15:
            if adlar and r.random() < 0.6:
                return r.choice(adlar)
            return self.sabit()
        k = r.randrange(16)

        def e():
            return self.ifade(d - 1, adlar)
        if k == 0:
            return "(%s %s %s)" % (e(), r.choice(["+", "-", "*", "/", "%"]), e())
        if k == 1:
            return "%s %s %s" % (e(), r.choice(["<", "<=", ">", ">=", "==", "!="]), e())
        if k == 2:
            return "(%s %s %s)" % (e(), r.choice(["ve", "veya"]), e())
        if k == 3:
            return "(değil %s)" % e()
        if k == 4:
            return "(-%s)" % e()
        if k == 5:
            return "[%s]" % ", ".join(e() for _ in range(r.randint(0, 4)))
        if k == 6:
            return "{%s}" % ", ".join('"%s": %s' % (r.choice("abcxyz"), e()) for _ in range(r.randint(0, 3)))
        if k == 7:
            return "%s[%s]" % (e(), e())
        if k == 8:
            return "%s[%s:%s]" % (e(), r.choice(["", e()]), r.choice(["", e()]))
        if k == 9:
            f = r.choice(["metin", "sayı", "tür", "uzunluk", "mutlak", "taban", "ters", "sırala",
                          "büyük_harf", "kırp", "anahtarlar", "değerler", "karekök", "aralık", "yuvarla"])
            return "%s(%s)" % (f, e())
        if k == 10:
            return "%s içinde %s" % (e(), e())
        if k == 11:
            return "(%s)" % e()
        if k == 12:
            return "%s.%s" % (e(), r.choice(["ad", "x", "tanıt"]))
        if k == 13:
            return "%s(%s)" % (r.choice(["birleştir", "böl", "bul", "al", "ekle", "değiştir"]),
                               ", ".join(e() for _ in range(r.randint(1, 3))))
        if k == 14:
            return "(%s = %s)" % (r.choice(adlar) if adlar else "q", e())
        return self.sabit()

    def blok(self, d, adlar, girinti, ic):
        sat = []
        yerel = list(adlar)
        for _ in range(self.r.randint(1, 4)):
            sat += self.deyim(d - 1, yerel, girinti, ic)
        return sat

    def deyim(self, d, adlar, g, ic):
        """ic: (donguda_mi, fonksiyonda_mi)."""
        r = self.r
        p = "    " * g
        donguda, fonda = ic
        if d <= 0:
            k = r.choice([0, 1, 1, 2])
        else:
            k = r.randrange(17)
        if k == 0:
            ad = self.yeni("v")
            s = p + "değişken %s = %s" % (ad, self.ifade(r.randint(1, 5), adlar))
            adlar.append(ad)
            return [s]
        if k == 1:
            return [p + "yaz(%s)" % self.ifade(r.randint(1, 5), adlar)]
        if k == 2:
            if adlar:
                return [p + "%s %s %s" % (r.choice(adlar), r.choice(["=", "+=", "-=", "*="]),
                                          self.ifade(2, adlar))]
            return [p + "yaz(1)"]
        if k == 3:
            s = [p + "eğer %s:" % self.ifade(3, adlar)] + self.blok(d, adlar, g + 1, ic)
            if r.random() < 0.5:
                s += [p + "değilse eğer %s:" % self.ifade(2, adlar)] + self.blok(d, adlar, g + 1, ic)
            if r.random() < 0.5:
                s += [p + "değilse:"] + self.blok(d, adlar, g + 1, ic)
            return s
        if k == 4:
            v = self.yeni("i")
            return [p + "her %s içinde aralık(%d):" % (v, r.randint(0, 6))] + \
                self.blok(d, adlar + [v], g + 1, (True, fonda))
        if k == 5:
            v = self.yeni("c")
            return [p + "değişken %s = 0" % v, p + "iken %s < %d:" % (v, r.randint(0, 5)),
                    p + "    %s += 1" % v] + self.blok(d, adlar + [v], g + 1, (True, fonda))
        if k == 6 and donguda:
            return [p + r.choice(["kır", "devam"])]
        if k == 7 and fonda:
            return [p + "dön" + r.choice(["", " " + self.ifade(2, adlar)])]
        if k in (8, 9):
            s = [p + "dene:"] + self.blok(d, adlar, g + 1, ic)
            s += [p + r.choice(["yakala e:", "yakala:"])]
            s += self.blok(d, adlar, g + 1, ic)
            return s
        if k == 10:
            return [p + "fırlat %s" % self.ifade(2, adlar)]
        if k == 11:
            ad = self.yeni("fn")
            ps = [self.yeni("p") for _ in range(r.randint(0, 3))]
            s = [p + "fonksiyon %s(%s):" % (ad, ", ".join(ps))]
            s += self.blok(d, adlar + ps + [ad], g + 1, (False, True))
            adlar.append(ad)
            return s
        if k == 12:
            a, b = self.yeni("k"), self.yeni("k")
            return [p + "fonksiyon %s(n):" % a,
                    p + "    değişken s = n",
                    p + "    fonksiyon %s():" % b,
                    p + "        s += 1",
                    p + "        dön s",
                    p + "    dön %s" % b,
                    p + "yaz(%s(%d)())" % (a, r.randint(0, 9))]
        if k == 13:
            return self.sinif(g)
        if k == 14:
            ad = self.yeni("r")
            return [p + "fonksiyon %s(n):" % ad,
                    p + "    eğer n <= 0:",
                    p + "        dön 0",
                    p + "    dön 1 + %s(n - 1)" % ad,
                    p + "yaz(%s(%d))" % (ad, r.choice([0, 10, 500, 1000, 1023, 1024, 1025, 5000, 100000]))]
        if k == 15:
            return [p + "yaz(%s)" % self.ifade(r.randint(6, 14), adlar)]
        if k == 16:
            return [p + "kullan %s" % r.choice(["matematik", "json", "tr", "rastgele", "matematik olarak m"])]
        return [p + "yaz(1)"]

    def sinif(self, g):
        r = self.r
        p = "    " * g
        ad = self.yeni("S")
        s = [p + "sınıf %s:" % ad,
             p + "    fonksiyon kur(x):",
             p + "        bu.x = x",
             p + "    fonksiyon al():",
             p + "        dön bu.x",
             p + "    fonksiyon dolan(n):",
             p + "        eğer n == 0:",
             p + "            dön bu",
             p + "        dön bu.dolan(n - 1)"]
        alt = self.yeni("T")
        s += [p + "sınıf %s(%s):" % (alt, ad),
              p + "    fonksiyon kur(x):",
              p + "        üst.kur(x)",
              p + "        bu.y = x",
              p + "    fonksiyon al():",
              p + "        dön üst.al() + %s" % r.choice(["1", "bu.y", "bu.z", '"s"'])]
        o = self.yeni("o")
        s += [p + "değişken %s = %s(%s)" % (o, r.choice([ad, alt]), r.choice(["1", "", "1, 2", '"a"'])),
              p + "yaz(%s.%s(%s))" % (o, r.choice(["al", "dolan", "yok"]), r.choice(["", "3", "1000"]))]
        return s

    def program(self):
        r = self.r
        k = r.randrange(8)
        sat = []
        if k == 0:  # cok derin ic ice ifade
            d = r.choice([10, 50, 120, 200, 400, 1000])
            e = "1"
            op = r.choice(["(%s + 1)", "-%s", "[%s]", "(değil %s)", "f(%s)", "(%s)", "{\"a\": %s}", "%s[0]"])
            for _ in range(d):
                e = op % e
            sat = ["fonksiyon f(x):", "    dön x", "yaz(%s)" % e]
        elif k == 1:  # cok derin blok
            d = r.choice([5, 30, 63, 64, 65, 100, 300])
            tur = r.choice(["eğer doğru:", "iken doğru:", "her i içinde [1]:", "dene:"])
            for i in range(d):
                sat.append("    " * i + tur)
            sat.append("    " * d + ("kır" if tur == "iken doğru:" else "yaz(1)"))
            if tur == "iken doğru:":
                # her dugumde sonsuz dongu kalmasin diye her duzeyde kir
                for i in range(d - 1, -1, -1):
                    sat.append("    " * i + "kır")
            if tur == "dene:":
                for i in range(d - 1, -1, -1):
                    sat.append("    " * i + "yakala:")
                    sat.append("    " * (i + 1) + "yaz(2)")
        elif k == 2:  # cok uzun satir
            n = r.choice([100, 1000, 10000, 50000])
            sat = ["yaz(%s)" % " + ".join(["1"] * n)]
            if r.random() < 0.5:
                sat = ["değişken l = [%s]" % ", ".join(["1"] * n), "yaz(uzunluk(l))"]
        elif k == 3:  # cok yerel
            n = r.choice([100, 255, 256, 257, 300, 1000])
            if r.random() < 0.5:
                sat = ["fonksiyon f():"] + ["    değişken v%d = %d" % (i, i) for i in range(n)] + \
                    ["    dön v0", "yaz(f())"]
            else:
                sat = ["eğer doğru:"] + ["    değişken v%d = %d" % (i, i) for i in range(n)] + ["    yaz(v0)"]
        elif k == 4:  # cok sabit
            n = r.choice([255, 256, 257, 1000, 65535, 65536, 70000])
            if r.random() < 0.5:
                sat = ["değişken t = 0"] + ["t += %d.5" % i for i in range(n)] + ["yaz(t)"]
            else:
                sat = ["fonksiyon f():", "    değişken t = 0"] + ["    t += %d.5" % i for i in range(n)] + \
                    ["    dön t", "yaz(f())"]
        elif k == 5:  # devasa liste/sozluk
            n = r.choice([100, 1000, 20000, 100000])
            if r.random() < 0.5:
                sat = ["değişken l = [", ",\n".join('"%d"' % i for i in range(n)), "]", "yaz(uzunluk(l))"]
            else:
                sat = ["değişken d = {", ",\n".join('"k%d": %d' % (i, i) for i in range(n)), "}",
                       "yaz(uzunluk(d))"]
        else:  # genel yapisal
            adlar = []
            for _ in range(r.randint(1, 8)):
                sat += self.deyim(r.randint(2, 6), adlar, 0, (False, False))
        return ("\n".join(sat) + "\n").encode("utf-8")


# ------------------------------------------------------------ (d) sinirlar
def sinir(r):
    k = r.randrange(24)
    L = "\n"
    if k == 0:
        return b""
    if k == 1:
        return b" " * r.randint(1, 5000)
    if k == 2:
        return ("# yorum" + L).encode() * r.randint(1, 50)
    if k == 3:
        return b"# yorum"
    if k == 4:
        return b"\xef\xbb\xbf" + b'yaz("a")\n'
    if k == 5:
        return b"\xef\xbb\xbf"
    if k == 6:
        return "değişken x = 1\r\nyaz(x)\r\neğer x == 1:\r\n    yaz(2)\r\n".encode()
    if k == 7:
        return "eğer doğru:\n\tyaz(1)\n    yaz(2)\n".encode()
    if k == 8:
        return "eğer doğru:\n    yaz(1)\n\tyaz(2)\n".encode()
    if k == 9:
        return b'yaz("\xff\xfe\xfd")\n'
    if k == 10:
        return b"yaz(1)\n\xc3\n"
    if k == 11:
        return b"\xc0\xaf\xed\xa0\x80\xf4\x90\x80\x80\n"
    if k == 12:
        n = r.choice([100, 1000, 100000, 1000000])
        return ("değişken " + "a" * n + " = 1\nyaz(" + "a" * n + ")\n").encode()
    if k == 13:
        n = r.choice([1000, 100000, 3000000])
        return ('yaz("' + "x" * n + '")\n').encode()
    if k == 14:
        n = r.choice([255, 256, 257, 300])
        return ("fonksiyon f():\n" + "".join("    değişken v%d = 1\n" % i for i in range(n)) + "f()\n").encode()
    if k == 15:
        n = r.choice([65535, 65536, 65537, 70000])
        return ("değişken s = 0\n" + "".join("s += %d.25\n" % i for i in range(n))).encode()
    if k == 16:
        d = r.choice([63, 64, 65, 200])
        return ("".join("    " * i + "eğer doğru:\n" for i in range(d)) + "    " * d + "yaz(1)\n").encode()
    if k == 17:
        n = r.choice([255, 256, 299, 300, 301, 1000])
        return ("yaz(" + "(" * n + "1" + ")" * n + ")\n").encode()
    if k == 18:
        n = r.choice([255, 256, 300])
        return ("yaz(" + ", ".join(["1"] * n) + ")\n").encode()
    if k == 19:
        n = r.choice([255, 256, 300])
        return ("fonksiyon f(" + ", ".join("p%d" % i for i in range(n)) + "):\n    dön p0\n").encode()
    if k == 20:
        return ("eğer doğru:\n" + "\n" * 100000 + "    yaz(1)\n").encode()
    if k == 21:
        return b"\r" * r.randint(1, 100) + b"yaz(1)"
    if k == 22:
        return b"\x00" * r.randint(1, 20) + b"yaz(1)\n\x00"
    return ("yaz(" + "[" * r.choice([100, 300, 5000]) + "]" * r.choice([100, 300, 5000]) + ")\n").encode()


def uret(r, tohumlar, strateji=None):
    s = strateji or r.choice(["bozma", "bozma", "belirtec", "yapisal", "yapisal", "sinir"])
    if s == "bozma":
        return s, bozma(r, r.choice(tohumlar))
    if s == "belirtec":
        return s, belirtec(r)
    if s == "yapisal":
        return s, Uretec(r).program()
    return s, sinir(r)


def calistir(jus, veri, zaman_asimi, gecici):
    ad = os.path.join(gecici, "g_%d.jus" % threading.get_ident())
    with open(ad, "wb") as f:
        f.write(veri)
    try:
        p = subprocess.run([jus, ad], stdin=subprocess.DEVNULL, stdout=subprocess.DEVNULL,
                           stderr=subprocess.DEVNULL, timeout=zaman_asimi, cwd=gecici)
        return p.returncode
    except subprocess.TimeoutExpired:
        return "zaman_asimi"


def main():
    ap = argparse.ArgumentParser(description="JUS fuzz betigi")
    ap.add_argument("--jus", required=True, help="yorumlayici yolu")
    ap.add_argument("--adet", type=int, default=1000)
    ap.add_argument("--tohum", type=int, default=1)
    ap.add_argument("--is", dest="is_sayisi", type=int, default=8)
    ap.add_argument("--zaman-asimi", type=float, default=5.0)
    ap.add_argument("--cikti", default=os.path.join(KOK, "bulgular", "ham"))
    ap.add_argument("--strateji", choices=["bozma", "belirtec", "yapisal", "sinir"])
    a = ap.parse_args()

    r = random.Random(a.tohum)
    tohumlar = tohum_dosyalari()
    os.makedirs(a.cikti, exist_ok=True)
    gecici = tempfile.mkdtemp(prefix="jusfuzz_")
    # girdiler once tek iplikte uretilir (tekrarlanabilirlik)
    girdiler = [uret(r, tohumlar, a.strateji) for _ in range(a.adet)]
    sayac = {}
    bulgu = []
    kilit = threading.Lock()

    def is_(i):
        s, veri = girdiler[i]
        kod = calistir(a.jus, veri, a.zaman_asimi, gecici)
        with kilit:
            sayac[s] = sayac.get(s, 0) + 1
            if kod == "zaman_asimi" or kod not in GECERLI:
                ad = "t%d_%05d_%s_%s.jus" % (a.tohum, i, s, kod)
                with open(os.path.join(a.cikti, ad), "wb") as f:
                    f.write(veri)
                bulgu.append((ad, kod))
                print("BULGU %s cikis=%s" % (ad, kod), flush=True)

    try:
        with ThreadPoolExecutor(a.is_sayisi) as ex:
            list(ex.map(is_, range(a.adet)))
    finally:
        shutil.rmtree(gecici, ignore_errors=True)
    print("tohum=%d adet=%d stratejiler=%s bulgu=%d" % (a.tohum, a.adet, sayac, len(bulgu)))
    return 1 if bulgu else 0


if __name__ == "__main__":
    sys.exit(main())
