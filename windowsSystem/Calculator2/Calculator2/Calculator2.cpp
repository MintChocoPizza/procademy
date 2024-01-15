// Calculator.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

enum { DIV = 1, MUL, ADD, MIN, ELSE, EXIT };

DWORD ShowMenu();
void Divide(double, double);
void Multiple(double, double);
void Add(double, double);
void Min(double, double);

int _tmain(int argc, TCHAR* argv[])
{
    STARTUPINFO si = { 0, };
    PROCESS_INFORMATION pi;
    si.cb = sizeof(si);

    TCHAR command[] = _T("calc.exe");
    SetCurrentDirectory(_T(""));    // 디렉토리 설정

    DWORD sel;
    double num1, num2;
    while (true)
    {
        sel = ShowMenu();
        if (sel == EXIT)
            return 0;

        if (sel != ELSE)
        {
            _fputts(_T("Input Num1 Num2: "), stdout);
            // _tscanf(_T("%lf %lf"), &num1, &num2);
            _tscanf_s(_T("%lf %lf"), &num1, &num2);
        }

        switch (sel)
        {
        case DIV:
            Divide(num1, num2);
            break;
        case MUL:
            Multiple(num1, num2);
            break;
        case ADD:
            Add(num1, num2);
            break;
        case MIN:
            Min(num1, num2);
            break;
        case ELSE:
            ZeroMemory(&pi, sizeof(pi));
            CreateProcess(NULL, command, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
            /*CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);*/
            break;
        }
    }
    return 0;
}

DWORD ShowMenu()
{
    DWORD sel;
    _fputts(_T("-----Menu----- \n"), stdout);
    _fputts(_T("num 1: Divide \n"), stdout);
    _fputts(_T("num 2: Multiple \n"), stdout);
    _fputts(_T("num 3: Add \n"), stdout);
    _fputts(_T("num 4: Minus \n"), stdout);
    _fputts(_T("num 5: Any other operations \n"), stdout);
    _fputts(_T("num 6: Exit \n"), stdout);
    _fputts(_T("num 7: SELECTION >>"), stdout);

    // _tscanf(_T("%d"), &sel);
    _tscanf_s(_T("%d"), &sel);

    return sel;
}
void Divide(double a, double b)
{
    _tprintf(_T("%f/%f=%f \n\n"), a, b, a / b);
}
void Multiple(double a, double b)
{
    _tprintf(_T("%f*%f=%f \n\n"), a, b, a * b);
}
void Add(double a, double b)
{
    _tprintf(_T("%f+%f=%f \n\n"), a, b, a + b);
}
void Min(double a, double b)
{
    _tprintf(_T("%f-%f=%f \n\n"), a, b, a - b);
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
