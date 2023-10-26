def test_nTorsionPoints(n, a, b, q):
    t1 = nTorsionPoints(n, a, b, q)
    F = t1[0][0].parent()
    E = EllipticCurve(F, [F(a), F(b)])
    t2 = [n * E(P) == E(0) for P in t1]
    return [len(set(t1)), len(set(t2)), t2[0]]


def nTorsion_extension_deg(n, a, b, q):
    """
    TESTS::
    sage: nTorsion_extension_deg(3, 3, 17, 23)
    2

    sage: nTorsion_extension_deg(2, 1, 11, 41)
    1

    sage: nTorsion_extension_deg(5, 2, 21, 53)
    4

    sage: nTorsion_extension_deg(7, 1, 7, 11)
    21
    """
    E = EllipticCurve(GF(q), [a, b])
    x, y = var("x,y")
    if n % 2 == 0:
        pr = 1
    else:
        pr = 0
    psi_n = _bezy(n, a, b, q, pr)

    razl = psi_n.factor()
    razl = [i[0] for i in razl]
    l = lcm([i.degree() for i in razl])
    for i in razl:
        if l % (2 * i.degree()) != 0:
            fi = i
            di = i.degree()
            break
    q1 = pow(q, di)
    gf.<a> = GF(q1, 'a')
    ro = [i[0] for i in fi.roots(ring=gf)]
    xi = ro[0]
    ur = (
        pow(
            xi,
            3,
        )
        + a * xi
        + b
    )
    ku = ur.is_square()
    if ku == -1:
        return 2 * l
    dd = lcm(Mod(q, n).multiplicative_order(), di)
    if dd == l or l == n * dd:
        return l
    return 2 * l


def _psi(n, a, b, q, pr):
    x, y = var("x,y")
    if n % 2 == 0:
        if n == 0:
            return 0
        if n == 2:
            if pr == 0:
                return 2 * y
            else:
                return 2 * 2 * (x ^ 3 + a * x + b)
        if n == 4:
            if pr == 0:
                return (
                    4
                    * y
                    * (
                        x
                        ^ 6 + 5 * a * x
                        ^ 4 + 20 * b * x
                        ^ 3 - 5 * a
                        ^ 2 * x
                        ^ 2 - 4 * a * b * x - 8 * b
                        ^ 2 - a
                        ^ 3
                    )
                )
            else:
                return (
                    4
                    * 2
                    * (x ^ 3 + a * x + b)
                    * (
                        x
                        ^ 6 + 5 * a * x
                        ^ 4 + 20 * b * x
                        ^ 3 - 5 * a
                        ^ 2 * x
                        ^ 2 - 4 * a * b * x - 8 * b
                        ^ 2 - a
                        ^ 3
                    )
                )

    else:
        if n == 1:
            return 1
        if n == 3:
            return 3 * x ^ 4 + 6 * a * x ^ 2 + 12 * b * x - a ^ 2

    if n % 2 != 0:
        pr = 0
        return (
            _psi(((n - 1) / 2 + 2), a, b, q, pr) * (_psi(((n - 1) / 2), a, b, q, pr))
            ^ 3
            - _psi(((n - 1) / 2 - 1), a, b, q, pr)
            * (_psi(((n - 1) / 2 + 1), a, b, q, pr))
            ^ 3
        )
    if n % 2 == 0:
        if pr == 0:
            pr = 1
            return (
                (1 / (2 * y))
                * _psi(n / 2, a, b, q, pr)
                * (
                    _psi((n / 2 + 2), a, b, q, pr) * (_psi((n / 2 - 1), a, b, q, pr))
                    ^ 2
                    - _psi((n / 2 - 2), a, b, q, pr) * (_psi((n / 2 + 1), a, b, q, pr))
                    ^ 2
                )
            )
        else:
            return (
                (1 / (2 * 2 * (x ^ 3 + a * x + b)))
                * _psi(n / 2, a, b, q, pr)
                * (
                    _psi((n / 2 + 2), a, b, q, pr) * (_psi((n / 2 - 1), a, b, q, pr))
                    ^ 2
                    - _psi((n / 2 - 2), a, b, q, pr) * (_psi((n / 2 + 1), a, b, q, pr))
                    ^ 2
                )
            )


def _bezy(n, a, b, q, pr):
    psi2 = _psi(n, a, b, q, pr)
    x, y = var("x,y")
    if n % 2 == 0:
        for i in range(2, 100):
            psi2 = psi2.substitution_delayed(y ^ i, (x ^ 3 + a * x + b) ^ (i / 2))
        for j in range(2, 100):
            psi2 = psi2.substitution_delayed(1 / y ^ j, (x ^ 3 + a * x + b) ^ (j / 2))
    else:
        for i in range(2, 100):
            psi2 = psi2.substitution_delayed(y ^ i, (x ^ 3 + a * x + b) ^ (i / 2))
        for j in range(2, 100):
            psi2 = psi2.substitution_delayed(1 / y ^ j, (x ^ 3 + a * x + b) ^ (j / 2))

    R.<x> = PolynomialRing(GF(q))
    psi2 = R(psi2)
    psi2 = psi2.change_ring(Zmod(q))
    return psi2


def nTorsionPoints(n, a, b, q):
    """
    TESTS::
    sage: test_nTorsionPoints(3, 3, 17, 23)
    [9, 1, True]

    sage: test_nTorsionPoints(2, 1, 11, 41)
    [4, 1, True]

    sage: test_nTorsionPoints(5, 2, 21, 53)
    [25, 1, True]

    sage: test_nTorsionPoints(7, 1, 7, 11)
    [49, 1, True]
    """
    E = EllipticCurve(GF(q), [a, b])
    x, y = var("x,y")
    points = []
    if n == 1:
        return print("[0, infinity]")

    psi_n = E.division_polynomial(n)
    d = nTorsion_extension_deg(n, a, b, q)
    razl = psi_n.factor()
    razl = [i[0] for i in razl]

    for k in razl:
        r = [i for i in [i[0] for i in k.roots(ring=GF(q ^ d))]]
        points += [(i, sqrt(pow(i, 3) + a * i + b)) for i in r]
    if n != 2:
        points += [(i, -j) for i, j in points]
    points += [(0, 1, 0)]
    return points
