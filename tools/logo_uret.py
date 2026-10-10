#!/usr/bin/env python3
"""JUS logosunu ve simgelerini üretir.

Kullanım: python tools/logo_uret.py

Logo hazır bir yazı tipine dayanmaz: J, U ve S harfleri tek kalınlıkta,
yuvarlak uçlu çizgilerden kurulur; ardından satır imlecini andıran bir blok
gelir. Biçim bu dosyadaki sayılarla tanımlıdır; SVG ve PNG çıktıları aynı
geometriden üretilir.

SVG çıktıları için ek paket gerekmez; PNG çıktıları için Pillow gerekir
(pip install pillow).
"""
import math
import os

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

INK = "#16202e"      # yazı rengi (açık zemin)
PAPER = "#ffffff"    # yazı rengi (koyu zemin)
ACCENT = "#2f6fed"   # vurgu: imleç ve simge zemini
ACCENT_SOFT = "#a9c6ff"  # simgede, vurgu zemini üzerindeki imleç

HEIGHT = 100.0   # harf yüksekliği (çizgi ekseninden eksene)
STROKE = 21.0    # çizgi kalınlığı
GAP = 19.0       # harfler arası boşluk (çizgi kenarından kenara)


def polar(cx, cy, r, degrees):
    """Açı saat yönünde artar (SVG'de y aşağı doğrudur)."""
    a = math.radians(degrees)
    return (cx + r * math.cos(a), cy + r * math.sin(a))


class Path:
    """Doğru ve yay parçalarından oluşan bir çizgi; hem SVG'ye hem noktalara dökülür."""

    def __init__(self, x, y):
        self.commands = ["M %.2f %.2f" % (x, y)]
        self.points = [(x, y)]

    def line(self, x, y):
        self.commands.append("L %.2f %.2f" % (x, y))
        x0, y0 = self.points[-1]
        steps = max(2, int(math.hypot(x - x0, y - y0) * 4))
        for i in range(1, steps + 1):
            t = i / steps
            self.points.append((x0 + (x - x0) * t, y0 + (y - y0) * t))
        return self

    def arc(self, cx, cy, r, start, end):
        """Merkezi (cx, cy) olan çemberde start açısından end açısına yay."""
        sweep = 1 if end > start else 0
        large = 1 if abs(end - start) > 180 else 0
        x, y = polar(cx, cy, r, end)
        self.commands.append("A %.2f %.2f 0 %d %d %.2f %.2f" % (r, r, large, sweep, x, y))
        steps = max(8, int(abs(end - start) * r * math.pi / 180 * 4))
        for i in range(1, steps + 1):
            self.points.append(polar(cx, cy, r, start + (end - start) * i / steps))
        return self

    def svg(self):
        return " ".join(self.commands)


def build():
    """Harf çizgilerini ve imleç dikdörtgenini döndürür: (yollar, imleç, sınır kutusu)."""
    half = STROKE / 2
    paths = []

    # J: sağda düz gövde, altta sola dönen kanca.
    hook = 24.0
    stem = 2 * hook
    j = Path(stem, 0).line(stem, HEIGHT - hook).arc(hook, HEIGHT - hook, hook, 0, 152)
    paths.append(j)
    right = stem + half

    # U: iki gövde ve alt yay.
    bowl = 27.5
    left = right + GAP + half
    u = (Path(left, 0).line(left, HEIGHT - bowl)
         .arc(left + bowl, HEIGHT - bowl, bowl, 180, 0)
         .line(left + 2 * bowl, 0))
    paths.append(u)
    right = left + 2 * bowl + half

    # S: üstte küçük, altta büyük iki yay; görsel denge için üst yay biraz dardır.
    top, bottom = 23.5, 26.5
    left = right + GAP - 4 + half  # yuvarlak harf, düz gövdeye biraz daha yakın durur
    cx = left + bottom
    s = (Path(*polar(cx, top, top, -28))
         .arc(cx, top, top, -28, -270)
         .arc(cx, 2 * top + bottom, bottom, -90, 152))
    paths.append(s)
    right = cx + bottom + half

    # İmleç: taban çizgisinde duran, köşeli bir blok.
    cursor = (right + GAP - 5, HEIGHT + half - STROKE, 38.0, STROKE)

    min_x = min(x for path in paths for x, _ in path.points) - half
    max_x = cursor[0] + cursor[2]
    box = (min_x, -half, max_x - min_x, HEIGHT + STROKE)
    return paths, cursor, box


def wordmark_svg(ink, accent, padding=0.0):
    paths, cursor, box = build()
    x, y, w, h = box[0] - padding, box[1] - padding, box[2] + 2 * padding, box[3] + 2 * padding
    lines = [
        '<svg xmlns="http://www.w3.org/2000/svg" viewBox="%.2f %.2f %.2f %.2f" role="img" aria-label="JUS">'
        % (x, y, w, h),
        '  <title>JUS</title>',
        '  <g fill="none" stroke="%s" stroke-width="%.2f" stroke-linecap="round" stroke-linejoin="round">'
        % (ink, STROKE),
    ]
    for path in paths:
        lines.append('    <path d="%s"/>' % path.svg())
    lines.append('  </g>')
    lines.append('  <rect x="%.2f" y="%.2f" width="%.2f" height="%.2f" fill="%s"/>' % (cursor + (accent,)))
    lines.append('</svg>')
    return "\n".join(lines) + "\n"


