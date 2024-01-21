// ThreadAdderTwo.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

static int total = 0;

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	DWORD* nPtr = (DWORD*)lpParam;
	DWORD numOne = *nPtr;
	DWORD numTwo = *(nPtr + 1);

	for (DWORD i = numOne; i <= numTwo; ++i)
	{
		total += i;
	}

	return 0;
}

int _tmain(int argc, TCHAR* argv[])
{
    DWORD dwThreadID[3];
    HANDLE hThread[3];

    DWORD paramThread[] = { 1,3,4,7,8,10 };
    DWORD result = 0;

    hThread[0] = CreateThread(NULL, 0, ThreadProc, (LPVOID)(&paramThread[0]), 0, &dwThreadID[0]); // paramThread[0]의 주소값을 전달한다.
    hThread[1] = CreateThread(NULL, 0, ThreadProc, (LPVOID)(&paramThread[2]), 0, &dwThreadID[1]); // paramThread[2]의 주소값을 전달한다.
    hThread[2] = CreateThread(NULL, 0, ThreadProc, (LPVOID)(&paramThread[4]), 0, &dwThreadID[2]); // paramThread[4]의 주소값을 전달한다.

    if (hThread[0] == NULL || hThread[1] == NULL || hThread[2] == NULL)
    {
        _tprintf_s(_T("Thread creation fault! \n"));
        return -1;
    }

    WaitForMultipleObjects(3, hThread, TRUE, INFINITE);

    _tprintf_s(_T("total (1~10): %d \n"), total);

    CloseHandle(hThread[0]);
    CloseHandle(hThread[1]);
    CloseHandle(hThread[2]);

    return 0;
}

// 프로그램 실행: <Ctrl+F5> 또는 [디
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
