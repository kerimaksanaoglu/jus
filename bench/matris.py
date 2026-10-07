# İç içe liste: matris çarpımı
n = 140
a = []
b = []
for i in range(n):
    sa = []
    sb = []
    for j in range(n):
        sa.append((i + j) % 10)
        sb.append((i * j) % 10)
    a.append(sa)
    b.append(sb)
c = []
for i in range(n):
    satir = []
    for j in range(n):
        t = 0
        for k in range(n):
            t += a[i][k] * b[k][j]
        satir.append(t)
    c.append(satir)
iz = 0
for i in range(n):
    iz += c[i][i]
print(iz, c[0][0], c[n - 1][n - 1])
