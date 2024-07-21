// hw16_Actor_Model.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <process.h>
#include <string>
#include <list>
#include <Windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <iostream>

#include "C_Ring_Buffer.h"

#pragma comment(lib, "Winmm.lib")

#define MAX_THREAD      4
#define MAX_MSG_TYPE    6

#define dfJOB_ADD       0
#define dfJOB_DEL       1
#define dfJOB_SORT      2
#define dfJOB_FIND      3
#define dfJOB_PRINT     4       // 출력 or 저장/ 읽기만 하는 느린 행동
#define dfJOB_QUIT      5


struct st_MSG_HEAD
{
    short shType;
    short shPayloadLen;
};

//-----------------------------------------------
// 동기화 객체
//-----------------------------------------------
SRWLOCK g_srwlock_G_LIST; 
SRWLOCK g_srwlock_G_MSGQ;

//-----------------------------------------------
// 자동 리셋 이벤트 객체
//-----------------------------------------------
HANDLE g_h_AutoResetEvent;

//-----------------------------------------------
// 컨텐츠 부, 문자열 리스트
//-----------------------------------------------
std::list<std::wstring> g_List;

//-----------------------------------------------
// 스레드 메시지 큐 (사이즈 넉넉하게 크게 4~5만 바이트)
//-----------------------------------------------
C_RING_BUFFER g_msgQ(50000);

//-----------------------------------------------
// 임의의 문자열
//-----------------------------------------------
const wchar_t* strList[] = 
{       L"apple", L"orange", L"banana", L"grape", L"cherry",
        L"pear", L"peach", L"plum", L"apricot", L"melon",
        L"kiwi", L"mango", L"pineapple", L"blueberry", L"strawberry",
        L"raspberry", L"blackberry", L"lime", L"lemon", L"watermelon",
        L"dog", L"cat", L"elephant", L"tiger", L"lion",
        L"zebra", L"giraffe", L"panda", L"kangaroo", L"koala",
        L"red", L"blue", L"green", L"yellow", L"purple",
        L"black", L"white", L"orange", L"pink", L"brown",
        L"USA", L"Canada", L"France", L"Germany", L"Japan",
        L"China", L"India", L"Brazil", L"Australia", L"Russia" 
};

int g_strListLen;

unsigned __stdcall WorkerThread(void* pArg);
unsigned __stdcall Monitoring(void* pArg);

