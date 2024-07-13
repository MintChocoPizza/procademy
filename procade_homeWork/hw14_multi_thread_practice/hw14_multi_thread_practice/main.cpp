
#include <process.h>
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Synchronization.lib")


#define MY_SYNCHRONIZATON_OBJECT	0
#define CRITICALSECTION				1
#define SRW_LOCK					2
#define SYNCHRONIZATON_OBJECT		1

LONG g_Flag;
CRITICAL_SECTION g_CS;
SRWLOCK g_srwlock;

LONG g_Data = 0;
LONG g_Connect = 0;
bool g_Shutdown = false;
HANDLE g_hManualResetEvent = NULL;

unsigned _stdcall AccepThread(void* pArg);
unsigned _stdcall DisconnectThread(void* pArg);
unsigned _stdcall UpdateThread(void* pArg);
void Initialize(LONG* flag);
void Lock(LONG* flag);
void UnLock(LONG* flag);

int wmain(int argc, wchar_t *argv[])
{
	HANDLE hThread[5];
	unsigned int threadID[5];
	int iCnt;
	DWORD dwStartTime;
	DWORD dwEndTime;
	DWORD dwShutdownTime;
	DWORD dwStatus;
	BOOL bStatus;

	timeBeginPeriod(1);
	srand((unsigned)time(NULL));

	/////////////////////////////////////////////////////
	// 동기화 객체 초기화
	// 
	/////////////////////////////////////////////////////
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
#endif


	/////////////////////////////////////////////////////
	// 수동 리셋 이벤트를 생성한다. 
	// 
	/////////////////////////////////////////////////////
	g_hManualResetEvent = CreateEvent(
		NULL,				// NULL 이면 자식 프로세스에서 핸들을 상속할 수 없다.
		TRUE,				// TRUE: 수동 리셋 이벤트, FALSE: 자동 리셋 이벤트
		FALSE,				// 초기 시그널 상태를 지정한다. 
		NULL				// 이벤트 객체의 이름을 나타낸다. NULL: 이름 없는 이벤트 객체 생성
	);
	if (g_hManualResetEvent == NULL)
	{
		wprintf(L"CreateEvent ManualResetEvetn false \n");
		return -1;
	}

	/////////////////////////////////////////////////////
	// 스레드를 Suspend 상태로 생성한다. 
	// 
	/////////////////////////////////////////////////////
	hThread[0] = (HANDLE)_beginthreadex(
		NULL,				// 보안 속성 (NULL은 핸들을 상속할 수 없음)
		0,					// 스택 크기 (0은 기본 1mb)
		AccepThread,		// 실행할 함수
		NULL,				// 함수에 전달할 인수
		NULL,				// 생성 플래그 (CREATE_SUSPENDED: 정지상태로 생성)
		&threadID[0]		// 생성된 스레드ID 저장
	);
	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, DisconnectThread, NULL, NULL, &threadID[1]);
	for (iCnt = 2; iCnt < 5; ++iCnt)
	{
		hThread[iCnt] = (HANDLE)_beginthreadex(NULL, 0, UpdateThread, NULL, NULL, &threadID[iCnt]);
	}

	/////////////////////////////////////////////////////
	// 생성 스레드 에러 확인
	// 
	/////////////////////////////////////////////////////
	for (iCnt = 0; iCnt < 5; ++iCnt)
	{
		if (!hThread[iCnt])
		{
			wprintf(L"thread - error creating child threads \n");
			return -2;
		}
		else
		{
			wprintf(L"thread %d - id=0x%08x : Create!! \n", iCnt, threadID[iCnt]);
		}
	}

	/////////////////////////////////////////////////////
	// 수동 리셋 이벤트를 시그널 상태로 만들어 
	// 
	// 스레드를 시작한다.
	// 
	// 이때 시간도 초기화
	// 
	/////////////////////////////////////////////////////
	wprintf(L"signaling manual-reset event. \n");
	bStatus = SetEvent(g_hManualResetEvent);
	dwStartTime = timeGetTime();
	dwShutdownTime = dwStartTime;
	if (bStatus == false)
	{
		wprintf(L"SetEvent() false \n");
		return -3;
	}
	
	/////////////////////////////////////////////////////
	// main 함수 메인 로직
	// 
	/////////////////////////////////////////////////////
	while (1)
	{
		dwEndTime = timeGetTime();
		if (dwEndTime - dwStartTime >= 1000)
		{
			// 1초가 지난 경우 
			wprintf(L"main thread - g_Connect: %d \n", g_Connect);
			dwStartTime += 1000;
			
			// 20초가 지난 경우
			if (dwEndTime - dwShutdownTime >= 20000)
			{
				g_Shutdown = true;
				break;
			}
		}
		Sleep(1000 - (dwEndTime - dwStartTime));
	}

	/////////////////////////////////////////////////////
	// 모든 스레드 종료(시그널이 오기를) 기다린다.
	// 
	/////////////////////////////////////////////////////
	wprintf(L"Wait For All Thread!! \n");
	dwStatus = WaitForMultipleObjects(
		5,				// 커널 객체 배열의 길이
		hThread,		// 커널 객체의 핸들로 구성된 배열의 포인터
		TRUE,			// TRUE: 모든 커널 객체 시그널, FALSE: 커널 객체 중 하나라도 시그널
		INFINITE		// 언제까지 block 될것인가
	);

	if(dwStatus == WAIT_OBJECT_0)
	{
		wprintf(L"ALL Thread normal shutdown !!!! \n");
	}
	else
	{
		wprintf(L"WaitForMultipleObject %d error \n", GetLastError());
	}

	wprintf(L"Hello World \n");
	timeEndPeriod(1);
	return 0;
}

