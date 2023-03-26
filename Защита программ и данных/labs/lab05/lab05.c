#include <stdio.h>
#include <windows.h>
#include <wincrypt.h>

#define PROVIDER_NAME NULL
#define PROVIDER_TYPE PROV_RSA_FULL

HCRYPTPROV CreateContainer(LPCTSTR ContainerName);
void DestroyContainer(LPCTSTR ContainerName);
void DestroyContainers();
void CreateContainers(HCRYPTPROV* Container1, HCRYPTPROV* Container2, HCRYPTPROV* Container3);
BOOL Task34(HCRYPTPROV Container);
BOOL Task5(HCRYPTPROV Container);
BOOL Task6();
BOOL Task7(HCRYPTPROV Container);
BOOL Task8(HCRYPTPROV Container);
BOOL Task9(HCRYPTPROV Container);
BOOL Task10(HCRYPTPROV Container);
BOOL Task11(HCRYPTPROV Container);

int main()
{
	HCRYPTPROV Container1;
	HCRYPTPROV Container2;
	HCRYPTPROV Container3;

	CreateContainers(&Container1, &Container2, &Container3);

	if (!Task34(Container1))
	{
		printf("Halting.\n");
		DestroyContainers();
		exit(1);
	}

	if (!Task5(Container2))
	{
		printf("Halting.\n");
		DestroyContainers();
		exit(1);
	}

	if (!Task6())
	{
		printf("Halting.\n");
		DestroyContainers();
		exit(1);
	}

	if (!Task7(Container1))
	{
		printf("Halting.\n");
		DestroyContainers();
		exit(1);
	}

	if (!Task8(Container2))
	{
		printf("Halting.\n");
		DestroyContainers();
		exit(1);
	}

	if (!Task9(Container3))
	{
		printf("Halting.\n");
		DestroyContainers();
		exit(1);
	}

	if (!Task10(Container1))
	{
		printf("Halting.\n");
		DestroyContainers();
		exit(1);
	}

	if (!Task11(Container3))
	{
		printf("Halting.\n");
		DestroyContainers();
		exit(1);
	}


	DestroyContainers();
	return 0;
}

BOOL Task11(HCRYPTPROV Container)
{
	FILE* File;
	DWORD BlobLength;
	BYTE* Blob;
	HCRYPTKEY Key;

	printf("Task 11:\n");

	File = fopen("KeyX Private Unencrypted.bin", "rb");
	if (File == NULL)
	{
		printf("Error: cant open file \"KeyX Private Unencrypted.bin\".\n");
		return FALSE;
	}
	fseek(File, 0, SEEK_END);
	BlobLength = ftell(File);
	rewind(File);
	Blob = malloc(BlobLength * sizeof(BYTE));
	fread(Blob, BlobLength, 1, File);
	fclose(File);
	printf("File \"KeyX Private Unencrypted.bin\" has been readed to blob.\n");

	if (!CryptImportKey(Container, Blob, BlobLength, 0, CRYPT_EXPORTABLE, &Key))
	{
		printf("Error: cant import key. Error code: %x\n", GetLastError());
		free(Blob);
		return FALSE;
	}
	printf("Key has been imported.\n");

	free(Blob);
	CryptDestroyKey(Key);
	return TRUE;
}

