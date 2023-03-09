#include <stdio.h>
#include <windows.h>

int main()
{
    DWORD bufferLength = 0;

    GetUserName(NULL, &bufferLength);

    TCHAR* nameBuffer = malloc(bufferLength * sizeof(TCHAR));

    GetUserName(nameBuffer, &bufferLength);

    printf("Username: %s (length: %d)", nameBuffer, bufferLength);

    free(nameBuffer);

    return 0;
}