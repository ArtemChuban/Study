# 1. Как проверить сингулярность?
# 2. Зачем нужно переходить в расширение в FindExtension?
# 3. Зачем нужен j-Invariant? И где используется?


import random


def jInvariant(a1, a2, a3, a4, a6, q):
    """
    Check is the curve y^2 + a1*x*y + a3*y = x^3 + a2*x^2 + a4*x+a6 is elliptic
    If yes, compute its j-invariant
    If no, raise an error
      :param a1, a2, a3, a4, a6: the coeffs of the input curve
      :param q: char=size of the base field

    TESTS::
        sage: jInvariant(1, 2, 1, 5, 1, 0)
        6128487/5329

        sage: jInvariant(1, 2, 1, 5, 1, 5)
        3

        sage: jInvariant(0, 1, 0, 0, 0, 0)
        Exception: the curve has a node
    """
    if q != 0:
        if (q in Primes()) == false:
            return "q-не простое"
        else:
            d2 = Mod((a1 ^ 2 + 4 * a2), q)
            d4 = Mod((2 * a4 + a1 * a3), q)
            d6 = Mod((a3 ^ 2 + 4 * a6), q)
            d8 = Mod(
                (a1 ^ 2 * a6 + 4 * a2 * a6 - a1 * a3 * a4 + a2 * a3 ^ 2 - a4 ^ 2), q
            )
            c4 = Mod((d2 ^ 2 - 24 * d4), q)
            dd = Mod((-d2 ^ 2 * d8 - 8 * d4 ^ 3 - 27 * d6 ^ 2 + 9 * d2 * d4 * d6), q)
            if dd == 0 and c4 != 0:
                return print("Exception: the curve has a node")
            elif dd == 0 and c4 == 0:
                return print("Exception: the curve has a cusp")
            else:
                je = Mod((c4 ^ 3 / dd), q)
                return je
    else:
        d2 = a1 ^ 2 + 4 * a2
        d4 = 2 * a4 + a1 * a3
        d6 = a3 ^ 2 + 4 * a6
        d8 = a1 ^ 2 * a6 + 4 * a2 * a6 - a1 * a3 * a4 + a2 * a3 ^ 2 - a4 ^ 2
        c4 = d2 ^ 2 - 24 * d4
        dd = -d2 ^ 2 * d8 - 8 * d4 ^ 3 - 27 * d6 ^ 2 + 9 * d2 * d4 * d6
        if dd == 0 and c4 != 0:
            return print("Exception: the curve has a node")
        elif dd == 0 and c4 == 0:
            return print("Exception: the curve has a cusp")
        else:
            je = c4 ^ 3 / dd
            return je


