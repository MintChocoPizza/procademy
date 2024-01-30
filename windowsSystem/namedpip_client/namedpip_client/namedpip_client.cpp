// namedpip_client.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define BUF_SIZE 1024

int _tmain(int argc, TCHAR *argv[])
{
    HANDLE hPipe;
    TCHAR readDataBuf[BUF_SIZE + 1];
    TCHAR pipeName[] = _T("\\\\.\\pipe\\ simple_pipe");

    while (1)
    {
        hPipe = CreateFile(
            pipeName,   // 연결할 파이프 이름
            GENERIC_READ | GENERIC_WRITE,       // 일기, 쓰기 모드 동시 지정
            0,
            NULL,
            OPEN_EXISTING,
            0,
            NULL
        );
        if (hPipe != INVALID_HANDLE_VALUE)
            break;

        if (GetLastError() != ERROR_PIPE_BUSY)
        {
            _tprintf_s(_T("Could not open pipe \n"));
            return 0;
        }

        if (!WaitNamedPipe(pipeName, 20000))
        {
            _tprintf_s(_T("Could not open pipe \n"));
            return 0;
        }
    }
        DWORD pipeMode = PIPE_READMODE_MESSAGE | PIPE_WAIT;         // 메시지 기반으로 모드 변경
        BOOL isSuccess = SetNamedPipeHandleState(
            hPipe,              // 서버 파이프와 연결된 핸들
            &pipeMode,          // 변경할 모드 정보
            NULL,
            NULL
        );

        if (!isSuccess)
        {
            _tprintf_s(_T("SetNamedPipeHandleState failed"));
            return 0;
        }

        TCHAR fileName[] = _T("news.txt");
        DWORD bytesWritten = 0;
        

        // 뻑남
        isSuccess = WriteFile(
            hPipe,               // 서버 파이프와 연결된 핸들
            fileName,           // 전송할 메시지
            (_tcslen(fileName)+1) * sizeof(TCHAR),   // 메시지 길이
            &bytesWritten,          // 전송된 바이트 수
            NULL
        );

        if (!isSuccess)
        {
            _tprintf_s(_T("WriteFile failed"));
            return 0;
        }

        DWORD bytesRead = 0;
        while (1)
        {
            isSuccess = ReadFile(
                hPipe,                      // 서버 파이프와 연결된 핸들
                readDataBuf,                // 데이터 수신할 버퍼
                BUF_SIZE * sizeof(TCHAR),    // 버퍼 사이즈
                //BUF_SIZE,
                &bytesRead ,                 // 수신한 바이트 수
                NULL
            );

            if (!isSuccess && GetLastError() != ERROR_MORE_DATA)
                break;
            bytesRead /= sizeof(TCHAR);
            readDataBuf[bytesRead] = '\0';
            _tprintf_s(_T("%s \n"), readDataBuf);
            // printf_s("%s \n", readDataBuf);
        }

        CloseHandle(hPipe);
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
