// CommandPrmpt_Four.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <tchar.h>
#include <locale>
#include <Windows.h>
#include <TlHelp32.h>

#define STR_LEN 256
#define CMD_TOKEN_NUM 10

TCHAR ERROR_CMD[] = _T("'%s'은(는) 실행할 수 있는 프로그램이 아닙니다. \n");

TCHAR cmdString[STR_LEN];
TCHAR cmdTokenList[CMD_TOKEN_NUM][STR_LEN];
TCHAR seps[] = _T(" ,\t\n");
TCHAR* next_token = nullptr;

int CmdReadTokenize(void);
int CmdProcessing(int);
TCHAR* StrLower(TCHAR*);
void ListProcessInfo(void);
void KillProcess(void);

int _tmain(int argc, TCHAR* argv[])
{
    // 한글 입력을 가능케 하기 위해
    _tsetlocale(LC_ALL, _T("Korean"));

    // 매개변수 전달있는 경우 처리
    if (argc > 2)
    {
        for (int i = 1; i < argc; i++)
            _tcscpy_s(cmdTokenList[i - 1], STR_LEN, argv[i]);
        CmdProcessing(argc - 1);
    }

    DWORD isExit = NULL;
    while (1)
    {
        int tokenNum = CmdReadTokenize();

        if (tokenNum == 0) // 공백(Enter) 입력
            continue;

        isExit = CmdProcessing(tokenNum);
        if (isExit == TRUE)
        {
            _fputts(_T("명렁어 처리를 종료합니다. \n"), stdout);
            break;
        }
    }
    return 0;
}

void ListProcessInfo(void)
{
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        _tprintf_s(_T("CreateToolhelp32Snapshot error! \n"));
        return;
    }
    PROCESSENTRY32 pe32;

    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap, &pe32))
    {
        _tprintf_s(_T("Process32First error! \n"));
        CloseHandle(hProcessSnap);
        return ;
    }

    do
    {
        _tprintf_s(_T("%25s %5d \n"), pe32.szExeFile, pe32.th32ParentProcessID);
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
}

void KillProcess(void)
{
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hProcessSnap == INVALID_HANDLE_VALUE)
    {
        _tprintf_s(_T("CreateToolhelp32Snapshot error! \n"));
        return ;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap, &pe32))
    {
        _tprintf_s(_T("Process32First error! \n"));
        CloseHandle(hProcessSnap);
        return ;
    }

    HANDLE hProcess;
    BOOL isKill = FALSE;

    do
    {
        // 입력된 프로세스 이름과 비교한다. 
        if (_tcscmp(pe32.szExeFile, cmdTokenList[1]) == 0)
        {
            // 프로세스 ID를 핸들로 변환하는 과정
            hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);

            if (hProcess != NULL)
            {
                // 프로세스를 강제 종료 시킨다. 
                TerminateProcess(hProcess, -1);
                isKill = TRUE;
            }

            CloseHandle(hProcess);
            break;
        }
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    if (isKill == FALSE)
        _tprintf_s(_T("Kill process fail, Try again! \n"));
}

// 명령어를 받기, 명령어 분리, 명령어 갯수 확인
int CmdReadTokenize(void)
{
    _fputts(_T("Best command prompt>> "), stdout);
    _fgetts(cmdString, STR_LEN, stdin);

    TCHAR* token = _tcstok_s(cmdString, seps, &next_token);

    int tokenNum = 0;
    while (token != NULL)
    {
        // _tcscpy(cmdTokenList[tokenNum++], StrLower(token));
        _tcscpy_s(cmdTokenList[tokenNum++], STR_LEN, StrLower(token));
        token = _tcstok_s(NULL, seps, &next_token);
    }

    return tokenNum;
}

int CmdProcessing(int tokenNum)
{
    BOOL isRun;
    STARTUPINFO si = { 0, };
    PROCESS_INFORMATION pi;

    TCHAR cmdStringWithOptions[STR_LEN] = { 0, };
    TCHAR optString[STR_LEN] = { 0, };

    si.cb = sizeof(si);

    if (!_tcscmp(cmdTokenList[0], _T("exit")))
    {
        return TRUE;
    }
    else if (!_tcscmp(cmdTokenList[0], _T("start")))
    {
        if (tokenNum > 1) // start echo test good 와 같은 입력을 처리하기 위하여
        {
            for (int i = 1; i < tokenNum; i++)
                _stprintf_s(optString, _T("%s %s"), optString, cmdTokenList[i]); // optString 에 cmdTokenList[i] 문자를 "%s %s" 형식으로 채워서 저장

            _stprintf_s(cmdStringWithOptions, _T("%s %s"), _T("CmdProject.exe"), optString);
        }
        else
        {
            // start만 입력되는 경우
            _stprintf_s(cmdStringWithOptions, _T("%s %s"), _T("CmdProject.exe"));
            isRun = CreateProcess(NULL, cmdStringWithOptions, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
    }
    else if (!_tcscmp(cmdTokenList[0], _T("echo")))
    {
        TCHAR command[] = _T("echo message: %s \n");
        for (int i = 1; i < tokenNum; ++i)
            _stprintf_s(command, optString, cmdTokenList[i]);
    }
    else if (!_tcscmp(cmdTokenList[0], _T("lp")))
    {
        ListProcessInfo();
    }
    else if (!_tcscmp(cmdTokenList[0], _T("kp")))
    {
        if (tokenNum < 2)
        {
            _tprintf_s(_T("usage: kp <process name> \n"));
            return 0;
        }
        KillProcess();
    }
    else
    {
        _tcscpy_s(cmdStringWithOptions, cmdTokenList[0]);

        for (int i = 1; i < tokenNum; i++)
            _stprintf_s(cmdStringWithOptions, _T("%s %s"), cmdStringWithOptions, cmdTokenList[i]);

        isRun = CreateProcess(NULL, cmdStringWithOptions, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);

        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);

        if (isRun == FALSE)
            _tprintf(ERROR_CMD, cmdTokenList[0]);
    }
    return 0;
}

TCHAR* StrLower(TCHAR* pStr)
{
    TCHAR* ret = pStr;

    while (*pStr)
    {
        if (_istupper(*pStr))
            *pStr = _totlower(*pStr);
        pStr++;
    }
    return ret;
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