unsigned _stdcall AccepThread(void*)
{
	int iRand;

	srand((unsigned)time(NULL));

	wprintf(L"AccepThread id=0x%08x - WaitForSignal %d  \n", GetCurrentThreadId(), g_Connect);
	if (WaitForSingleObject(g_hManualResetEvent, INFINITE) == WAIT_OBJECT_0)
	{
		wprintf(L"AccepThread id=0x%08x - Start %d  \n", GetCurrentThreadId(), g_Connect);
	}

	while (g_Shutdown == false)
	{
		// 100ms ~ 1000ms Sleep
		iRand = rand() % (1000 - 100 + 1) + 100;
		Sleep(iRand);

		InterlockedIncrement(&g_Connect);
	}

	wprintf(L"AccepThread id=0x%08x - closethread %d \n", GetCurrentThreadId(), g_Connect);
	return 0;
}

unsigned _stdcall DisconnectThread(void*)
{
	int iRand;

	srand((unsigned)time(NULL));

	wprintf(L"DisconnectThread id=0x%08x - WaitForSignal %d  \n", GetCurrentThreadId(), g_Connect);
	if (WaitForSingleObject(g_hManualResetEvent, INFINITE) == WAIT_OBJECT_0)
	{
		wprintf(L"DisconnectThread id=0x%08x - Start %d  \n", GetCurrentThreadId(), g_Connect);
	}

	while (g_Shutdown == false)
	{
		// 100ms ~ 1000ms Sleep
		iRand = rand() % (1000 - 100 + 1) + 100;
		Sleep(iRand);

		InterlockedDecrement(&g_Connect);
	}

	wprintf(L"DisconnectThread id=0x%08x - closethread %d \n", GetCurrentThreadId(), g_Connect);
	return 0;
}

unsigned _stdcall UpdateThread(void*)
{
	wprintf(L"UpdateThread id=0x%08x - WaitForSignal %d  \n", GetCurrentThreadId(), g_Data);
	if (WaitForSingleObject(g_hManualResetEvent, INFINITE) == WAIT_OBJECT_0)
	{
		wprintf(L"UpdateThread id=0x%08x - Start %d  \n", GetCurrentThreadId(), g_Data);
	}

	while (g_Shutdown == false)
	{
		Sleep(10);

#if SYNCHRONIZATON_OBJECT == MY_SYNCHRONIZATON_OBJECT
		{
			Lock(&g_Flag);
			g_Data++;

			if (g_Data % 1000 == 0)
			{
				wprintf(L"thread id=0x%08x / g_Data: %d  \n", GetCurrentThreadId(), g_Data);
			}

			UnLock(&g_Flag);
		}
#elif SYNCHRONIZATON_OBJECT == CRITICALSECTION
		{
			EnterCriticalSection(&g_CS);
			g_Data++;
			
			if (g_Data % 1000 == 0)
			{
				wprintf(L"thread id=0x%08x / g_Data: %d  \n", GetCurrentThreadId(), g_Data);
			}
			
			LeaveCriticalSection(&g_CS);
		}
#elif SYNCHRONIZATON_OBJECT == SRW_LOCK
		{
			AcquireSRWLockExclusive(&g_srwlock);
			g_Data++;

			if (g_Data % 1000 == 0)
			{
				wprintf(L"thread id=0x%08x / g_Data: %d  \n", GetCurrentThreadId(), g_Data);
			}

			ReleaseSRWLockExclusive(&g_srwlock);
		}
#endif
	}

	wprintf(L"UpdateThread id=0x%08x - closethread %d  \n", GetCurrentThreadId(), g_Data);
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
		break;
	}
}

void UnLock(LONG* flag)
{
	WakeByAddressSingle(flag);
	InterlockedExchange(flag, 0);
}
