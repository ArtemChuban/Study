def xgcd(a,b):
	"""xgcd(a,b) returns a tuple of form (g,x,y), where g is gcd(a,b) and
	x,y satisfy the equation g = ax + by."""
	a1=1; b1=0; a2=0; b2=1; aneg=1; bneg=1
	if(a < 0):
		a = -a; aneg=-1
	if(b < 0):
		b = -b; bneg=-1
	while (1):
		quot = -(a // b)
		a = a % b
		a1 = a1 + quot*a2; b1 = b1 + quot*b2
		if(a == 0):
			return (b, a2*aneg, b2*bneg)
		quot = -(b // a)
		b = b % a
		a2 = a2 + quot*a1; b2 = b2 + quot*b1
		if(b == 0):
			return (a, a1*aneg, b1*bneg)

N = 1176879950087
e1 = 550169
e2 = 376237

blocks1 = """236505725833
12096288569
1062670335800
541231133081
529745761698
79574674510
518908160088
195753762481
284194617926
861518052504
844805726716
575330762793
319168661888
377123370130"""

blocks2 = """169179266140
617962027334
332483986069
1065692323879
420409290920
733896529297
201622748685
457529162746
1037225648947
732504268577
1172056967964
1002467039854
850197148213
279510203667"""

y1 = list(map(int, blocks1.split("\n")))
y2 = list(map(int, blocks2.split("\n")))

_, r, s = xgcd(e1, e2)
print(f"r = {r}\ns = {s}")

for i in range(len(y1)):
	x = pow(y1[i], r, N) * pow(y2[i], s, N) % N
	print(x)
