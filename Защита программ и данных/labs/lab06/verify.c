#include <stdio.h>
#include <windows.h>
#include <wincrypt.h>

#define CONTAINER_NAME "Container"

int main()
{
	HCRYPTPROV hProv;
	BYTE* pbKeyBlob;
	DWORD dwBlobLen;
	HCRYPTKEY hPubKey;
	FILE* File;
	HCRYPTHASH hHash;
	BYTE* pbBuffer;
	DWORD dwBufferLen;
	BYTE* pbSignature;
	DWORD dwSigLen;

	if(!CryptAcquireContext(&hProv, CONTAINER_NAME, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET))
	{
		printf("Error %x during CryptAcquireContext.\n", GetLastError());
		exit(1);
	}
	printf("CSP context acquired.\n");

	File = fopen("public.key", "rb");
	if (File == NULL)
	{
		printf("Error cant open file.\n");
		CryptAcquireContext(&hProv, CONTAINER_NAME, NULL, PROV_RSA_FULL, CRYPT_DELETE_KEYSET);
		exit(1);
	}
	fseek(File, 0, SEEK_END);
	dwBlobLen = ftell(File);
	rewind(File);
	pbKeyBlob = malloc(dwBlobLen * sizeof(BYTE));
	fread(pbKeyBlob, dwBlobLen, 1, File);
	fclose(File);
	printf("File with key has been readed.\n");

	if(!CryptImportKey(hProv, pbKeyBlob, dwBlobLen, 0, 0, &hPubKey))
	{
		printf("Error: cant import key to blob. Error code: %x\n", GetLastError());
		free(pbKeyBlob);
		CryptAcquireContext(&hProv, CONTAINER_NAME, NULL, PROV_RSA_FULL, CRYPT_DELETE_KEYSET);
		exit(1);
	}
	printf("The key has been imported.\n");

	if(!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash))
	{
		printf("Error: cant create hash object. Error code: %x", GetLastError());
		free(pbKeyBlob);
		CryptAcquireContext(&hProv, CONTAINER_NAME, NULL, PROV_RSA_FULL, CRYPT_DELETE_KEYSET);
		exit(1);
	}
	printf("The hash object has been created. \n");

	File = fopen("test.txt", "rb");
	if (File == NULL)
	{
		printf("Error cant open file.\n");
		CryptDestroyHash(hHash);
		free(pbKeyBlob);
		CryptAcquireContext(&hProv, CONTAINER_NAME, NULL, PROV_RSA_FULL, CRYPT_DELETE_KEYSET);
		exit(1);
	}
	fseek(File, 0, SEEK_END);
	dwBufferLen = ftell(File);
	rewind(File);
	pbBuffer = malloc(dwBufferLen * sizeof(BYTE));
	fread(pbBuffer, dwBufferLen, 1, File);
	fclose(File);
	printf("File with data has been readed.\n");

	if(!CryptHashData(hHash, pbBuffer, dwBufferLen, 0))
	{
		printf("Error: cant hash data. Error code: %x", GetLastError());
		free(pbBuffer);
		CryptDestroyHash(hHash);
		free(pbKeyBlob);
		CryptAcquireContext(&hProv, CONTAINER_NAME, NULL, PROV_RSA_FULL, CRYPT_DELETE_KEYSET);
		exit(1);
	}
	printf("The data has been hashed.\n");

	File = fopen("signature.bin", "rb");
	if (File == NULL)
	{
		printf("Error cant open file.\n");
		free(pbBuffer);
		CryptDestroyHash(hHash);
		free(pbKeyBlob);
		CryptAcquireContext(&hProv, CONTAINER_NAME, NULL, PROV_RSA_FULL, CRYPT_DELETE_KEYSET);
		exit(1);
	}
	fseek(File, 0, SEEK_END);
	dwSigLen = ftell(File);
	rewind(File);
	pbSignature = malloc(dwSigLen * sizeof(BYTE));
	fread(pbSignature, dwSigLen, 1, File);
	fclose(File);
	printf("File with data has been readed.\n");

	if(CryptVerifySignature(hHash, pbSignature, dwSigLen, hPubKey, NULL, 0))
	{
		printf("The signature has been verified.\n");
	}
	else
	{
		printf("Signature not validated!\n");
	}

	free(pbSignature);
	free(pbBuffer);
	CryptDestroyHash(hHash);
	free(pbKeyBlob);
	CryptAcquireContext(&hProv, CONTAINER_NAME, NULL, PROV_RSA_FULL, CRYPT_DELETE_KEYSET);
	printf("Container has been destroyed.\n");
}