BOOL Task10(HCRYPTPROV Container)
{
	HCRYPTKEY Key;
	DWORD BlobLength;
	BYTE* Blob;
	FILE* File;

	printf("Task 10:\n");

	if (!CryptGetUserKey(Container, AT_KEYEXCHANGE, &Key))
	{
		printf("Error: cant get key from container. Error code: %x\n", GetLastError());
		return FALSE;
	}
	printf("Getting key from container.\n");

	if (!CryptExportKey(Key, 0, PRIVATEKEYBLOB, 0, NULL, &BlobLength))
	{
		printf("Error: cant get needed length of private key blob. Error code: %x\n", GetLastError());
		CryptDestroyKey(Key);
		return FALSE;
	}
	printf("Needed length of private key blob: %d.\n", BlobLength);
	Blob = malloc(BlobLength * sizeof(BYTE));

	if (!CryptExportKey(Key, 0, PRIVATEKEYBLOB, 0, Blob, &BlobLength))
	{
		printf("Error: cant export key. Error code: %x\n", GetLastError());
		free(Blob);
		CryptDestroyKey(Key);
		return FALSE;
	}
	printf("Key has been exported to private key blob.\n");

	File = fopen("KeyX Private Unencrypted.bin", "w");
	if (File == NULL)
	{
		printf("Error: cant write private key blob to file. Error code: %x\n", GetLastError());
		fclose(File);
		free(Blob);
		CryptDestroyKey(Key);
		return FALSE;
	}
	printf("Private key blob has been writed to file \"KeyX Private Unencrypted.bin\".\n");
	fwrite(Blob, BlobLength, 1, File);

	fclose(File);
	free(Blob);
	CryptDestroyKey(Key);
	return TRUE;
}

BOOL Task9(HCRYPTPROV Container)
{
	FILE* File;
	HCRYPTKEY Key;
	HCRYPTKEY SessionKey;
	DWORD BlobLength;
	BYTE* Blob;
	HCRYPTHASH Hash;
	CHAR Password[] = "password";
	DWORD PasswordLength = strlen(Password);

	printf("Task 9:\n");

	if(!CryptCreateHash(Container, CALG_MD5, 0, 0, &Hash))
	{
		printf("Error: cant create empty hash object. Error code: %x\n", GetLastError());
		return FALSE;
	}
	printf("An empty hash object has been created.\n");

	if(!CryptHashData(Hash, (BYTE*)Password, PasswordLength, 0))
	{
		printf("Error: cant hash password. Error code: %x\n", GetLastError());
		CryptDestroyHash(Hash);
		return FALSE;
	}
	printf("The password has been hashed.\n");

	if(!CryptDeriveKey(Container, CALG_DES, Hash, CRYPT_EXPORTABLE | CRYPT_NO_SALT, &SessionKey))
	{
		printf("Error: cant derive key. Error code: %x\n", GetLastError());
		CryptDestroyHash(Hash);
		return FALSE;
	}
	printf("The key has been derived.\n");
	CryptDestroyHash(Hash);

	File = fopen("DS Private Encrypted.bin", "rb");
	if (File == NULL)
	{
		printf("Error: cant open file \"DS Private Encrypted.bin\".\n");
		return FALSE;
	}
	fseek(File, 0, SEEK_END);
	BlobLength = ftell(File);
	rewind(File);
	Blob = malloc(BlobLength * sizeof(BYTE));
	fread(Blob, BlobLength, 1, File);
	fclose(File);
	printf("File \"DS Private Encrypted.bin\" has been readed to blob.\n");

	// TODO: This `if` doesnt work properly: throws NTE_BAD_DATA.
	if (CryptImportKey(Container, Blob, BlobLength, SessionKey, CRYPT_EXPORTABLE | CRYPT_NO_SALT, &Key))
	{
		printf("Error: cant import key. Error code: %x\n", GetLastError());
		free(Blob);
		CryptDestroyKey(SessionKey);
		return FALSE;
	}
	printf("DS key has been imported.\n");

	free(Blob);
	CryptDestroyKey(Key);
	CryptDestroyKey(SessionKey);
	return TRUE;
}

