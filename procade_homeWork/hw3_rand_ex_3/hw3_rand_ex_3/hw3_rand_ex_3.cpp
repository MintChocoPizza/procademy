// hw3_rand_ex_3.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

struct st_ITEM
{
	char cItemName[36];
	int Rate;
};

st_ITEM g_Gatcha[] = {
			{"칼",				20},
			{"방패",				20},
			{"신발",				20},
			{"물약",				20},
			{"초강력레어무기",		5},
			{"초강력방패",			5},
			{"초강력레어레어레어신발1", 1},
			{"초강력레어레어레어신발2", 1},
			{"초강력레어레어레어신발3", 1}
};	// 93개

bool g_ItemVisited[100];// 뽑은 아이템을 저장하기 위한 배열
int g_MaxItem;			// 아이템의 갯수
int g_MaxWeight;		// 아이템 가중치를 전부 더한 값, 랜덤에 사용
int g_CurGetItemCnt;	// 현재까지 뽑은 아이템 갯수, 배열 초기화에 사용
int g_iCount;			// 실행 횟수


// 실행 처음 아이템들의 갯수를 저장한다.
// Rand 함수의 범위를 지정하기 위하여 가중치의 합을 계산한다.
void GetMaxItem(void)
{
	int iItemCnt = sizeof(g_Gatcha) / sizeof(st_ITEM);
	g_MaxItem = iItemCnt;

	for (int i = 0; i < iItemCnt; ++i)
	{
		g_MaxWeight += g_Gatcha[i].Rate;
	}
}

void Gatcha(void)
{
	int iRand;

	// 아이템을 다 뽑은 경우 
	// 배열을 0으로 밀어버린다.
	if (g_CurGetItemCnt >= g_MaxWeight)
	{
		memset(g_ItemVisited, 0, sizeof(g_ItemVisited));
		g_CurGetItemCnt = 0;
	}
	
	// 나오는 범위 1~g_MaxWeight
	iRand = rand() % g_MaxWeight + 1;

	// 이미 뽑은 적 있는 아이템이면 
	// 다시 뽑는다.
	while (g_ItemVisited[iRand] == 1)
	{
		iRand = rand() % g_MaxWeight + 1;
	}

	// 뽑은 아이템을 배열에 저장하고
	// 뽑은 아이템의 갯수를 증가시킨다.
	g_ItemVisited[iRand] = 1;
	++g_CurGetItemCnt;
	++g_iCount;

	// 출력
	// iRand를 출력하기 위해 가중치를 계산하여 어떤 아이템인지 확인해야 한다.
	int checkItem = 0;
	for (int i = 0; i < g_MaxItem; ++i)
	{
		// 1~20 까지는 칼이 나와야함
		// 21~40까지는 방패가 나와야함
		// 
		checkItem += g_Gatcha[i].Rate;
		if (iRand <= checkItem)
		{
			printf_s("%d    %d    %d    %s    \n", g_iCount, iRand, g_Gatcha[i].Rate, g_Gatcha[i].cItemName);
			break;
		}
	}
}



int main()
{
	srand(time(NULL));

	GetMaxItem();

	while (1)
	{
		char a = _getch();
		Gatcha();
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
