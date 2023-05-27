#include <stdio.h>
#include <windows.h>
#include <bcrypt.h>
#include <iostream>
#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)

int main() {
    ULONG pAlgCount;
    BCRYPT_ALGORITHM_IDENTIFIER* pAlgList = NULL;

    std::cout << "BCRYPT_CIPHER_OPERATION:" << "\n";
    if (BCryptEnumAlgorithms(BCRYPT_CIPHER_OPERATION, &pAlgCount, &pAlgList, 0) != STATUS_SUCCESS)
    {
        std::cout << "Error while call BCryptEnumAlgorithms for BCRYPT_CIPHER_OPERATION" << "\n";
        BCryptFreeBuffer(pAlgList);
    }
    for (int i = 0; i < pAlgCount; i++)
        std::wcout << "\tAlgorithm name: " << pAlgList[i].pszName << "\n";

    std::cout << "BCRYPT_CIPHER_OPERATION:" << "\n";
    if (BCryptEnumAlgorithms(BCRYPT_HASH_OPERATION, &pAlgCount, &pAlgList, 0) != STATUS_SUCCESS)
    {
        std::cout << "Error while call BCryptEnumAlgorithms for BCRYPT_HASH_OPERATION" << "\n";
        BCryptFreeBuffer(pAlgList);
    }
    for (int i = 0; i < pAlgCount; i++)
        std::wcout << "\tAlgorithm name: " << pAlgList[i].pszName << "\n";

    std::cout << "BCRYPT_CIPHER_OPERATION:" << "\n";
    if (BCryptEnumAlgorithms(BCRYPT_ASYMMETRIC_ENCRYPTION_OPERATION, &pAlgCount, &pAlgList, 0) != STATUS_SUCCESS)
    {
        std::cout << "Error while call BCryptEnumAlgorithms for BCRYPT_ASYMMETRIC_ENCRYPTION_OPERATION" << "\n";
        BCryptFreeBuffer(pAlgList);
    }
    for (int i = 0; i < pAlgCount; i++)
        std::wcout << "\tAlgorithm name: " << pAlgList[i].pszName << "\n";

    std::cout << "BCRYPT_CIPHER_OPERATION:" << "\n";
    if (BCryptEnumAlgorithms(BCRYPT_SECRET_AGREEMENT_OPERATION, &pAlgCount, &pAlgList, 0) != STATUS_SUCCESS)
    {
        std::cout << "Error while call BCryptEnumAlgorithms for BCRYPT_SECRET_AGREEMENT_OPERATION" << "\n";
        BCryptFreeBuffer(pAlgList);
    }
    for (int i = 0; i < pAlgCount; i++)
        std::wcout << "\tAlgorithm name: " << pAlgList[i].pszName << "\n";

    std::cout << "BCRYPT_CIPHER_OPERATION:" << "\n";
    if (BCryptEnumAlgorithms(BCRYPT_SIGNATURE_OPERATION, &pAlgCount, &pAlgList, 0) != STATUS_SUCCESS)
    {
        std::cout << "Error while call BCryptEnumAlgorithms for BCRYPT_SIGNATURE_OPERATION" << "\n";
        BCryptFreeBuffer(pAlgList);
    }
    for (int i = 0; i < pAlgCount; i++)
        std::wcout << "\tAlgorithm name: " << pAlgList[i].pszName << "\n";

    std::cout << "BCRYPT_CIPHER_OPERATION:" << "\n";
    if (BCryptEnumAlgorithms(BCRYPT_RNG_OPERATION, &pAlgCount, &pAlgList, 0) != STATUS_SUCCESS)
    {
        std::cout << "Error while call BCryptEnumAlgorithms for BCRYPT_RNG_OPERATION" << "\n";
        BCryptFreeBuffer(pAlgList);
    }
    for (int i = 0; i < pAlgCount; i++)
        std::wcout << "\tAlgorithm name: " << pAlgList[i].pszName << "\n";

    return 0;
}