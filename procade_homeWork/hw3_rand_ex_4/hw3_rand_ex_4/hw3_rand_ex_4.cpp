// hw3_rand_ex_4.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <time.h>
#include <random>
#include <conio.h>

struct st_ITEM
{
	char cItempName[36];
	int Rate;
};

st_ITEM g_Gatcha[] =
{
			{"칼",				20},
			{"방패",				20},
			{"신발",				20},
			{"물약",				20},
			{"초강력레어무기",		5},
			{"초강력방패",			5},
			{"초강력레어레어레어신발1", 1},
			{"초강력레어레어레어신발2", 1},
			{"초강력레어레어레어신발3", 1}
};		// 93;

int g_MaxItem;
int g_MaxRate;
int* g_Arr;
int g_iCount;


void initArr(void)
{
	g_Arr = (int*)malloc(sizeof(int) * g_MaxRate); // 범위 0 ~ g_MaxRate-1

	for (int i = 0; i < g_MaxRate; ++i)
	{
		g_Arr[i] = i + 1;
	}

	// 셔플
	for (int i = 0; i < g_MaxRate; ++i)
	{
		int iRand = rand() % g_MaxRate;
		int temp = g_Arr[i];
		g_Arr[i] = g_Arr[iRand];
		g_Arr[iRand] = temp;
	}
}

void init(void)
{
	g_MaxItem = sizeof(g_Gatcha) / sizeof(st_ITEM);
	for (int i = 0; i < g_MaxItem; ++i)
	{
		g_MaxRate += g_Gatcha[i].Rate;	// 93
	}

	initArr();
}


void Gatcha(void)
{
	if (g_iCount >= g_MaxRate)
	{
		g_iCount = 0;
		//free(g_Arr);
		//initArr();
	}

	int temp = g_Arr[g_iCount];
	for (int i = 0; i < g_MaxItem; ++i)
	{
		temp -= g_Gatcha[i].Rate;
		if (temp > 0)
			continue;

		printf_s("%d    %d    %d    %s \n", g_iCount, g_Arr[g_iCount], g_Gatcha[i].Rate, g_Gatcha[i].cItempName);
		break;
	}
	++g_iCount;
}

int main()
{
	srand(time(NULL));

	init();

	while (1)
	{
		_getch();
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
