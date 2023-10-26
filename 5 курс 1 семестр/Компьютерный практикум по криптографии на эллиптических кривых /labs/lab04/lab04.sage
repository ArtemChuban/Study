def test_orderBSGS(i):
    q = Primes().next(2 ^ (16 * i) + 1)
    a = ZZ.random_element(1, q - 1)
    b = ZZ.random_element(1, q - 1)
    F = GF(q)
    E = EllipticCurve(F, [a, b])
    return orderBSGS(a, b, q) == E.order()


def randP(a, b, q):
    E = EllipticCurve(GF(q), [a, b])
    return E.random_point()


def reduce(M, fac, P, E):
    M = int(M)
    for pi in fac:
        M2 = int(M // pi[0])
        if M2 * E(P) == E(0):
            M = int(M)
    return M


def steps(q, P, E):
    Q = (q + 1) * P
    m = ceil(sqrt(sqrt(q)))
    L1x = set()
    L1y = set()
    L1 = []
    M = 0

    z = E(0)
    for j in range(0, m + 1):
        L1.append((z))
        L1x.add(int((z)[0]))
        L1y.add(int((z)[1]))
        z += P

    mnoj = 2 * m * P

    z2 = Q - mnoj * m
    for k in range(-m, m + 1):
        z20 = (z2)[0]
        z21 = (z2)[1]
        if int(z20) in L1x:
            if int(z21) in L1y:
                j = L1.index(z2)
                M = q + 1 + 2 * m * k - j
                break

            elif (-int(z21)) % q in L1y:
                j = L1.index(-z2)
                M = q + 1 + 2 * m * k + j
                break
        z2 += mnoj
    fac = factor(M)

    M = reduce(M, fac, P, E)
    return M


def orderBSGS(a, b, q):
    """
    TESTS::
    sage: [test_orderBSGS(i) for i in range(1,5)]
    [True, True, True, True]
    """
    F = GF(q)
    E = EllipticCurve(F, [a, b])
    P = randP(a, b, q)
    M1 = steps(q, P, E)
    L = 1
    k = M1
    L = lcm(L, k)
    if L >= 4 * ceil(sqrt(q)) and L < q + 1 - 2 * ceil(sqrt(q)):
        m = q + 1 + 2 * sqrt(q)
        return m
    return L
