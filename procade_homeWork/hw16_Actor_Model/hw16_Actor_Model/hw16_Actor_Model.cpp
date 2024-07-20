// hw16_Actor_Model.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <string>
#include <list>
#include <Windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#include "C_Ring_Buffer.h"

#define MAX_THREAD      3

#define dfJOB_ADD       0
#define dfJOB_DEL       1
#define dfJOB_SORT      2
#define dfJOB_FIND      3
#define dfJOB_PRINT     4       // 출력 or 저장/ 읽기만 하는 느린 행동
#define dfJOB_QUIT      5

//-----------------------------------------------
// 컨텐츠 부, 문자열 리스트
//-----------------------------------------------
std::list<std::wstring> g_List;

//-----------------------------------------------
// 스레드 메시지 큐 (사이즈 넉넉하게 크게 4~5만 바이트)
//-----------------------------------------------
C_RING_BUFFER g_msgQ(50000);

struct st_MSG_HEAD
{
    short shType;
    short shPayloadLen;
};

int wmain()
{
    HANDLE hThread[MAX_THREAD];
    wchar_t c;
    int iCnt;
    DWORD dwStatus;
    bool b_Shutdown;

    timeBeginPeriod(1);

    b_Shutdown = false;
    srand((unsigned)time(NULL));

    wprintf(L"메인 스레드의 종료는 Q를 누르세요.... \n");

    // 메인 스레드 반복
    while (!b_Shutdown)
    {
        if (_kbhit())
        {
            c = _getwch();
            if (c == L'Q' || c == L'q')
                b_Shutdown = true;
        }

        
        
    }

    /////////////////////////////////////////////////////
    // 모든 스레드 종료(시그널이 오기를) 기다린다.
    // 
    /////////////////////////////////////////////////////
    wprintf(L"Wait For All Thread!! \n");
    dwStatus - WaitForMultipleObjects(MAX_THREAD, hThread, TRUE, INFINITE);
    if (dwStatus == WAIT_OBJECT_0)
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
    for (iCnt = 0; iCnt < MAX_THREAD; iCnt++)
    {
        CloseHandle(hThread[iCnt]);
    }
    timeEndPeriod(1);


    std::cout << "Hello World!\n";

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
