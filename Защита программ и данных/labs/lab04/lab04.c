#include <stdio.h>
#include <windows.h>
#include <wincrypt.h>

void Containers(HCRYPTPROV Provider)
{
	DWORD Flags = CRYPT_FIRST;
	DWORD DataLength;
	
	CryptGetProvParam(Provider, PP_ENUMCONTAINERS, NULL, &DataLength, Flags);
	const DWORD BufferSize = DataLength;
	BYTE* Data = malloc(BufferSize);
	
	while (TRUE)
	{
		DataLength = BufferSize;
		if (CryptGetProvParam(Provider, PP_ENUMCONTAINERS, Data, &DataLength, Flags))
		{
			printf("%s\n", (LPCTSTR*)Data);
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

void ErrorHandle(LPCTSTR description)
{
	printf("%s.\n", description);
	exit(1);
}

int main()
{
	HCRYPTPROV Provider;
	LPCTSTR ContainerName = "Test Container";

	if (CryptAcquireContext(&Provider, ContainerName, NULL, PROV_RSA_FULL, 0))
	{
		printf("A crypto context with the %s jey container has been acquired.\n");
	}
	else
	{

		if (GetLastError() == NTE_BAD_KEYSET)
		{
			if (CryptAcquireContext(&Provider, ContainerName, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET))
			{
				printf("A new key container has been created.\n");
			}
			else
			{
				
			}
		}
	}

	Containers(Provider);

	// CryptAcquireContext(&Provider, ContainerName, ProviderName, ProviderType, CRYPT_DELETE_KEYSET);
	CryptReleaseContext(Provider, 0);

	return 0;
}
