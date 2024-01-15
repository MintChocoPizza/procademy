// #define UNICODE
// #define _UNICODE
#undef UNICODE
#undef _UNICODE

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>


int wmain(void)
{
	TCHAR str[] = _T("1234567");
	int size = sizeof(str);
	printf("string length : %d \n", size);
	return 0;
}
