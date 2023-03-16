from functools import reduce


def lsfr(tap: list[int], s: list[int]) -> tuple[list[int], int]:
    new_bit = reduce(lambda x, y: x ^ y, [s[t - 1] for t in tap])
    s.insert(0, new_bit)
    return s, s.pop()


def iteration(f1: list[int], f2: list[int], gamma_fcsr: int, S1: list[int], S2: list[int])\
        -> tuple[int, list[int], list[int]]:
    out3 = gamma_fcsr
    S1, out1 = lsfr(f1, S1)
    S2, out2 = lsfr(f2, S2)
    f = (out1 and out2) ^ (out1 and out3) ^ (out2 and out3)
    return f, S1, S2


f1 = [5, 4, 2, 1, 0]
f2 = [9, 5, 3, 1, 0]
S1 = [1] * max(f1)
S2 = [1] * max(f2)


with open("input.txt","r") as file:
    gamma_fcsr = list(map(int, list(file.readline())))

n = len(gamma_fcsr)

with open("output.txt","w") as file:
    for i in range(n):
        out, S1, S2 = iteration(f1[0:-1], f2[0:-1], gamma_fcsr[i], S1, S2)
        file.write(str(out))
        
with open("output.txt","r") as file:
    start = 500
    gamma = list(map(int, list(file.readline())))
    init_len = 200
    init = gamma[start:init_len + start]
    for i in range(start + 1, len(gamma) - init_len):
        if init == gamma[i:i + init_len]:
            print(f"Период: {i - start}")
            break
