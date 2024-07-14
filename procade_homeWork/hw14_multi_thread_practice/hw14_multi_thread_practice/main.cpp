
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
#define SYNCHRONIZATON_OBJECT		2


/////////////////////////////////////////////////////
// 동기화 객체
//////////////////////////////////////////////////////
LONG g_Flag_Aceep_Disc;
LONG g_Flag_Update;
CRITICAL_SECTION g_CS_Aceep_Disc;
CRITICAL_SECTION g_CS_Update;
SRWLOCK g_srwlock_Aceep_Disc;
SRWLOCK g_srwlock_Update;

LONG g_Data = 0;
LONG g_Connect = 0;
bool g_Shutdown = false;
bool g_Shutdown_Aecc_Disc = false;
bool g_Shutdown_Update = false;
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
		Initialize(&g_Flag_Aceep_Disc);
		Initialize(&g_Flag_Update);
	}
#elif SYNCHRONIZATON_OBJECT == CRITICALSECTION
	{
		InitializeCriticalSection(&g_CS_Aceep_Disc);
		InitializeCriticalSection(&g_CS_Update);
	}
#elif SYNCHRONIZATON_OBJECT == SRW_LOCK
	{
		InitializeSRWLock(&g_srwlock_Aceep_Disc);
		InitializeSRWLock(&g_srwlock_Update);
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

		// 20초가 지난 경우
		if (dwEndTime - dwShutdownTime >= 20000)
		{
			g_Shutdown = true;
			break;
		}

		if (dwEndTime - dwStartTime < 1000)
		{
			dwStartTime += 1000;
			Sleep(dwStartTime - dwEndTime);

#if SYNCHRONIZATON_OBJECT == MY_SYNCHRONIZATON_OBJECT
			Lock(&g_Flag_Aceep_Disc);
#elif SYNCHRONIZATON_OBJECT == CRITICALSECTION
			EnterCriticalSection(&g_CS_Aceep_Disc);
#elif SYNCHRONIZATON_OBJECT == SRW_LOCK
			AcquireSRWLockExclusive(&g_srwlock_Aceep_Disc);
#endif
			{
				/////////////////////////////////////////////////////////////////
				// 단순  출력
				/////////////////////////////////////////////////////////////////
				wprintf(L"main thread - g_Connect: %d \n", g_Connect);
			}
#if SYNCHRONIZATON_OBJECT == MY_SYNCHRONIZATON_OBJECT
			UnLock(&g_Flag_Aceep_Disc);
#elif SYNCHRONIZATON_OBJECT == CRITICALSECTION
			LeaveCriticalSection(&g_CS_Aceep_Disc);
#elif SYNCHRONIZATON_OBJECT == SRW_LOCK
			ReleaseSRWLockExclusive(&g_srwlock_Aceep_Disc);
#endif
		}
		else
		{
			dwStartTime += 1000;
		}
	}

	/////////////////////////////////////////////////////
	// 각각의 스레드의 종료 조건 변수에 대하여 
	// 동기화를 걸고 종료를 갱신해준다. 
	/////////////////////////////////////////////////////
	if (g_Shutdown == true)
	{
#if SYNCHRONIZATON_OBJECT == MY_SYNCHRONIZATON_OBJECT
		Lock(&g_Flag_Aceep_Disc);
#elif SYNCHRONIZATON_OBJECT == CRITICALSECTION
		EnterCriticalSection(&g_CS_Aceep_Disc);
#elif SYNCHRONIZATON_OBJECT == SRW_LOCK
		AcquireSRWLockExclusive(&g_srwlock_Aceep_Disc);
#endif
		{
			g_Shutdown_Aecc_Disc = true;
		}
#if SYNCHRONIZATON_OBJECT == MY_SYNCHRONIZATON_OBJECT
		UnLock(&g_Flag_Aceep_Disc);
#elif SYNCHRONIZATON_OBJECT == CRITICALSECTION
		LeaveCriticalSection(&g_CS_Aceep_Disc);
#elif SYNCHRONIZATON_OBJECT == SRW_LOCK
		ReleaseSRWLockExclusive(&g_srwlock_Aceep_Disc);
#endif

#if SYNCHRONIZATON_OBJECT == MY_SYNCHRONIZATON_OBJECT
		Lock(&g_Flag_Update);
#elif SYNCHRONIZATON_OBJECT == CRITICALSECTION
		EnterCriticalSection(&g_CS_Update);
#elif SYNCHRONIZATON_OBJECT == SRW_LOCK
		AcquireSRWLockExclusive(&g_srwlock_Update);
#endif
		{
			g_Shutdown_Update = true;
		}
#if SYNCHRONIZATON_OBJECT == MY_SYNCHRONIZATON_OBJECT
		UnLock(&g_Flag_Update);
#elif SYNCHRONIZATON_OBJECT == CRITICALSECTION
		LeaveCriticalSection(&g_CS_Update);
#elif SYNCHRONIZATON_OBJECT == SRW_LOCK
		ReleaseSRWLockExclusive(&g_srwlock_Update);
#endif
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

	/////////////////////////////////////////////////////
	// 사용한 모든 것들을 정리해준다.
	/////////////////////////////////////////////////////
	for (iCnt = 0; iCnt < 5; iCnt++)
	{
		CloseHandle(hThread[iCnt]);
	}
	CloseHandle(g_hManualResetEvent);
	timeEndPeriod(1);


	wprintf(L"Hello World \n");
	return 0;
}

