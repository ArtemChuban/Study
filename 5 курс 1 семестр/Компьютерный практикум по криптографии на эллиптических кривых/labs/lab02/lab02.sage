def Sum(a, b, q, x1, y1, x2, y2):
    """
    TESTS::
        sage: Sum(3, 10, 11, 4, 3, 1, 5)
        [4, 8]

        sage: Sum(978, 8052, 10007, 5593, 1759, 1298, 1966)
        [3420, 5599]

        sage: Sum(37, 33, 59, 34, 11, 0, infinity)
        [34, 11]

        sage: Sum(17, 29, 59, 42, 14, 42, 45)
        [0, infinity]

        sage: Sum(14, 6, 23, 18, 8, 4, 12)
        Error: the point [4,12] is not on E
    """
    K = GF(q)
    # случай 5: сложение с беск. удал. точкой
    if (x1 == 0) and ((y1) == infinity):
        return [x2, y2]
    if (x2 == 0) and ((y2) == infinity):
        return [x1, y1]
    if pow(y1, 2, q) != (pow(x1, 3, q) + a * x1 + b) % q:
        return print("Error: the point [" + str(x1) + "," + str(y1) + "] is not on E")
    if pow(y2, 2, q) != (pow(x2, 3, q) + a * x2 + b) % q:
        return print("Error: the point [" + str(x2) + "," + str(y2) + "] is not on E")

    # случай 4: когда точки равны и ордината 0 или 2
    # случай: когда абсциссы одинаковые ординаты разные
    if ((x1 == x2) and (y1 == y2) and (y1 == 0)) or ((x1 == x2) and (y1 != y2)):
        x3 = 0
        y3 = "infinity"
        return print("[" + str(x3) + ", " + str(y3) + "]")

    # случай 3: точки равны и ордината отлична от 0
    if (x1 == x2) and (y1 == y2) and (y1 != 0):
        m = ((3 * pow(x1, 2, q) + a) / (2 * y1)) % q
        x3 = (pow(m, 2, q) - 2 * x1) % q
        y3 = (m * (x1 - x3) - y1) % q
        return [K(x3), K(y3)]

    # случай 1: разные абсциссы
    if x1 != x2:
        m = ((y2 - y1) / (x2 - x1)) % q
        x3 = (pow(m, 2, q) - x1 - x2) % q
        y3 = (m * (x1 - x3) - y1) % q
        return [K(x3), K(y3)]


def SumProj(a, b, q, x1, y1, z1, x2, y2, z2):
    """
    TESTS::
        sage: SumProj(57, 1, 59, 18, 29, 1, 5, 23, 1)
        [2,51,1]

        sage: SumProj(49, 41, 59, 57, 42, 1, 1, 0, 1)
        [57,42,1]
    """
    # бесконечно удаленная точка
    if [x1, y1, z1] == [0, 1, 0]:
        return print("[{},{},{}]".format(x2, y2, z2))
    if [x2, y2, z2] == [0, 1, 0]:
        return print("[{},{},{}]".format(x1, y1, z1))

    if (
        pow(y1, 2, q) * z1 % q
        != (pow(x1, 3, q) + a * x1 * pow(z1, 2, q) + b * pow(z1, 3, q)) % q
    ):
        return print("[{},{},{}]".format(x2, y2, z2))
    if (
        pow(y2, 2, q) * z2 % q
        != (pow(x2, 3, q) + a * x2 * pow(z2, 2, q) + b * pow(z2, 3, q)) % q
    ):
        return print("[{},{},{}]".format(x1, y1, z1))
    u = Mod(y2 * z1 - y1 * z2, q)
    v = Mod(x2 * z1 - x1 * z2, q)
    w = Mod(pow(u, 2, q) * z1 * z2 - pow(v, 3, q) - 2 * pow(v, 2, q) * x1 * z2, q)
    z3 = Mod(pow(v, 3, q) * z1 * z2, q)
    x3 = Mod(v * w, q)
    y3 = Mod(u * (x1 * pow(v, 2, q) * z2 - w) - pow(v, 3, q) * z2 * y1, q)
    K = GF(q)
    return print("[{},{},{}]".format(K(x3 / z3), K(y3 / z3), K(z3 / z3)))


def Mul(a, b, q, x1, y1, k):
    """
    TESTS::
        sage: Mul(15, 2, 23, 8, 6, 19)
        [10, 5]

        sage: Mul(16, 27, 37, 19, 30, 24)
        [0, infinity]

        sage: Mul(1596531425664112104, 8469635381684191285, 17364269638771469903, 13402180624743596496, 13385993554720361919, 4872114054757385562)
        [7833260487853357138, 12663396679974011624]
    """
    if pow(y1, 2, q) != Mod(pow(x1, 3, q) + a * x1 + b, q):
        return print(f"Error: the point [x1,y1] is not on E")
    kras = [int(i) for i in list(bin(k)[2:])]
    E = EllipticCurve(GF(q), [a, b])
    O = E(0)
    P = E([x1, y1])
    Q = O
    for j in kras:
        Q = Q + Q
        if j == 1:
            Q = Q + P
    if Q == O:
        return print("[" + str(0) + ", " + str("infinity") + "]")
    else:
        return list(Q)[:2]
