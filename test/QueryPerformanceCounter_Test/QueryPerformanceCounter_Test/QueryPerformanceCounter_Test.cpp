// QueryPerformanceCounter_Test.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include <time.h>

#pragma comment (lib, "winmm.lib")



//// 안됨..... 실패함

int main()
{
    timeBeginPeriod(1);

    HKEY hKey;                      // 레지스트리 키에 대한 핸들 선언 
    DWORD dwMHz;                    // CPU의 기본 클럭을 저장할 변수 선언 
    DWORD dwSize = sizeof(dwMHz);   // 변수의 크기를 저장할 변수 선언 

    // 레지스트리 키를 엍음 
    LONG lError = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
        L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 
        0,
        KEY_READ,
        &hKey);
    if (lError != ERROR_SUCCESS)         // 레지스트리 키 열기에 실패한 경우
    {
        std::cout << "Failed to open registry key. Error code = 0x" << std::hex << lError << std::endl;
        return 1;   // 프로그램 종료
    }

    // 레지스트리 키의 -MHz 값에 접근 
    lError = RegQueryValueEx(hKey,
        L"-MHz",
        NULL,
        NULL,
        (LPBYTE)&dwMHz,
        &dwSize);

    if (lError != ERROR_SUCCESS)         // 레지스트리 값 읽기에 실패한 경우
    {
        std::cout << "Failed to read registry value. Error code = 0x"
            << std::hex << lError << std::endl;
        RegCloseKey(hKey);              // 레지스트리 키 닫음
        return 1;                       // 프로그램 종료
    }

    // 레지스트리 키를 닫음
    RegCloseKey(hKey);

    // CPU의 기본 클럭을 출력
    std::cout << "CPU Base Clock : " << dwMHz << " MHz" << std::endl;

    timeEndPeriod(1);
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
