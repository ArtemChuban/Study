#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <windows.h>
#include <bcrypt.h>

#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
#define STATUS_UNSUCCESSFUL ((NTSTATUS)0xC0000001L)

std::pair<PBYTE, DWORD> encrypt(byte* data, int length);
std::pair<PBYTE, DWORD> hashing(byte* data, int length);
std::pair<PBYTE, DWORD> signing(byte* data, int length);

int main()
{
    byte* data{};
    char filename[] = "lab11.bin";
    long length;

    FILE* file = NULL;
    file = fopen(filename, "rb");

    if (!file)
    {
        std::cout << "Error while opening file" << std::endl;
    }

    fseek(file, 0L, SEEK_END);
    length = ftell(file);

    fseek(file, 0L, SEEK_SET);
    data = new byte[length];
    fread(data, 1, length, file);

    fclose(file);

    std::cout << "Length: " << length << std::endl << "Data: ";
    for (int i = 0; i < length; i++)
    {
        std::cout << std::hex << data[i];
        std::cout << " ";
    }
    std::cout << "\n" << std::endl;

    std::pair<PBYTE, DWORD> encryptDataPair = encrypt(data, length);
    PBYTE encryptData = encryptDataPair.first;
    DWORD encryptDataLength = encryptDataPair.second;

    std::cout << "Encrypted data: " << std::endl;
    for (int i = 0; i < encryptDataLength; i++) {
        std::wcout << std::hex << encryptData[i];
        std::cout << " ";
    }
    std::cout << "\n" << std::endl;

    std::pair<PBYTE, DWORD> hashDataPair = hashing(data, length);
    PBYTE hashData = hashDataPair.first;
    DWORD hashDataLength = hashDataPair.second;

    std::cout << "Hash data: " << std::endl;
    for (int i = 0; i < hashDataLength; i++) {
        std::wcout << std::hex << hashData[i];
        std::cout << " ";
    }
    std::cout << "\n" << std::endl;

    std::pair<PBYTE, DWORD> signDataPair = signing(data, length);
    PBYTE signData = signDataPair.first;
    DWORD signDataLength = signDataPair.second;

    std::cout << "Sign data: " << std::endl;
    for (int i = 0; i < signDataLength; i++) {
        std::wcout << std::hex << signData[i];
        std::cout << " ";
    }
    std::cout << "\n" << std::endl;

}

static const BYTE rgbIV[] =
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};

static const BYTE rgbAES128Key[] =
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};

