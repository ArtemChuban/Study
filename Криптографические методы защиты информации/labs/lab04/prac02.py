N = 414634315817
e = 1039187
blocks = """200343263939
13939901815
329718769183
169659670872
49667978685
11286581382
92461615100
173590557244
62542045222
310782145259
348390168011
308011216304
154928746700"""
Cs = list(map(int, blocks.split("\n")))

C = Cs[0]
Mp1 = pow(C, e, N)
M = 0
k = 0
while Mp1 != C:
	M = Mp1
	Mp1 = pow(M, e, N)
	k += 1

print(f"k = {k}")

for C in Cs:
	Mp1 = pow(C, e, N)
	M = 0

	while Mp1 != C:
		M = Mp1
		Mp1 = pow(M, e, N)

	print(C, M)
