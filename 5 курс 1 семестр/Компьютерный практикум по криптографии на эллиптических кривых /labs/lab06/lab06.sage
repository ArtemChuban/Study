def test_Prove_prime(p):
    Cert = Prove_prime(p)
    return Check_prime(p, Cert)


def Gen_curve(p):
    while True:
        while True:
            a = randrange(p)
            b = randrange(p)
            E = EllipticCurve(GF(p), [a, b])
            kol = E.order()
            d = 4 * pow(a, 3, p) + 27 * pow(b, 2, p)
            if (gcd(d, p) == 1) and (kol % 2 == 0):
                break
        q = kol // 2
        if (q % 2 != 0) and (q % 3 != 0) and (MR(p, q) == True):
            return a, b, q


def MR(n, a):
    k = 0
    q = n - 1
    while q % 2 == 0:
        k += 1
        q //= 2
    a = pow(a, q, n)
    if a == 1:
        return True
    for _ in range(k):
        if a == (n - 1):
            return True
        a = pow(a, 2, n)
    return False


def Find_point(p, q, a, b):
    while True:
        x = Mod(randrange(p), p)
        yy = pow(x, 3, p) + a * x + b
        if yy.is_square():
            break
    y = yy.nth_root(2)
    E = EllipticCurve(GF(p), [a, b])
    L = E(x, y)
    if q * L != E(0, 1, 0):
        return Find_point(p, q, a, b)
    return L


def Check_prime(p, Cert):
    p0 = p
    for (a, b), L, pi in Cert:
        assert p0 % 2 != 0
        assert p0 % 3 != 0
        assert gcd(4 * a ^ 3 + 27 * b ^ 2, p0) == 1
        assert pi > pow(sqrt(sqrt(p0)) + 1, 2)
        assert str(L) != "(0 : 1 : 0)"
        assert str(pi * L) == "(0 : 1 : 0)"
        p0 = pi
    return True


def Prove_prime(p):
    """
    TESTS::
      sage: test_Prove_prime(1000003)
      True

      sage: test_Prove_prime(100000000003)
      True

    """
    i = 0
    pi = p
    C = []
    lp = pow(log(p), log(log(p)))
    while pi > p // 5:
        a, b, pi1 = Gen_curve(pi)
        L = Find_point(pi, pi1, a, b)
        i += 1
        if (i >= lp) or (pi % 2 == 0) or (pi % 3 == 0) or (not is_prime(pi1)):
            return Prove_prime(p)
        C += [((a, b), L, pi1)]
        pi = pi1
    return C
