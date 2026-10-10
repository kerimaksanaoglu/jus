#!/usr/bin/env python3
"""Depodaki sürüm numaralarını günceller.

Kullanım: python tools/surum.py <yeni-sürüm>      (ör. 1.2.0)

Güncellenen yerler: src/common.h, docs/dil-tanimi.md, README.md,
editors/vscode/package.json, docs/rehber/README.md ve değişiklik günlüğündeki
"Yayımlanmadı" başlığı (altına yeni sürümün başlığı açılır).

Değişiklik günlüğünün içeriği ve yol haritasındaki durum sütunu elle yazılır.
"""
import datetime
import json
import os
import re
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def read(relative):
    with open(os.path.join(ROOT, relative), encoding="utf-8", newline="") as f:
        return f.read()


def write(relative, text):
    with open(os.path.join(ROOT, relative), "w", encoding="utf-8", newline="") as f:
        f.write(text)


def replace_once(relative, pattern, replacement):
    text = read(relative)
    updated, count = re.subn(pattern, replacement, text, count=1)
    if count != 1:
        sys.exit("%s içinde beklenen sürüm satırı bulunamadı: %s" % (relative, pattern))
    write(relative, updated)
    print("güncellendi:", relative)


def main():
    if len(sys.argv) != 2 or not re.fullmatch(r"\d+\.\d+\.\d+", sys.argv[1]):
        sys.exit(__doc__)
    version = sys.argv[1]

    replace_once("src/common.h", r'#define JUS_VERSION "[^"]+"', '#define JUS_VERSION "%s"' % version)
    replace_once("docs/dil-tanimi.md", r"(?m)^Sürüm \d+\.\d+\.\d+", "Sürüm %s" % version)
    replace_once("README.md", r"Geçerli sürüm: \*\*\d+\.\d+\.\d+\*\*", "Geçerli sürüm: **%s**" % version)

    package = json.loads(read("editors/vscode/package.json"))
    package["version"] = version
    write("editors/vscode/package.json", json.dumps(package, ensure_ascii=False, indent=2) + "\n")
    print("güncellendi: editors/vscode/package.json")

    # Rehberde 'jus --surum' çıktısı örnek olarak geçer.
    guide = read("docs/rehber/README.md")
    write("docs/rehber/README.md", re.sub(r"JUS \d+\.\d+\.\d+", "JUS %s" % version, guide))

    today = datetime.date.today().isoformat()
    changelog = read("CHANGELOG.md")
    newline = "\r\n" if "\r\n" in changelog else "\n"
    marker = "## [Yayımlanmadı]"
    if "## [%s]" % version in changelog:
        print("değişiklik günlüğünde %s zaten var." % version)
    elif marker not in changelog:
        sys.exit("CHANGELOG.md içinde '%s' başlığı yok." % marker)
    else:
        changelog = changelog.replace(marker, marker + newline + newline + "## [%s] - %s" % (version, today), 1)
        write("CHANGELOG.md", changelog)
        print("güncellendi: CHANGELOG.md")


if __name__ == "__main__":
    main()
