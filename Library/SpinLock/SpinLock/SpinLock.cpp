
#include <Windows.h>
#include "SpinLock.h"


void SpinLock(void* flag)
{
	while (1)
	{
		if (InterlockedExchange((LONG *)flag, 1) == 0 )
		{
			break;
		}
	}
}

void SpinUnlock(void* flag)
{
	*(LONG*)flag = 0;
}
