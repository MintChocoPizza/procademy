#include <stdio.h>
#include <Windows.h>
#include <time.h>

#pragma comment(lib, "winmm.lib")

DWORD v_Time;
DWORD c_Time;
DWORD s_Time;

int logic = 0;
int rander = 0;
int load = 0;

// version_1
//int main()
//{
//	timeBeginPeriod(1);
//	srand(clock());
//	v_Time = GetTickCount();
//	s_Time = v_Time;
//	while (1)
//	{
//
//		logic++;
//
//		c_Time = GetTickCount();
//
//		Sleep(20);
//
//		if (c_Time - s_Time >= 1000)
//		{
//			printf_s("로직: %d \n", logic);
//			s_Time += 1000;
//			logic = 0;
//		}
//	}
//	timeEndPeriod(1);
//	return 0;
//}

// version_2
//int main()
//{
//	timeBeginPeriod(1);
//	srand(clock());
//	v_Time = GetTickCount();
//	s_Time = v_Time;
//	while (1)
//	{
//
//		logic++;
//
//		c_Time = GetTickCount();
//
//		v_Time += 20;
//		Sleep(v_Time - c_Time);
//
//		if (c_Time - s_Time >= 1000)
//		{
//			printf_s("로직: %d \n", logic);
//			s_Time += 1000;
//			logic = 0;
//		}
//	}
//	timeEndPeriod(1);
//	return 0;
//}


// version_3 => 이건 음수가 나옴
//int main()
//{
//	timeBeginPeriod(1);
//	v_Time = GetTickCount();
//	s_Time = v_Time;
//	while (1)
//	{
//
//		logic++;
//
//		c_Time = GetTickCount();
//
//		printf("    %d \n", c_Time - v_Time);
//		if(1)
//		{
//			v_Time += 20;
//			Sleep(v_Time - c_Time);
//		}
//
//		if (c_Time - s_Time >= 1000)
//		{
//			printf_s("로직: %d \n", logic);
//			s_Time += 1000;
//			logic = 0;
//		}
//	}
//	timeEndPeriod(1);
//	return 0;
//}

// version_4  -> 음수가 안나옴
//int main()
//{
//	timeBeginPeriod(1);
//	v_Time = timeGetTime();
//	s_Time = v_Time;
//	while (1)
//	{
//
//		logic++;
//
//		c_Time = timeGetTime();
//
//		printf("    %d \n", c_Time - v_Time);
//		if(1)
//		{
//			v_Time += 20;
//			Sleep(v_Time - c_Time);
//		}
//
//		if (c_Time - s_Time >= 1000)
//		{
//			printf_s("로직: %d \n", logic);
//			s_Time += 1000;
//			logic = 0;
//		}
//	}
//	timeEndPeriod(1);
//	return 0;
//}

// version_5  -> 다행히 돌아감
//int main()
//{
//	timeBeginPeriod(1);
//	v_Time = timeGetTime();
//	s_Time = v_Time;
//	while (1)
//	{
//
//		logic++;
//
//		c_Time = timeGetTime();
//
//		if (c_Time - v_Time < 20)
//		{
//			v_Time += 20;
//			Sleep(v_Time - c_Time);
//		}
//
//		if (c_Time - s_Time >= 1000)
//		{
//			printf_s("로직: %d \n", logic);
//			s_Time += 1000;
//			logic = 0;
//		}
//	}
//	timeEndPeriod(1);
//	return 0;
//}


// version_6 -> 가끔 음수가 튀어나옴
//LARGE_INTEGER timer;
//LARGE_INTEGER l_v_Time;
//LARGE_INTEGER l_s_Time;
//LARGE_INTEGER l_c_Time;
//
//int main()
//{
//	timeBeginPeriod(1);
//	QueryPerformanceFrequency(&timer);
//	// v_Time = timeGetTime();
//	QueryPerformanceCounter(&l_v_Time);
//	// s_Time = v_Time;
//	l_s_Time = l_v_Time;
//	while (1)
//	{
//
//		logic++;
//
//		// c_Time = timeGetTime();
//		QueryPerformanceCounter(&l_c_Time);
//
//		// printf("    %d \n", c_Time - v_Time);
//		printf("    %f \n", (((double)l_c_Time.QuadPart - l_v_Time.QuadPart) / (double)timer.QuadPart) * 1000);
//		if(1)
//		{
//			// v_Time += 20;
//			l_v_Time.QuadPart += 20000;
//			// Sleep(v_Time - c_Time);
//			Sleep(    (unsigned long)(((double)(l_v_Time.QuadPart-l_c_Time.QuadPart) / (double)timer.QuadPart) * 1000)         );
//		}
//
//		// if (c_Time - s_Time >= 1000)
//		if(            ((double)(l_v_Time.QuadPart-l_s_Time.QuadPart) / (double)timer.QuadPart)*1000 >= 1000         )
//		{
//			printf_s("로직: %d \n", logic);
//			// s_Time += 1000;
//			l_s_Time.QuadPart += timer.QuadPart;
//			logic = 0;
//		}
//	}
//	timeEndPeriod(1);
//	return 0;
//}



void KeyBoardInput(void);
void Logic(void);
void Rander(void);
bool Skip(void);

int main()
{
	timeBeginPeriod(1);
	srand(timeGetTime());



	v_Time = timeGetTime();
	s_Time = v_Time;


	while (1)
	{
		KeyBoardInput();

		Logic();


		if (Skip())
			Rander();

	}
	timeEndPeriod(1);
	return 0;
}

void Logic(void)
{
	Sleep(rand() % 10 + load);
	logic++;
}
void Rander(void)
{
	Sleep(rand() % 15 + load);
	rander++;
}
bool Skip(void)
{
	int ret = true;
	c_Time = timeGetTime();

	// 로직이 빨리 끝나서 프레임을 맞추려고 기다리는 상황
	if (c_Time - v_Time < 20)
	{

		v_Time += 20;
		Sleep(v_Time - c_Time);
		ret = true;
	}
	// 랜더 + 로직이 1프레임(20ms)를 완전히 넘어가는 상황
	// else if (c_Time - v_Time > 20)
	// = 을 뺀 경우 로직의 실행이 더 많이 하는 경우가 발생한다.
	else if (c_Time - v_Time >= 20)
	{

		v_Time += 20;
		ret = false;
	}




	// 1초 후 출력
	if (c_Time - s_Time >= 1000)
	{
		printf_s("로직: %d  -  랜더: %d \n", logic, rander);
		s_Time += 1000;
		logic = 0;
		rander = 0;
	}

	return ret;
}

void KeyBoardInput(void)
{
	if (GetAsyncKeyState(VK_SPACE) & 0x8001)
		load++;

	if (GetAsyncKeyState('A') & 0x8001)
		load--;
	if (GetAsyncKeyState('S') & 0x8001)
		Sleep(10000);
}