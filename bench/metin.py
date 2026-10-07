# Metin: metne çevirme, birleştir/böl, büyük harf, karakter dolaşma
parcalar = []
for i in range(300000):
    parcalar.append(str(i))
uzun = ",".join(parcalar)
geri = uzun.split(",")
toplam = 0
for p in geri:
    toplam += len(p)
virgul = 0
for karakter in uzun:
    if karakter == ",":
        virgul += 1
print(len(uzun), len(geri), toplam, virgul, len(uzun.upper()))
