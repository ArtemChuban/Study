#include <stdio.h>
#include <windows.h>
#include <wincrypt.h>


void ErrorHandle(LPCTSTR description)
{
	printf("%s", description);
	exit(1);
}

void CreateKey(HCRYPTPROV Provider, DWORD KeySpec)
{
	HCRYPTKEY Key;

	if(!CryptGetUserKey(Provider, KeySpec, &Key))
	{
		if(GetLastError() == NTE_NO_KEY)
		{
			if(!CryptGenKey(Provider, KeySpec, 0, &Key))
			{
				printf("Error occurred creating a signature key.\n");
				return;
			}
		}
		else
		{
			printf("An error other than NTE_NO_KEY getting a key.\n");
			return;
		}
	}

	if(Key)
	{
		if(!(CryptDestroyKey(Key)))
		{
			printf("Error during CryptDestroyKey.");
		}
	}
}

void CreateContainer(LPCTSTR ContainerName, BOOL Signature, BOOL KeyExchange)
{
	HCRYPTPROV Provider;

	if(!CryptAcquireContext(&Provider, ContainerName, NULL, PROV_RSA_FULL, 0))
	{
		if(GetLastError() == NTE_BAD_KEYSET)
		{
			if(!CryptAcquireContext(&Provider, ContainerName, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET))
			{
				printf("Could not create a new key container with name %s.\n", ContainerName);
				return;
			}
		}
		else
		{
			printf("CryptAcquireContext failed while creating container with name %s.\n", ContainerName);
			return;
		}
	}

	if (Signature)
	{
		CreateKey(Provider, AT_SIGNATURE);
	}
	if (KeyExchange)
	{
		CreateKey(Provider, AT_KEYEXCHANGE);
	}

	if(Provider)
	{
		if(!(CryptReleaseContext(Provider, 0)))
		{
			printf("Error during CryptReleaseContext while creating container with name %s.\n", ContainerName);
		}
	}
}

void DestroyContainer(LPCTSTR ContainerName)
{
	HCRYPTPROV Provider;
	if(!CryptAcquireContext(&Provider, ContainerName, NULL, PROV_RSA_FULL, 0))
	{
		if(GetLastError() == NTE_BAD_KEYSET)
		{
			printf("Container with name %s does not exist.\n");
		}
		else
		{
			printf("CryptAcquireContext failed while destroying container with name %s.\n", ContainerName);
		}
		return;
	}
	CryptAcquireContext(&Provider, ContainerName, NULL, PROV_RSA_FULL, CRYPT_DELETE_KEYSET);
}

void Containers()
{
	HCRYPTPROV Provider;
	CryptAcquireContext(&Provider, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);
	DWORD Flags = CRYPT_FIRST;
	DWORD DataLength;

	CryptGetProvParam(Provider, PP_ENUMCONTAINERS, NULL, &DataLength, Flags);
	const DWORD BufferSize = DataLength;
	BYTE* Data = malloc(BufferSize);

	printf("%30s%15s%15s\n", "Container name", "Signature key", "Exchange key");

	while (TRUE)
	{
		DataLength = BufferSize;
		if (CryptGetProvParam(Provider, PP_ENUMCONTAINERS, Data, &DataLength, Flags))
		{
			LPCTSTR ContainerName = (LPCTSTR)Data;
			HCRYPTPROV ProviderWithContainer;

			printf("%30s", ContainerName);

			CryptAcquireContext(&ProviderWithContainer, ContainerName, NULL, PROV_RSA_FULL, 0);

			HCRYPTKEY Key;
			if (CryptGetUserKey(ProviderWithContainer, AT_SIGNATURE, &Key))
			{
				printf("%15s", "+");
			}
			else
			{
				printf("%15s", "-");
			}

			if (CryptGetUserKey(ProviderWithContainer, AT_KEYEXCHANGE, &Key))
			{
				printf("%15s", "+");
			}
			else
			{
				printf("%15s", "-");
			}
			printf("\n");

			CryptReleaseContext(ProviderWithContainer, 0);

			DataLength = BufferSize;
			Flags = CRYPT_NEXT;
		}
		else
		{
			DWORD error = GetLastError();
			if (error == ERROR_NO_MORE_ITEMS)
			{
				break;
			}
		}
	}
}

int main()
{
	CreateContainer("Empty container", FALSE, FALSE);
	CreateContainer("Only exchange container", FALSE, TRUE);
	CreateContainer("Only signature container", TRUE, FALSE);
	CreateContainer("Both container", TRUE, TRUE);

	Containers();

	DestroyContainer("Empty container");
	DestroyContainer("Only exchange container");
	DestroyContainer("Only signature container");
	DestroyContainer("Both container");

	return 0;
}