BOOL Task8(HCRYPTPROV Container)
{
	HCRYPTKEY Key;
	DWORD BlobLength;
	BYTE* Blob;
	FILE* File;
	HCRYPTKEY DerivedKey;
	HCRYPTHASH Hash;
	CHAR Password[] = "password";
	DWORD PasswordLength = strlen(Password);

	printf("Task 8:\n");

	if (!CryptGetUserKey(Container, AT_SIGNATURE, &Key))
	{
		printf("Error: cant get key from container. Error code: %x\n", GetLastError());
		return FALSE;
	}
	printf("Getting signature key from container.\n");

	if (!CryptExportKey(Key, 0, PRIVATEKEYBLOB, 0, NULL, &BlobLength))
	{
		printf("Error: cant get needed length of private key blob. Error code: %x\n", GetLastError());
		CryptDestroyKey(Key);
		return FALSE;
	}
	printf("Needed length of private key blob: %d.\n", BlobLength);
	Blob = malloc(BlobLength * sizeof(BYTE));

	if (!CryptExportKey(Key, 0, PRIVATEKEYBLOB, 0, Blob, &BlobLength))
	{
		printf("Error: cant export key. Error code: %x\n", GetLastError());
		free(Blob);
		CryptDestroyKey(Key);
		return FALSE;
	}
	printf("Key has been exported to private key blob.\n");

	File = fopen("DS Private Unencrypted.bin", "w");
	if (File == NULL)
	{
		printf("Error: cant write private key blob to file. Error code: %x\n", GetLastError());
		fclose(File);
		free(Blob);
		CryptDestroyKey(Key);
		return FALSE;
	}
	fwrite(Blob, BlobLength, 1, File);
	fclose(File);
	printf("Private key blob has been writed to file \"DS Private Unencrypted.bin\".\n");

	if(!CryptCreateHash(Container, CALG_MD5, 0, 0, &Hash))
	{
		printf("Error: cant create empty hash object. Error code: %x\n", GetLastError());
		free(Blob);
		CryptDestroyKey(Key);
		return FALSE;
	}
	printf("An empty hash object has been created.\n");

	if(!CryptHashData(Hash, (BYTE*)Password, PasswordLength, 0))
	{
		printf("Error: cant hash password. Error code: %x\n", GetLastError());
		free(Blob);
		CryptDestroyKey(Key);
		CryptDestroyHash(Hash);
		return FALSE;
	}
	printf("The password has been hashed.\n");

	if(!CryptDeriveKey(Container, CALG_DES, Hash, CRYPT_EXPORTABLE | CRYPT_NO_SALT, &DerivedKey))
	{
		printf("Error: cant derive key. Error code: %x\n", GetLastError());
		free(Blob);
		CryptDestroyKey(Key);
		CryptDestroyHash(Hash);
		return FALSE;
	}
	printf("The key has been derived.\n");
	CryptDestroyHash(Hash);
	free(Blob);

	if (!CryptExportKey(Key, DerivedKey, PRIVATEKEYBLOB, 0, NULL, &BlobLength))
	{
		printf("Error: cant get needed length of private key blob. Error code: %x\n", GetLastError());
		CryptDestroyKey(Key);
		CryptDestroyKey(DerivedKey);
		return FALSE;
	}
	printf("Needed length of private key blob: %d.\n", BlobLength);
	Blob = malloc(BlobLength * sizeof(BYTE));

	if (!CryptExportKey(Key, DerivedKey, PRIVATEKEYBLOB, 0, Blob, &BlobLength))
	{
		printf("Error: cant export key. Error code: %x\n", GetLastError());
		free(Blob);
		CryptDestroyKey(Key);
		CryptDestroyKey(DerivedKey);
		return FALSE;
	}
	printf("Key has been exported to private key blob.\n");

	File = fopen("DS Private Encrypted.bin", "w");
	if (File == NULL)
	{
		printf("Error: cant write private key blob to file. Error code: %x\n", GetLastError());
		fclose(File);
		free(Blob);
		CryptDestroyKey(Key);
		CryptDestroyKey(DerivedKey);
		return FALSE;
	}
	fwrite(Blob, BlobLength, 1, File);
	fclose(File);
	printf("Private key blob has been writed to file \"DS Private Encrypted.bin\".\n");

	free(Blob);
	CryptDestroyKey(Key);
	CryptDestroyKey(DerivedKey);
	CryptReleaseContext(Container, 0);

	return TRUE;
}

