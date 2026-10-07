# Metin ekleme: bir metne döngüde parça eklemek (s += parça)
s = ""
for i in range(20000):
    s += "ab"
t = ""
for i in range(5000):
    t = t + str(i) + ";"
print(len(s), len(t))
