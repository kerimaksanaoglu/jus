# Eratosthenes eleği: liste dizinleme ve atama
n = 2000000
asal = []
for i in range(n + 1):
    asal.append(True)
asal[0] = False
asal[1] = False
i = 2
while i * i <= n:
    if asal[i]:
        j = i * i
        while j <= n:
            asal[j] = False
            j += i
    i += 1
sayi_ = 0
for k in asal:
    if k:
        sayi_ += 1
print(sayi_)
