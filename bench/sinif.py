# Sınıf: nesne oluşturma, yöntem çağrısı, alan erişimi, kalıtım
class Sayac:
    def __init__(self, adim):
        self.adim = adim
        self.deger = 0

    def artir(self):
        self.deger += self.adim

    def al(self):
        return self.deger


class CiftSayac(Sayac):
    def artir(self):
        super().artir()
        self.deger += 1


a = Sayac(2)
b = CiftSayac(3)
for i in range(1000000):
    a.artir()
    b.artir()
nesne_toplam = 0
for i in range(100000):
    s = Sayac(i)
    s.artir()
    nesne_toplam += s.al()
print(a.al(), b.al(), nesne_toplam)
