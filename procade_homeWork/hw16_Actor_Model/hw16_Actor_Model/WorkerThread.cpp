
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
extern SRWLOCK g_srwlock_G_LIST;
extern SRWLOCK g_srwlock_G_MSGQ;

//-----------------------------------------------
// 자동 리셋 이벤트 객체
//-----------------------------------------------
extern HANDLE g_h_AutoResetEvent;

//-----------------------------------------------
// 컨텐츠 부, 문자열 리스트
//-----------------------------------------------
extern std::list<std::wstring*> g_List;

//-----------------------------------------------
// 스레드 메시지 큐 (사이즈 넉넉하게 크게 4~5만 바이트)
//-----------------------------------------------
extern C_RING_BUFFER g_msgQ;

//-----------------------------------------------
// 임의의 문자열
//-----------------------------------------------
extern const wchar_t* strList[];

//-----------------------------------------------
// 임의의 문자열 길이
//-----------------------------------------------
extern int g_strListLen;

//-----------------------------------------------
// 메인 스레드에서 일이 생성되는 주기
//-----------------------------------------------
extern unsigned int g_WorkGenerationCycle;

//-----------------------------------------------
// 프로그램 종료 플래그
//-----------------------------------------------
extern bool g_b_Shutdown;

//-----------------------------------------------
// 초당 메시지 잡 스레드별 처리수
//-----------------------------------------------
extern LONG g_TPS[MAX_THREAD];

//-----------------------------------------------
// 스레드 아이디 저장
//-----------------------------------------------
extern unsigned int g_ThreadIDs[MAX_THREAD];

//-----------------------------------------------
// 메시지 타입별 처리 수
//-----------------------------------------------
extern unsigned int g_MSG_Type_Count[MAX_MSG_TYPE];



