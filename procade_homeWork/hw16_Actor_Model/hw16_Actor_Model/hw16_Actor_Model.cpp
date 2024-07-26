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
std::list<std::wstring*> g_List;

//-----------------------------------------------
// 스레드 메시지 큐 (사이즈 넉넉하게 크게 4~5만 바이트)
//-----------------------------------------------
//C_RING_BUFFER g_msgQ(50000);
C_RING_BUFFER g_msgQ(5000);

//-----------------------------------------------
// 임의의 문자열
//-----------------------------------------------
const wchar_t* g_strList[] = 
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

//-----------------------------------------------
// 임의의 문자열 길이
//-----------------------------------------------
int g_strListSize;

//-----------------------------------------------
// 메인 스레드에서 일이 생성되는 주기
//-----------------------------------------------
unsigned int g_WorkGenerationCycle;

//-----------------------------------------------
// 프로그램 종료 플래그
//-----------------------------------------------
bool g_b_Shutdown;

//-----------------------------------------------
// 초당 메시지 잡 스레드별 처리수
//-----------------------------------------------
LONG g_TPS[MAX_THREAD];

//-----------------------------------------------
// 스레드 아이디 저장
//-----------------------------------------------
unsigned int g_ThreadIDs[MAX_THREAD];

//-----------------------------------------------
// 메시지 타입별 처리 수
//-----------------------------------------------
unsigned int g_MSG_Type_Count[MAX_MSG_TYPE];

//-----------------------------------------------
// 메인 스레드의 일 생산 프레임
//-----------------------------------------------
DWORD g_dwFPS;

unsigned __stdcall WorkerThread(void* pArg);
unsigned __stdcall Monitoring(void* pArg);

