#include <stdio.h>
#include <Windows.h>

#include "Console.h"
#include "Buffer.h"

enum SCENE
{
	TITLE,
	LOAD,
	GAME,
	CLEAR,
	OVER
};


int g_Scene;

//----------------------------------------
// 게임 시작 시 처음 Title 화면을 보여준다.
// 
//----------------------------------------
void sceneTitle(void);

int main()
{
	//-----------------------------------------------
	// 정확한 시간을 위해 타임 인터벌을 변경한다.
	//-----------------------------------------------
	timeBeginPeriod(1);
	cs_Initial();

	//while (1)
	//{
	//	switch (g_Scene)
	//	{
	//	case TITLE:
	//		sceneTitle();
	//		

	//		break;
	//	case LOAD:
	//		break;
	//	case GAME:
	//		break;
	//	case CLEAR:
	//		break;
	//	case OVER:
	//		break;
	//	}
	//}

	sceneTitle();





	//-----------------------------------------------
	// 타임 인터벌 초기화
	//-----------------------------------------------
	timeEndPeriod(1);

	return 0;
}









//----------------------------------------
// 게임 시작 시 처음 Title 화면을 보여준다.
// 
//----------------------------------------
void sceneTitle(void)
{
	errno_t err;
	FILE* pFile;
	long lFileSize;
	char* pFileMemory;

	int iCnt;

	// 버퍼에 그리기 전 버퍼 초기화
	Buffer_Clear();

	// 타이틀 화면의 백그라운드 배경을 가져온다.
	err = fopen_s(&pFile, "Title", "r");
	if (err == NULL)
	{
		fseek(pFile, 0, SEEK_END);
		lFileSize = ftell(pFile);
		fseek(pFile, 0, SEEK_SET);

		pFileMemory = (char*)malloc(lFileSize);
		if (pFileMemory == NULL)
		{
			printf_s("sceneTitle 메모리 할당 실패 \n");
			return;
		}
		// window 텍스트 파일의 0D0A 에서 0D를 지우고 읽음
		fread_s(pFileMemory, lFileSize, 1, lFileSize, pFile);
		fclose(pFile);


		memcpy_s(szScreenBuffer, sizeof(szScreenBuffer), pFileMemory, sizeof(szScreenBuffer));
		for (iCnt = 0; iCnt < dfSCREEN_HEIGHT; ++iCnt)
		{
			szScreenBuffer[iCnt][dfSCREEN_WIDTH - 1] = '\0';
		}

	}

	
	// 화면 중앙에 게임 이름을 작성한다. 
	char cGameName[] = "Shooting Game";

	for(iCnt =0; iCnt<sizeof(cGameName);++iCnt) 
	{
		szScreenBuffer[dfSCREEN_HEIGHT / 2 - 2][sizeof(cGameName) / 2]
	}

}