// hw9_file_packing.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <_stdio.h>
#include <stdio.h>
#include <Windows.h>

#define MAX_FILE_COUNT 10

#pragma pack(push, 1)
struct st_PACKING_HEADER
{
    WORD wType;            // OP /oreopizza
    DWORD dwFileNumber;     // 패킹 되어있는 파일의 수
    // 이 구조체의 사이즈가 필요한가?

};
#pragma pop()

#pragma pack(push, 1)
struct st_PACKING_INFOHEADER
{
    char cFileName[64];
    DWORD dwFileSize;
};
#pragma pack(pop)

bool packing(char *_cFileName[64], int _iFileCount)
{
    st_PACKING_HEADER stPackingHeader;
    st_PACKING_INFOHEADER stPackingInfoHeader;

    stPackingHeader.wType = "OP";
    stPackingHeader.dwFileNumber = _iFileCount;

    FILE *pFile;
    errno_t;
}

int main()
{
    int num;
    int iFileCount;
    char cFileName[MAX_FILE_COUNT][64];

    while(1)
    {
        system("cls");
        printf_s("1: 패킹하기 \n");
        printf_s("2: 패킹풀기 \n");

        scanf_s("%d", &num);

        switch(num)
        {
            case 1:
                printf_s("패킹 할 파일 개수를 입력해주세요: ");
                scanf_s("%d", &iFileCount);

                for(int i=1; i<=iFileCount; ++i)
                {
                    printf_s("%d 번 파일 이름: ", i);
                    scanf_s("%d", cFileName[i]);
                }
                if(!packing(cFileName, iFileCount))
                {
                    printf_s("패킹에 실패했습니다. \n");
                }

                break;
            case 2:
                break;
            default:
                break;
        }


    }
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
