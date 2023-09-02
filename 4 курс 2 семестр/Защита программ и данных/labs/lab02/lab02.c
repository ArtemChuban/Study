#include <stdio.h>
#include <windows.h>
#include <wincrypt.h>

void ImplementationType(HCRYPTPROV Provider)
{
	DWORD DataLength = sizeof(BYTE) * 4;
	BYTE* Data = malloc(DataLength);
	printf("Implementation: ");
	if (CryptGetProvParam(Provider, PP_IMPTYPE, Data, &DataLength, 0))
	{
		switch(*Data)
		{
		case CRYPT_IMPL_HARDWARE:
			printf("Hardware\n");
			break;
		case CRYPT_IMPL_SOFTWARE:
			printf("Software\n");
			break;
		case CRYPT_IMPL_MIXED:
			printf("Mixed\n");
			break;
		case CRYPT_IMPL_REMOVABLE:
			printf("Removable\n");
			break;
		case CRYPT_IMPL_UNKNOWN:
			printf("Unknown\n");
			break;
		default:
			printf("Wrong implementation type\n");
		}
	}
	else
	{
		printf("Cant define implementation type\n");
	}
	free(Data);
}

void Version(HCRYPTPROV Provider)
{
	DWORD DataLength = sizeof(BYTE) * 4;
	BYTE* Data = malloc(DataLength);
	if (CryptGetProvParam(Provider, PP_VERSION, Data, &DataLength, 0))
	{
		BYTE minor = Data[0];
		BYTE major = Data[1];
		printf("Version: %d.%d\n", major, minor);
	}
	else
	{
		printf("Cant define version\n");
	}
	free(Data);
}

void Containers(HCRYPTPROV Provider)
{
	const DWORD BufferSize = sizeof(PROV_ENUMALGS_EX);
	PROV_ENUMALGS_EX* Data = malloc(BufferSize);
	DWORD Flags = CRYPT_FIRST;
	DWORD DataLength;
	
	printf("%5s %20s %20s %40s %14s %10s %10s\n", "ID", "Class", "Name", "Long name", "Default length", "Min length", "Max length");
	while (TRUE)
	{
		DataLength = BufferSize;
		if (CryptGetProvParam(Provider, PP_ENUMALGS_EX, (BYTE*)Data, &DataLength, Flags))
		{
			DWORD class = GET_ALG_CLASS(Data->aiAlgid);
			LPCTSTR className;
			switch (class)
			{
			case ALG_CLASS_DATA_ENCRYPT:
				className = "Data encryption";
				break;
			case ALG_CLASS_MSG_ENCRYPT:
				className = "Message encryption";
				break;
			case ALG_CLASS_HASH:
				className = "Hash";
				break;
			case ALG_CLASS_KEY_EXCHANGE:
				className = "Exchange";
				break;
			case ALG_CLASS_SIGNATURE:
				className = "Signature";
				break;
			default:
				className = "Other";
			}
			printf("%5d %20s %20s %40s %14d %10d %10d\n",
				Data->aiAlgid, className, Data->szName, Data->szLongName, Data->dwDefaultLen, Data->dwMinLen, Data->dwMaxLen);
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

	HCRYPTPROV Provider;
	BOOL result;
	LPCTSTR ProviderName = "Microsoft Base Cryptographic Provider v1.0";

	if (!CryptAcquireContext(&Provider, NULL, ProviderName, 1, CRYPT_VERIFYCONTEXT))
	{
		printf("Cant connect to provider");
	}

	// ImplementationType(Provider);
	// Version(Provider);
	Containers(Provider);

	CryptReleaseContext(Provider, 0);

	return 0;
}
