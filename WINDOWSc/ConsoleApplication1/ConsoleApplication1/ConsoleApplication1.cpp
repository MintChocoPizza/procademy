// ConsoleApplication1.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include "../../source/CommonFiles/CmnHdr.h"

extern "C" const IMAGE_DOS_HEADER __ImageBase;

void DumpModule()
{
    // 수행 중인 애플리케이션의 시작 주소을 가져온다. 
    // 수행 중인 코드가 DLL 내에 있는 경우 다른 값이 얻어질 수 있다. 
    HMODULE hModule = GetModuleHandle(NULL);
    _tprintf(_T("with getModuleHandle(NULL) = 0x%x\r\n"), hModule);

    // 현재 모듀의 시작 주소 (hModule/ hInstance)를 얻기 위해 
    // 가상변수인 __ImageBase를 사용한다.
    _tprintf(_T("with __ImageBase = 0x%x\r\n"), (HINSTANCE)&__ImageBase);

    // 현재 수행중인 DumpModule의 주소를 GetModuleHandleEx에 매개 변수로 
    // 전달하여 현재 수행 중인 모듈의 시작 주소 (hModule/ hInstance)를 얻어온다.
    hModule = NULL;
    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (PCTSTR)DumpModule, &hModule);
    _tprintf(_T("with GetModuleHandleEx = 0x%x\r\n"), hModule);
}

int _tmain(int argc, TCHAR * argv[])
{
    DumpModule();

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
