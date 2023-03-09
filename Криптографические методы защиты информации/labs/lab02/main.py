import random


def LCSR(taps, S ,M):
    accumulator = sum([S[t] for t in taps]) + M
    out = S.pop()
    S.insert(0, accumulator % 2)
    return S,  accumulator // 2, out


def GammaGenerate(taps,S,M,n):
    arr = []
    with open("output.txt","w") as output:
        S, M, element = LCSR(taps,S,M)
        print(str(element), file=output, end="")
        arr.append(element)
        for _ in range(n):
            S,M,element = LCSR(taps,S,M)
            output.write(str(element))
            arr.append(element)
    return arr


def GenTapSeq(q):
    bin_q = bin(q + 1)[2:][::-1]
    n = len(bin_q)
    taps = [i - 1 for i in range(0, n) if bin_q[i] == "1"]
    return taps, n


q = 5483
taps, n = GenTapSeq(q)
T_max = q - 1
print(f"Точки съема: {taps}")
print(f"Теоретический максимальный период: {T_max}")


S =[random.randrange(2) for _ in range(max(taps) + 1)]
M = random.randrange(max(taps) - 2)
print(f"Начальное состояние: {S}")



GammaGenerate(taps, S, M, 10 ** 6) 
with open("output.txt", "r") as file:
    init_len = T_max + 10
    start = 1000
    gamma = list(map(int, list(file.readline())))
    init = gamma[start:init_len+start]
    for i in range(start+1,len(gamma)-init_len):
        if init == gamma[i:i+init_len]:
            period = i-(start)
            print(f"Фактический период: {period}")
            break

count = 0 
for i in range(10 ** 5 * 2):
    if gamma[i:i + T_max] != gamma[i+T_max:i+T_max + T_max]:
        count+=1
    else:
        break
print(f"Хвост до периода: {count}")

# import itertools as it
# mas_S = list(it.product([0,1], repeat=max(taps)+1))
# for M in range(1,1190):
#     print(M)
#     check = False
#     for S in mas_S:
#         S = list(S)
#         gamma = GammaGenerate(taps,S,M,300)
#         init = gamma[len(gamma)//2:-1]
#         if init == [1 for i in range(0,len(gamma)//2)] or init == [0 for i in range(0,len(gamma)//2)]:
#             print(f"S = {S}")
#             print(f"M = {M}")
#             print()
#             check = True
#             break
#     if check: break