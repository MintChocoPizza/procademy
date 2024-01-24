// StringEvent2.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <process.h>

unsigned int WINAPI OutPutThreadFunction(LPVOID lpParam);
unsigned int WINAPI CountThreadFunction(LPVOID lpParam);

TCHAR string[100];
HANDLE hEvent;

int _tmain(int argc, TCHAR * argv[])
{	
	HANDLE hThread[2];
	DWORD dwThreadID[2];

	hEvent = CreateEvent(		// event object 생성
		NULL,		// 상속불가
		TRUE,		// manual-reset mode로 생성
		FALSE,		// non-signald 상태로 생성
		NULL		// 이름없는 event
	);

	if (hEvent == NULL)
	{
		_fputts(_T("Event object creation error \n"), stdout);
		return -1;
	}

	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, OutPutThreadFunction, NULL, 0, (unsigned*)&dwThreadID[0]);
	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, CountThreadFunction, NULL, 0, (unsigned*)&dwThreadID[1]);

	if (hThread[0] == NULL || hThread[1] == NULL)
	{
		_fputts(_T("Thread creation error \n"), stdout);
		return -1;
	}

	_fputts(_T("Insert string: "), stdout);
	_fgetts(string, 30, stdin);

	SetEvent(hEvent);		// event의 state를 signaled 상태로 변경, 이 전까지 쓰레드 는 그냥 기다림 블럭상태 아님

	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);		// 종료하기 전 쓰레드 종료 기다림

	CloseHandle(hEvent);
	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);

	return 0;
}

unsigned int WINAPI OutPutThreadFunction(LPVOID lpParam)
{
	WaitForSingleObject(hEvent, INFINITE);
	_fputts(_T("output string: "), stdout);
	_fputts(string, stdout);
	return 0;
}
unsigned int WINAPI CountThreadFunction(LPVOID lpParam)
{
	WaitForSingleObject(hEvent, INFINITE);
	_tprintf_s(_T("output string length: %d \n"), _tcslen(string) - 1);
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
