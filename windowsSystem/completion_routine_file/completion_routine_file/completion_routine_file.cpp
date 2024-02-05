// completion_routine_file.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

TCHAR strData[] =
_T("Nobody was farther off base than the pundits who said \n")
_T("Royal Liverpool was outdated and not worthy of hosting ~ \n")
_T("for the first time since 1967. The Hoylake track ~ \n")
_T("Here's the solution to modern golf technology -- firm, \n")
_T("fast fairways, penal bunkers, firm greens and, with any ~\n");

VOID WINAPI FileIOCompletionRoutine(DWORD, DWORD, LPOVERLAPPED);

int _tmain(int argc, TCHAR * argv[])
{
    TCHAR fileName[] = _T("data.txt");
    HANDLE hFile = CreateFile(fileName, GENERIC_WRITE, FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, 0);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        _tprintf_s(_T("File creation fault! \n"));
        return -1;
    }

    OVERLAPPED overlappenInst;
    memset(&overlappenInst, 0, sizeof(overlappenInst));
    overlappenInst.hEvent = (HANDLE)1234;
    WriteFileEx(hFile, strData, sizeof(strData), &overlappenInst, FileIOCompletionRoutine);

    // SleepEx(INFINITE, TRUE);
    CloseHandle(hFile);
    return 1;
}

VOID WINAPI FileIOCompletionRoutine(DWORD errorCode, DWORD numOfBytesTransfered, LPOVERLAPPED overlapped)
{
    _tprintf_s(_T("***********File write result ***************** \n"));
    _tprintf_s(_T("Error code: %u, \n", errorCode));
    _tprintf_s(_T("Transgered bytes len: %u \n"), numOfBytesTransfered);
    _tprintf_s(_T("The other info: %u \n"), (DWORD)overlapped->hEvent);
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