BOOL Task7(HCRYPTPROV Container)
{
	FILE* File;
	BYTE* Blob;
	DWORD BlobLength;
	HCRYPTKEY EncryptedKey;
	HCRYPTKEY PlainKey;
	HCRYPTKEY PublicKey;
	DWORD EncryptedKeyBlobLength;
	DWORD PlainKeyBlobLength;
	BYTE* PlainKeyBlob;
	BYTE* EncryptedKeyBlob;

	printf("Task 7:\n");

	File = fopen("Session Key Encrypted.bin", "rb");
	if (File == NULL)
	{
		printf("Error: cant open file \"Session Key Encrypted.bin\".\n");
		return FALSE;
	}
	fseek(File, 0, SEEK_END);
	BlobLength = ftell(File);
	rewind(File);
	Blob = malloc(BlobLength * sizeof(BYTE));
	fread(Blob, BlobLength, 1, File);
	fclose(File);
	printf("File \"Session Key Encrypted.bin\" has been readed to blob.\n");

	if (!CryptGetUserKey(Container, AT_KEYEXCHANGE, &PublicKey))
	{
		printf("Error: cant get public key from container. Error code: %x\n", GetLastError());
		free(Blob);
		return FALSE;
	}
	printf("Getting public key from container.\n");

	// TODO: sometimes throws 0x80090005 error.
	if (!CryptImportKey(Container, Blob, BlobLength, PublicKey, CRYPT_EXPORTABLE, &EncryptedKey))
	{
		printf("Error: cant import encrypted session key. Error code: %x\n", GetLastError());
		free(Blob);
		CryptDestroyKey(PublicKey);
		return FALSE;
	}
	printf("Encrypted session key has been imported.\n");
	free(Blob);

	File = fopen("Session Key Plaintext.bin", "rb");
	if (File == NULL)
	{
		printf("Error: cant open file \"Session Key Plaintext.bin\".\n");
		CryptDestroyKey(EncryptedKey);
		CryptDestroyKey(PublicKey);
		return FALSE;
	}
	fseek(File, 0, SEEK_END);
	BlobLength = ftell(File);
	rewind(File);
	Blob = malloc(BlobLength * sizeof(BYTE));
	fread(Blob, BlobLength, 1, File);
	fclose(File);
	printf("File \"Session Key Plaintext.bin\" has been readed to blob.\n");

	if (!CryptImportKey(Container, Blob, BlobLength, 0, CRYPT_EXPORTABLE, &PlainKey))
	{
		printf("Error: cant import plaint text session key. Error code: %x\n", GetLastError());
		free(Blob);
		CryptDestroyKey(PublicKey);
		CryptDestroyKey(EncryptedKey);
		return FALSE;
	}
	printf("Plaint text session key has been imported.\n");
	free(Blob);

	if (!CryptExportKey(EncryptedKey, 0, PLAINTEXTKEYBLOB, 0, NULL, &EncryptedKeyBlobLength))
	{
		printf("Error: cant get length of encrypted key blob. Error code: %x\n", GetLastError());
		CryptDestroyKey(PlainKey);
		CryptDestroyKey(EncryptedKey);
		CryptDestroyKey(PublicKey);
		return FALSE;
	}
	printf("Length of encrypted key blob: %d.\n", EncryptedKeyBlobLength);
	if (!CryptExportKey(PlainKey, 0, PLAINTEXTKEYBLOB, 0, NULL, &PlainKeyBlobLength))
	{
		printf("Error: cant get length of plain text key blob. Error code: %x\n", GetLastError());
		CryptDestroyKey(PlainKey);
		CryptDestroyKey(EncryptedKey);
		CryptDestroyKey(PublicKey);
		return FALSE;
	}
	printf("Length of plain text key blob: %d.\n", PlainKeyBlobLength);
	EncryptedKeyBlob = malloc(EncryptedKeyBlobLength * sizeof(BYTE));
	PlainKeyBlob = malloc(PlainKeyBlobLength * sizeof(BYTE));

	if (!CryptExportKey(EncryptedKey, 0, PLAINTEXTKEYBLOB, 0, EncryptedKeyBlob, &EncryptedKeyBlobLength))
	{
		printf("Error: cant export encrypted key to blob. Error code: %x\n", GetLastError());
		CryptDestroyKey(PlainKey);
		CryptDestroyKey(EncryptedKey);
		CryptDestroyKey(PublicKey);
		free(EncryptedKeyBlob);
		free(PlainKeyBlob);
		return FALSE;
	}
	printf("Encrypted key has been exported to plain text key blob.\n");
	if (!CryptExportKey(PlainKey, 0, PLAINTEXTKEYBLOB, 0, PlainKeyBlob, &PlainKeyBlobLength))
	{
		printf("Error: cant export plain text key to blob. Error code: %x\n", GetLastError());
		CryptDestroyKey(PlainKey);
		CryptDestroyKey(EncryptedKey);
		CryptDestroyKey(PublicKey);
		free(EncryptedKeyBlob);
		free(PlainKeyBlob);
		return FALSE;
	}
	printf("Plain text key has been exported to plain text key blob.\n");

	if (EncryptedKeyBlobLength != PlainKeyBlobLength)
	{
		printf("Encrypted Session Key length != Plain Session Key length\n");
		CryptDestroyKey(PlainKey);
		CryptDestroyKey(EncryptedKey);
		CryptDestroyKey(PublicKey);
		free(EncryptedKeyBlob);
		free(PlainKeyBlob);
		return FALSE;
	}

	for (DWORD i = 0; i < EncryptedKeyBlobLength; i++)
	{
		if (EncryptedKeyBlob[i] != PlainKeyBlob[i])
		{
			printf("Encrypted Session Key != Plain Session Key\n");
			CryptDestroyKey(PlainKey);
			CryptDestroyKey(EncryptedKey);
			CryptDestroyKey(PublicKey);
			free(EncryptedKeyBlob);
			free(PlainKeyBlob);
			return FALSE;
		}
	}
	printf("Encrypted Session Key == Plain Session Key\n");

	CryptDestroyKey(PlainKey);
	CryptDestroyKey(EncryptedKey);
	CryptDestroyKey(PublicKey);
	free(EncryptedKeyBlob);
	free(PlainKeyBlob);
	return TRUE;
}

