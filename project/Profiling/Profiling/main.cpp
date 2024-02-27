#include <Windows.h>

#include "Profiling.h"


int main()
{

	Profile p1(_T("ABC"));

	Sleep(1000);

	ProfileDataOutText(_T("hello.txt"));

	return 0;
}