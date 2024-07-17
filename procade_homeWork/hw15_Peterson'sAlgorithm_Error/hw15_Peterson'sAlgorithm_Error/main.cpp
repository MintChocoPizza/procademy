

#include <process.h>
#include <Windows.h>
#include <stdio.h>

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Synchronization.lib")


////////////////////////////////////////////////////////////
// g_Flag 값은 해당 스레드에서만 변경한다. 
// 그래서 굳이 Interlock을 사용하지 않았다. 
// 
// 하지만!!!! 
// 
// g_Turn 값은 2개의 스레드에서 변경한다. 
// 그래서 정확한 이전 값을 저장하고 싶다면 
// Interlock을 사용해야 한다고 생각했다.
////////////////////////////////////////////////////////////
#define CopyDebug 1

////////////////////////////////////////////////////////////
// 처음에는 Interlock을 사용하였지만, 
// 어짜피 다른 스레드에서 값을 변경하게되면,
// 동기화가 제대로 된것이 아니기 때문에 
// 굳이 Interlock를 사용할 필요가 없다고 생각한다.
////////////////////////////////////////////////////////////
#define debug 1

constexpr int MAX_THREAD = 2;

LONG g_Interlock_Flag;

//------------------------------------------------------------------
// PetersonLock_1 에서 사용된 값들을 저장하고 있다.
LONG g_Prev_Used_Lock_1_Flag;
LONG g_Prev_Used_Lock_1_Turn;

//------------------------------------------------------------------
// PetersonLock_0 에서 사용된 값들을 저장하고 있다.
LONG g_Prev_Used_Lock_0_Flag;
LONG g_Prev_Used_Lock_0_Turn;

LONG g_Data;
LONG g_Flag[2];
LONG g_Turn;

unsigned __stdcall Thread0(void* pParam);
unsigned __stdcall Thread1(void* pParam);

void PetersonLock_0(void);
void PetersonLock_1(void);
void PetersonUnLock_0(void);
void PetersonUnLock_1(void);

int wmain()
{
	timeBeginPeriod(1);

	HANDLE hThread[MAX_THREAD];
	unsigned int iThreadID[MAX_THREAD];
	DWORD dwStatus;

	g_Data = 0;

	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, Thread0, NULL, NULL, &iThreadID[0]);
	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, Thread1, NULL, NULL, &iThreadID[1]);

	dwStatus = WaitForMultipleObjects(MAX_THREAD, hThread, TRUE, INFINITE);
	
	if (dwStatus == WAIT_OBJECT_0)
	{
		printf("ALL Thread normal shutdown !!!! \n");
	}
	else
	{
		printf("WaitForMultipleObject %d error \n", GetLastError());
	}

	printf("%d \n", g_Data);

	timeEndPeriod(1);
	return 0;
}

unsigned __stdcall Thread0(void* pParam)
{
	int iCnt;

	for (iCnt = 0; iCnt < 100000000; ++iCnt)
	{
		PetersonLock_0();
#if debug == 0
		InterlockedExchange(&g_Interlock_Flag, 0);
		//g_Interlock_Flag = 0;
#endif

		g_Data++;

#if debug == 0
		if (InterlockedExchange(&g_Interlock_Flag, 0) != 0)
		//if(g_Interlock_Flag != 0)
		{
			printf("\
					Current Thread : 0 \n\n \
					Thread_0: g_Flag[0]: % d / g_Turn: % d \n \
					g_Prev_Used_Lock_0_Flag = %d \n \
					g_Prev_Used_Lock_0_Turn = %d \n\n \
					Thread_1: g_Flag[1]: %d / g_Turn: %d \n \
					g_Prev_Used_Lock_1_Flag = %d \n \
					g_Prev_Used_Lock_1_Turn = %d \n",
					g_Flag[0], g_Turn,
					g_Prev_Used_Lock_0_Flag,
					g_Prev_Used_Lock_0_Turn,
					g_Flag[1], g_Turn,
					g_Prev_Used_Lock_1_Flag,
					g_Prev_Used_Lock_1_Turn
			);
			__debugbreak();
		}
#endif

		PetersonUnLock_0();
	}

	return 0;
}

unsigned __stdcall Thread1(void* pParam)
{
	int iCnt;

	for (iCnt = 0; iCnt < 100000000; ++iCnt)
	{
		PetersonLock_1();

#if debug == 0
		InterlockedExchange(&g_Interlock_Flag, 1);
		//g_Interlock_Flag = 1;
#endif

		g_Data++;

#if debug == 0
		if (InterlockedExchange(&g_Interlock_Flag, 1) != 1)
		//if(g_Interlock_Flag != 1)
		{
			printf("\
					Current Thread : 1 \n \
					Thread_0: g_Flag[0]: % d / g_Turn: % d \n \
					g_Prev_Used_Lock_0_Flag = %d \n \
					g_Prev_Used_Lock_0_Turn = %d \n\n \
					Thread_1: g_Flag[1]: %d / g_Turn: %d \n \
					g_Prev_Used_Lock_1_Flag = %d \n \
					g_Prev_Used_Lock_1_Turn = %d \n",
					g_Flag[0], g_Turn,
					g_Prev_Used_Lock_0_Flag,
					g_Prev_Used_Lock_0_Turn,
					g_Flag[1], g_Turn,
					g_Prev_Used_Lock_1_Flag,
					g_Prev_Used_Lock_1_Turn
			);
			__debugbreak();
		}
#endif

		PetersonUnLock_1();
	}

	return 0;
}

void PetersonLock_0(void)
{

#if CopyDebug == 0
	//------------------------------------------------------------------
	// PetersonLock_1 에서 사용된 값들을 저장하고 있다.
	//g_Prev_Used_Lock_1_Flag = InterlockedExchange(&g_Flag[0], false);
	//g_Prev_Used_Lock_1_Turn = InterlockedExchange(&g_Turn, 1);
	g_Prev_Used_Lock_1_Flag = g_Flag[0];
	g_Prev_Used_Lock_1_Turn = g_Turn;
#endif

	g_Flag[0] = true;
	g_Turn = 0;

	//while (g_Flag[1] == true && g_Turn == 0)
	while(g_Turn == 0 && g_Flag[1] == true)
	//while(1)
	{
		//if (g_Flag[1] == false)
		//	break;
		//if (g_Turn != 0)
		//	break;

		// g_Flag[1] == false || g_Trun == 1
		// 이면 루프 탈출
	}
}

void PetersonLock_1(void)
{

#if CopyDebug == 0
	//------------------------------------------------------------------
	// PetersonLock_1 에서 사용된 값들을 저장하고 있다.
	//g_Prev_Used_Lock_0_Flag = InterlockedExchange(&g_Flag[1], false);
	g_Prev_Used_Lock_0_Turn = InterlockedExchange(&g_Turn, 0);
	g_Prev_Used_Lock_0_Flag = g_Flag[1];
	//g_Prev_Used_Lock_0_Turn = g_Turn;
#endif

	g_Flag[1] = true;
	g_Turn = 1;

	//while (g_Flag[0] == true && g_Turn == 1)
	while(g_Turn == 1 && g_Flag[0] == true)
	//while(1)
	{
		//if (g_Flag[0] == false)
		//	break;
		//if (g_Turn != 1)
		//	break;

		// g_Flag[0] == false || g_Trun == 0
		// 이면 루프 탈출
	}
}

void PetersonUnLock_0(void)
{
	g_Flag[0] = false;
}

void PetersonUnLock_1(void)
{
	g_Flag[1] = false;
}

