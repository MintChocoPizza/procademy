﻿// CreateProcess.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define DIR_LEN MAX_PATH+1

int _tmain(int argc, TCHAR *argv[])
{
    STARTUPINFO si = { 0, };
    PROCESS_INFORMATION pi;

    si.cb = sizeof(si);
    si.dwFlags = STARTF_USEPOSITION | STARTF_USESIZE;
    si.dwX = 100;
    si.dwY = 200;
    si.dwXSize = 300;
    si.dwYSize = 200;
    TCHAR title[] = _T("I am a boy!");
    si.lpTitle = title;

    TCHAR command[] = _T("AdderProcess.exe 10 20");
    TCHAR cDir[DIR_LEN];
    BOOL state;

    GetCurrentDirectory(DIR_LEN, cDir);     // 현재 디렉터리 확인
    _fputts(cDir, stdout);
    _fputts(_T("\n"), stdout);

    SetCurrentDirectory(_T("C:\\Users\\User\\Desktop\\procademy_project\\procademy\\windowsSystem\\AdderProcess\\x64\\Debug"));

    GetCurrentDirectory(DIR_LEN, cDir);     // 현재 디렉터리 확인
    _fputts(cDir, stdout);
    _fputts(_T("\n"), stdout);

    state = CreateProcess(NULL, command, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

    if (state != 0)
        _fputts(_T("Creation OK! \n"), stdout);
    else
        _fputts(_T("Creation Error! \n"), stdout);

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
