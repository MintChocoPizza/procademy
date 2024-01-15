#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

UINT CalDistance(UINT a, UINT b)
{
	return a - b;
}

int _tmain(void)
{
	INT val1 = 10;
	INT val2 = 20;

	_tprintf(_T("Position %u, %u \n"), (UINT)&val1, (UINT)&val2);
	_tprintf(_T("distance : %u \n"), CalDistance((UINT)&val1, (UINT)&val2));

	return 0;
}