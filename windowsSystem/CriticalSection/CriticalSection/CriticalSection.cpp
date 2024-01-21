// CriticalSection.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <process.h>

#define NUM_OF_GATE 6

LONG gTotalCount = 0;

void IncreaseCount()
{
    gTotalCount++;          // 임계 영역(Critical section)
}


unsigned int  WINAPI ThreadProc(LPVOID lpParam)
{
    for (DWORD i = 0; i < 1000; i++)
    {
        IncreaseCount();
    }
    return 0;
}

int _tmain(int argc, TCHAR * argv[])
{
    DWORD dwThreadId[NUM_OF_GATE];
    HANDLE hThread[NUM_OF_GATE];

    for (DWORD i = 0; i < NUM_OF_GATE; ++i)
    {
        hThread[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, NULL, CREATE_SUSPENDED, (unsigned*)&dwThreadId[i]);

        if (hThread[i] == NULL)
        {
            _tprintf_s(_T("Thread createion fault! \n"));
            return -1;
        }
    }

    for (DWORD i = 0; i < NUM_OF_GATE; ++i)
    {
        ResumeThread(hThread[i]);
    }

    WaitForMultipleObjects(NUM_OF_GATE, hThread, TRUE, INFINITE);

    _tprintf_s(_T("total count: %d \n"), gTotalCount);

    for (DWORD i = 0; i < NUM_OF_GATE; ++i)
    {
        CloseHandle(hThread[i]);
    }
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
