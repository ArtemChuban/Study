#include <stdio.h>
#include <windows.h>
#include <wincrypt.h>

#define DHKEYSIZE 512

void ErrorLog(char* message);
void DebugLog(char* message);

int main()
{
    BOOL fReturn;
    HCRYPTPROV hProvParty1, hProvParty2;
    HCRYPTKEY hPrivateKey1, hPrivateKey2, hSessionKey1, hSessionKey2;
    PBYTE P, G, pbData, pbKeyBlob1, pbKeyBlob2;
    DWORD PLength, GLength, dwDataLen1, dwDataLen2;
    DATA_BLOB dbP, dbG;

    if (!CryptAcquireContext(&hProvParty1, NULL, MS_ENH_DSS_DH_PROV, PROV_DSS_DH, CRYPT_VERIFYCONTEXT))
    {
        ErrorLog("Cant acquire provider");
        goto ErrorExit;
    }
    DebugLog("Provider acquired");

    if (!CryptGenKey(hProvParty1, CALG_DH_EPHEM, DHKEYSIZE << 16 | CRYPT_EXPORTABLE, &hPrivateKey1))
    {
        ErrorLog("Cant generate key");
        goto ErrorExit;
    }
    DebugLog("Key generated");

    if (!CryptGetKeyParam(hPrivateKey1, KP_P, NULL, &PLength, 0))
    {
        ErrorLog("Cant get P length");
        goto ErrorExit;
    }
    DebugLog("Get P length");
    P = malloc(sizeof(BYTE) * PLength);

    if (!CryptGetKeyParam(hPrivateKey1, KP_P, P, &PLength, 0))
    {
        ErrorLog("Cant get P");
        goto ErrorExit;
    }
    DebugLog("Get P");


    if (!CryptGetKeyParam(hPrivateKey1, KP_G, NULL, &GLength, 0))
    {
        ErrorLog("Cant get G length");
        goto ErrorExit;
    }
    DebugLog("Get G length");
    G = malloc(sizeof(BYTE) * GLength);

    if (!CryptGetKeyParam(hPrivateKey1, KP_G, G, &GLength, 0))
    {
        ErrorLog("Cant get G");
        goto ErrorExit;
    }
    DebugLog("Get G");

    if (!CryptSetKeyParam(hPrivateKey1, KP_X, NULL, 0))
    {
        ErrorLog("Cant generate X");
        goto ErrorExit;
    }
    DebugLog("X generated");

    if (!CryptAcquireContext(&hProvParty2, NULL, MS_ENH_DSS_DH_PROV, PROV_DSS_DH, CRYPT_VERIFYCONTEXT))
    {
        ErrorLog("Cant acquire provider");
        goto ErrorExit;
    }
    DebugLog("Provider acquired");

    if (!CryptGenKey(hProvParty2, CALG_DH_EPHEM, DHKEYSIZE << 16 | CRYPT_EXPORTABLE | CRYPT_PREGEN, &hPrivateKey2))
    {
        ErrorLog("Cant generate key");
        goto ErrorExit;
    }
    DebugLog("Key generated");

    dbP.cbData = PLength;
    dbP.pbData = P;

    if (!CryptSetKeyParam(hPrivateKey2, KP_P, (PBYTE)&dbP, 0))
    {
        ErrorLog("Cant set P");
        goto ErrorExit;
    }
    DebugLog("Set P");

    dbG.cbData = GLength;
    dbG.pbData = G;

    if (!CryptSetKeyParam(hPrivateKey2, KP_G, (PBYTE)&dbG, 0))
    {
        ErrorLog("Cant set G");
        goto ErrorExit;
    }
    DebugLog("Set G");

    if (!CryptSetKeyParam(hPrivateKey2, KP_X, NULL, 0))
    {
        ErrorLog("Cant generate X");
        goto ErrorExit;
    }
    DebugLog("X generated");

    if (!CryptExportKey(hPrivateKey1, 0, PUBLICKEYBLOB, 0, NULL, &dwDataLen1))
    {
        ErrorLog("Cant get length of public key blob");
        goto ErrorExit;
    }
    DebugLog("Public key blob length found");

    if(!(pbKeyBlob1 = malloc(dwDataLen1)))
    {
        ErrorLog("Cant allocate memory");
        goto ErrorExit;
    }
    DebugLog("Memory allocated");

    if (!CryptExportKey(hPrivateKey1, 0, PUBLICKEYBLOB, 0, pbKeyBlob1, &dwDataLen1))
    {
        ErrorLog("Cant export public key");
        goto ErrorExit;
    }
    DebugLog("Public key exported");

    if (!CryptExportKey(hPrivateKey2, 0, PUBLICKEYBLOB, 0, NULL, &dwDataLen2))
    {
        ErrorLog("Cant get length of public key blob");
        goto ErrorExit;
    }
    DebugLog("Public key blob length found");

    if(!(pbKeyBlob2 = malloc(dwDataLen2)))
    {
        ErrorLog("Cant allocate memory");
        goto ErrorExit;
    }
    DebugLog("Memory allocated");

    if (!CryptExportKey(hPrivateKey2, 0, PUBLICKEYBLOB, 0, pbKeyBlob2, &dwDataLen2))
    {
        ErrorLog("Cant export public key");
        goto ErrorExit;
    }
    DebugLog("Public key exported");

    if (!CryptImportKey(hProvParty1, pbKeyBlob2, dwDataLen2, hPrivateKey1, 0, &hSessionKey2))
    {
        ErrorLog("Cant import key");
        goto ErrorExit;
    }
    DebugLog("Key imported");

    if (!CryptImportKey(hProvParty2, pbKeyBlob1, dwDataLen1, hPrivateKey2, 0, &hSessionKey1))
    {
        ErrorLog("Cant import key");
        goto ErrorExit;
    }
    DebugLog("Key imported");

ErrorExit:

    free(P);
    free(G);

    if(pbData)
    {
        free(pbData);
        pbData = 0;
    }

    if(hSessionKey2)
    {
        CryptDestroyKey(hSessionKey2);
        hSessionKey2 = 0;
    }

    if(hSessionKey1)
    {
        CryptDestroyKey(hSessionKey1);
        hSessionKey1 = 0;
    }

    if(pbKeyBlob2)
    {
        free(pbKeyBlob2);
        pbKeyBlob2 = 0;
    }

    if(pbKeyBlob1)
    {
        free(pbKeyBlob1);
        pbKeyBlob1 = 0;
    }

    if(hPrivateKey2)
    {
        CryptDestroyKey(hPrivateKey2);
        hPrivateKey2 = 0;
    }

    if(hPrivateKey1)
    {
        CryptDestroyKey(hPrivateKey1);
        hPrivateKey1 = 0;
    }

    if(hProvParty2)
    {
        CryptReleaseContext(hProvParty2, 0);
        hProvParty2 = 0;
    }

    if(hProvParty1)
    {
        CryptReleaseContext(hProvParty1, 0);
        hProvParty1 = 0;
    }

    return 0;
}

void ErrorLog(char* message)
{
    printf("Error: %s. Error code: %x.\n", message, GetLastError());
}

void DebugLog(char* message)
{
    printf("Debug: %s.\n", message);
}
