# Hata yakalama: fırlat/yakala ve dil hatası yakalama
class Hata(Exception):
    def __init__(self, deger):
        self.deger = deger


def kontrol(i):
    if i % 3 == 0:
        raise Hata(i)
    return i


yakalanan = 0
tamam = 0
for i in range(2000000):
    try:
        tamam += kontrol(i)
    except Hata as e:
        yakalanan += e.deger
sifir = 0
bolme = 0
for i in range(600000):
    try:
        x = i / sifir
    except ZeroDivisionError:
        bolme += 1
print(yakalanan, tamam, bolme)