int wmain()
{
    HANDLE hThread[MAX_THREAD];
    // unsigned int threadIDs[MAX_THREAD];
    st_MSG_HEAD st_Header;
    DWORD dwStatus;
    DWORD dwStartTime;
    DWORD dwEndTime;
    DWORD dwSecondTime;
    DWORD dwFrameTick;
    DWORD dwFPS;
    wchar_t c;
    int iCnt;
    int iMSG_Number;
    short shRandomType;
    bool b_Ret_SetEvent;
    bool b_Shutdown;

    timeBeginPeriod(1);

    for (iCnt = 0; iCnt < MAX_THREAD; ++iCnt)
    {
        g_TPS[iCnt] = 0;
    }
    for (iCnt = 0; iCnt < MAX_MSG_TYPE; ++iCnt)
    {
        g_MSG_Type_Count[iCnt] = 0;
    }
    dwFrameTick = 1000;
    dwFPS = 0;
    g_WorkGenerationCycle = 20;
    g_b_Shutdown = false;
    b_Shutdown = false;
    g_strListSize = sizeof(g_strList) / sizeof(g_strList[0]);        // 현재 50개
    srand((unsigned)time(NULL));                                // 랜덤 시드값 초기화
    InitializeSRWLock(&g_srwlock_G_LIST);                       // srwlock 초기화
    InitializeSRWLock(&g_srwlock_G_MSGQ);                       // srwlock 초기화
    

    printf("메인 스레드의 종료는 Q를 누르세요.... \n");

    /////////////////////////////////////////////////////
	// 자동 리셋 이벤트를 생성한다. 
	// 
	/////////////////////////////////////////////////////
    g_h_AutoResetEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (g_h_AutoResetEvent == NULL)
    {
        wprintf(L"CreateEvent g_h_AutoResetEvent false \n");
        return 0;
    }

    /////////////////////////////////////////////////////
	// 스레드를 생성한다. 
	// 
	/////////////////////////////////////////////////////
    for (iCnt = 0; iCnt < MAX_THREAD - 1; ++iCnt)
    {
        hThread[iCnt] = (HANDLE)_beginthreadex(NULL, 0, WorkerThread, (void *)iCnt, NULL, &g_ThreadIDs[iCnt]);
    }
    hThread[iCnt] = (HANDLE)_beginthreadex(NULL, 0, Monitoring, NULL, NULL, &g_ThreadIDs[iCnt]);

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
            printf("thread %d - id=0x%08x : Create!! \n", iCnt, g_ThreadIDs[iCnt]);
        }
    }

    /////////////////////////////////////////////////////
    // 메시지 생성 전 이상한 메시지가 있는지 확인한다. 
    /////////////////////////////////////////////////////
    for (iCnt = 0; iCnt < g_strListSize; ++iCnt)
    {
        if (wcslen(g_strList[iCnt]) > 10)
            __debugbreak();
    }


    /////////////////////////////////////////////////////
    // 메인 스레드 반복
    /////////////////////////////////////////////////////
    dwStartTime = timeGetTime();
    dwSecondTime = dwStartTime;
    while (b_Shutdown == false)
    {
        if (_kbhit())
        {
            c = _getwch();
            if (c == L'Q' || c == L'q')
                b_Shutdown = true;
            continue;
        }

        //// 일을 스레드 들이 정상적으로 처리 한다면 
        //// 일 생성 주기르 늘린다. 
        //// 만역 버퍼가 가득 찼다면, 하던 작업까지는 마무리 하고 종료시킨다.
        //Sleep(g_WorkGenerationCycle);
        //if (g_msgQ.GetUseSize() == 0 && g_WorkGenerationCycle != 0)
        //{
        //    g_WorkGenerationCycle--;
        //}
        //else if (g_msgQ.GetUseSize() == g_msgQ.GetBufferSize())
        //{
        //    g_b_Shutdown = true;
        //    continue;
        //}
        //else if (g_msgQ.GetUseSize() != 0)
        //    g_WorkGenerationCycle++;
        //
        /////////////////////////////////////////////////////
        // 프레임 단위로 변경하기
        /////////////////////////////////////////////////////
        dwEndTime = timeGetTime();
        if (dwEndTime - dwStartTime < g_WorkGenerationCycle)
        {
            dwStartTime += g_WorkGenerationCycle;
            Sleep(dwStartTime - dwEndTime);
        }
        else
        {
            dwStartTime += g_WorkGenerationCycle;
            continue;
        }

        if (dwEndTime - dwSecondTime >= dwFrameTick)
        {
            dwSecondTime += dwFrameTick;
            g_dwFPS = dwFPS;
            dwFPS = 0;
        }
        dwFPS++;
        
        if (g_msgQ.GetUseSize() == g_msgQ.GetBufferSize())
        {
            b_Shutdown = true;
        }
        else if (g_msgQ.GetUseSize() > g_msgQ.GetBufferSize() * 8 / 10)
        {
            g_WorkGenerationCycle = 85;
        }
        else if (g_msgQ.GetUseSize() > g_msgQ.GetBufferSize() * 7 / 10)
        {
            g_WorkGenerationCycle = 50;
        }
        else if (g_msgQ.GetUseSize() > g_msgQ.GetBufferSize() * 6 / 10)
        {
            g_WorkGenerationCycle = 40;
        }
        else if (g_msgQ.GetUseSize() > g_msgQ.GetBufferSize() * 5 / 10)
        {
            g_WorkGenerationCycle = 25;
        }
        else
        {
            g_WorkGenerationCycle = 20;
        }

        // 강제 부하
        //Sleep(5);
        //printf("JobQ %llu \n", g_msgQ.GetUseSize());

        // 랜덤한 메시지 타입
        shRandomType = rand() % MAX_MSG_TYPE;
        
        switch (shRandomType)
        {
        case dfJOB_ADD:
            // 전달할 메시지 랜덤하게 생성한다. 
            iMSG_Number = rand() % g_strListSize;
            st_Header.shType = dfJOB_ADD;
            st_Header.shPayloadLen = (short)wcslen(g_strList[iMSG_Number]);

            // lock을 걸고 할 일을 넣는다. 
            AcquireSRWLockExclusive(&g_srwlock_G_MSGQ);
            g_msgQ.Enqueue((char*)&st_Header, sizeof(st_Header));
            g_msgQ.Enqueue((char*)g_strList[iMSG_Number], st_Header.shPayloadLen);
            ReleaseSRWLockExclusive(&g_srwlock_G_MSGQ);
            break;
        case dfJOB_DEL:
            // 삭제할 메시지를 랜덤하게 생성한다. 
            iMSG_Number = rand() % g_strListSize;
            st_Header.shType = dfJOB_DEL;
            st_Header.shPayloadLen = (short)wcslen(g_strList[iMSG_Number]);

            // lock을 걸고 할 일을 넣는다. 
            AcquireSRWLockExclusive(&g_srwlock_G_MSGQ);
            g_msgQ.Enqueue((char*)&st_Header, sizeof(st_Header));
            g_msgQ.Enqueue((char*)g_strList[iMSG_Number], st_Header.shPayloadLen);
            ReleaseSRWLockExclusive(&g_srwlock_G_MSGQ);
            break;
        case dfJOB_SORT:
            // 정렬 메시지를 보낸다. 페이로드 없음
            st_Header.shType = dfJOB_SORT;
            st_Header.shPayloadLen = (short)0;

            // lock을 걸고 할 일을 넣는다. 
            AcquireSRWLockExclusive(&g_srwlock_G_MSGQ);
            g_msgQ.Enqueue((char*)&st_Header, sizeof(st_Header));
            ReleaseSRWLockExclusive(&g_srwlock_G_MSGQ);
            break;
        case dfJOB_FIND:
            iMSG_Number = rand() % g_strListSize;
            st_Header.shType = dfJOB_FIND;
            st_Header.shPayloadLen = (short)wcslen(g_strList[iMSG_Number]);

            // lock을 걸고 할 일을 넣는다. 
            AcquireSRWLockExclusive(&g_srwlock_G_MSGQ);
            g_msgQ.Enqueue((char*)&st_Header, sizeof(st_Header));
            g_msgQ.Enqueue((char*)g_strList[iMSG_Number], st_Header.shPayloadLen);
            ReleaseSRWLockExclusive(&g_srwlock_G_MSGQ);
            break;
        case dfJOB_PRINT:
            st_Header.shType = dfJOB_PRINT;
            st_Header.shPayloadLen = (short)0;

            // lock을 걸고 할 일을 넣는다. 
            AcquireSRWLockExclusive(&g_srwlock_G_MSGQ);
            g_msgQ.Enqueue((char*)&st_Header, sizeof(st_Header));
            ReleaseSRWLockExclusive(&g_srwlock_G_MSGQ);
            break;
        }

        // 일을 생성했으니까 시그널을 킨다. 
        b_Ret_SetEvent = SetEvent(g_h_AutoResetEvent);
        if (b_Ret_SetEvent == NULL)
        {
            printf("SetEvent - Error: %d \n", GetLastError());
            __debugbreak();
        }

    }

    // 프로그램이 종료되어야 하므로, OUIT 메시지를 보낸다.
    st_Header.shType = dfJOB_QUIT;
    st_Header.shPayloadLen = (short)0;
    AcquireSRWLockExclusive(&g_srwlock_G_MSGQ);
    g_msgQ.Enqueue((char*)&st_Header, sizeof(st_MSG_HEAD));
    ReleaseSRWLockExclusive(&g_srwlock_G_MSGQ);
    b_Ret_SetEvent = SetEvent(g_h_AutoResetEvent);
    if (b_Ret_SetEvent == NULL)
    {
        printf("SetEvent - Error: %d \n", GetLastError());
        __debugbreak();
    }

    /////////////////////////////////////////////////////
    // 모든 스레드 종료(시그널이 오기를) 기다린다.
    // 
    /////////////////////////////////////////////////////
    wprintf(L"Wait For All Thread!! \n");
    dwStatus = WaitForMultipleObjects(MAX_THREAD-1, hThread, TRUE, INFINITE);
    if (dwStatus == WAIT_OBJECT_0)
    {
        wprintf(L"ALL Thread normal shutdown !!!! \n");
        wprintf(L"JOB QUEUE SIZE: %llu !!!! \n", g_msgQ.GetUseSize());
    }
    else
    {
        wprintf(L"WaitForMultipleObject %d error \n", GetLastError());
    }
    g_b_Shutdown = true; // 모니터링 스레드를 종료한다.
    dwStatus = WaitForSingleObject(hThread[MAX_THREAD - 1], INFINITE);
    if (dwStatus == WAIT_OBJECT_0)
    {
        wprintf(L"Monitoring Thread Shutdown !!!! \n");
    }
    else
    {
        wprintf(L"WaitForSingleObject - Monitoring: Error: %d \n", GetLastError());
    }

    /////////////////////////////////////////////////////
    // 사용한 모든 것들을 정리해준다.
    // g_List도 정리해줘야함
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