def isIsomorphic(a1, a2, a3, a4, a6, _a1, _a2, _a3, _a4, _a6, q):
    """
    If a_i's and b_i's define elliptic curves E1, E2, solve a system of non-lin. equations to find
    [u,r,s,t] over F_q / QQ that define an isomorphism btw. E1 and E2. It returns all the u's found and
    one tuple  [u,r,s,t] if there is at least one u.

    :param a1, a2, a3, a4, a6: the coeffs of the input curve E1
    :param b1, b2, b3, b4, b6: the coeffs of the input curve E2
    :param q: char (=size) of the base field

    TESTS::
        sage: isIsomorphic(0,1,3,1,0,7706571724/19547240159, -133630307391190597856/3438851380502601107529, 52923479675/201660161417379101919146238171333, -510738511897151494016/11825698817184645424683867953329377420485841, 195487310213712167833665086975/40666820702883394956306193463183779725021510167778808819862996889,0)
        ([58641720477, -58641720477],
        [58641720477, 5803716513, 11559857586, 26461739836])

        sage: isIsomorphic(0, 1, 3, 1, 2, 27, 18, 1, 43, 38, 53)
        ([48, 5], [48, 21, 12, 42])

        sage: isIsomorphic(0,1,3,1,2,12,19,17,10,10, 53)
        'non-isomorphic'
    """
    if q != 0:
        try:
            EllipticCurve(GF(q), [a1, a2, a3, a4, a6])
            EllipticCurve(GF(q), [_a1, _a2, _a3, _a4, _a6])
        except ArithmeticError:
            return "the input curve is singular!"
        uu = []
        K = [a for a in GF(q)]
        K.reverse()
        for u in K:
            s = Mod((u * _a1 - a1) / 2, q)
            r = Mod((u ^ 2 * _a2 - a2 + s * a1 + s ^ 2) / 3, q)
            t = Mod((u ^ 3 * _a3 - a3 - r * a1) / 2, q)
            a4p = a4 - s * a3 + 2 * r * a2 - a1 * (t + r * s) + 3 * r ^ 2 - 2 * s * t
            a6p = a6 + r * a4 + r ^ 2 * a2 + r ^ 3 - t * a3 - t ^ 2 - r * t * a1
            if (_a4 * u ^ 4 == a4p) and (_a6 * u ^ 6 == a6p):
                uu.append(u)
        for u in K:
            s = Mod((u * _a1 - a1) / 2, q)
            r = Mod((u ^ 2 * _a2 - a2 + s * a1 + s ^ 2) / 3, q)
            t = Mod((u ^ 3 * _a3 - a3 - r * a1) / 2, q)
            a4p = a4 - s * a3 + 2 * r * a2 - a1 * (t + r * s) + 3 * r ^ 2 - 2 * s * t
            a6p = a6 + r * a4 + r ^ 2 * a2 + r ^ 3 - t * a3 - t ^ 2 - r * t * a1
            if (_a4 * u ^ 4 == a4p) and (_a6 * u ^ 6 == a6p):
                return uu, [u, r, s, t]
        return "non-isomorphic"
    else:
        try:
            EllipticCurve([a1, a2, a3, a4, a6])
            EllipticCurve([_a1, _a2, _a3, _a4, _a6])
        except ArithmeticError:
            return "the input curve is singular!"

        u, r, s, t = var("u, r, s, t")
        eq1 = s == (u * _a1 - a1) / 2
        eq2 = r == (u ^ 2 * _a2 - a2 + s * a1 + s ^ 2) / 3
        eq3 = t == (u ^ 3 * _a3 - a3 - r * a1) / 2
        eq4 = (
            _a6 * u ^ 6
            == a6 + r * a4 + r ^ 2 * a2 + r ^ 3 - t * a3 - t ^ 2 - r * t * a1
        )
        eq5 = (
            _a4 * u ^ 4
            == a4 - s * a3 + 2 * r * a2 - a1 * (t + r * s) + 3 * r ^ 2 - 2 * s * t
        )
        UU = []
        U = solve([eq1, eq2, eq3, eq4, eq5], u, r, s, t)
        U_new = [[U[j][i].right() for i in range(4)] for j in range(len(U))]
        for i in range(len(U)):
            UU.append(U[i][0].right())
        UU.reverse()
        if all(U_new[0][i] in QQ for i in range(4)):
            return UU, U_new[1]
        else:
            return "non-isomorphic", U_new[0][0]


def test_randIsomorphic(a1, a2, a3, a4, a6, q):
    if q == 0:
        E1 = EllipticCurve([a1, a2, a3, a4, a6])
    else:
        K = GF(q)
        E1 = EllipticCurve([K(a1), a2, a3, a4, a6])
    E2 = EllipticCurve(randIsomorphic(a1, a2, a3, a4, a6, q))
    return E1.is_isomorphic(E2)


