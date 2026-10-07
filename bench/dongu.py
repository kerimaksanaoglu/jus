# İç içe döngü ile toplam: döngü, kalan ve toplama
toplam = 0
for i in range(3000):
    for j in range(2000):
        toplam += (i * j) % 7
print(toplam)
