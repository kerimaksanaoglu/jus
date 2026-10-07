# Sözlük: sayı ve metin anahtarlarla ekleme ve okuma
d = {}
for i in range(400000):
    d[i] = i * 2
toplam = 0
for i in range(400000):
    toplam += d[i]
m = {}
for i in range(300000):
    m["k" + str(i % 20000)] = i
for i in range(300000):
    toplam += m["k" + str(i % 20000)]
print(toplam, len(d), len(m))
