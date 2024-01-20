// hw3_rand_ex_realRand.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>

struct st_ITEM
{
    char Name[35];
    int Rate;
};

st_ITEM g_Gatch[] =
{
			{"칼",		20},
			{"방패",		20},
			{"신발",		20},
			{"물약",		20},
			{"초강력레어무기",	5},
			{"초강력방패",		5},
			{"초강력레어레어레어신발", 1}
}; // 91 개

int iCount = 0;

void Gatcha()
{
	// 전체 아이템들의 비율 총 합을 구한다.
	int sumProbability = 0;		// 아이템의 총 갯수
	int stSize = sizeof(g_Gatch) / sizeof(st_ITEM);		// 아이템 목록 갯수
	for (int i = 0; i < stSize; ++i)
	{
		sumProbability += g_Gatch[i].Rate;
	}

	// rand() 함수로 확률을 구한다. 
	int iRand = rand() % sumProbability+1;	// 1 ~ 총갯수

	// 전체 아이템 테이블을 돌면서 위에서 구한 iRand값에 해당 지점의 아이템을 찾는다.
	/*
		1~20 = 칼
		21~40 = 방패
		......
		91 = 초강력레어레어레어신발
	*/
	int checkItem = 0;
	for (int i = 0; i < stSize; i++)
	{
		checkItem += g_Gatch[i].Rate;

		if (iRand <= checkItem)
		{
			printf_s("%d    %d    %d    %s  \n", iCount, iRand, g_Gatch[i].Rate, g_Gatch[i].Name);
			++iCount;
			break;
		}
	}
}

int _tmain()
{
	srand(time(NULL));
	while(1)
	{
		// 아무 입력 하나에 아이템 하나가 나온다.
		char n = _getch();
		Gatcha();
	}
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
