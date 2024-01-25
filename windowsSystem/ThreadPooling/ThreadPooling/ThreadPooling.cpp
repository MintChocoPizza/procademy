// ThreadPooling.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define WORK_MAX 10000
#define THREAD_MAX 50

typedef void (*WORK) (void); // 간편하게 함수 포인터를 사용할 수 있다. 

DWORD AddWorkToPool(WORK work);                 // 새로운 Work을 등록할 때 호출하는 함수이다.
WORK GetWorkFromPool(void);                     // 쓰레드 풀에서 Work을 가져올 때 호출하는 함수이다.
DWORD MakeThreadToPool(DWORD numOfThread);      // 쓰레드 풀이 생성된 이후에 풀에 쓰레드를 생성(등록)하는 함수이다. 인자로 전달되는 수 만큼 쓰레드가 생성된다.
void WorkerThreadFunction(LPVOID pParam);       // 쓰레드가 생성되자마자 호출하는 쓰레드의 main함수이다. 이 함수의 구성을 봐야만 어떻게 Work을 할당받아서 처리하는지, 그리고 Work이 없을 때의 쓰레드 상태들을 알 수 있다.

typedef struct __WorkerThread
{
    HANDLE hThread;
    DWORD idThread;
} WorkerThread;

// Work와 Thread 관리를 위한 구조체
struct __ThreadPool
{
    // Work을 등록하기 위한 배열 
    WORK workList[WORK_MAX];

    // Thread 정보와 각 Thread별 Event 오브젝트
    WorkerThread workerThreadList[THREAD_MAX];      // Thread만 핸들과 ID를 한번에 보관함. 사실 그에 해당하는 이벤트도 같이 보관해도 되보임
    HANDLE workerEventList[THREAD_MAX];

    // Work에 대한 정보
    DWORD idxOfCurrentWork;             // 대기 1순위 Work Index
    DWORD idxOfLastAddedWork;           // 마지막 추가 Work + 1

    // Number of Thread;
    DWORD threadIdx;                    // Pool에 존재하는 Thread의 개수

    TCHAR treadName[THREAD_MAX][100];
} gThreadPool;


// Mutex 관련 함수들
// gThreadPool에 대한 접글은 보호하기 위해 정의
static HANDLE mutex = NULL;

void InitMutex(void)
{
    mutex = CreateMutex(NULL, FALSE, NULL);
}

void DeInitMutex(void)
{
    BOOL ret = CloseHandle(mutex);
}

void AcquireMutex(void)
{
    DWORD ret = WaitForSingleObject(mutex, INFINITE);
    if (ret == WAIT_FAILED)
        _tprintf_s(_T("Error Occur! \n"));
}

void ReleaseMutex(void)
{
    BOOL ret = ReleaseMutex(mutex);
    if (ret == NULL)
        _tprintf_s(_T("Error Occur! \n"));
}







// Thread Pool에 Work을 등록시키기 위한 함수 
DWORD AddWorkToPool(WORK work)
{
    AcquireMutex();
    if (gThreadPool.idxOfLastAddedWork >= WORK_MAX)
    {
        _tprintf_s(_T("AddWorkToPool fail! \n"));
        return NULL;
    }

    // Work 등록
    gThreadPool.workList[gThreadPool.idxOfLastAddedWork++] = work;

    // Work 등록 후, 대기 중인 쓰레드들을 모두 깨워 일을 시작하도록 함
    // 모두 깨울 필요가 없으므로 정교함이 떨어지는 부분이다. 
    for (DWORD i = 0; i < gThreadPool.threadIdx; i++)
        SetEvent(gThreadPool.workerEventList[i]);

    ReleaseMutex();
    return 1;
}

// Thread에서 Work을 가져올 때 호출되는 함수
// gThreadPool 에 대한 접근을 보호하기 위해 정의
WORK GetWorkFromPool()
{
    WORK work = NULL;

    AcquireMutex();
    // 현재 처리해야 할 Work이 존재하지 않는다면 '!'
    if (!(gThreadPool.idxOfCurrentWork < gThreadPool.idxOfLastAddedWork))
    {
        ReleaseMutex();
        return NULL;
    }

    work = gThreadPool.workList[gThreadPool.idxOfCurrentWork++];
    ReleaseMutex();

    return work;
}

// Thread Pool 생성
// 전달되는 인자값에 따라 Thread 생성
DWORD MakeThreadToPool(DWORD numOfThread)
{
    InitMutex();
    DWORD capacity = WORK_MAX - (gThreadPool.threadIdx);        // 현재 생성할 수 있는 Thread 수

    if (capacity < numOfThread)                                 // 현재 생성할 수 있는 수 보다 많이 원한다면, 수를 줄인다.
        numOfThread = capacity;

    for (DWORD i = 0; i < numOfThread; i++)
    {
        DWORD idThread;
        HANDLE hThread;

        // 이게 맞나??? i가 들어가는게 맞는거 아닌가? ㄴㄴ 현재 생성된 쓰레드가 index가 되어야함
        gThreadPool.workerEventList[gThreadPool.threadIdx] = CreateEvent(NULL, FALSE, FALSE, NULL);
        // gThreadPool.workerEventList[i] = CreateEvent(NULL, FALSE, FALSE, NULL);

        hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WorkerThreadFunction, (LPVOID)gThreadPool.threadIdx, 0, &idThread);

        gThreadPool.workerThreadList[gThreadPool.threadIdx].hThread = hThread;
        // gThreadPool.workerThreadList[i].hThread = hThread;
        gThreadPool.workerThreadList[gThreadPool.threadIdx].idThread = idThread;
        // gThreadPool.workerThreadList[i].idThread = idThread;

        // 쓰레드에 이름을 넣는다.
        _stprintf_s(gThreadPool.treadName[gThreadPool.threadIdx], _T("This thread name is %d. "), gThreadPool.threadIdx);

        gThreadPool.threadIdx++;
    }
    return numOfThread;             // 생성된 쓰레드의 수를 반환한다.
}

// 전달되어야 할 대상
void WorkerThreadFunction(LPVOID pParam)
{
    WORK workFunction;
    HANDLE event = gThreadPool.workerEventList[(DWORD)pParam];

    while (1)
    {
        workFunction = GetWorkFromPool();
        if (workFunction == NULL)
        {
            // Work가 할당될 때까지 Blocked 상태에 있게 된다
            WaitForSingleObject(event, INFINITE);
            continue;
        }
        workFunction();
    }
}


// Simple Work Function
void TestFunction()
{
    // i는 static이므로 둘 이상의 쓰레드에 의해 동시접근 가능하다
    // 따라서 동기화가 필요하지만 동기화를 생략한다

    static int i = 0;
    i++;

    _tprintf_s(_T("Good Test: --%d __ Processing thread: --%d __ \n\n"), i, GetCurrentThreadId());
}
int _tmain(int argc, TCHAR * argv[])
{
    MakeThreadToPool(3);

    // 다수의 Work을 등록!
    for (int i = 0; i < 100; ++i)
    {
        AddWorkToPool(TestFunction);
    }

    Sleep(50000); // main 쓰레드가 먼저 소멸되는 현상 방지 위해
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