unsigned __stdcall Monitoring(void* pArg)
{
    printf("WorketThread id=0x%08x - Start!! \n", GetCurrentThreadId());

    /////////////////////////////////////////////////////
    //  @ 특정 스레드는 1초마다 현 상태 모니터링 
    //
    //  * 메시지 잡큐 의 사이즈(사용량)  출력
    //  * 초당 메시지 잡 전체 처리 수(TPS) + 스레드 별 처리수
    //  * 초당 타입별 잡 처리 수(메시지 별 TPS)
    /////////////////////////////////////////////////////

    DWORD dwStartTime;
    DWORD dwEndTime;
    DWORD tick;
    LONG Thread_TPS[MAX_THREAD];
    LONG ALL_TPS;
    unsigned int MSG_Type_Count[MAX_MSG_TYPE];

    int iCnt;

    dwStartTime = timeGetTime();
    tick = 1000;

    while (g_b_Shutdown == false)
    {
        dwEndTime = timeGetTime();

        if (dwEndTime - dwStartTime < tick)
        {
            dwStartTime += tick;
            Sleep(dwStartTime - dwEndTime);


            ALL_TPS = 0;

            printf("\nmain Thread FPS - %d\n", g_dwFPS);
            printf("-----------------------------------------------------------------------\n");
            printf("JOB Queue Size - %llu \n", g_msgQ.GetUseSize());

            for (iCnt = 0; iCnt < MAX_THREAD-1; ++iCnt)
            {
                Thread_TPS[iCnt] = InterlockedExchange(&g_TPS[iCnt], 0);
                ALL_TPS += Thread_TPS[iCnt];
                printf("WorkerThread - id=0x%08x / TPS %d \n", g_ThreadIDs[iCnt], Thread_TPS[iCnt]);
            }
            printf("WorkerThread _ ALL TPS: %d \n", ALL_TPS);

            for (iCnt = 0; iCnt < MAX_MSG_TYPE; ++iCnt)
            {
                MSG_Type_Count[iCnt] = InterlockedExchange(&g_MSG_Type_Count[iCnt], 0);
            }
            printf("dfJOB_ADD TPS - %d \n", MSG_Type_Count[0]);
            printf("dfJOB_DEL TPS - %d \n", MSG_Type_Count[1]);
            printf("dfJOB_SORT TPS - %d \n", MSG_Type_Count[2]);
            printf("dfJOB_FIND TPS - %d \n", MSG_Type_Count[3]);
            printf("dfJOB_PRINT TPS - %d \n", MSG_Type_Count[4]);
            printf("-----------------------------------------------------------------------\n");


        }
        else
        {
            // 프레임 스킵
            dwStartTime += tick;

        }
    }


    printf("WorketThread id = 0x%08x - return \n", GetCurrentThreadId());

    return 0;
}
