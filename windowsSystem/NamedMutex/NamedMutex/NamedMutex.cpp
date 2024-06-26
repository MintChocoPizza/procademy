﻿// NamedMutex.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <process.h>

HANDLE hMutex;
DWORD dwWaitResult;

void ProcessBaseCriticalSection()
{
    dwWaitResult = WaitForSingleObject(hMutex, INFINITE);

	switch (dwWaitResult)
	{
		// 쓰레드가 뮤텍스를 소유하였다.
	case WAIT_OBJECT_0:
		_tprintf_s(_T("thread got mutex ! \n"));
		break;


		// time-out 발생하였다.
	case WAIT_TIMEOUT:
		_tprintf_s(_T("timer expired ! \n"));
		return;

		// 뮤텍스 반환이 적절이 이뤄지지 않았다.
	case WAIT_ABANDONED:
		return;
	default:
		break;
	}

	for (DWORD i = 0; i < 5; ++i)
	{
		_tprintf_s(_T("Thread Running ! \n"));
		Sleep(10000);
	}

	ReleaseMutex(hMutex);
}

int _tmain(int argc, TCHAR *argv[])
{
#if 1
	hMutex = CreateMutex(NULL, FALSE, _T("NamedMutex"));	// named mutex
#else 
	hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, _T("NamedMutex"));
#endif
	if (hMutex == NULL)
	{
		_tprintf_s(_T("CreateMutex error: %d\n"), GetLastError());
		return -1;
	}

	ProcessBaseCriticalSection();

	CloseHandle(hMutex);
	return 0;
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
