// DynamicArray.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define MAX_PAGE 10         // 할당할 최대 페이지 개수

int* nextPageAddr;
DWORD pageCnt = 0;
DWORD pageSize;
int PageFaultExceptionFilter(DWORD);

int _tmain(int argc, TCHAR * argv[])
{
    LPVOID baseAddr;
    int* lpPtr;
    SYSTEM_INFO sSysInfo;

    GetSystemInfo(&sSysInfo);
    pageSize = sSysInfo.dwPageSize;     // 페이지 사이즈 획득


    // MAX_PAGE의 갯수만큼 페이지 RESERVE!
    baseAddr = VirtualAlloc(
        NULL,                       // 임의 주소 할당
        MAX_PAGE * pageSize,        // 예약 메모리 크기
        MEM_RESERVE,                // RESERVE!
        PAGE_NOACCESS               // NO ACCCESS
    );
    if (baseAddr == NULL)
        _tprintf_s(_T("VirtualAlloc reserve failed"));
    
    lpPtr = (int*)baseAddr;             // 배열의 시작 번지와 같은 의미 부여
    nextPageAddr = (int*)(baseAddr);




    // page fault 발생 시 예외발생
    for (int i = 0; i < (MAX_PAGE * pageSize) / sizeof(int); i++)
    {
        __try
        {
            lpPtr[i] = i;
        }
        __except (PageFaultExceptionFilter(GetExceptionCode()))
        {
            ExitProcess(GetLastError());        //예외처리 문제 발생 시 종료!
        }
    }

    //for (int i = 0; i < (MAX_PAGE * pageSize) / sizeof(int); i++)
        //_tprintf_s(_T("%d"), lpPtr[i]);
    BOOL isSuccess = VirtualFree(
        baseAddr,                           // 해제할 메모리의 시작 번지
        0,
        MEM_RELEASE                         // FREE 상태로 변경
    );
    if (isSuccess)
        _tprintf_s(_T("Release succeeded!"));
    else
        _tprintf_s(_T("Release failed!"));
}

int PageFaultExceptionFilter(DWORD exptCode)
{
    // 예외의 원인이 'page fault'가 아니라면
    if (exptCode != EXCEPTION_ACCESS_VIOLATION)
    {
        _tprintf_s(_T("Exception code = %d \n"), exptCode);
        return EXCEPTION_EXECUTE_HANDLER;
    }
    _tprintf_s(_T("Exception is a page fault \n"));

    if (pageCnt >= MAX_PAGE)
    {
        _tprintf_s(_T("Exception: out of pages \n"));
        return EXCEPTION_EXECUTE_HANDLER;
    }

    LPVOID lpvResult = VirtualAlloc(
        (LPVOID)nextPageAddr,
        pageSize,
        MEM_COMMIT,
        PAGE_READWRITE
    );
    if (lpvResult == NULL)
    {
        _tprintf_s(_T("VirtualAlloc failed \n"));
        return EXCEPTION_EXECUTE_HANDLER;
    }
    else
    {
        _tprintf_s(_T("Allocating another page. \n"));
    }

    pageCnt++;
    nextPageAddr += pageSize / sizeof(int);         // 실수 주의
    //page fault가 발생한 지점부터 실행을 계속
    return EXCEPTION_CONTINUE_EXECUTION;
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
