from hashlib import sha3_224
custom_hash = lambda x: F("0x" + str(sha3_224(str(x).encode()).hexdigest()))

p = 101
E = EllipticCurve(GF(p),[2,3])
G = E.gens()[0]
n1 = G.additive_order()
F = Integers(n1)
gamma = F.random_element()
H = G * gamma
n = 2
print(f"{n1 = }")

print("1.")
xij = [[F.random_element() for j in range (n)] for i in range(2)]
print(f"{xij = }")

Pij = [[xij[i][j] * G for j in range(n)] for i in range (2)]
print(f"{Pij = }")

alpha = [F.random_element() for j in range(n)]
print(f"{alpha = }")

Lij = [[alpha[j] * G for j in range(n)] for i in range(2)]
print(f"{Lij = }")

print("2.")
cij1 = [[custom_hash(Lij[1][j]) for j in range(n)], [custom_hash(Lij[0][j]) for j in range(n)]]
print(f"{cij1 = }")

si1 = [[F.random_element() for j in range(n)], [F.random_element() for j in range(n)]]
print(f"{si1 = }")

print("3.")
Lij1 = [[si1[0][j] * G + cij1[0][j] * H for j in range(n)], [si1[1][j] * G + cij1[1][j] * H for j in range(n)]]
print(f"{Lij1 = }\n")

print("4.")
cij = [[custom_hash(Lij1[1][j]) for j in range(n)], [custom_hash(Lij1[0][j]) for j in range(n)]]
print(f"{cij = }\n")

sij = [[alpha[j] - cij[0][j] * xij[0][j] for j in range(n)], [alpha[j] - cij[1][j] * xij[1][j] for j in range(n)]]
print(f"{sij = }\n")

s = F(sum(sij[0]))
print(f"signature.\n{Lij[0] = }\n{sij[1] = }\n{s = }")

print("verifying signature.")
c1j = [custom_hash(Lij[0][j]) for j in range(n)]
print(f"{c1j = }")
L1j = [si1[1][j] * G + c1j[j] * H for j in range(n)]
print(f"{L1j = }")
c0j = [custom_hash(L1j[j]) for j in range(n)]
print(f"{c0j = }")
rig = [c0j[j] * Pij[0][j] for j in range(n)]
print(f"{sum(Lij[0]) = }")
print(f"{sum(s * G + sum(rig)) = }")
if sum(Lij[0]) == s * G + sum(rig):
	print('correct')
else:
	print('incorrect')
