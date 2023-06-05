from hashlib import sha3_224

custom_hash = lambda x: F("0x" + str(sha3_224(str(x).encode()).hexdigest()))
p = ZZ('0xFFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFE FFFFFC2F')
E = EllipticCurve(GF(p), [0, 7])
G = E.gens()[0]
n = ZZ("0xFFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFE BAAEDCE6 AF48A03B BFD25E8C D0364141")
F = GF(n)
m = "0x319b9b3bfaab268d80b43e3d12289cafa42e8674247c883810ad106494db5496"
Nsigners = 6
Nrounds = 14
pi = 2

Pij = [[E.random_point() for j in range(Nrounds)] for i in range(Nsigners)]  # Открытые ключи всех участников
print(f"{Pij = }")

xPi = [F.random_element() for j in range(Nrounds)]
KpubPi = [xPi[j] * G for j in range(Nrounds)]
Pij[pi] = KpubPi
print(f"{KpubPi = }")

sji = [[F.random_element() for i in range(Nsigners)] for j in range(Nrounds)]  # Случайные скаляры
for i in range(Nrounds):
	sji[i][pi] = 0
print(f"{sji = }\n")

Ij = [F(xPi[j]) * custom_hash(Pij[pi][j]) for j in range(Nrounds)]
print(f"{Ij = }\n")

print("1.")
alpha = [F.random_element() for j in range(Nrounds)]
Lpi = [alpha[j] * G for j in range(Nrounds)]
Rpi = [alpha[j] * custom_hash(Pij[pi][j]) for j in range(Nrounds)]
print(f"{Lpi = }\n")
print(f"{alpha = }\n")
print(f"{Rpi = }\n")

print("2.")
Hash_object = sha3_224()
Hash_object.update(m.encode())
for j in range(Nrounds):
	Hash_object.update((str(Lpi[j]) + str(Rpi[j])).encode())
cPi1 = F('0x' + str(Hash_object.hexdigest()))
print(f"{cPi1 = }\n")

print("3.")
R = Integers(Nsigners)
mas = []
ci = cPi1
mas.append(ci)
c0 = 0
i = R(pi) + 1
while i != pi:
	Lji = [sji[j][i] * G + ci * Pij[i][j] for j in range(Nrounds)]
	Rji = [sji[j][i] * custom_hash(Pij[i][j]) + ci * Ij[j] for j in range(Nrounds)]
	Hash_object = sha3_224()
	Hash_object.update(m.encode())
	i += 1
	for j1 in range(Nrounds):
		Hash_object.update((str(Lji[j1]) + str(Rji[j1])).encode())
	ci = F('0x' + str(Hash_object.hexdigest()))
	mas.append(ci)
	if i == 0:
		c0 = ci
print(f"{Lji = }\n")
print(f"{Rji = }\n")

print("4.")
Hash_object = sha3_224()
Hash_object.update(m.encode())
for j in range(Nrounds):
	Hash_object.update((str(Lji[j]) + str(Rji[j])).encode())
cPi = F('0x' + str(Hash_object.hexdigest()))
print(f"{cPi = }\n")

print("5.")
sjPi = [F(alpha[j] - xPi[j] * cPi) for j in range(Nrounds)]
for j in range(Nrounds):
	sji[j][pi] = sjPi[j]
print(f"{sjPi = }\n")

print(f"signature.\n{Ij = },\n {c0 = },\n {sji = }")

print("verifying signature.")
ci = c0
i = 0
while i != Nsigners:
	Lji = [sji[j][i] * G + ci * Pij[i][j] for j in range(Nrounds)]
	Rji = [sji[j][i] * custom_hash(Pij[i][j]) + ci * Ij[j] for j in range(Nrounds)]
	Hash_object = sha3_224()
	Hash_object.update(m.encode())
	for j1 in range(Nrounds):
		Hash_object.update((str(Lji[j1]) + str(Rji[j1])).encode())
	ci = F('0x' + str(Hash_object.hexdigest()))
	i += 1
Hash_object = sha3_224()
Hash_object.update(m.encode())
for j1 in range(Nrounds):
	Hash_object.update((str(Lji[j1]) + str(Rji[j1])).encode())
c_n = F('0x' + str(Hash_object.hexdigest()))

print(f"{c0 = } {'==' if c0 == c_n else '!='} {c_n = }")