std::pair<PBYTE, DWORD> encrypt(byte data[], int length)
{

    BCRYPT_ALG_HANDLE hAesAlg = NULL;
    BCRYPT_KEY_HANDLE hKey = NULL;
    NTSTATUS status = STATUS_UNSUCCESSFUL;
    DWORD cbCipherText = 0, cbPlainText = 0, cbData = 0, cbKeyObject = 0, cbBlockLen = 0, cbBlob = 0;
    PBYTE pbCipherText = NULL, pbPlainText = NULL, pbKeyObject = NULL, pbIV = NULL, pbBlob = NULL;

    if (!NT_SUCCESS(status = BCryptOpenAlgorithmProvider(&hAesAlg, BCRYPT_AES_ALGORITHM, NULL, 0)))
    {
        wprintf(L"**** Error 0x%x returned by BCryptOpenAlgorithmProvider\n", status);
        goto Cleanup;
    }

    if (!NT_SUCCESS(status = BCryptGetProperty(hAesAlg, BCRYPT_OBJECT_LENGTH, (PBYTE)&cbKeyObject, sizeof(DWORD), &cbData, 0)))
    {
        wprintf(L"**** Error 0x%x returned by BCryptGetProperty\n", status);
        goto Cleanup;
    }

    pbKeyObject = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbKeyObject);
    if (NULL == pbKeyObject)
    {
        wprintf(L"**** memory allocation failed\n");
        goto Cleanup;
    }

    if (!NT_SUCCESS(status = BCryptGetProperty(hAesAlg, BCRYPT_BLOCK_LENGTH, (PBYTE)&cbBlockLen, sizeof(DWORD), &cbData, 0)))
    {
        wprintf(L"**** Error 0x%x returned by BCryptGetProperty\n", status);
        goto Cleanup;
    }

    if (cbBlockLen > sizeof(rgbIV))
    {
        wprintf(L"**** block length is longer than the provided IV length\n");
        goto Cleanup;
    }

    pbIV = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbBlockLen);
    if (NULL == pbIV)
    {
        wprintf(L"**** memory allocation failed\n");
        goto Cleanup;
    }

    memcpy(pbIV, rgbIV, cbBlockLen);

    if (!NT_SUCCESS(status = BCryptSetProperty(hAesAlg, BCRYPT_CHAINING_MODE, (PBYTE)BCRYPT_CHAIN_MODE_CBC, sizeof(BCRYPT_CHAIN_MODE_CBC), 0)))
    {
        wprintf(L"**** Error 0x%x returned by BCryptSetProperty\n", status);
        goto Cleanup;
    }

    if (!NT_SUCCESS(status = BCryptGenerateSymmetricKey(hAesAlg, &hKey, pbKeyObject, cbKeyObject, (PBYTE)rgbAES128Key, sizeof(rgbAES128Key), 0)))
    {
        wprintf(L"**** Error 0x%x returned by BCryptGenerateSymmetricKey\n", status);
        goto Cleanup;
    }

    if (!NT_SUCCESS(status = BCryptExportKey(hKey, NULL, BCRYPT_OPAQUE_KEY_BLOB, NULL, 0, &cbBlob, 0)))
    {
        wprintf(L"**** Error 0x%x returned by BCryptExportKey\n", status);
        goto Cleanup;
    }

    pbBlob = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbBlob);
    if (NULL == pbBlob)
    {
        wprintf(L"**** memory allocation failed\n");
        goto Cleanup;
    }

    if (!NT_SUCCESS(status = BCryptExportKey(hKey, NULL, BCRYPT_OPAQUE_KEY_BLOB, pbBlob, cbBlob, &cbBlob, 0)))
    {
        wprintf(L"**** Error 0x%x returned by BCryptExportKey\n", status);
        goto Cleanup;
    }

    cbPlainText = length;
    pbPlainText = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbPlainText);
    if (NULL == pbPlainText)
    {
        wprintf(L"**** memory allocation failed\n");
        goto Cleanup;
    }

    memcpy(pbPlainText, data, length);

    if (!NT_SUCCESS(status = BCryptEncrypt(hKey, pbPlainText, cbPlainText, NULL, pbIV, cbBlockLen, NULL, 0, &cbCipherText, BCRYPT_BLOCK_PADDING)))
    {
        wprintf(L"**** Error 0x%x returned by BCryptEncrypt\n", status);
        goto Cleanup;
    }

    pbCipherText = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbCipherText);
    if (NULL == pbCipherText)
    {
        wprintf(L"**** memory allocation failed\n");
        goto Cleanup;
    }

    if (!NT_SUCCESS(status = BCryptEncrypt(hKey, pbPlainText, cbPlainText, NULL, pbIV, cbBlockLen, pbCipherText, cbCipherText, &cbData, BCRYPT_BLOCK_PADDING)))
    {
        wprintf(L"**** Error 0x%x returned by BCryptEncrypt\n", status);
        goto Cleanup;
    }

    if (!NT_SUCCESS(status = BCryptDestroyKey(hKey)))
    {
        wprintf(L"**** Error 0x%x returned by BCryptDestroyKey\n", status);
        goto Cleanup;
    }
    hKey = 0;

    if (pbPlainText)
    {
        HeapFree(GetProcessHeap(), 0, pbPlainText);
    }

    pbPlainText = NULL;

    memset(pbKeyObject, 0, cbKeyObject);
    memcpy(pbIV, rgbIV, cbBlockLen);

    if (!NT_SUCCESS(status = BCryptImportKey(hAesAlg, NULL, BCRYPT_OPAQUE_KEY_BLOB, &hKey, pbKeyObject, cbKeyObject, pbBlob, cbBlob, 0)))
    {
        wprintf(L"**** Error 0x%x returned by BCryptGenerateSymmetricKey\n", status);
        goto Cleanup;
    }

    if (!NT_SUCCESS(status = BCryptDecrypt(hKey, pbCipherText, cbCipherText, NULL, pbIV, cbBlockLen, NULL, 0, &cbPlainText, BCRYPT_BLOCK_PADDING)))
    {
        wprintf(L"**** Error 0x%x returned by BCryptDecrypt\n", status);
        goto Cleanup;
    }

    pbPlainText = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbPlainText);
    if (NULL == pbPlainText)
    {
        wprintf(L"**** memory allocation failed\n");
        goto Cleanup;
    }

    if (!NT_SUCCESS(status = BCryptDecrypt(hKey, pbCipherText, cbCipherText, NULL, pbIV, cbBlockLen, pbPlainText, cbPlainText, &cbPlainText, BCRYPT_BLOCK_PADDING)))
    {
        wprintf(L"**** Error 0x%x returned by BCryptDecrypt\n", status);
        goto Cleanup;
    }

    if (0 != memcmp(pbPlainText, (PBYTE)data, length))
    {
        wprintf(L"Expected decrypted text comparison failed.\n");
        goto Cleanup;
    }

    wprintf(L"Success encrypt data!\n");
    return std::make_pair(pbCipherText, cbCipherText);

