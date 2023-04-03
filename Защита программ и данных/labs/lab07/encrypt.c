#include <stdio.h>
#include <windows.h>
#include <wincrypt.h>
#include <CloseAll.h>

#define KEYLENGTH  0x00800000
#define ENCRYPT_ALGORITHM CALG_RC4
#define ENCRYPT_BLOCK_SIZE 8


BOOL EncryptFile(LPTSTR SourceFileName, LPTSTR DestinationFileName, LPTSTR Password);

int main(int argc, char** argv)
{
	if(argc != 4)
	{
		printf("Usage: <%s> <source file> <destination file> <password>\n", argv[0]);
		return 1;
	}

	LPTSTR Source = argv[1];
	LPTSTR Destination = argv[2];
	LPTSTR Password = argv[3];

	if(!EncryptFile(Source, Destination, Password))
	{
		printf("Error encrypting file! Error code: %x\n", GetLastError());
		return 1;
	}
	printf("Encryption of the file %s was successful. \n", Source);
	printf("The encrypted data is in file %s.\n", Destination);

	return 0;
}

BOOL EncryptFile(LPTSTR SourceFileName, LPTSTR DestinationFileName, LPTSTR Password)
{
	HANDLE SourceFile = INVALID_HANDLE_VALUE;
	HANDLE DestinationFile = INVALID_HANDLE_VALUE;

	HCRYPTPROV CryptProv = 0;
	HCRYPTKEY Key = 0;
	HCRYPTKEY XchgKey = 0;
	HCRYPTHASH Hash = 0;

	PBYTE KeyBlob = NULL;
	DWORD KeyBlobLen;

	PBYTE Buffer = NULL;
	DWORD BlockLen;
	DWORD BufferLen;
	DWORD Count;

	SourceFile = CreateFile(SourceFileName, FILE_READ_DATA, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(INVALID_HANDLE_VALUE == SourceFile)
	{
		printf("Error opening source plaintext file! Error code: %x\n", GetLastError());
		CloseAll(SourceFile, DestinationFile, Buffer, Hash, Key, CryptProv);
		return FALSE;
	}
	printf("The source plaintext file, %s, is open.\n", SourceFileName);

	DestinationFile = CreateFile(DestinationFileName, FILE_WRITE_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(INVALID_HANDLE_VALUE == DestinationFile)
	{
		printf("Error opening destination file! Error code: %x\n", GetLastError());
		CloseAll(SourceFile, DestinationFile, Buffer, Hash, Key, CryptProv);
		return FALSE;
	}
	printf("The destination file, %s, is open.\n", DestinationFileName);

	if(!CryptAcquireContext(&CryptProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT))
	{
		printf("Error during CryptAcquireContext! Error code: %x\n", GetLastError());
		CloseAll(SourceFile, DestinationFile, Buffer, Hash, Key, CryptProv);
		return FALSE;
	}
	printf("A cryptographic provider has been acquired.\n");

	if(!Password[0])
	{
		printf("Zero password length.\n");
		CloseAll(SourceFile, DestinationFile, Buffer, Hash, Key, CryptProv);
		return FALSE;
	}

	if(!CryptCreateHash(CryptProv, CALG_MD5, 0, 0, &Hash))
	{
		printf("Error during CryptCreateHash! Error code: %x\n", GetLastError());
		CloseAll(SourceFile, DestinationFile, Buffer, Hash, Key, CryptProv);
		return FALSE;
	}
	printf("A hash object has been created.\n");

	if(!CryptHashData(Hash, (BYTE*)Password, lstrlen(Password), 0))
	{
		printf("Error during CryptHashData. Error code: %x\n", GetLastError());
		CloseAll(SourceFile, DestinationFile, Buffer, Hash, Key, CryptProv);
		return FALSE;
	}
	printf("The password has been added to the hash.\n");

	if(!CryptDeriveKey(CryptProv, ENCRYPT_ALGORITHM, Hash, KEYLENGTH, &Key))
	{
		printf("Error during CryptDeriveKey! Error code: %x\n", GetLastError());
		CloseAll(SourceFile, DestinationFile, Buffer, Hash, Key, CryptProv);
		return FALSE;
	}
	printf("An encryption key is derived from the password hash.\n");

	BlockLen = 1000 - 1000 % ENCRYPT_BLOCK_SIZE;

	if(ENCRYPT_BLOCK_SIZE > 1)
	{
		BufferLen = BlockLen + ENCRYPT_BLOCK_SIZE;
	}
	else
	{
		BufferLen = BlockLen;
	}

	if(!(Buffer = (BYTE *)malloc(BufferLen)))
	{
		printf("Out of memory.\n");
		CloseAll(SourceFile, DestinationFile, Buffer, Hash, Key, CryptProv);
		return FALSE;
	}
	printf("Memory has been allocated for the buffer.\n");

	BOOL fEOF = FALSE;
	do
	{
		if(!ReadFile(SourceFile, Buffer, BlockLen, &Count, NULL))
		{
			printf("Error reading plaintext! Error code: %x\n", GetLastError());
			CloseAll(SourceFile, DestinationFile, Buffer, Hash, Key, CryptProv);
			return FALSE;
		}

		if(Count < BlockLen)
		{
			fEOF = TRUE;
		}

		if(!CryptEncrypt(Key, 0, fEOF, 0, Buffer, &Count, BufferLen))
		{
			printf("Error during CryptEncrypt. Error code: %x\n", GetLastError());
			CloseAll(SourceFile, DestinationFile, Buffer, Hash, Key, CryptProv);
			return FALSE;
		}

		if(!WriteFile(DestinationFile, Buffer, Count, &Count, NULL))
		{
			printf("Error writing ciphertext. Error code: %x\n", GetLastError());
			CloseAll(SourceFile, DestinationFile, Buffer, Hash, Key, CryptProv);
			return FALSE;
		}
	} while(!fEOF);

	return TRUE;
}