def randIsomorphic(a1, a2, a3, a4, a6, q):
    """
    If a_i's define an elliptic curve E, output the coeffs of a random curve isomorphic to E over F_q
    or over QQ (if q = 0)
    :param a1, a2, a3, a4, a6: the coeffs of the input curve
    :param q: char (=size) of the base field !!! Not tested if q is non-prime

    TESTS::
        sage: test_randIsomorphic(0, 1, 0, 0, 1, 0)
        True

        sage: test_randIsomorphic(1, 2, 1, 5, 1, 5)
        True

        sage: randIsomorphic(0, 0, 0, 0, 0, 5)
        Exception: the input curve is singular
    """
    if q == 0:
        try:
            EllipticCurve(QQ, [a1, a2, a3, a4, a6])
        except:
            return print("Exception: the input curve is singular")
        n = 10 ^ 2
        u = random.randint(1, n)
        r = random.randint(1, n)
        s = random.randint(1, n)
        t = random.randint(1, n)
        a01 = (a1 + 2 * s) / u
        a02 = (a2 - s * a1 + 3 * r - s ^ 2) / u ^ 2
        a03 = (a3 + r * a1 + 2 * t) / u ^ 3
        a04 = (
            a4 - s * a3 + 2 * r * a2 - (t + r * s) * a1 + 3 * r ^ 2 - 2 * s * t
        ) / u ^ 4
        a06 = (a6 + r * a4 + r ^ 2 * a2 + r ^ 3 - t * a3 - t ^ 2 - r * t * a1) / u ^ 6
        b1 = int(a01)
        b2 = int(a02)
        b3 = int(a03)
        b4 = int(a04)
        b6 = int(a06)
        try:
            EllipticCurve(QQ, [b1, b2, b3, b4, b6])
        except:
            return randIsomorphic(a1, a2, a3, a4, a6, q)
        if jInvariant(b1, b2, b3, b4, b6, q) == jInvariant(a1, a2, a3, a4, a6, q):
            M = [b1, b2, b3, b4, b6]
            return M
        else:
            return randIsomorphic(a1, a2, a3, a4, a6, q)
    else:
        try:
            EllipticCurve(GF(q), [a1, a2, a3, a4, a6])
        except:
            return print("Exception: the input curve is singular")
        K = GF(q)
        u = random.randint(1, q - 1)
        r = random.randint(1, q - 1)
        s = random.randint(1, q - 1)
        t = random.randint(1, q - 1)
        a01 = mod((a1 + 2 * s) / u, q)
        a02 = mod((a2 - s * a1 + 3 * r - s ^ 2) / u ^ 2, q)
        a03 = mod((a3 + r * a1 + 2 * t) / u ^ 3, q)
        a04 = mod(
            (a4 - s * a3 + 2 * r * a2 - (t + r * s) * a1 + 3 * r ^ 2 - 2 * s * t) / u
            ^ 4,
            q,
        )
        a06 = mod(
            (a6 + r * a4 + r ^ 2 * a2 + r ^ 3 - t * a3 - t ^ 2 - r * t * a1) / u ^ 6, q
        )
        b1 = int(a01)
        b2 = int(a02)
        b3 = int(a03)
        b4 = int(a04)
        b6 = int(a06)
        if jInvariant(b1, b2, b3, b4, b6, q) == jInvariant(a1, a2, a3, a4, a6, q):
            M = [K(b1), b2, b3, b4, b6]
            return M
        else:
            return randIsomorphic(a1, a2, a3, a4, a6, q)


