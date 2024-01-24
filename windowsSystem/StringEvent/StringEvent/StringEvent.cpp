// StringEvent.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <process.h>

unsigned int WINAPI OutputThreadFunction(LPVOID lpParam);

TCHAR string[100];
HANDLE hEvent;

int _tmain(int argc, TCHAR* argv[])
{
	HANDLE hThread;
	DWORD dwThreadID;

	hEvent = CreateEvent(
		NULL,		// 상속 불가
		TRUE,		// manual-reset mode로 생성
		FALSE,		// non-siginaled 상태로 생성
		NULL		// 이름없는 event
		);
	if (hEvent == NULL)
	{
		_fputts(_T("Event boject creation error \n"), stdout);
		return -1;
	}

	hThread = (HANDLE)_beginthreadex(NULL, 0, OutputThreadFunction, NULL, 0, (unsigned*)&dwThreadID);

	if (hThread == NULL)
	{
		_fputts(_T("Thread creation error \n"), stdout);
		return -1;
	}

	_fputts(_T("Insert string: "), stdout);
	_fgetts(string, 30, stdin);

	SetEvent(hEvent);	// event의 state를 signaled 상태로 변경 type A


	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hEvent);
	CloseHandle(hThread);

	return 0;
}

unsigned int WINAPI OutputThreadFunction(LPVOID lpParam)
{
	// event 가 signaled 상태가 되기를 기다린다. type A
	WaitForSingleObject(hEvent, INFINITE);

	/* 
	* 1. Event 가 non-signed 상태로 생성되었음 -> 커널 오브젝트 상태=>실행중
	* 2. WaitFor~~() 함수는 해당 핸들의 오브젝트가 끝날 때 까지 기다린다. 커널 오브젝트 (non-signed -> signed) 변경되길 기다린다. 
	* 3. SetEvent(hEvent)를 호출하여 signend로 상태를 변경시킴.
	* 4. 쓰레드가 실행됨
	*/

	_fputts(_T("Output string: "), stdout);
	_fputts(string, stdout);

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
