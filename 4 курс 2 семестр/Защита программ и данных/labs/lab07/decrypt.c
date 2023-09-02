#include <stdio.h>
#include <windows.h>
#include <wincrypt.h>
#include "CloseAll.h"

#define KEYLENGTH  0x00800000
#define ENCRYPT_ALGORITHM CALG_RC4
#define ENCRYPT_BLOCK_SIZE 8

BOOL MyDecryptFile(LPTSTR pszSourceFile, LPTSTR pszDestinationFile, LPTSTR pszPassword);

int main(int argc, char** argv)
{
	if(argc != 4)
	{
		printf("Usage: %s <source file> <destination file> <password>\n", argv[0]);
		return 1;
	}

	LPTSTR pszSource = argv[1];
	LPTSTR pszDestination = argv[2];
	LPTSTR pszPassword = argv[3];

	if(!MyDecryptFile(pszSource, pszDestination, pszPassword))
	{
		printf("Error decrypting file!\n", GetLastError());
	}
	else
	{
		printf("Decryption of the file %s was successful. \n", pszSource);
		printf("The decrypted data is in file %s.\n", pszDestination);
	}

	return 0;
}

BOOL MyDecryptFile(LPTSTR pszSourceFile, LPTSTR pszDestinationFile, LPTSTR pszPassword)
{
	HANDLE SourceFile = INVALID_HANDLE_VALUE;
	HANDLE DestinationFile = INVALID_HANDLE_VALUE;
	HCRYPTKEY Key = 0;
	HCRYPTHASH Hash = 0;

	HCRYPTPROV CryptProv = 0;

	DWORD Count;
	PBYTE Buffer = NULL;
	DWORD BlockLen;
	DWORD BufferLen;

	SourceFile = CreateFile(pszSourceFile, FILE_READ_DATA, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(INVALID_HANDLE_VALUE == SourceFile)
	{
		printf("Error opening source plaintext file!\n", GetLastError());
		CloseAll(SourceFile, DestinationFile, Buffer, Hash, Key, CryptProv);
		return FALSE;
	}
	printf("The source encrypted file, %s, is open. \n", pszSourceFile);

	DestinationFile = CreateFile(pszDestinationFile, FILE_WRITE_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(INVALID_HANDLE_VALUE == DestinationFile)
	{
		printf("Error opening destination file!\n", GetLastError());
		CloseAll(SourceFile, DestinationFile, Buffer, Hash, Key, CryptProv);
		return FALSE;
	}
	printf("The destination file, %s, is open. \n", pszDestinationFile);

	if(!CryptAcquireContext(&CryptProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
	{
		printf("Error during CryptAcquireContext!\n", GetLastError());
		CloseAll(SourceFile, DestinationFile, Buffer, Hash, Key, CryptProv);
		return FALSE;
	}
	printf("A cryptographic provider has been acquired.\n");

	if(!CryptCreateHash(CryptProv, CALG_MD5, 0, 0, &Hash))
	{
		printf("Error during CryptCreateHash!\n", GetLastError());
		CloseAll(SourceFile, DestinationFile, Buffer, Hash, Key, CryptProv);
		return FALSE;
	}

	if(!CryptHashData(Hash, (BYTE*)pszPassword, lstrlen(pszPassword), 0))
	{
		printf("Error during CryptHashData!\n", GetLastError());
		CloseAll(SourceFile, DestinationFile, Buffer, Hash, Key, CryptProv);
		return FALSE;
	}

	if(!CryptDeriveKey(CryptProv, ENCRYPT_ALGORITHM, Hash, KEYLENGTH, &Key))
	{
		printf("Error during CryptDeriveKey!\n", GetLastError());
		CloseAll(SourceFile, DestinationFile, Buffer, Hash, Key, CryptProv);
		return FALSE;
	}

	BlockLen = 1000 - 1000 % ENCRYPT_BLOCK_SIZE;
	BufferLen = BlockLen;

	if(!(Buffer = (PBYTE)malloc(BufferLen)))
	{
		printf("Out of memory!\n");
		CloseAll(SourceFile, DestinationFile, Buffer, Hash, Key, CryptProv);
		return FALSE;
	}

	BOOL fEOF = FALSE;
	do
	{
		if(!ReadFile(SourceFile, Buffer, BlockLen, &Count, NULL))
		{
			printf("Error reading from source file!\n", GetLastError());
			CloseAll(SourceFile, DestinationFile, Buffer, Hash, Key, CryptProv);
			return FALSE;
		}

		if(Count < BlockLen)
		{
			fEOF = TRUE;
		}

		if(!CryptDecrypt(Key, 0, fEOF, 0, Buffer, &Count))
		{
			printf("Error during CryptDecrypt!\n", GetLastError());
			CloseAll(SourceFile, DestinationFile, Buffer, Hash, Key, CryptProv);
			return FALSE;
		}

		if(!WriteFile(DestinationFile, Buffer, Count, &Count, NULL))
		{
			printf("Error writing ciphertext.\n", GetLastError());
			CloseAll(SourceFile, DestinationFile, Buffer, Hash, Key, CryptProv);
			return FALSE;
		}
	} while(!fEOF);

	return TRUE;
}