BOOL Task6()
{
	HCRYPTPROV Provider;
	HCRYPTKEY SessionKey;
	HCRYPTKEY PublicKey;
	DWORD BlobLength;
	BYTE* Blob;
	FILE* File;

	printf("Task 6:\n");
	if (!CryptAcquireContext(&Provider, NULL, PROVIDER_NAME, PROVIDER_TYPE, CRYPT_VERIFYCONTEXT))
	{
		printf("Error: cant connect to provider. Error code: %x\n", GetLastError());
		return FALSE;
	}
	printf("Connected to provider w\\o container.\n");


	if (!CryptGenKey(Provider, CALG_DES, CRYPT_EXPORTABLE, &SessionKey))
	{
		printf("Error: cant generate session key. Error code: %x\n", GetLastError());
		CryptReleaseContext(Provider, 0);
		return FALSE;
	}
	printf("Session key generated.\n");

	if (!CryptExportKey(SessionKey, 0, PLAINTEXTKEYBLOB, 0, NULL, &BlobLength))
	{
		printf("Error: cant get length of plain ttext key blob. Error code: %x\n", GetLastError());
		CryptDestroyKey(SessionKey);
		CryptReleaseContext(Provider, 0);
		return FALSE;
	}
	printf("Length of plain text key blob: %d.\n", BlobLength);
	Blob = malloc(BlobLength * sizeof(BYTE));

	if (!CryptExportKey(SessionKey, 0, PLAINTEXTKEYBLOB, 0, Blob, &BlobLength))
	{
		printf("Error: cant export session key. Error code: %x\n", GetLastError());
		free(Blob);
		CryptDestroyKey(SessionKey);
		CryptReleaseContext(Provider, 0);
		return FALSE;
	}
	printf("Key has been imported to plain text key blob.\n");

	File = fopen("Session Key Plaintext.bin", "w");
	if (File == NULL)
	{
		printf("Error: cant write plaint text key blob to file. Error code: %x\n", GetLastError());
		free(Blob);
		CryptDestroyKey(SessionKey);
		CryptReleaseContext(Provider, 0);
		return FALSE;
	}
	fwrite(Blob, BlobLength, 1, File);
	fclose(File);
	printf("Plaint text key blob has been writed to file.\n");

	free(Blob);

	File = fopen("KeyX Public.bin", "rb");
	if (File == NULL)
	{
		printf("Error: cant open file \"KeyX Public.bin\". Error code: %x\n", GetLastError());
		CryptReleaseContext(Provider, 0);
		return FALSE;
	}

	fseek(File, 0, SEEK_END);
	BlobLength = ftell(File);
	rewind(File);
	Blob = malloc(BlobLength * sizeof(BYTE));
	fread(Blob, BlobLength, 1, File);
	fclose(File);
	printf("File \"KeyX Public.bin\" has been readed.\n");

	if (!CryptImportKey(Provider, Blob, BlobLength, 0, 0, &PublicKey))
	{
		printf("Error: cant import public key from blob. Error code: %x\n", GetLastError());
		free(Blob);
		CryptReleaseContext(Provider, 0);
		return FALSE;
	}
	printf("Public key has been imported from blob.\n");
	free(Blob);

	if (!CryptExportKey(SessionKey, PublicKey, SIMPLEBLOB, 0, NULL, &BlobLength))
	{
		printf("Error: cant get length of encrypted key. Error code: %x\n", GetLastError());
		CryptReleaseContext(Provider, 0);
		CryptDestroyKey(SessionKey);
		CryptDestroyKey(PublicKey);
		return FALSE;
	}
	printf("Length of encrypted key: %d.\n", BlobLength);
	Blob = malloc(BlobLength  * sizeof(BYTE));
	if (!CryptExportKey(SessionKey, PublicKey, SIMPLEBLOB, 0, Blob, &BlobLength))
	{
		printf("Error: cant export key to simple key blob. Error code: %x\n", GetLastError());
		CryptReleaseContext(Provider, 0);
		CryptDestroyKey(SessionKey);
		CryptDestroyKey(PublicKey);
		free(Blob);
		return FALSE;
	}
	printf("Session key has been exported to simple key blob.\n");

	File = fopen("Session Key Encrypted.bin", "w");
	if (File == NULL)
	{
		printf("Error: cant open file \"Session Key Encrypted.bin\". Error code: %x\n", GetLastError());
		CryptDestroyKey(SessionKey);
		CryptDestroyKey(PublicKey);
		CryptReleaseContext(Provider, 0);
		free(Blob);
		return FALSE;
	}
	fwrite(Blob, BlobLength, 1, File);
	fclose(File);
	printf("Simple key blob has been writed to file \"Session Key Encrypted.bin\".\n");
	free(Blob);
	CryptDestroyKey(SessionKey);
	CryptDestroyKey(PublicKey);
	CryptReleaseContext(Provider, 0);
	return TRUE;
}

