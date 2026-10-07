#!/usr/bin/env python3
"""lib/ altındaki JUS modüllerini yorumlayıcıya gömmek için src/embedded.c dosyasını üretir.

Kullanım: python tools/gomulu_uret.py

lib/ altındaki bir dosya değiştirildiğinde yeniden çalıştırılmalı ve üretilen
dosya da depoya eklenmelidir.
"""
import os

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
LIB = os.path.join(ROOT, "lib")
OUT = os.path.join(ROOT, "src", "embedded.c")


def c_string(text):
    out = []
    for byte in text.encode("utf-8"):
        if byte in (0x22, 0x5C):
            out.append("\\" + chr(byte))
        elif 0x20 <= byte < 0x7F:
            out.append(chr(byte))
        else:
            out.append("\\%03o" % byte)
    return '"' + "".join(out) + '"'


def main():
    names = sorted(f[:-4] for f in os.listdir(LIB) if f.endswith(".jus"))
    lines = [
        "/* Bu dosya tools/gomulu_uret.py tarafından lib/ klasöründen üretilir; elle düzenlemeyin. */",
        "",
        "#include <string.h>",
        "",
        '#include "stdlib_modules.h"',
        "",
    ]
    for index, name in enumerate(names):
        with open(os.path.join(LIB, name + ".jus"), encoding="utf-8", newline="") as f:
            source = f.read().replace("\r\n", "\n")
        # Uzun dize sabitleri taşınabilir değildir; kaynak bayt dizisi olarak yazılır.
        data = source.encode("utf-8") + b"\0"
        lines.append("static const char source%d[] = {" % index)
        for start in range(0, len(data), 20):
            chunk = data[start:start + 20]
            lines.append("    " + ", ".join(str(b if b < 128 else b - 256) for b in chunk) + ",")
        lines.append("};")
        lines.append("")

    lines.append("const char *embeddedModuleSource(const char *name) {")
    for index, name in enumerate(names):
        lines.append("    if (strcmp(name, %s) == 0) return source%d;" % (c_string(name), index))
    lines.append("    (void)name;")
    lines.append("    return NULL;")
    lines.append("}")
    lines.append("")

    with open(OUT, "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(lines))
    print("src/embedded.c üretildi:", ", ".join(names))


if __name__ == "__main__":
    main()