unsigned _stdcall AccepThread(void*)
{
	int iRand;

	srand((unsigned)time(NULL));

	wprintf(L"AccepThread id=0x%08x - WaitForSignal \n", GetCurrentThreadId());
	if (WaitForSingleObject(g_hManualResetEvent, INFINITE) == WAIT_OBJECT_0)
	{
		wprintf(L"AccepThread id=0x%08x - Start \n", GetCurrentThreadId());
	}

	while (1)
	{

		// 100ms ~ 1000ms Sleep
		iRand = rand() % (1000 - 100 + 1) + 100;
		Sleep(iRand);


#if SYNCHRONIZATON_OBJECT == MY_SYNCHRONIZATON_OBJECT
		Lock(&g_Flag_Aceep_Disc);
#elif SYNCHRONIZATON_OBJECT == CRITICALSECTION
		EnterCriticalSection(&g_CS_Aceep_Disc);
#elif SYNCHRONIZATON_OBJECT == SRW_LOCK
		AcquireSRWLockExclusive(&g_srwlock_Aceep_Disc);
#endif
		if (g_Shutdown_Aecc_Disc == true)
		{
			wprintf(L"AccepThread id=0x%08x - closethread %d \n", GetCurrentThreadId(), g_Connect);
#if SYNCHRONIZATON_OBJECT == MY_SYNCHRONIZATON_OBJECT
			UnLock(&g_Flag_Aceep_Disc);
#elif SYNCHRONIZATON_OBJECT == CRITICALSECTION
			LeaveCriticalSection(&g_CS_Aceep_Disc);
#elif SYNCHRONIZATON_OBJECT == SRW_LOCK
			ReleaseSRWLockExclusive(&g_srwlock_Aceep_Disc);
#endif
			break;
		}

		++g_Connect;

#if SYNCHRONIZATON_OBJECT == MY_SYNCHRONIZATON_OBJECT
		UnLock(&g_Flag_Aceep_Disc);
#elif SYNCHRONIZATON_OBJECT == CRITICALSECTION
		LeaveCriticalSection(&g_CS_Aceep_Disc);
#elif SYNCHRONIZATON_OBJECT == SRW_LOCK
		ReleaseSRWLockExclusive(&g_srwlock_Aceep_Disc);
#endif
	}

	return 0;
}

