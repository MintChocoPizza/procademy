// namedpipe_asynch_server.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define BUF_SIZE 1024
int CommToClient(HANDLE);

int _tmain(int argc, TCHAR* argv[])
{
    TCHAR name[] = _T("\\\\.\\pipe\\ simple_pipe");
    LPTSTR pipeName = name;

    HANDLE hPipe;
    while (1)
    {
        hPipe = CreateNamedPipe(pipeName,
            PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,  // 비동기 특성 부여
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
            PIPE_UNLIMITED_INSTANCES,
            BUF_SIZE / 2,         // 출력 버퍼 사이즈
            //BUF_SIZE,         // 출력 버퍼 사이즈
            BUF_SIZE / 2,         // 입력 버퍼 사이즈
            //BUF_SIZE,         // 입력 버퍼 사이즈
            20000, NULL);

        if (hPipe == INVALID_HANDLE_VALUE)
        {
            _tprintf_s(_T("CreateNamedPipe failed"));
            return -1;
        }

        BOOL isSuccess;
        isSuccess = ConnectNamedPipe(hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);
        if (isSuccess)
            CommToClient(hPipe);
        else
            CloseHandle(hPipe);
    }
    return 1;
}

int CommToClient(HANDLE hPipe)
{
    TCHAR fileName[MAX_PATH];
    TCHAR dataBuf[BUF_SIZE];
    BOOL isSuccess;
    DWORD fileNameSize;

    isSuccess = ReadFile(hPipe, fileName, 
        // MAX_PATH * sizeof(TCHAR),
        MAX_PATH,
        &fileNameSize, NULL);

    if (!isSuccess || fileNameSize == 0)
    {
        _tprintf_s(_T("Pipe read message error! \n"));
        return -1;
    }

    // FILE* filePtr = _tfopen(fileName, _T("r"));
    FILE* filePtr;

    // UTF-8로 읽기
    errno_t err = _tfopen_s(&filePtr, fileName, _T("r, ccs=UTF-8"));
    if (err != NULL)
    {
        _tprintf_s(_T("File open fault! \n"));
        return -1;
    }

    OVERLAPPED overlappedInst;
    memset(&overlappedInst, 0, sizeof(overlappedInst));
    overlappedInst.hEvent = CreateEvent(
        NULL,       // default security attribute
        TRUE,       // manual-reset event
        TRUE,       // initial state = non-signaled
        NULL        // unnamed event object
    );
    DWORD bytesWritten = 0, bytesRead = 0;
    DWORD bytesWrite = 0, bytesTreansfer = 0;

    while (!feof(filePtr))
    {
        bytesRead = fread(dataBuf, 1, BUF_SIZE, filePtr);
        bytesWrite = bytesRead;
        isSuccess = WriteFile(hPipe, dataBuf, bytesWrite, &bytesWritten, 
            //&overlappedInst
            NULL
        );
        
        if (!isSuccess && GetLastError() != ERROR_IO_PENDING)
        {
            _tprintf_s(_T("Pipe write message error! \n"));
            break;
        }

        // ********* 다른 작업을 할 수 있는 기회
        WaitForSingleObject(overlappedInst.hEvent, INFINITE);
        GetOverlappedResult(hPipe, &overlappedInst, &bytesTreansfer, FALSE);
        _tprintf_s(_T("Transferred data size: %u \n"), bytesTreansfer);
    }

    FlushFileBuffers(hPipe);
    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);
    return 1;
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
