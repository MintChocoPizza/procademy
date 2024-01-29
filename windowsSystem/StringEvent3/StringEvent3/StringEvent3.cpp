// StringEvent3.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <process.h>

unsigned int WINAPI OutputThreadFunction(LPVOID lpParam);
unsigned int WINAPI CountThreadFunction(LPVOID lpParam);


struct _SynchString {
    TCHAR string[100];
    HANDLE hEvent;
    HANDLE hMutex;
};

_SynchString gSynString;

int _tmain(int argc, TCHAR * argv[])
{
    HANDLE hThreads[2];
    DWORD dwThreadIDs[2];
    gSynString.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    gSynString.hMutex = CreateMutex(NULL, FALSE, NULL);

    if (gSynString.hEvent == NULL || gSynString.hMutex == NULL)
    {
        _fputts(_T("kernel object creation error \n"), stdout);
        return -1;
    }

    hThreads[0] = (HANDLE)_beginthreadex(NULL, 0, OutputThreadFunction, NULL, 0, (unsigned*)&dwThreadIDs[0]);

    hThreads[1] = (HANDLE)_beginthreadex(NULL, 0, CountThreadFunction, NULL, 0, (unsigned*)&dwThreadIDs[1]);

    if (hThreads[0] == 0 || hThreads[1] == 0)
    {
        _fputts(_T("Thread creation error \n"), stdout);
        return -1;
    }

    _fputts(_T("Insert string: "), stdout);
    _fgetts(gSynString.string, 30, stdin);

    SetEvent(gSynString.hEvent);        // 위의 입력을 기다림 Event값이 signaled 값으로 변경함

    WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);

    CloseHandle(gSynString.hEvent);
    CloseHandle(gSynString.hMutex);
    CloseHandle(hThreads[0]);
    CloseHandle(hThreads[1]);

    return 0;
}

unsigned int WINAPI OutputThreadFunction(LPVOID lpParam)
{
    WaitForSingleObject(gSynString.hEvent, INFINITE);
    WaitForSingleObject(gSynString.hMutex, INFINITE);

    _fputts(_T("Output string: "), stdout);
    _fputts(gSynString.string, stdout);

    ReleaseMutex(gSynString.hMutex);
    return 0;
}
unsigned int WINAPI CountThreadFunction(LPVOID lpParam)
{
    WaitForSingleObject(gSynString.hEvent, INFINITE);
    WaitForSingleObject(gSynString.hMutex, INFINITE);
    _tprintf(_T("Output string length: %d \n"), _tcslen(gSynString.string) - 1);
    ReleaseMutex(gSynString.hMutex);
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
