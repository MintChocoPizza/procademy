
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
};

int g_MaxItem;
int g_MaxRate;
int g_tempMaxRate;
st_ITEM* g_temp;

int g_iCount;

void initTemp(void)
{
	g_temp = (st_ITEM*)malloc(sizeof(g_Gatcha));

	memcpy(g_temp, g_Gatcha, sizeof(g_Gatcha));
}

void init(void)
{
	g_MaxItem = sizeof(g_Gatcha) / sizeof(st_ITEM);
	
	for (int i = 0; i < g_MaxItem; ++i)
	{
		g_MaxRate += g_Gatcha[i].Rate;
	}

	g_tempMaxRate = g_MaxRate;

	initTemp();
}


void Gatch(void)
{
	g_iCount++;
	if (g_iCount > g_MaxRate)
	{
		for (int i = 0; i < g_MaxItem; ++i)
		{
			printf_s("%d ", g_temp[i].Rate);
		}
		printf_s("\n\n\n");
		g_iCount = 1;
		free(g_temp);
		initTemp();
		g_tempMaxRate = g_MaxRate;
	}

	int iRand = rand() % g_tempMaxRate + 1;
	int temp = iRand;
	for (int i = 0; i < g_MaxItem; ++i)
	{
		iRand -= g_temp[i].Rate;
		if (iRand > 0)
			continue;

		--g_temp[i].Rate;
		--g_tempMaxRate;

		printf_s("%d    %d    %d    %s \n", g_iCount, temp, g_tempMaxRate, g_Gatcha[i].cItempName);
		break;
	}
}

int main()
{
	srand(time(NULL));

	init();

	while (1)
	{
		_getch();
		Gatch();
	}


	return 0;
}