def icon_layout(size):
    """Simgede logonun ölçeği ve konumu: (ölçek, x kayması, y kayması)."""
    _, _, box = build()
    scale = size * 0.70 / box[2]
    dx = (size - box[2] * scale) / 2 - box[0] * scale
    dy = (size - box[3] * scale) / 2 - box[1] * scale
    return scale, dx, dy


def icon_svg(size=256):
    paths, cursor, _ = build()
    scale, dx, dy = icon_layout(size)
    lines = [
        '<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 %d %d" role="img" aria-label="JUS">' % (size, size),
        '  <title>JUS</title>',
        '  <rect width="%d" height="%d" rx="%.1f" fill="%s"/>' % (size, size, size * 0.22, ACCENT),
        '  <g transform="translate(%.3f %.3f) scale(%.5f)">' % (dx, dy, scale),
        '    <g fill="none" stroke="%s" stroke-width="%.2f" stroke-linecap="round" stroke-linejoin="round">'
        % (PAPER, STROKE),
    ]
    for path in paths:
        lines.append('      <path d="%s"/>' % path.svg())
    lines.append('    </g>')
    lines.append('    <rect x="%.2f" y="%.2f" width="%.2f" height="%.2f" fill="%s"/>' % (cursor + (ACCENT_SOFT,)))
    lines.append('  </g>')
    lines.append('</svg>')
    return "\n".join(lines) + "\n"


def hex_color(value):
    value = value.lstrip("#")
    return tuple(int(value[i:i + 2], 16) for i in (0, 2, 4)) + (255,)


def draw_wordmark(draw, scale, dx, dy, ink, accent):
    """Logoyu verilen ölçek ve kaymayla çizer; yuvarlak uçlar için çizgi boyunca daire basar."""
    paths, cursor, _ = build()
    radius = STROKE / 2 * scale
    for path in paths:
        for x, y in path.points:
            px, py = x * scale + dx, y * scale + dy
            draw.ellipse([px - radius, py - radius, px + radius, py + radius], fill=ink)
    x, y, w, h = cursor
    draw.rectangle([x * scale + dx, y * scale + dy, (x + w) * scale + dx, (y + h) * scale + dy], fill=accent)


def wordmark_png(width, ink, accent, background=None, padding=0.12):
    from PIL import Image, ImageDraw

    _, _, box = build()
    over = 4  # kenar yumuşatma için büyük çizip küçült
    pad = box[2] * padding
    scale = width * over / (box[2] + 2 * pad)
    height = int(round((box[3] + 2 * pad) * scale / over))
    image = Image.new("RGBA", (width * over, height * over), background or (0, 0, 0, 0))
    draw = ImageDraw.Draw(image)
    draw_wordmark(draw, scale, (pad - box[0]) * scale, (pad - box[1]) * scale, hex_color(ink), hex_color(accent))
    return image.resize((width, height), Image.LANCZOS)


def icon_png(size):
    from PIL import Image, ImageDraw

    over = 8
    big = size * over
    image = Image.new("RGBA", (big, big), (0, 0, 0, 0))
    draw = ImageDraw.Draw(image)
    draw.rounded_rectangle([0, 0, big - 1, big - 1], radius=big * 0.22, fill=hex_color(ACCENT))
    scale, dx, dy = icon_layout(big)
    draw_wordmark(draw, scale, dx, dy, hex_color(PAPER), hex_color(ACCENT_SOFT))
    return image.resize((size, size), Image.LANCZOS)


def write_text(relative, text):
    path = os.path.join(ROOT, relative)
    os.makedirs(os.path.dirname(path), exist_ok=True)
    with open(path, "w", encoding="utf-8", newline="\n") as f:
        f.write(text)
    print("yazildi:", relative)


def write_image(relative, image):
    path = os.path.join(ROOT, relative)
    os.makedirs(os.path.dirname(path), exist_ok=True)
    image.save(path)
    print("yazildi:", relative)


def main():
    write_text("marka/logo.svg", wordmark_svg(INK, ACCENT))
    write_text("marka/logo-acik.svg", wordmark_svg(PAPER, ACCENT))
    write_text("marka/simge.svg", icon_svg())
    # Belge sitesinin ana sayfası için.
    write_text("docs/logo.svg", wordmark_svg(INK, ACCENT))
    write_text("docs/logo-acik.svg", wordmark_svg(PAPER, ACCENT))

    write_image("marka/logo.png", wordmark_png(1200, INK, ACCENT))
    write_image("marka/logo-acik.png", wordmark_png(1200, PAPER, ACCENT))
    write_image("marka/simge.png", icon_png(512))
    write_image("editors/vscode/simge.png", icon_png(256))
    write_image("playground/simge.png", icon_png(64))
    write_image("docs/simge.png", icon_png(64))


if __name__ == "__main__":
    main()