int wmain()
{
    HANDLE hThread[MAX_THREAD];
    unsigned int threadIDs[MAX_THREAD];
    st_MSG_HEAD st_Header;
    DWORD dwStatus;
    wchar_t c;
    int iCnt;
    int iMSG_Number;
    short shRandomType;
    bool b_Shutdown;
    bool b_Ret_SetEvent;

    timeBeginPeriod(1);

    b_Shutdown = false;
    g_strListLen = sizeof(strList) / sizeof(strList[0]);        // 현재 50개
    srand((unsigned)time(NULL));                                // 랜덤 시드값 초기화
    InitializeSRWLock(&g_srwlock_G_LIST);                       // srwlock 초기화
    

    wprintf(L"메인 스레드의 종료는 Q를 누르세요.... \n");

    /////////////////////////////////////////////////////
	// 자동 리셋 이벤트를 생성한다. 
	// 
	/////////////////////////////////////////////////////
    g_h_AutoResetEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

    /////////////////////////////////////////////////////
	// 스레드를 생성한다. 
	// 
	/////////////////////////////////////////////////////
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

    /////////////////////////////////////////////////////
    // 메인 스레드 반복
    /////////////////////////////////////////////////////
    while (!b_Shutdown)
    {
        if (_kbhit())
        {
            c = _getwch();
            if (c == L'Q' || c == L'q')
                b_Shutdown = true;
        }

        // 

        // 랜덤한 메시지 타입
        shRandomType = rand() % MAX_MSG_TYPE;
        
        switch (shRandomType)
        {
        case dfJOB_ADD:
            // 전달할 메시지 랜덤하게 생성한다. 
            iMSG_Number = rand() % g_strListLen;
            st_Header.shType = dfJOB_ADD;
            st_Header.shPayloadLen = (short)wcslen(strList[iMSG_Number]);

            // lock을 걸고 할 일을 넣는다. 
            AcquireSRWLockExclusive(&g_srwlock_G_LIST);
            g_msgQ.Enqueue((char*)&st_Header, sizeof(st_Header));
            g_msgQ.Enqueue((char*)strList[iMSG_Number], st_Header.shPayloadLen);
            ReleaseSRWLockExclusive(&g_srwlock_G_LIST);
            break;
        case dfJOB_DEL:
            // 삭제할 메시지를 랜덤하게 생성한다. 
            iMSG_Number = rand() % g_strListLen;
            st_Header.shType = dfJOB_DEL;
            st_Header.shPayloadLen = (short)wcslen(strList[iMSG_Number]);

            // lock을 걸고 할 일을 넣는다. 
            AcquireSRWLockExclusive(&g_srwlock_G_LIST);
            g_msgQ.Enqueue((char*)&st_Header, sizeof(st_Header));
            g_msgQ.Enqueue((char*)strList[iMSG_Number], st_Header.shPayloadLen);
            ReleaseSRWLockExclusive(&g_srwlock_G_LIST);
            break;
        case dfJOB_SORT:
            // 정렬 메시지를 보낸다. 페이로드 없음
            st_Header.shType = dfJOB_SORT;
            st_Header.shPayloadLen = (short)0;

            // lock을 걸고 할 일을 넣는다. 
            AcquireSRWLockExclusive(&g_srwlock_G_LIST);
            g_msgQ.Enqueue((char*)&st_Header, sizeof(st_Header));
            ReleaseSRWLockExclusive(&g_srwlock_G_LIST);
            break;
        case dfJOB_FIND:
            iMSG_Number = rand() % g_strListLen;
            st_Header.shType = dfJOB_FIND;
            st_Header.shPayloadLen = (short)wcslen(strList[iMSG_Number]);

            // lock을 걸고 할 일을 넣는다. 
            AcquireSRWLockExclusive(&g_srwlock_G_LIST);
            g_msgQ.Enqueue((char*)&st_Header, sizeof(st_Header));
            g_msgQ.Enqueue((char*)strList[iMSG_Number], st_Header.shPayloadLen);
            ReleaseSRWLockExclusive(&g_srwlock_G_LIST);
            break;
        case dfJOB_PRINT:
            st_Header.shType = dfJOB_PRINT;
            st_Header.shPayloadLen = (short)0;

            // lock을 걸고 할 일을 넣는다. 
            AcquireSRWLockExclusive(&g_srwlock_G_LIST);
            g_msgQ.Enqueue((char*)&st_Header, sizeof(st_Header));
            ReleaseSRWLockExclusive(&g_srwlock_G_LIST);
            break;
        }

        // 일을 생성했으니까 시그널을 킨다. 
        b_Ret_SetEvent = SetEvent(g_h_AutoResetEvent);
    }

    // 프로그램이 종료되어야 하므로, OUIT 메시지를 보낸다.
    st_Header.shType = dfJOB_QUIT;
    st_Header.shPayloadLen = (short)0;
    AcquireSRWLockExclusive(&g_srwlock_G_LIST);
    g_msgQ.Enqueue((char*)&st_Header, sizeof(st_Header));
    ReleaseSRWLockExclusive(&g_srwlock_G_LIST);

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
    CloseHandle(g_h_AutoResetEvent);
    timeEndPeriod(1);


    std::cout << "Hello World!\n";

    return 0;
}



unsigned __stdcall WorkerThread(void* pArg)
{
    // 메인스레드 에서 메시지를 넣지 않았다면 블락됨
    DWORD dwStatus;
    st_MSG_HEAD st_Header;
    bool b_Shutdown;

    //----------------------------------------------------
    // 스레드 시작 로그 ID 출력
    //----------------------------------------------------
    printf("WorketThread id=0x%08x - Start!! \n", GetCurrentThreadId());
    b_Shutdown = false;

    while (1)
    {
        //----------------------------------------------------
        // Lock은 필수로 걸어야 한다. 
        // 
        // 1. 메시지 큐에서 메시지를 뽑는다. 
        // 2. 헤더 타입에 맞게끔 동작
        //----------------------------------------------------

        if (b_Shutdown == true)
        {
            break;
        }

        dwStatus = WaitForSingleObject(g_h_AutoResetEvent, INFINITE);
        if (dwStatus != WAIT_OBJECT_0)
        {
            printf("WorketThread id = 0x%08x - Error: %d \n", GetCurrentThreadId(), GetLastError());
        }

        // 실행 조건 체크
        // 1. 깨어났으면 무조건 동기화 객체를 얻어야 한다. 
        AcquireSRWLockExclusive(&g_srwlock_G_LIST);
        // 2. 깨어 났으면 데이터가 있는지 확인한다. 
        //  동기화를 걸어서 넣었기 때문에, 헤더와 페이로드가 분리되는 상황은 고려하지 않는다. 
        if (g_List.empty())
        {
            // 깨어 났지만 할 일이 없다면 다시 블락된다. 
            ReleaseSRWLockExclusive(&g_srwlock_G_LIST);
            continue;
        }

        g_msgQ.Dequeue((char*)&st_Header, sizeof(st_MSG_HEAD));

        switch (st_Header.shType)
        {
        case dfJOB_ADD:
            
        case dfJOB_DEL:
        case dfJOB_SORT:
        case dfJOB_FIND:
        case dfJOB_PRINT:
        case dfJOB_QUIT:
            b_Shutdown = true;
            break;
        }
        
        
        ReleaseSRWLockExclusive(&g_srwlock_G_LIST);
    }

    return 0;
}

unsigned __stdcall Monitoring(void* pArg)
{
    return 0;
}
