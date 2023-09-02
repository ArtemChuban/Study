import math

blocks = """2887763929737
14268468183889
17106478222082
11308338337725
22932870001788
22780920502986
3159009422412
22191880208231
24883589317156
20042326937734
21464252061935
6743660373779"""
N = 33644210466973
e = 5285461
Cs = list(map(int, blocks.split("\n")))

for t in range(math.floor(math.sqrt(N)) + 1, N + 1):
	sqra = pow(t, 2) - N
	a = int(math.sqrt(sqra))
	if pow(a, 2) == sqra:
		s = a
		break
print(f"t = {t}\ns = {s}")

p = t - s
q = t + s
print(f"p = {p}\nq = {q}")

phiN = (p - 1) * (q - 1)
d = pow(e, -1, phiN)

print(f"phiN = {phiN}\nd = {d}")

Ms = [pow(C, d, N) for C in Cs]

for i in range(len(Cs)):
	print(Cs[i], Ms[i])
