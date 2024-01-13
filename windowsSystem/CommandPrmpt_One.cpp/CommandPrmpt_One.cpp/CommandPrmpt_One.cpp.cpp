// CommandPrmpt_One.cpp.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <locale.h>
#include <Windows.h> 

#define STR_LEN 256
#define CMD_TOKEN_NUM 10

TCHAR ERROR_CMD[] = _T("'%s'은(는) 실행할 수 있는 프로그램이 아닙니다. \n");

int CmdProcessing(void);
TCHAR* StrLower(TCHAR*);

int _tmain(int argc, TCHAR *argv[])
{
    // 한글 입력을 가능케 하기 위해
    _tsetlocale(LC_ALL, _T("Korean"));

    DWORD isExit;
    while (1)
    {
        isExit = CmdProcessing();
        if (isExit == TRUE)
        {
            _fputts(_T("명렁어 처리를 종료합니다. \n"), stdout);
            break;
        }
    }
    return 0;
}

TCHAR cmdString[STR_LEN];
TCHAR cmdTokenList[CMD_TOKEN_NUM][STR_LEN];
TCHAR seps[] = _T(" ,\t\n");
TCHAR* next_token = nullptr;

int CmdProcessing(void)
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

    if (!_tcscmp(cmdTokenList[0], _T("exit")))
    {
        return TRUE;
    }
    else if (!_tcscmp(cmdTokenList[0], _T("추가 되는 명령어 1")))
    {
    }
    else if (!_tcscmp(cmdTokenList[0], _T("추가 되는 명령어 2")))
    {
    }
    else
    {
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
