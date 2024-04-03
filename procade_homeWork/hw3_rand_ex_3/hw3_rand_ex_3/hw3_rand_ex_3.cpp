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

int g_MaxItem;
int g_MaxRate;
bool* Arr;
int g_iCount;


void initArr(void)
{
	Arr = (bool*)malloc(sizeof(bool) * g_MaxRate+1);
	memset(Arr, 0, sizeof(bool) *g_MaxRate+1);
}

void init(void)
{
	g_MaxItem = sizeof(g_Gatcha) / sizeof(st_ITEM);

	for (int i = 0; i < g_MaxItem; ++i)
	{
		g_MaxRate += g_Gatcha[i].Rate;
	}
	initArr();
}


// 배열에 저장하여 중복을 제거한다. 
void Gatcha(void)
{
	++g_iCount;
	if (g_iCount > g_MaxRate)
	{
		free(Arr);
		g_iCount = 1;
		initArr();
	}

	
	int iRand = rand() % g_MaxRate + 1;
	while (Arr[iRand] == true)
	{
		iRand = rand() % g_MaxRate + 1;
	}
	Arr[iRand] = true;


	int temp = iRand;
	for(int i=0; i<g_MaxItem; ++i)
	{
		iRand -= g_Gatcha[i].Rate;
		if (iRand > 0)
			continue;

		printf_s("%d    %d    %d    %s \n", g_iCount, temp, g_Gatcha[i].Rate, g_Gatcha[i].cItemName);
		break;
	}

}


int main()
{
	srand(time(NULL));

	init();

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
