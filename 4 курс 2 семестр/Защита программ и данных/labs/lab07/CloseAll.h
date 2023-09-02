#ifndef CLOSE_ALL_H
#define CLOSE_ALL_H

#include <stdio.h>
#include <windows.h>
#include <wincrypt.h>

void CloseAll(HANDLE SourceFile,
			  HANDLE DestinationFile,
			  BYTE* Buffer,
			  HCRYPTHASH Hash,
			  HCRYPTKEY Key,
			  HCRYPTPROV CryptProv)
{
	if(SourceFile)
	{
		CloseHandle(SourceFile);
	}

	if(DestinationFile)
	{
		CloseHandle(DestinationFile);
	}

	if(Buffer)
	{
		free(Buffer);
	}

	if(Hash)
	{
		if(!(CryptDestroyHash(Hash)))
		{
			printf("Error during CryptDestroyHash.\n", GetLastError());
		}
		Hash = 0;
	}

	if(Key)
	{
		if(!(CryptDestroyKey(Key)))
		{
			printf("Error during CryptDestroyKey!\n", GetLastError());
		}
	}

	if(CryptProv)
	{
		if(!(CryptReleaseContext(CryptProv, 0)))
		{
			printf("Error during CryptReleaseContext!\n", GetLastError());
		}
	}
}

#endif