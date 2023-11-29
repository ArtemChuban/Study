def Sp(n, q, lam):
    r = max(prime_factors(n))
    # Если lambda = 256, то какое будет k?
    k = 24 if lam <= 128 else 32
    for i in range(k):
        if Mod(pow(q, i + 1)- 1, r) == 0: 
            return False
    return True


def Check_curve(p, a, b, l):
    """
    TESTS::
    sage: Check_curve(8493869, 7716998, 8380837, 12)
    True

    sage: Check_curve(16185823, 1696091, 5442612, 12)
    False

    sage: Check_curve(20130339667708248456904929384531448722932664153581, 17129189218710592817862284027173420974003721615799, 9933953273954952502486306587094192265174690390760, 80)
    False

    sage: Check_curve(18517062478089064091750711292984871722854947888561, 16409803157791443980951396504074324345406928955572, 8566257301100838622549891376490782909808842330172, 80)
    True

    sage: Check_curve(65229303612666993838487548193316216197113003608034571328105362153899, 53897376765104893430449475283994604973710245797009962721888058654198, 9261518103108318988773972757009869873708914052885630873937644761546, 112)
    False

    sage: Check_curve(409013755073188693650591431115381887523332154671755341927870523529923, 26707281847917853313956618577696843880528443302488143377136407323763, 159677408287411098813469573525383793092218678960537036028752956177, 112)
    True

    sage: Check_curve(302436443231099890936304321538187886712840058607361932404994076011536837773503467, -1, 0, 132)
    False

    sage: Check_curve(730750818665451459112596905638433048232067471723, 425706413842211054102700238164133538302169176474, 203362936548826936673264444982866339953265530166, 160)
    False  
    """
    E = EllipticCurve(GF(p), [a, b])
    ordE = E.order()
    return len(str(ordE)) > ordE // max(prime_factors(ordE)) and \
           len(str(ordE)) > ordE // (2 ^ (2 * l)) and \
           is_prime(p) and \
           Sp(ordE, p, l) and \
           ordE != p
