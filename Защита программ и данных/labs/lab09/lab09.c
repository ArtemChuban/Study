#include <stdio.h>
#include <windows.h>
#include <wincrypt.h>

int main() {
    HCRYPTPROV  hProv;
    HCRYPTHASH  hHash;
    HCRYPTKEY   hKey;
    HCRYPTHASH  hHmacHash;
    PBYTE       pbHash = NULL;
    DWORD       dataLen;
    HMAC_INFO   HmacInfo;
    CHAR filename[50];
    CHAR password[15];
	FILE* file;
	DWORD fileSize;
    BYTE* data;
	BYTE* bpassword;

    printf("Input filename: ");
    scanf("%s", filename);

    printf("Input password: ");
    scanf("%s", password);
    password[strcspn(password, "\n")] = 0;
    bpassword = malloc(sizeof(password));
    memcpy(bpassword, password, sizeof(password));

    file = fopen(filename, "rb");

    if (!file)
    {
        printf("Error while opening file!\n");
        goto Exit;
    }
    else
    {
        printf("Success file open\n");
    }

    fseek(file, 0L, SEEK_END);
    fileSize = ftell(file);
    fseek(file, 0L, SEEK_SET);

    data = malloc(fileSize);

    fread(data, 1, fileSize, file);
    printf("Success file read\n");
    fclose(file);

    ZeroMemory(&HmacInfo, sizeof(HmacInfo));
    HmacInfo.HashAlgid = CALG_SHA1;

    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
    {
        printf("Error in AcquireContext, %x", GetLastError());
        goto Exit;
    }

    if (!CryptCreateHash(hProv, CALG_SHA1, 0, 0, &hHash))
    {
        printf("Error in CryptCreateHash, %x", GetLastError());
        goto Exit;
    }

    if (!CryptHashData(hHash, bpassword, sizeof(bpassword), 0))
    {
        printf("Error in CryptHashData, %x", GetLastError());
        goto Exit;
    }

    if (!CryptDeriveKey(hProv, CALG_RC4, hHash, 0, &hKey))
    {
        printf("Error in CryptDeriveKey, %x", GetLastError());
        goto Exit;
    }

    if (!CryptCreateHash(hProv, CALG_HMAC, hKey, 0, &hHmacHash))
    {
        printf("Error in CryptCreateHash, %x", GetLastError());
        goto Exit;
    }

    if (!CryptSetHashParam(hHmacHash, HP_HMAC_INFO, (BYTE *)&HmacInfo, 0))
    {
        printf("Error in CryptSetHashParam, %x", GetLastError());
        goto Exit;
    }

    if (!CryptHashData(hHmacHash, data, sizeof(data), 0))
    {
        printf("Error in CryptHashData, %x", GetLastError());
        goto Exit;
    }

    if (!CryptGetHashParam(hHmacHash, HP_HASHVAL, NULL, &dataLen, 0))
    {
        printf("Error in CryptGetHashParam, %x", GetLastError());
        goto Exit;
    }

    pbHash = (BYTE *)malloc(dataLen);
    if (NULL == pbHash)
    {
        printf("unable to allocate memory, %x", GetLastError());
        goto Exit;
    }

    if (!CryptGetHashParam(hHmacHash, HP_HASHVAL, pbHash, &dataLen, 0))
    {
        printf("Error in CryptGetHashParam, %x", GetLastError());
        goto Exit;
    }

    printf("The hash is:  ");
    for (DWORD i = 0; i < dataLen; i++)
    {
        printf("%2.2x ", pbHash[i]);
    }
    printf("\n");

    Exit:
        if (hHmacHash)
            CryptDestroyHash(hHmacHash);
        if (hKey)
            CryptDestroyKey(hKey);
        if (hHash)
            CryptDestroyHash(hHash);
        if (hProv)
            CryptReleaseContext(hProv, 0);
        if (hHash)
            free(pbHash);
		free(bpassword);
		free(data);
		free(pbHash);
	return 0;
}