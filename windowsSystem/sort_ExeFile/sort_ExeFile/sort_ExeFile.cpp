// sort_ExeFile.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

#define MAX_STRING_NUM 100
#define MAX_STRING_LEN 100

void SortString(void)
{
    TCHAR stringArr[MAX_STRING_NUM][MAX_STRING_LEN];
    
    // 콘솔로부터 문자열을 읽어 들인다. 
    int nStr;
    for (nStr = 0; nStr < MAX_STRING_NUM; nStr++)
    {
        TCHAR* isEOF = _fgetts(stringArr[nStr], MAX_STRING_LEN, stdin);
        if (isEOF == NULL)   // EOF는 Ctrl+z
            break;
    }

    // STring Bubble Sort. 성능 고려하지 않고 문자열 단위 연산
    TCHAR strTemp[MAX_STRING_LEN];

    for (int i = 0; i < nStr; i++)
    {
        for (int j = nStr - 1; j > i; j--)
        {
            if (_tcscmp(stringArr[j - 1], stringArr[j]) > 0)
            {
                _tcscpy(strTemp, stringArr[j - 1]);
                _tcscpy(stringArr[j - 1], stringArr[j]);
                _tcscpy(stringArr[j], strTemp);
            }
        }
    }
    for (int i = 0; i < nStr; i++)
        _fputts(stringArr[i], stdout);
}

int _tmain(int argc, TCHAR * argv[])
{
    SortString();
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
