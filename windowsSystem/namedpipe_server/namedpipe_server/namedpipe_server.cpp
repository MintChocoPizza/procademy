// namedpipe_server.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define BUF_SIZE 1024
int CommToClient(HANDLE);

int _tmain(int argc, TCHAR * argv[])
{
    TCHAR pipeName[] = _T("\\\\.\\pipe\\ simple_pipe");
    HANDLE hPipe;
    
    while (1)
    {
        hPipe = CreateNamedPipe(
            pipeName,               // 파이프 이름
            PIPE_ACCESS_DUPLEX,         // 읽기, 쓰기 모드 지정
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,      // 메시지 기반
            PIPE_UNLIMITED_INSTANCES,       // 최대 파이프 개수
            BUF_SIZE,                       // 출력 버퍼 사이즈
            BUF_SIZE,                       // 입력 버퍼 사이즈
            20000,                          // 클라이언트 타임-아웃
            NULL                            // 디폴트 보안 속성
        );

        if (hPipe == INVALID_HANDLE_VALUE)
        {
            _tprintf_s(_T("CreatePipe failed"));
            return -1;
        }

        BOOL isSuccess = 0;
        isSuccess = ConnectNamedPipe(hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

        if (isSuccess)
            CommToClient(hPipe);
        else
            CloseHandle(hPipe);
    }

    return 1;
}


// 1. txt로 쓰여진 데이터는 utf-8
int CommToClient(HANDLE hPipe)
{
    TCHAR fileName[MAX_PATH];
    DWORD dataBuf[BUF_SIZE];

    BOOL isSuccess;
    DWORD fileNameSize;
    isSuccess = ReadFile(
        hPipe,                  // 파이프 핸들
        fileName,               // read 버퍼 지정
        MAX_PATH,               // read 버퍼 사이즈
        &fileNameSize,          // 수신한 데이터 크기
        NULL                    
    );

    if (!isSuccess || fileNameSize == 0)
    {
        _tprintf_s(_T("Pipe read messageerror! \n"));
        return -1;
    }

    FILE* filePtr;
    errno_t err = _tfopen_s(&filePtr, fileName, _T("r"));
    if (err != NULL)
    {
        _tprintf_s(_T("File open fault! \n"));
        return -1;
    }

    DWORD bytesWritten = 0;
    DWORD bytesRead = 0;

    while (!feof(filePtr))
    {
        bytesRead = fread(dataBuf, 1, BUF_SIZE, filePtr);

        WriteFile(
            hPipe,              // 파이프 핸들
            dataBuf,            // 전송할 데이터 버퍼
            bytesRead,          // 전송할 데이터 크기
            &bytesWritten,      // 전송된 데이터 크기
            NULL
        );
        if (bytesRead != bytesWritten)
        {
            _tprintf_s(_T("Pipe write message error! \n"));
            break;
        }
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
