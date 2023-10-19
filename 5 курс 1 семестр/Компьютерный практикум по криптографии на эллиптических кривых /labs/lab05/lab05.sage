import random

def Mnozhitel(a, N, P, Q):    # находим множитель из группового закона
    if Q == [0, 1, 0]: 
        return 'точка', P
    if P == [0, 1, 0]: 
        return 'точка', Q
    x1, y1, z1, x2, y2, z2 = P + Q
    
    if x1 == x2 and y1 == -y2 : 
        return 'точка', [0, 1, 0]
    d = gcd(x1 - x2, N)
    if (d != 1) and (d != N): 
        return 'делитель', d
    if x1 == x2:
        d = gcd(y1 + y2, N)
        if d > 1: 
            return 'делитель', d
        al = Mod((3*pow(x1, 2) + a) / (y1 + y2), N)
    else: 
        al = Mod((y2 - y1)/(x2 - x1), N)
    bet = Mod(y1 - al*x1, N)    
    x3 = Mod(pow(al, 2) - x1 - x2, N)
    y3 = Mod(-(al*x3 + bet), N)
    return 'точка', [x3, y3, 1]


def Multiplication(a, N, P, k):       
    k_bits = [int(i) for i in list(bin(k)[2:])]
    Q = [0, 1, 0]
    for j in k_bits:
        flag, Q = Mnozhitel(a, N, Q, Q)
        if flag == 'делитель': 
            return 'делитель', Q
        if j:
            flag, Q = Mnozhitel(a, N, Q, P)
            if flag == 'делитель': 
                return 'делитель', Q
            
    if Q == [0, 1, 0]: 
        return 'точка', [0, 1, 0]
    else: 
        return 'точка', Q
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
    # **** Place your code here *****
    while (True):
        a = Integers(N).random_element()
        x = Integers(N).random_element()
        y = Integers(N).random_element()
        b = Mod(y^2 - x^3 - a*x,N)
        qq= Mod(4*a^3 + 27*b^2,N)
        g = gcd(qq, N)
        if g != 1:
            return g
        if (g==1):
            P = [x, y, 1]
            b1 = ceil(exp((1/sqrt(2))*log(N)^0.5*log(log(N))^0.5))
            
            for p in Primes():
                if p > b1: 
                    break
                e = 1 
                pe = p
                while pe < b1:
                    flag, P = Multiplication(a, N, P, pe)
                    if flag == 'делитель':
                        return [int(P), N // int(P)]
                    e += 1
                    pe = pow(p, e)
            continue
        if g != N:
            return [int(g), N // int(g)]
            