from enum import Enum


class PorD(Enum):
    POINT = "Point"
    DIVISOR = "Divisor"


def Multiplier(a, N, P, Q):
    if Q == [0, 1, 0]:
        return PorD.POINT, P
    if P == [0, 1, 0]:
        return PorD.POINT, Q

    x1, y1, _ = P
    x2, y2, _ = Q
    if (x1, y1) == (x2, -y2):
        return PorD.POINT, [0, 1, 0]

    d = gcd(x1 - x2, N)
    if d not in (1, N):
        return PorD.DIVISOR, d

    if x1 == x2:
        d = gcd(y1 + y2, N)
        if d > 1:
            return PorD.DIVISOR, d
        al = Mod((3 * pow(x1, 2) + a) / (y1 + y2), N)
    else:
        al = Mod((y2 - y1) / (x2 - x1), N)
    bet = Mod(y1 - al * x1, N)
    x3 = Mod(pow(al, 2) - x1 - x2, N)
    y3 = Mod(-(al * x3 + bet), N)
    return PorD.POINT, [x3, y3, 1]


def Multiplication(a, N, P, k):
    k_bits = list(map(int, bin(k)[2:]))
    Q = [0, 1, 0]
    for bit in k_bits:
        pord, Q = Multiplier(a, N, Q, Q)
        if pord == PorD.DIVISOR:
            return pord, Q
        if bit == 1:
            pord, Q = Multiplier(a, N, Q, P)
            if pord == PorD.DIVISOR:
                return pord, Q
    return PorD.POINT, Q


def factorECM(N):
    """
    TESTS::
      sage: factorECM(100070000190133)
      [10007, 10000000019]
      sage: factorECM(100181800505809010267)
      [5009090003, 20000000089]
      sage: factorECM(6986389896254914969335451)
      [833489857, 8382093480298843]
    """
    b1 = ceil(exp((1 / sqrt(2)) * log(N) ^ 0.5 * log(log(N)) ^ 0.5))

    while True:
        a = Integers(N).random_element()
        x = Integers(N).random_element()
        y = Integers(N).random_element()
        b = Mod(y ^ 2 - x ^ 3 - a * x, N)
        g = gcd(Mod(4 * a ^ 3 + 27 * b ^ 2, N), N)
        if g == N:
            continue
        if g != 1:
            return sorted([int(g), N // int(g)])

        P = [x, y, 1]
        for p in Primes():
            if p > b1:
                break
            e = 1
            pe = p
            while pe < b1:
                pord, P = Multiplication(a, N, P, pe)
                if pord == PorD.DIVISOR:
                    return sorted([int(P), N // int(P)])
                e += 1
                pe = pow(p, e)