BOOL Task5(HCRYPTPROV Container)
{
	HCRYPTKEY Key;
	DWORD BlobLength;
	BYTE* Blob;
	FILE* File;

	printf("Task 5:\n");

	if (!CryptGenKey(Container, AT_SIGNATURE, CRYPT_EXPORTABLE, &Key))
	{
		printf("Error: cant create AT_SIGNATURE key. Error code: %x\n", GetLastError());
		return FALSE;
	}
	printf("AT_SIGNATURE key has been craeted.\n");


	if (!CryptExportKey(Key, 0, PUBLICKEYBLOB, 0, NULL, &BlobLength))
	{
		printf("Error: cant get needed length of public key blob. Error code: %x\n", GetLastError());
		CryptDestroyKey(Key);
		return FALSE;
	}
	printf("Needed length of public key blob: %d.\n", BlobLength);
	Blob = malloc(BlobLength * sizeof(BYTE));

	if (!CryptExportKey(Key, 0, PUBLICKEYBLOB, 0, Blob, &BlobLength))
	{
		printf("Error: cant export key. Error code: %x\n", GetLastError());
		free(Blob);
		CryptDestroyKey(Key);
		return FALSE;
	}
	printf("Key has been exported to public key blob.\n");

	File = fopen("DS Public.bin", "w");
	if (File == NULL)
	{
		printf("Error: cant write public key blob to file. Error code: %x\n", GetLastError());
		fclose(File);
		free(Blob);
		CryptDestroyKey(Key);
		return FALSE;
	}
	printf("Public key blob has been writed to file \"DS Public.bin\".\n");

	fwrite(Blob, BlobLength, 1, File);

	fclose(File);
	free(Blob);
	CryptDestroyKey(Key);
	return TRUE;
}