Cleanup:
    if (hAesAlg)
    {
        BCryptCloseAlgorithmProvider(hAesAlg, 0);
    }
    if (hKey)
    {
        BCryptDestroyKey(hKey);
    }
    if (pbCipherText)
    {
        HeapFree(GetProcessHeap(), 0, pbCipherText);
    }
    if (pbPlainText)
    {
        HeapFree(GetProcessHeap(), 0, pbPlainText);
    }
    if (pbKeyObject)
    {
        HeapFree(GetProcessHeap(), 0, pbKeyObject);
    }
    if (pbIV)
    {
        HeapFree(GetProcessHeap(), 0, pbIV);
    }
}

std::pair<PBYTE, DWORD> hashing(byte data[], int length)
{
    BCRYPT_ALG_HANDLE hAlg = NULL;
    BCRYPT_HASH_HANDLE hHash = NULL;
    NTSTATUS status = STATUS_UNSUCCESSFUL;
    DWORD cbData = 0, cbHash = 0, cbHashObject = 0;
    PBYTE pbHashObject = NULL;
    PBYTE pbHash = NULL;

    if (!NT_SUCCESS(status = BCryptOpenAlgorithmProvider(&hAlg, BCRYPT_SHA256_ALGORITHM, NULL, 0)))
    {
        wprintf(L"**** Error 0x%x returned by BCryptOpenAlgorithmProvider\n", status);
        goto Cleanup;
    }

    if (!NT_SUCCESS(status = BCryptGetProperty(hAlg, BCRYPT_OBJECT_LENGTH, (PBYTE)&cbHashObject, sizeof(DWORD), &cbData, 0)))
    {
        wprintf(L"**** Error 0x%x returned by BCryptGetProperty\n", status);
        goto Cleanup;
    }

    pbHashObject = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbHashObject);
    if (NULL == pbHashObject)
    {
        wprintf(L"**** memory allocation failed\n");
        goto Cleanup;
    }

    if (!NT_SUCCESS(status = BCryptGetProperty(hAlg, BCRYPT_HASH_LENGTH, (PBYTE)&cbHash, sizeof(DWORD), &cbData, 0)))
    {
        wprintf(L"**** Error 0x%x returned by BCryptGetProperty\n", status);
        goto Cleanup;
    }

    pbHash = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbHash);
    if (NULL == pbHash)
    {
        wprintf(L"**** memory allocation failed\n");
        goto Cleanup;
    }

    if (!NT_SUCCESS(status = BCryptCreateHash(hAlg, &hHash, pbHashObject, cbHashObject, NULL, 0, 0)))
    {
        wprintf(L"**** Error 0x%x returned by BCryptCreateHash\n", status);
        goto Cleanup;
    }

    if (!NT_SUCCESS(status = BCryptHashData(hHash, (PBYTE)data, length, 0)))
    {
        wprintf(L"**** Error 0x%x returned by BCryptHashData\n", status);
        goto Cleanup;
    }

    if (!NT_SUCCESS(status = BCryptFinishHash(hHash, pbHash, cbHash, 0)))
    {
        wprintf(L"**** Error 0x%x returned by BCryptFinishHash\n", status);
        goto Cleanup;
    }

    wprintf(L"Success hashing file!\n");

    return std::make_pair(pbHash, cbHash);

