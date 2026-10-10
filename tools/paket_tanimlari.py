#!/usr/bin/env python3
"""Bir sürüm için Scoop ve winget paket tanımlarını üretir.

Kullanım: python tools/paket_tanimlari.py <sürüm> <windows-zip-dosyası>

Örnek:    python tools/paket_tanimlari.py 1.1.0 jus-1.1.0-windows-x64.zip

Üretilenler:
  bucket/jus.json                 Scoop tanımı (bu depo bir Scoop kovasıdır)
  paketleme/winget/<sürüm>/       winget tanım dosyaları (microsoft/winget-pkgs
                                  deposuna gönderilecek üç dosya)

Sürüm yayımlandıktan sonra çalıştırılır; zip dosyasının özeti (SHA-256)
tanımlara yazılır. Yayın iş akışı bunu kendiliğinden yapar.
"""
import hashlib
import json
import os
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
REPO = "https://github.com/kerimaksanaoglu/jus"
PACKAGE_ID = "JUSBil.JUS"
PUBLISHER = "JUSBil"
DESCRIPTION_TR = "Türkçe sözdizimli, genel amaçlı betik dili"
DESCRIPTION_EN = "General-purpose scripting language with Turkish syntax"
WINGET_SCHEMA = "1.6.0"


def write(relative, text):
    path = os.path.join(ROOT, relative)
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w", encoding="utf-8", newline="\n") as f:
        f.write(text)
    print("yazildi:", relative)


def scoop_manifest(version, sha):
    folder = "jus-%s-windows-x64" % version
    manifest = {
        "version": version,
        "description": "JUS: %s" % DESCRIPTION_TR,
        "homepage": REPO,
        "license": "MIT",
        "architecture": {
            "64bit": {
                "url": "%s/releases/download/v%s/%s.zip" % (REPO, version, folder),
                "hash": sha,
                "extract_dir": folder,
            }
        },
        "bin": "jus.exe",
        "checkver": "github",
        "autoupdate": {
            "architecture": {
                "64bit": {
                    "url": "%s/releases/download/v$version/jus-$version-windows-x64.zip" % REPO,
                    "extract_dir": "jus-$version-windows-x64",
                }
            }
        },
    }
    return json.dumps(manifest, ensure_ascii=False, indent=4) + "\n"


def winget_manifests(version, sha):
    folder = "jus-%s-windows-x64" % version
    header = "# yaml-language-server: $schema=https://aka.ms/winget-manifest.%s.%s.schema.json\n\n"
    common = "PackageIdentifier: %s\nPackageVersion: %s\n" % (PACKAGE_ID, version)

    version_file = (header % ("version", WINGET_SCHEMA) + common +
                    "DefaultLocale: en-US\n"
                    "ManifestType: version\n"
                    "ManifestVersion: %s\n" % WINGET_SCHEMA)

    installer_file = (header % ("installer", WINGET_SCHEMA) + common +
                      "InstallerType: zip\n"
                      "NestedInstallerType: portable\n"
                      "NestedInstallerFiles:\n"
                      "- RelativeFilePath: %s\\jus.exe\n"
                      "  PortableCommandAlias: jus\n"
                      "Installers:\n"
                      "- Architecture: x64\n"
                      "  InstallerUrl: %s/releases/download/v%s/%s.zip\n"
                      "  InstallerSha256: %s\n"
                      "ManifestType: installer\n"
                      "ManifestVersion: %s\n" % (folder, REPO, version, folder, sha.upper(), WINGET_SCHEMA))

    def locale_file(kind, locale, description):
        return (header % (kind, WINGET_SCHEMA) + common +
                "PackageLocale: %s\n"
                "Publisher: %s\n"
                "PublisherUrl: https://github.com/kerimaksanaoglu\n"
                "PublisherSupportUrl: %s/issues\n"
                "PackageName: JUS\n"
                "PackageUrl: %s\n"
                "License: MIT\n"
                "LicenseUrl: %s/blob/v%s/LICENSE\n"
                "ShortDescription: %s\n"
                "%s"
                "Tags:\n"
                "- programming-language\n"
                "- interpreter\n"
                "- turkish\n"
                "- education\n"
                "ReleaseNotesUrl: %s/releases/tag/v%s\n"
                "ManifestType: %s\n"
                "ManifestVersion: %s\n"
                % (locale, PUBLISHER, REPO, REPO, REPO, version, description,
                   # Kısa ad yalnızca varsayılan dil dosyasında bulunabilir.
                   "Moniker: jus\n" if kind == "defaultLocale" else "",
                   REPO, version, kind, WINGET_SCHEMA))

    base = "paketleme/winget/%s/%s" % (version, PACKAGE_ID)
    return {
        base + ".yaml": version_file,
        base + ".installer.yaml": installer_file,
        base + ".locale.en-US.yaml": locale_file("defaultLocale", "en-US", DESCRIPTION_EN),
        base + ".locale.tr-TR.yaml": locale_file("locale", "tr-TR", DESCRIPTION_TR),
    }


def main():
    if len(sys.argv) != 3:
        sys.exit(__doc__)
    version, archive = sys.argv[1], sys.argv[2]
    with open(archive, "rb") as f:
        sha = hashlib.sha256(f.read()).hexdigest()

    write("bucket/jus.json", scoop_manifest(version, sha))
    for relative, text in winget_manifests(version, sha).items():
        write(relative, text)
    print("SHA-256:", sha)


if __name__ == "__main__":
    main()