BOOL Task34(HCRYPTPROV Container)
{
	HCRYPTKEY Key;
	DWORD BlobLength;
	BYTE* Blob;
	FILE* File;

	printf("Task 3:\n");

	if (!CryptGenKey(Container, AT_KEYEXCHANGE, CRYPT_EXPORTABLE, &Key))
	{
		printf("Error: cant create AT_KEYEXCHANGE key. Error code: %x\n", GetLastError());
		return FALSE;
	}
	printf("AT_KEYEXCHANGE key has been craeted.\n");

	printf("Task 4:\n");
	if (!CryptExportKey(Key, 0, PUBLICKEYBLOB, 0, NULL, &BlobLength))
	{
		printf("Error: cant get needed length of public key blob. Error code: %x\n", GetLastError());
		CryptDestroyKey(Key);
		return FALSE;
	}
	printf("Needed length of public key blob: %d.\n", BlobLength);
	Blob = malloc(BlobLength * sizeof(BYTE));

	if (!CryptExportKey(Key, 0, PUBLICKEYBLOB, 0, Blob, &BlobLength))
	{
		printf("Error: cant export key. Error code: %x\n", GetLastError());
		free(Blob);
		CryptDestroyKey(Key);
		return FALSE;
	}
	printf("Key has been exported to public key blob.\n");

	File = fopen("KeyX Public.bin", "w");
	if (File == NULL)
	{
		printf("Error: cant write public key blob to file. Error code: %x\n", GetLastError());
		fclose(File);
		free(Blob);
		CryptDestroyKey(Key);
		return FALSE;
	}
	printf("Public key blob has been writed to file \"KeyX Public.bin\".\n");
	fwrite(Blob, BlobLength, 1, File);

	fclose(File);
	free(Blob);
	CryptDestroyKey(Key);
	return TRUE;
}

void CreateContainers(HCRYPTPROV* Container1, HCRYPTPROV* Container2, HCRYPTPROV* Container3)
{
	printf("Task 2:\n");
	*Container1 = CreateContainer("Container1");
	if (!Container1)
	{
		printf("Halting.\n");
		exit(1);
	}
	*Container2 = CreateContainer("Container2");
	if (!Container2)
	{
		DestroyContainer("Container1");
		printf("Halting.\n");
		exit(1);
	}
	*Container3 = CreateContainer("Container3");
	if (!Container3)
	{
		DestroyContainer("Container1");
		DestroyContainer("Container2");
		printf("Halting.\n");
		exit(1);
	}
}

HCRYPTPROV CreateContainer(LPCTSTR ContainerName)
{
	HCRYPTPROV Provider;
	if (CryptAcquireContext(&Provider, ContainerName, PROVIDER_NAME, PROVIDER_TYPE, CRYPT_NEWKEYSET))
	{
		printf("Container with name \"%s\" has been created.\n", ContainerName);
		return Provider;
	}
	printf("Error: cant create container with name \"%s\". Error code: %x\n", ContainerName, GetLastError());
	return 0;
}

void DestroyContainer(LPCTSTR ContainerName)
{
	HCRYPTPROV Provider;
	if (CryptAcquireContext(&Provider, ContainerName, PROVIDER_NAME, PROVIDER_TYPE, CRYPT_DELETE_KEYSET))
	{
		printf("Container with name \"%s\" has been destroyed.\n", ContainerName);
	}
	else
	{
		printf("Error: cant destroy container with name \"%s\". Error code: %x\n", ContainerName, GetLastError());
	}
}

void DestroyContainers()
{
	printf("Cleaning:\n");
	DestroyContainer("Container1");
	DestroyContainer("Container2");
	DestroyContainer("Container3");
}