Cleanup:
    if (hAlg)
    {
        BCryptCloseAlgorithmProvider(hAlg, 0);
    }
    if (hHash)
    {
        BCryptDestroyHash(hHash);
    }
    if (pbHashObject)
    {
        HeapFree(GetProcessHeap(), 0, pbHashObject);
    }
    if (pbHash)
    {
        HeapFree(GetProcessHeap(), 0, pbHash);
    }
}

std::pair<PBYTE, DWORD> signing(byte data[], int length)
{
    NCRYPT_PROV_HANDLE hProv = NULL;
    NCRYPT_KEY_HANDLE hKey = NULL;
    BCRYPT_KEY_HANDLE hTmpKey = NULL;
    SECURITY_STATUS secStatus = ERROR_SUCCESS;
    BCRYPT_ALG_HANDLE hHashAlg = NULL, hSignAlg = NULL;
    BCRYPT_HASH_HANDLE hHash = NULL;
    NTSTATUS status = STATUS_UNSUCCESSFUL;
    DWORD cbData = 0, cbHash = 0, cbBlob = 0, cbSignature = 0, cbHashObject = 0;
    PBYTE pbHashObject = NULL;
    PBYTE pbHash = NULL, pbBlob = NULL, pbSignature = NULL;

    if (!NT_SUCCESS(status = BCryptOpenAlgorithmProvider(&hHashAlg, BCRYPT_SHA1_ALGORITHM, NULL, 0)))
    {
        wprintf(L"**** Error 0x%x returned by BCryptOpenAlgorithmProvider\n", status);
        goto Cleanup;
    }

    if (!NT_SUCCESS(status = BCryptOpenAlgorithmProvider(&hSignAlg, BCRYPT_ECDSA_P256_ALGORITHM, NULL, 0)))
    {
        wprintf(L"**** Error 0x%x returned by BCryptOpenAlgorithmProvider\n", status);
        goto Cleanup;
    }

    if (!NT_SUCCESS(status = BCryptGetProperty(hHashAlg, BCRYPT_OBJECT_LENGTH, (PBYTE)&cbHashObject, sizeof(DWORD), &cbData, 0)))
    {
        wprintf(L"**** Error 0x%x returned by BCryptGetProperty\n", status);
        goto Cleanup;
    }

    pbHashObject = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbHashObject);
    if (NULL == pbHashObject)
    {
        wprintf(L"**** memory allocation failed\n");
        goto Cleanup;
    }

    if (!NT_SUCCESS(status = BCryptGetProperty(hHashAlg, BCRYPT_HASH_LENGTH, (PBYTE)&cbHash, sizeof(DWORD), &cbData, 0)))
    {
        wprintf(L"**** Error 0x%x returned by BCryptGetProperty\n", status);
        goto Cleanup;
    }

    pbHash = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbHash);
    if (NULL == pbHash)
    {
        wprintf(L"**** memory allocation failed\n");
        goto Cleanup;
    }

    if (!NT_SUCCESS(status = BCryptCreateHash(hHashAlg, &hHash, pbHashObject, cbHashObject, NULL, 0, 0)))
    {
        wprintf(L"**** Error 0x%x returned by BCryptCreateHash\n", status);
        goto Cleanup;
    }

    if (!NT_SUCCESS(status = BCryptHashData(hHash, (PBYTE)data, length, 0)))
    {
        wprintf(L"**** Error 0x%x returned by BCryptHashData\n", status);
        goto Cleanup;
    }

    if (!NT_SUCCESS(status = BCryptFinishHash(hHash, pbHash, cbHash, 0)))
    {
        wprintf(L"**** Error 0x%x returned by BCryptFinishHash\n", status);
        goto Cleanup;
    }

    if (FAILED(secStatus = NCryptOpenStorageProvider(&hProv, MS_KEY_STORAGE_PROVIDER, 0)))
    {
        wprintf(L"**** Error 0x%x returned by NCryptOpenStorageProvider\n", secStatus);
        goto Cleanup;
    }

    if (FAILED(secStatus = NCryptCreatePersistedKey(hProv, &hKey, NCRYPT_ECDSA_P256_ALGORITHM, L"my ecc key", 0, 0)))
    {
        wprintf(L"**** Error 0x%x returned by NCryptCreatePersistedKey\n", secStatus);
        goto Cleanup;
    }

    if (FAILED(secStatus = NCryptFinalizeKey(hKey, 0)))
    {
        wprintf(L"**** Error 0x%x returned by NCryptFinalizeKey\n", secStatus);
        goto Cleanup;
    }

    if (FAILED(secStatus = NCryptSignHash(hKey, NULL, pbHash, cbHash, NULL, 0, &cbSignature, 0)))
    {
        wprintf(L"**** Error 0x%x returned by NCryptSignHash\n", secStatus);
        goto Cleanup;
    }

    pbSignature = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbSignature);
    if (NULL == pbSignature)
    {
        wprintf(L"**** memory allocation failed\n");
        goto Cleanup;
    }

    if (FAILED(secStatus = NCryptSignHash(hKey, NULL, pbHash, cbHash, pbSignature, cbSignature, &cbSignature, 0)))
    {
        wprintf(L"**** Error 0x%x returned by NCryptSignHash\n", secStatus);
        goto Cleanup;
    }

    if (FAILED(secStatus = NCryptExportKey(hKey, NULL, BCRYPT_ECCPUBLIC_BLOB, NULL, NULL, 0, &cbBlob, 0)))
    {
        wprintf(L"**** Error 0x%x returned by NCryptExportKey\n", secStatus);
        goto Cleanup;
    }

    pbBlob = (PBYTE)HeapAlloc(GetProcessHeap(), 0, cbBlob);
    if (NULL == pbBlob)
    {
        wprintf(L"**** memory allocation failed\n");
        goto Cleanup;
    }

    if (FAILED(secStatus = NCryptExportKey(hKey, NULL, BCRYPT_ECCPUBLIC_BLOB, NULL, pbBlob, cbBlob, &cbBlob, 0)))
    {
        wprintf(L"**** Error 0x%x returned by NCryptExportKey\n", secStatus);
        goto Cleanup;
    }

    if (!NT_SUCCESS(status = BCryptImportKeyPair(hSignAlg, NULL, BCRYPT_ECCPUBLIC_BLOB, &hTmpKey, pbBlob, cbBlob, 0)))
    {
        wprintf(L"**** Error 0x%x returned by BCryptImportKeyPair\n", status);
        goto Cleanup;
    }

    if (!NT_SUCCESS(status = BCryptVerifySignature(hTmpKey, NULL, pbHash, cbHash, pbSignature, cbSignature, 0)))
    {
        wprintf(L"**** Error 0x%x returned by BCryptVerifySignature\n", status);
        goto Cleanup;
    }

    if (FAILED(secStatus = NCryptOpenKey(hProv, &hKey, L"my ecc key", 0, 0)))
    {
        wprintf(L"**** Error 0x%x returned by NCryptOpenKey\n", secStatus);
        goto Cleanup;
    }

    if (FAILED(secStatus = NCryptDeleteKey(hKey, 0)))
    {
        wprintf(L"**** Error 0x%x returned by NCryptDeleteKey\n", secStatus);
        goto Cleanup;
    }

    wprintf(L"Success signing file!\n");
    return std::make_pair(pbSignature, cbSignature);

Cleanup:
    if (hHashAlg)
    {
        BCryptCloseAlgorithmProvider(hHashAlg, 0);
    }

    if (hSignAlg)
    {
        BCryptCloseAlgorithmProvider(hSignAlg, 0);
    }

    if (hHash)
    {
        BCryptDestroyHash(hHash);
    }

    if (pbHashObject)
    {
        HeapFree(GetProcessHeap(), 0, pbHashObject);
    }

    if (pbHash)
    {
        HeapFree(GetProcessHeap(), 0, pbHash);
    }

    if (pbSignature)
    {
        HeapFree(GetProcessHeap(), 0, pbSignature);
    }

    if (pbBlob)
    {
        HeapFree(GetProcessHeap(), 0, pbBlob);
    }

    if (hTmpKey)
    {
        BCryptDestroyKey(hTmpKey);
    }

    if (hKey)
    {
        NCryptDeleteKey(hKey, 0);
    }

    if (hProv)
    {
        NCryptFreeObject(hProv);
    }
}