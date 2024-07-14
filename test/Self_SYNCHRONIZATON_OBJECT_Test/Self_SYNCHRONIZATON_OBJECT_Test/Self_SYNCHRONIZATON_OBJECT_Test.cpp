// Self_SYNCHRONIZATON_OBJECT_Test.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <process.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <iostream>

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Synchronization.lib")



#define MY_SYNCHRONIZATON_OBJECT	0
#define CRITICALSECTION				1
#define SRW_LOCK					2
#define SPIN_LOCK					3

#define SYNCHRONIZATON_OBJECT		0

constexpr int MAX_NUM = 10;

LONG g_Flag;
CRITICAL_SECTION g_CS;
SRWLOCK g_srwlock;
LONG g_SpinLockFlag;

int g_Var;

unsigned _stdcall Thread(void* pArg);


void Initialize(LONG* flag);
void Lock(LONG* flag);
void UnLock(LONG* flag);

void SpinLockInitialize(LONG* flag);
void SpinLock(LONG* flag);
void SpinUnlock(LONG* flag);

int main()
{
	timeBeginPeriod(1);

	HANDLE hThread[MAX_NUM];
	unsigned int threadID[MAX_NUM];
	DWORD dwStatus;
	int iCnt;

#if SYNCHRONIZATON_OBJECT == MY_SYNCHRONIZATON_OBJECT
	{
		Initialize(&g_Flag);
	}
#elif SYNCHRONIZATON_OBJECT == CRITICALSECTION
	{
		InitializeCriticalSection(&g_CS);
	}
#elif SYNCHRONIZATON_OBJECT == SRW_LOCK
	{
		InitializeSRWLock(&g_srwlock);
	}
#elif SYNCHRONIZATON_OBJECT == SPIN_LOCK
	{
		SpinLockInitialize(&g_SpinLockFlag);
	}
#endif
	g_Var = 0;

	for (iCnt = 0; iCnt < MAX_NUM; iCnt++)
	{
		hThread[iCnt] = (HANDLE)_beginthreadex(NULL, 0, Thread, NULL, NULL, &threadID[iCnt]);
	}

	dwStatus = WaitForMultipleObjects(
		MAX_NUM,				// 커널 객체 배열의 길이
		hThread,		// 커널 객체의 핸들로 구성된 배열의 포인터
		TRUE,			// TRUE: 모든 커널 객체 시그널, FALSE: 커널 객체 중 하나라도 시그널
		INFINITE		// 언제까지 block 될것인가
	);

	if (dwStatus == WAIT_OBJECT_0)
	{
		printf("ALL Thread normal shutdown !!!! \n");
	}
	else
	{
		printf("WaitForMultipleObject %d error \n", GetLastError());
	}

	printf("%d \n", g_Var);

    std::cout << "Hello World!\n";
	timeEndPeriod(1);

}

unsigned _stdcall Thread(void* pArg)
{
	int iCnt;
	for (iCnt = 0; iCnt < 10000; ++iCnt)
	{
#if SYNCHRONIZATON_OBJECT == MY_SYNCHRONIZATON_OBJECT
		Lock(&g_Flag);
#elif SYNCHRONIZATON_OBJECT == CRITICALSECTION
		EnterCriticalSection(&g_CS);
#elif SYNCHRONIZATON_OBJECT == SRW_LOCK
		AcquireSRWLockExclusive(&g_srwlock);
#elif SYNCHRONIZATON_OBJECT == SPIN_LOCK
		SpinLock(&g_SpinLockFlag);
#endif
		++g_Var;
#if SYNCHRONIZATON_OBJECT == MY_SYNCHRONIZATON_OBJECT
		UnLock(&g_Flag);
#elif SYNCHRONIZATON_OBJECT == CRITICALSECTION
		LeaveCriticalSection(&g_CS);
#elif SYNCHRONIZATON_OBJECT == SRW_LOCK
		ReleaseSRWLockExclusive(&g_srwlock);
#elif SYNCHRONIZATON_OBJECT == SPIN_LOCK
		SpinUnlock(&g_SpinLockFlag);
#endif
	}

	return 0;
}

void Initialize(LONG* flag)
{
	*flag = 0;
}

void Lock(LONG* flag)
{
	LONG compare = 1;
	while (InterlockedExchange(flag, 1) == 1)
	{
		WaitOnAddress(flag, &compare, sizeof(LONG), INFINITE);
	}
}

void UnLock(LONG* flag)
{
	WakeByAddressSingle(flag);
	InterlockedExchange(flag, 0);
}


void SpinLockInitialize(LONG* flag)
{
	*flag = 0;
}

void SpinLock(LONG* flag)
{
	while (InterlockedExchange(flag, 1) == 1)
	{
		YieldProcessor();
	}
}

void SpinUnlock(LONG* flag)
{
	//*(LONG*)flag = 0;
	InterlockedExchange(flag, 0);
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
