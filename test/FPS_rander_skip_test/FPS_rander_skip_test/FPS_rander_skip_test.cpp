#include <stdio.h>
#include <Windows.h>
#include <time.h>

#pragma comment(lib, "winmm.lib")

DWORD v_Time;
DWORD c_Time;
DWORD s_Time;

int logic = 0;

int main()
{
	timeBeginPeriod(1);

	v_Time = timeGetTime();
	while (1)
	{

		logic++;
		Sleep(20);

		if (timeGetTime() - v_Time >= 1000)  // 1000 이후라면
		{
			v_Time += 1000;
			printf("%d \n",logic);
			logic = 0;

		}
	}
	timeEndPeriod(1);
	return 0;
}