def findExtension(a1, a2, a3, a4, a6, b1, b2, b3, b4, b6, q):
    """
    If a_i's and b_i's define elliptic curves E1, E2, solve a system of non-lin. equations to find
    [u,r,s,t] over F_q / QQ that define an isomorphism btw. E1 and E2.
    If q != 0 and no solution over F_q found, constructs an extension of F_q by adjoing a root of quadratic polynomial
    Similar for QQ.

    If the curves are isomorphic, the function returns one tuple  [u,r,s,t] either over the base field,
    or its quadratic extension


    :param a1, a2, a3, a4, a6: the coeffs of the input curve E1
    :param b1, b2, b3, b4, b6: the coeffs of the input curve E2
    :param q: char (=size) of the base field

    TESTS::
    sage: findExtension(0, 1, 3, 1, 2, 4, 48, 9, 16, 24, 53)
    ('E1, E2 are isomorphic over the base field', [44, 8, 35, 4])

    sage: findExtension(0, 1, 3, 1, 2, 47, 45, 15, 39, 8, 53)
    ('E1, E2 are isomorphic over', Univariate Quotient Polynomial Ring in alpha over Ring of integers modulo 53 with modulus alpha^2 + 5, [alpha, 51, 50*alpha, 42*alpha + 25])

    sage: findExtension(1, 3, 0, 7, 11, 149285191107/32287120829, -5571517070849439150752/1042458171426445647241, 214924457885/33657972940024045898471277482789, -11715167770447055620209/1086719039173768740089384002472795410912081, 54783704351463028601416544457970/1132859142431390916000129067971120246444103784564827936191218521,0)
    ('E1, E2 are isomorphic over the base field', [32287120829, 37979606869, 74642595553, 88472425508])

    sage: findExtension(1,2,3,4,5, 23/7,-13/7,50/343,-4/2401,2031/117649, 0)
    ('E1, E2 are isomorphic over the base field', [7, 13, 11, 17])

    sage: findExtension(1,2,3,4,5, 1,2,3,248904403/16,2094318345083/64, 0)
    ('E1, E2 are isomoirphic over', Number Field in alpha with defining polynomial x^2 - 1/2020, [alpha, -6057/8080, 1/2*alpha - 1/2, 3/4040*alpha - 18183/16160])
    """
    try:
        if q != 0:
            EllipticCurve(GF(q), [a1, a2, a3, a4, a6])
            EllipticCurve(GF(q), [b1, b2, b3, b4, b6])
        else:
            EllipticCurve([a1, a2, a3, a4, a6])
            EllipticCurve([b1, b2, b3, b4, b6])
    except:
        return "Exception: the input curve is singular"
    if q != 0:
        res = []
        c = 0
        for u in GF(q):
            if u == 0:
                u += 1
            s = (b1 * u - a1) / 2
            r = (u ^ 2 * b2 - a2 + s ^ 2 + s * a1) / 3
            t = (u ^ 3 * b3 - a3 - r * a1) / 2
            if (
                b6
                == (a6 + r * a4 + r ^ 2 * a2 + r ^ 3 - t * a3 - t ^ 2 - r * t * a1) / u
                ^ 6
            ):
                res = [u, r, s, t]
                c += 1
        if c != 0:
            return "E1, E2 are isomorphic over the base field", res
        else:
            i = 0
            for i in range(5, 53):
                try:
                    Z = Integers(q)
                    R = PolynomialRing(Z, "alpha")
                    stri = "alpha^2 + " + str(i)
                    S = R.quotient(stri, "alpha")
                    for u in S:
                        if u == 0:
                            u += 1
                        s = (b1 * u - a1) / 2
                        r = (u ^ 2 * b2 - a2 + s ^ 2 + s * a1) / 3
                        t = (u ^ 3 * b3 - a3 - r * a1) / 2
                        if (
                            b6
                            == (
                                a6 + r * a4 + r
                                ^ 2 * a2 + r
                                ^ 3 - t * a3 - t
                                ^ 2 - r * t * a1
                            )
                            / u
                            ^ 6
                        ):
                            res = [u, r, s, t]
                            c += 1
                            return "E1, E2 are isomorphic over", S, res
                    if c != 0:
                        return "E1, E2 are isomorphic over", S, res

                except:
                    pass

    else:
        resmass = []
        umass = []
        u = var("u")
        r = var("r")
        s = var("s")
        t = var("t")
        mass = solve(
            [
                b1 == (a1 + 2 * s) / u,
                b2 == (a2 - s * a1 + 3 * r - s ^ 2) / u ^ 2,
                b3 == (a3 + r * a1 + 2 * t) / u ^ 3,
                b4
                == (a4 - s * a3 + 2 * r * a2 - (t + r * s) * a1 + 3 * r ^ 2 - 2 * s * t)
                / u
                ^ 4,
                b6
                == (a6 + r * a4 + r ^ 2 * a2 + r ^ 3 - t * a3 - t ^ 2 - r * t * a1) / u
                ^ 6,
            ],
            u,
            r,
            s,
            t,
        )
        for i in range(0, len(mass)):
            x = mass[i][0].right()
            umass.append(x)
        for i in range(len(mass[len(mass) - 1])):
            resmass.append(mass[len(mass) - 1][i].right())
        for el in resmass:
            if el in QQ:
                return "E1, E2 are isomorphic over the base field", resmass
            else:
                E = NumberField(QQ[resmass[0]].polynomial(), "alpha")
                u = E.gen()
                s = (u * b1 - a1) / 2
                r = (u ^ 2 * b2 - a2 + s * a1 + s ^ 2) / 3
                t = (u ^ 3 * b3 - a3 - r * a1) / 2
                return "E1, E2 are isomoirphic over", E, [u, r, s, t]
