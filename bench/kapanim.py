# Kapanım: yakalanan değişkeni değiştiren iç fonksiyon çağrıları
def sayac_yap():
    n = 0

    def artir(k):
        nonlocal n
        n += k
        return n

    return artir


f = sayac_yap()
g = sayac_yap()
son = 0
for i in range(2000000):
    son = f(1)
    g(2)
print(son, g(0))
