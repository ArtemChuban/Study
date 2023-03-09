#include <stdio.h>
#include <windows.h>
#include <wincrypt.h>

int main()
{
    DWORD ProvType;
    DWORD ProvNameMaxLength = 50 * sizeof(TCHAR);
    DWORD ProvNameLength = ProvNameMaxLength;
    LPTSTR ProvName = malloc(ProvNameLength);

    DWORD error;
    
    printf("Index\tType\tName\n");
    for (DWORD i = 0; error != ERROR_NO_MORE_ITEMS; i++)
    {
        if (!CryptEnumProviders(i, NULL, 0, &ProvType, ProvName, &ProvNameLength))
        {
            error = GetLastError();
            if (error == ERROR_MORE_DATA)
            {
                free(ProvName);
                ProvName = malloc(ProvNameLength);
                ProvNameMaxLength = ProvNameLength;
                continue;
            }
            return error;
        }
        printf("%5d\t%4d\t%s\n", i, ProvType, ProvName);
        ProvNameLength = ProvNameMaxLength;
    }

    free(ProvName);

    return 0;
}