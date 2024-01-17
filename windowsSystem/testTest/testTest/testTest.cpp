// testTest.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <stdlib.h>

struct stData
{
    char a; // 1 + 3
    int b;  // 4 + 0
    short c; // 1 + 3
    int d;  // 4 + 0 // sum-> 4*4 = 16
};

int main()
{
    printf_s("구조체 크기: %d \n", sizeof(stData));

    char* p = (char*)malloc(1000);

    stData* pData1 = (stData*)(p + 2);
    stData* pData2 = (stData*)(p + 10);

    pData1->a = 0xff;
    pData1->b = 0xff;
    pData1->c = 0xff;
    pData1->d = 0xff;

    pData2->a = 0xaa;
    pData2->b = 0xaa;
    pData2->c = 0xaa;
    pData2->d = 0xaa;


    printf_s("%d \n", pData1->a);

    printf_s("Hello World \n");
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