unsigned __stdcall WorkerThread(void* pArg)
{
    // 메인스레드 에서 메시지를 넣지 않았다면 블락됨
    DWORD dwStatus;
    st_MSG_HEAD st_Header;
    bool b_Shutdown;
    std::list<std::wstring*>::iterator iter_G_LIST;

    C_RING_BUFFER rb_Temp;
    std::wstring* ws_p_Temp;

    int i_My_Thread_Num;
    wchar_t *wc_Temp;


    //----------------------------------------------------
    // 스레드 시작 로그 ID 출력
    //----------------------------------------------------
    printf("WorketThread id=0x%08x - Start!! \n", GetCurrentThreadId());
    b_Shutdown = false;
    i_My_Thread_Num = (int)pArg;
    srand((unsigned)GetCurrentThreadId());

    while (b_Shutdown == false)
    {
        //----------------------------------------------------
        // Lock은 필수로 걸어야 한다. 
        // 
        // 1. 메시지 큐에서 메시지를 뽑는다. 
        // 2. 헤더 타입에 맞게끔 동작
        // 
        // 어떻게 느린 동작을 최대한 빠르게 할까??
        // -> 1. 메시지 큐에 동기화를 걸고 메시지를 뺀다. 
        // -> 2. 작업을 진행한다. 
        // -> 3. 동기화를 걸고 작업을 저장한다.
        //----------------------------------------------------



        dwStatus = WaitForSingleObject(g_h_AutoResetEvent, INFINITE);
        if (dwStatus != WAIT_OBJECT_0)
        {
            printf("WorketThread id = 0x%08x - Error: %d \n", GetCurrentThreadId(), GetLastError());
        }

        // 실행 조건 체크
        // 1. 깨어났으면 무조건 동기화 객체를 얻어야 한다. 
        AcquireSRWLockExclusive(&g_srwlock_G_MSGQ);
        // 2. 깨어 났으면 데이터가 있는지 확인한다. 
        // 어떻게 보면 TryLock 이랑 비슷하다.
        if (g_msgQ.GetUseSize() == 0)
        {
            // 깨어 났지만 할 일이 없다면 다시 블락된다. 
            ReleaseSRWLockExclusive(&g_srwlock_G_MSGQ);
            continue;
        }

        //----------------------------------------------------
        // dfJOB_OUT인 경우 메시지를 빼지 않는다. 
        // 
        // 메시지를 스레드 개인 버퍼에 먼저 저장한뒤 
        // 
        // 나중에 처리한다.
        //----------------------------------------------------
        g_msgQ.Peek((char*)&st_Header, sizeof(st_MSG_HEAD));
        if (st_Header.shType != dfJOB_QUIT)
        {
            g_msgQ.MoveFront(sizeof(st_MSG_HEAD));

            memcpy(rb_Temp.GetRearBufferPtr(), g_msgQ.GetFrontBufferPtr(), st_Header.shPayloadLen);
            g_msgQ.MoveFront(st_Header.shPayloadLen);
        }
        ReleaseSRWLockExclusive(&g_srwlock_G_MSGQ);


        switch (st_Header.shType)
        {
        case dfJOB_ADD:
            ws_p_Temp = new std::wstring();
            ws_p_Temp->assign((wchar_t*)rb_Temp.GetFrontBufferPtr());
            rb_Temp.MoveFront(st_Header.shPayloadLen);

            AcquireSRWLockExclusive(&g_srwlock_G_LIST);
            g_List.push_back(ws_p_Temp);
            ReleaseSRWLockExclusive(&g_srwlock_G_LIST);
            break;

        case dfJOB_DEL:
            ws_p_Temp = new std::wstring();
            ws_p_Temp->assign((wchar_t*)rb_Temp.GetFrontBufferPtr());
            rb_Temp.MoveFront(st_Header.shPayloadLen);

            AcquireSRWLockExclusive(&g_srwlock_G_LIST);
            for (iter_G_LIST = g_List.begin(); iter_G_LIST != g_List.end(); )
            {
                if ((**iter_G_LIST) != *ws_p_Temp)
                    ++iter_G_LIST;
                else
                {
                    g_List.erase(iter_G_LIST);
                    break;
                }
            }
            ReleaseSRWLockExclusive(&g_srwlock_G_LIST);
            break;

        case dfJOB_SORT:
            // 정렬하면 순서가 바뀌기 때문에 당연히 lock을 걸어야 한다.
            AcquireSRWLockExclusive(&g_srwlock_G_LIST);
            g_List.sort();
            ReleaseSRWLockExclusive(&g_srwlock_G_LIST);
            break;

        case dfJOB_FIND:
            ws_p_Temp = new std::wstring();
            ws_p_Temp->assign((wchar_t*)rb_Temp.GetFrontBufferPtr());
            rb_Temp.MoveFront(st_Header.shPayloadLen);

            AcquireSRWLockExclusive(&g_srwlock_G_LIST);
            for (iter_G_LIST = g_List.begin(); iter_G_LIST != g_List.end(); )
            {
                if ((**iter_G_LIST) != *ws_p_Temp)
                    ++iter_G_LIST;
                else
                {
                    break;
                }
            }
            ReleaseSRWLockExclusive(&g_srwlock_G_LIST);
            break;

        case dfJOB_PRINT:
            //-----------------------------------------------
            // 작업이 길 경우 쉐어드로 List를 복사하여
            // 
            // 작업을 따로 진행한다.
            //-----------------------------------------------
            AcquireSRWLockShared(&g_srwlock_G_LIST);
            for (iter_G_LIST = g_List.begin(); iter_G_LIST != g_List.end();)
            {
                wc_Temp = (wchar_t*)(*iter_G_LIST)->c_str();
                rb_Temp.Enqueue((char*)wc_Temp, wcslen(wc_Temp));
                ++iter_G_LIST;
            }
            ReleaseSRWLockShared(&g_srwlock_G_LIST);
            for (int i = 0; i < 1000000000; ++i)
            {
                int a = 0;
            }

            break;

        case dfJOB_QUIT:
            b_Shutdown = true;
            break;
        }

        //----------------------------------------------------
        // 모든 스레드가 동작중일 때, 여러개의 작업과 시그널이 생성되면 
        // 
        // 그 중 1개의 시그널만 작동한다. 
        // 
        // 그러므로 모든 스레드는 작업이 끝나면, 다른 스레드르 깨우고
        // 
        // 깨어난 스레드는 할 일이 있는지 없는지 판단하여 다시 블락된다.
        //----------------------------------------------------
        SetEvent(g_h_AutoResetEvent);

        //----------------------------------------------------
        // 모니터링용 로그 저장
        //----------------------------------------------------
        InterlockedIncrement(&g_TPS[i_My_Thread_Num]);
        InterlockedIncrement(&g_MSG_Type_Count[st_Header.shType]);

        rb_Temp.ClearBuffer();
    }

    printf("WorketThread id = 0x%08x - return \n", GetCurrentThreadId());
    return 0;
}