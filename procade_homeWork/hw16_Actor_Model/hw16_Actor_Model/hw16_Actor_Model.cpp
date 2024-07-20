// hw16_Actor_Model.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <process.h>
#include <string>
#include <list>
#include <Windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#include "C_Ring_Buffer.h"

#define MAX_THREAD      4
#define MAX_MSG_TYPE    6

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

unsigned __stdcall WorkerThread(void* pArg);
unsigned __stdcall Monitoring(void* pArg);

int wmain()
{
    HANDLE hThread[MAX_THREAD];
    unsigned int threadIDs[MAX_THREAD];
    wchar_t c;
    int iCnt;
    DWORD dwStatus;
    short shRandomType;
    bool b_Shutdown;

    timeBeginPeriod(1);

    b_Shutdown = false;
    srand((unsigned)time(NULL));

    wprintf(L"메인 스레드의 종료는 Q를 누르세요.... \n");

    // 스레드 생성
    for (iCnt = 0; iCnt < MAX_THREAD - 1; ++iCnt)
    {
        hThread[iCnt] = (HANDLE)_beginthreadex(NULL, 0, WorkerThread, NULL, NULL, &threadIDs[iCnt]);
    }
    hThread[iCnt] = (HANDLE)_beginthreadex(NULL, 0, Monitoring, NULL, NULL, &threadIDs[iCnt]);

    // 생성된 스레드 에러 체크
    for (iCnt = 0; iCnt < MAX_THREAD; ++iCnt)
    {
        if (!hThread[iCnt])
        {
            printf("thread - error creating child threads \n");
            return -1;
        }
        else
        {
            printf("thread %d - id=0x%08x : Create!! \n", iCnt, threadIDs[iCnt]);
        }
    }

    // 메인 스레드 반복
    while (!b_Shutdown)
    {
        if (_kbhit())
        {
            c = _getwch();
            if (c == L'Q' || c == L'q')
                b_Shutdown = true;
        }

        // 랜덤한 메시지 타입
        shRandomType = rand() % MAX_MSG_TYPE;

        
    }

    /////////////////////////////////////////////////////
    // 모든 스레드 종료(시그널이 오기를) 기다린다.
    // 
    /////////////////////////////////////////////////////
    wprintf(L"Wait For All Thread!! \n");
    dwStatus = WaitForMultipleObjects(MAX_THREAD, hThread, TRUE, INFINITE);
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



unsigned __stdcall WorkerThread(void* pArg)
{
    // 메인스레드 에서 메시지를 넣지 않았다면 블락됨

    //----------------------------------------------------
    // 스레드 시작 로그 ID 출력
    //----------------------------------------------------
    printf("WorketThread id=0x%08x - Start!! \n", GetCurrentThreadId());

    while (1)
    {
        //----------------------------------------------------
        // Lock은 필수로 걸어야 한다. 
        // 
        // 1. 메시지 큐에서 메시지를 뽑는다. 
        // 2. 헤더 타입에 맞게끔 동작
        //----------------------------------------------------

    }

    return 0;
}

unsigned __stdcall Monitoring(void* pArg)
{
    return 0;
}
