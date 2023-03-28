#include <stdio.h>
#include <windows.h>
#include <wincrypt.h>

#define CONTAINER_NAME "Container"

int main()
{
	HCRYPTPROV hProv;
	BYTE *pbBuffer= (BYTE *)"The data that is to be hashed and signed.";
	DWORD dwBufferLen = strlen((char *)pbBuffer)+1;
	HCRYPTHASH hHash;
	HCRYPTKEY hKey;
	BYTE *pbKeyBlob;
	BYTE *pbSignature;
	DWORD dwSigLen = 0;
	DWORD dwBlobLen;
	FILE* File;

	File = fopen("test.txt", "rb");
	if (File == NULL)
	{
		printf("Error cant open file.\n");
		exit(1);
	}
	fseek(File, 0, SEEK_END);
	dwBufferLen = ftell(File);
	rewind(File);
	pbBuffer = malloc(dwBufferLen * sizeof(BYTE));
	fread(pbBuffer, dwBufferLen, 1, File);
	fclose(File);
	printf("File has been readed.\n");

	if(!CryptAcquireContext(&hProv, CONTAINER_NAME, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET))
	{
		printf("Error %x during CryptAcquireContext.\n", GetLastError());
		exit(1);
	}
	printf("CSP context acquired.\n");

	if(!CryptGenKey(hProv, AT_SIGNATURE, CRYPT_EXPORTABLE, &hKey)) 
	{
		printf("Error %x during CryptGetUserKey for signkey.\n", GetLastError());
		CryptAcquireContext(&hProv, CONTAINER_NAME, NULL, PROV_RSA_FULL, CRYPT_DELETE_KEYSET);
		exit(1);
	}
	printf("The signature key has been acquired. \n");

	if(!CryptExportKey(hKey, 0, PUBLICKEYBLOB, 0, NULL, &dwBlobLen)) 
	{
		printf("Error %x computing BLOB length.\n", GetLastError());
		CryptDestroyKey(hKey);
		CryptAcquireContext(&hProv, CONTAINER_NAME, NULL, PROV_RSA_FULL, CRYPT_DELETE_KEYSET);
		exit(1);
	}
	printf("Size of the BLOB for the public key determined: %d\n", dwBlobLen);

	if(!(pbKeyBlob = (BYTE*)malloc(dwBlobLen)))
	{
		printf("Out of memory.\n");
		CryptDestroyKey(hKey);
		CryptAcquireContext(&hProv, CONTAINER_NAME, NULL, PROV_RSA_FULL, CRYPT_DELETE_KEYSET);
		exit(1);
	}
	printf("Memory has been allocated for the BLOB.\n");

	if(!CryptExportKey(hKey, 0, PUBLICKEYBLOB, 0, pbKeyBlob, &dwBlobLen))
	{
		printf("Error %x during CryptExportKey.\n", GetLastError());
		free(pbKeyBlob);
		CryptDestroyKey(hKey);
		CryptAcquireContext(&hProv, CONTAINER_NAME, NULL, PROV_RSA_FULL, CRYPT_DELETE_KEYSET);
		exit(1);
	}
	printf("Contents have been written to the BLOB.\n");

	File = fopen("public.key", "w");
	if (File == NULL)
	{
		printf("Error: cant write public key blob to file. Error code: %x\n", GetLastError());
		free(pbKeyBlob);
		CryptDestroyKey(hKey);
		CryptAcquireContext(&hProv, CONTAINER_NAME, NULL, PROV_RSA_FULL, CRYPT_DELETE_KEYSET);
		exit(1);
	}
	fwrite(pbKeyBlob, dwBlobLen, 1, File);
	fclose(File);
	printf("Public key blob has been writed to file.\n");

	if(!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) 
	{
		printf("Error %x during CryptCreateHash.\n", GetLastError());
		free(pbKeyBlob);
		CryptDestroyKey(hKey);
		CryptAcquireContext(&hProv, CONTAINER_NAME, NULL, PROV_RSA_FULL, CRYPT_DELETE_KEYSET);
		exit(1);
	}
	printf("Hash object created.\n");

	if(!CryptHashData(hHash, pbBuffer, dwBufferLen, 0))
	{
		printf("Error %x during CryptHashData.\n", GetLastError());
		CryptDestroyHash(hHash);
		free(pbKeyBlob);
		CryptDestroyKey(hKey);
		CryptAcquireContext(&hProv, CONTAINER_NAME, NULL, PROV_RSA_FULL, CRYPT_DELETE_KEYSET);
		exit(1);
	}
	printf("The data buffer has been hashed.\n");

	if(!CryptSignHash(hHash, AT_SIGNATURE, NULL, 0, NULL, &dwSigLen)) 
	{
		printf("Error %x during CryptSignHash.\n", GetLastError());
		CryptDestroyHash(hHash);
		free(pbKeyBlob);
		CryptDestroyKey(hKey);
		CryptAcquireContext(&hProv, CONTAINER_NAME, NULL, PROV_RSA_FULL, CRYPT_DELETE_KEYSET);
		exit(1);
	}
	printf("Signature length %d found.\n",dwSigLen);

	if(!(pbSignature = (BYTE *)malloc(dwSigLen)))
	{
		printf("Out of memory.\n");
		CryptDestroyHash(hHash);
		free(pbKeyBlob);
		CryptDestroyKey(hKey);
		CryptAcquireContext(&hProv, CONTAINER_NAME, NULL, PROV_RSA_FULL, CRYPT_DELETE_KEYSET);
		exit(1);
	}
	printf("Memory allocated for the signature.\n");

	if(!CryptSignHash(hHash, AT_SIGNATURE, NULL, 0, pbSignature, &dwSigLen)) 
	{
		printf("Error %x during CryptSignHash.\n", GetLastError());
		free(pbSignature);
		CryptDestroyHash(hHash);
		free(pbKeyBlob);
		CryptDestroyKey(hKey);
		CryptAcquireContext(&hProv, CONTAINER_NAME, NULL, PROV_RSA_FULL, CRYPT_DELETE_KEYSET);
		exit(1);
	}
	printf("pbSignature is the hash signature.\n");

	File = fopen("signature.bin", "w");
	if (File == NULL)
	{
		printf("Error: cant write signature to file. Error code: %x\n", GetLastError());
		free(pbSignature);
		CryptDestroyHash(hHash);
		free(pbKeyBlob);
		CryptDestroyKey(hKey);
		CryptAcquireContext(&hProv, CONTAINER_NAME, NULL, PROV_RSA_FULL, CRYPT_DELETE_KEYSET);
		exit(1);
	}
	fwrite(pbSignature, dwSigLen, 1, File);
	fclose(File);
	printf("Signature has been writed to file.\n");

	free(pbSignature);
	CryptDestroyHash(hHash);
	free(pbKeyBlob);
	CryptDestroyKey(hKey);
	CryptAcquireContext(&hProv, CONTAINER_NAME, NULL, PROV_RSA_FULL, CRYPT_DELETE_KEYSET);
	return 0;
}