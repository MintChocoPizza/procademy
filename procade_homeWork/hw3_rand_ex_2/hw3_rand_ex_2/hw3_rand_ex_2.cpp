// hw3_rand_ex_2.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

struct st_ITEM
{
	char	Name[36];
	int	Rate;		// 일반 랜덤 뽑기와 같은 용도
	int	WinTime;	// 이 아이템이 나올 뽑기 회차.
	// 0 이면 일반 아이템
	// 0 이 아니면 그 회차에만 나옴
};

st_ITEM g_Gatcha[] = {
			{"칼",				20, 0},
			{"방패",				20, 0},
			{"신발",				20, 0},
			{"물약",				20, 0},
			{"초강력레어무기",		5, 0},
			{"초강력방패",			5, 0},
			{"초강력레어레어레어신발1", 1, 50},
			{"초강력레어레어레어신발2", 1, 51},
			{"초강력레어레어레어신발3", 1, 10}

			// 마지막 3개의 아이템은 일반 확률로는 나오지 않으며
			// 뒤에 입력된 WinTime 회차때만 100% 로 나옴.
};

int iCount = 0;

void Gatcha()
{
	// 특정 회차에 무조건 해당 아이템이 나와야 함
	iCount++;

	// 전체 아이템들의 비율의 총 합. 
	// 단 winTime 이 0이 아닌 아이템들은 제외함
	int max = 0;
	int itemNum = 0;
	int itemNumList = sizeof(g_Gatcha) / sizeof(st_ITEM);
	for (int i = 0; i < itemNumList; ++i)
	{
		max += g_Gatcha[i].Rate;
		if (g_Gatcha[i].WinTime != 0)
			continue;

		itemNum += g_Gatcha[i].Rate;
	}

	// 이번 회차가 wintime아이템을 뽑아야 하는 회자면 아이템을 뽑고 중단.
	for (int i = 0; i < itemNumList; ++i)
	{
		if (iCount == g_Gatcha[i].WinTime)
		{
			printf_s("%d    WinTime    %s \n", iCount, g_Gatcha[i].Name);
			break;
		}
	}

	// 확률을 구한다. 
	int iRand = rand() % itemNum + 1;


	// 전체 아이템 테이블을 돌면서 뽑을 아이템을 구한다. 
	int count = 0;
	for (int i = 0; i < itemNumList; ++i)
	{
		count += g_Gatcha[i].Rate;
		if (iRand <= count)
		{
			printf_s("%d    %d    %d    %s \n", iCount, iRand, g_Gatcha[i].Rate, g_Gatcha[i].Name);
			break;
		}
	}


	// 뽑기 회차를 초기화 해야 할지 판단하여 초기화.... 왜??? 초기화해??? 턴 테이블이 최대가 된다면
	if (iCount >= max)
	{
		iCount = 0;
	}
}

int main()
{
	srand(time(NULL));
	while (1)
	{
		_getch();
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
