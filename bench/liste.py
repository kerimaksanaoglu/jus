# Liste: ekleme, dizinleme, sıralama
x = 12345
l = []
for i in range(900000):
    x = (x * 1103 + 12345) % 1000003
    l.append(x)
toplam = 0
for i in range(900000):
    toplam += l[i] % 10
s = sorted(l)
print(toplam, s[0], s[450000], s[-1])