unsigned _stdcall DisconnectThread(void*)
{
	int iRand;

	srand((unsigned)time(NULL));

	wprintf(L"DisconnectThread id=0x%08x - WaitForSignal \n", GetCurrentThreadId());
	if (WaitForSingleObject(g_hManualResetEvent, INFINITE) == WAIT_OBJECT_0)
	{
		wprintf(L"DisconnectThread id=0x%08x - Start \n", GetCurrentThreadId());
	}

	while (1)
	{

		// 100ms ~ 1000ms Sleep
		iRand = rand() % (1000 - 100 + 1) + 100;
		Sleep(iRand);

#if SYNCHRONIZATON_OBJECT == MY_SYNCHRONIZATON_OBJECT
		Lock(&g_Flag_Aceep_Disc);
#elif SYNCHRONIZATON_OBJECT == CRITICALSECTION
		EnterCriticalSection(&g_CS_Aceep_Disc);
#elif SYNCHRONIZATON_OBJECT == SRW_LOCK
		AcquireSRWLockExclusive(&g_srwlock_Aceep_Disc);
#endif

		if (g_Shutdown_Aecc_Disc == true)
		{
			wprintf(L"DisconnectThread id=0x%08x - closethread %d \n", GetCurrentThreadId(), g_Connect);
#if SYNCHRONIZATON_OBJECT == MY_SYNCHRONIZATON_OBJECT
			UnLock(&g_Flag_Aceep_Disc);
#elif SYNCHRONIZATON_OBJECT == CRITICALSECTION
			LeaveCriticalSection(&g_CS_Aceep_Disc);
#elif SYNCHRONIZATON_OBJECT == SRW_LOCK
			ReleaseSRWLockExclusive(&g_srwlock_Aceep_Disc);
#endif
			break;
		}

		--g_Connect;

#if SYNCHRONIZATON_OBJECT == MY_SYNCHRONIZATON_OBJECT
		UnLock(&g_Flag_Aceep_Disc);
#elif SYNCHRONIZATON_OBJECT == CRITICALSECTION
		LeaveCriticalSection(&g_CS_Aceep_Disc);
#elif SYNCHRONIZATON_OBJECT == SRW_LOCK
		ReleaseSRWLockExclusive(&g_srwlock_Aceep_Disc);
#endif
	}

	return 0;
}

unsigned _stdcall UpdateThread(void*)
{
	wprintf(L"UpdateThread id=0x%08x - WaitForSignal  \n", GetCurrentThreadId());
	if (WaitForSingleObject(g_hManualResetEvent, INFINITE) == WAIT_OBJECT_0)
	{
		wprintf(L"UpdateThread id=0x%08x - Start \n", GetCurrentThreadId());
	}

	
	while (1)
	{

		Sleep(10);

#if SYNCHRONIZATON_OBJECT == MY_SYNCHRONIZATON_OBJECT
		Lock(&g_Flag_Update);
#elif SYNCHRONIZATON_OBJECT == CRITICALSECTION
		EnterCriticalSection(&g_CS_Update);
#elif SYNCHRONIZATON_OBJECT == SRW_LOCK
		AcquireSRWLockExclusive(&g_srwlock_Update);
#endif
		if (g_Shutdown_Update == true)
		{
			wprintf(L"UpdateThread id=0x%08x - closethread %d  \n", GetCurrentThreadId(), g_Data);
#if SYNCHRONIZATON_OBJECT == MY_SYNCHRONIZATON_OBJECT
			UnLock(&g_Flag_Update);
#elif SYNCHRONIZATON_OBJECT == CRITICALSECTION
			LeaveCriticalSection(&g_CS_Update);
#elif SYNCHRONIZATON_OBJECT == SRW_LOCK
			ReleaseSRWLockExclusive(&g_srwlock_Update);
#endif
			break;
		}
		
		g_Data++;

		if (g_Data % 1000 == 0)
		{
			wprintf(L"thread id=0x%08x / g_Data: %d  \n", GetCurrentThreadId(), g_Data);
		}

#if SYNCHRONIZATON_OBJECT == MY_SYNCHRONIZATON_OBJECT
		UnLock(&g_Flag_Update);
#elif SYNCHRONIZATON_OBJECT == CRITICALSECTION
		LeaveCriticalSection(&g_CS_Update);
#elif SYNCHRONIZATON_OBJECT == SRW_LOCK
		ReleaseSRWLockExclusive(&g_srwlock_Update);
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
	InterlockedExchange(flag, 0);
	WakeByAddressSingle(flag);
}


