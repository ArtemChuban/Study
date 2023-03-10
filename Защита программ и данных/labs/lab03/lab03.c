#include <stdio.h>
#include <windows.h>
#include <wincrypt.h>

#define BUFSIZE 1024
#define MD5LEN 16

DWORD main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("Usage: executable file_to_hash\nExample: main.exe test.txt");
		return ERROR_BAD_ARGUMENTS;
	}

	DWORD Status = 0;
	BOOL Result = FALSE;
	HCRYPTPROV Provider = 0;
	HCRYPTHASH Hash = 0;
	HANDLE File = NULL;
	BYTE Buffer[BUFSIZE];
	DWORD FileChunk = 0;
	BYTE HashValue[MD5LEN];
	DWORD HashValueLength = 0;
	LPCSTR FileName = argv[1];

	File = CreateFile(FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if (INVALID_HANDLE_VALUE == File)
	{
		Status = GetLastError();
		printf("Error opening file %s\nError: %d\n", FileName, Status); 
		return Status;
	}

	if (!CryptAcquireContext(&Provider, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
	{
		Status = GetLastError();
		printf("CryptAcquireContext failed: %d\n", Status); 
		CloseHandle(File);
		return Status;
	}

	if (!CryptCreateHash(Provider, CALG_MD5, 0, 0, &Hash))
	{
		Status = GetLastError();
		printf("CryptAcquireContext failed: %d\n", Status); 
		CloseHandle(File);
		CryptReleaseContext(Provider, 0);
		return Status;
	}

	while (Result = ReadFile(File, Buffer, BUFSIZE, &FileChunk, NULL))
	{
		if (0 == FileChunk)
		{
			break;
		}

		if (!CryptHashData(Hash, Buffer, FileChunk, 0))
		{
			Status = GetLastError();
			printf("CryptHashData failed: %d\n", Status); 
			CryptReleaseContext(Provider, 0);
			CryptDestroyHash(Hash);
			CloseHandle(File);
			return Status;
		}
	}

	if (!Result)
	{
		Status = GetLastError();
		printf("ReadFile failed: %d\n", Status); 
		CryptReleaseContext(Provider, 0);
		CryptDestroyHash(Hash);
		CloseHandle(File);
		return Status;
	}

	HashValueLength = MD5LEN;
	if (CryptGetHashParam(Hash, HP_HASHVAL, HashValue, &HashValueLength, 0))
	{
		printf("MD5 hash of file %s is: ", FileName);
		for (DWORD i = 0; i < HashValueLength; i++)
		{
			printf("%02x", HashValue[i]);
		}
		printf("\n");
	}
	else
	{
		Status = GetLastError();
		printf("CryptGetHashParam failed: %d\n", Status); 
	}

	CryptDestroyHash(Hash);
	CryptReleaseContext(Provider, 0);
	CloseHandle(File);

	return Status; 
}
