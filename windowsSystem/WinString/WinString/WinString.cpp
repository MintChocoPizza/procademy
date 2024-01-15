#include <stdio.h>
#include <Windows.h>

int wmain(int argc, wchar_t* argv[])
{
	LPSTR str1 = "SBCS Style String 1";
	LPCWSTR str2 = L"WBCS Style String 1";

	char *temp = "hello world";
	char tmp[] = "hello world";

	CHAR arr1[] = "SBSC Style String 2";
	WCHAR arr2[] = L"WBCS Style String 2";

	LPCSTR cStr1 = arr1;
	LPCWSTR cStr2 = arr2;

	printf("%s\n", str1);
	printf("%s\n", arr1);

	wprintf(L"%s\n", str2);
	wprintf(L"%s\n", arr2);

	return 0;
}