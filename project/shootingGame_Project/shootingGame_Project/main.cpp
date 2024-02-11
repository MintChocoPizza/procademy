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
	OVER,
	GAME_ERROR
};

typedef enum
{
	FILE_ERROR = -1,
	RESULT_FALSE = 0,
	RESULT_TRUE = 1
} RESULT;



//---------------------------------------------------------------------------------
// 플레이어 구조체 선언 
// 
// 플레이어 인스턴스 선언. (1인용 이니 하나만 하자)
// 
//---------------------------------------------------------------------------------
struct st_PLAYER
{
	int iHP;				// 플레이어 HP


	int iX;					// 플레이어 좌표
	int iY;					//


	int iVisible;			// 플레이어 생존 여부.
} g_stPlayer;

int g_Scene;
int g_Stage = 3;

//---------------------------------------------------------------------------------
// 게임 시작 시 처음 Title 화면을 보여준다.
// 
//---------------------------------------------------------------------------------
void SceneTitle(void);


//---------------------------------------------------------------------------------
// 스테이지 클리어를 확인한다.
// 
//---------------------------------------------------------------------------------
RESULT IsGameClear(void);


//---------------------------------------------------------------------------------
// stageInfo 파일을 불러와 메모리에 저장하고,
// 현재 스테이지에 맞는 맵 정보를 불러온다.
// 
//---------------------------------------------------------------------------------
RESULT GameLoading(void);






//---------------------------------------------------------------------------------
// 키보드 입력을 받아서 플레이어를 처리한다.
// 
// 게임중 ESC를 누를 경우 false리턴. (종료 처리)
// 
//---------------------------------------------------------------------------------
bool KeyProcess(void);





























int main()
{
	//-----------------------------------------------
	// 정확한 시간을 위해 타임 인터벌을 변경한다.
	//-----------------------------------------------
	timeBeginPeriod(1);
	cs_Initial();

	//-----------------------------------------------
	// 
	//-----------------------------------------------
	
	while (1)
	{
		switch (g_Scene)
		{
		case TITLE:
		{
			// 키보드 입력
			if (GetAsyncKeyState(VK_RETURN) & 0x8001)
			{
				// 로직부
				g_Scene = LOAD;
			}

			// 로직부
			SceneTitle();

			// 랜더부
			buff_Buffer_Flip();
			break;
		}
		case LOAD:
		{
			int isClear = IsGameClear();
			switch (isClear)
			{
			case 1:
				g_Scene = CLEAR;
				break;
			case 0:
				GameLoading();
				g_Scene = GAME;
				break;
			case -1:
				g_Scene = GAME_ERROR;
				break;
			}
			break;
		}
		case GAME:
		{
			return 0;

			// 키보드 입력
			


			// 로직부



			// 랜더부
			buff_Buffer_Flip();

			break;
		}
		case CLEAR:
			break;
		case OVER:
			break;
		case GAME_ERROR:
			printf_s("게임이 비정상적으로 종료되었습니다.  \n");
			return -1;
		}
	}






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
void SceneTitle(void)
{
	struct st_GAMEINFO
	{
		char cGameName[30];
		char cGameVersion[10];
	} *pGameInfo;

	errno_t err;
	FILE* pFile;
	size_t szStrLen;

	char cControls[] = "Move: WASD,  Weapon: J";
	char cGuide[] = "Press Enter.....";


	int iCnt;

	// 버퍼에 그리기 전 버퍼 초기화
	buff_Buffer_Clear();

	//-----------------------------------------------------------------------------
	// 타이틀 화면의 백그라운드 배경을 가져온다.
	// 
	//-----------------------------------------------------------------------------
	buff_Sprite_Background("Title");





	//-----------------------------------------------------------------------------
	// 배경화면 위에 게임의 타이틀과 게임의 버전을 그린다.
	// 
	//-----------------------------------------------------------------------------
	err = fopen_s(&pFile, "GameInfo", "rb");
	if (err == NULL)
	{
		pGameInfo = (st_GAMEINFO*)malloc(sizeof(st_GAMEINFO));
		if (pGameInfo != NULL)
		{
			fread_s(pGameInfo, sizeof(st_GAMEINFO), 1, sizeof(st_GAMEINFO), pFile);
			fclose(pFile);

			szStrLen = strnlen_s(pGameInfo->cGameName, sizeof(pGameInfo->cGameName));
			// 버퍼에 게임 타이틀 출력
			for (iCnt = 0; iCnt < szStrLen; ++iCnt)
			{
				buff_Sprite_Draw((dfSCREEN_HEIGHT / 2 - 2), (dfSCREEN_WIDTH / 2) - ((int)szStrLen - 1)/2 + iCnt, pGameInfo->cGameName[iCnt]);
			}



			szStrLen = strnlen_s(pGameInfo->cGameVersion, sizeof(pGameInfo->cGameVersion));
			// 좌측 하단에 게임의 버전을 출력한다. 
			for (iCnt = 0; iCnt < szStrLen; ++iCnt)
			{
				buff_Sprite_Draw(dfSCREEN_HEIGHT - 2, 1 + iCnt, pGameInfo->cGameVersion[iCnt]);
			}

			free(pGameInfo);
		}
	}




	//-----------------------------------------------------------------------------
	// 조작에 대한 설명을 버퍼에 그린다.
	// 
	//-----------------------------------------------------------------------------
	for (iCnt = 0; iCnt < sizeof(cControls) - 1; ++iCnt)
	{
		buff_Sprite_Draw((dfSCREEN_HEIGHT / 2 + 2), (dfSCREEN_WIDTH / 2) - (sizeof(cControls)/2 - 1) + iCnt, cControls[iCnt]);
	}
	for (iCnt = 0; iCnt < sizeof(cGuide) - 1; ++iCnt)
	{
		buff_Sprite_Draw((dfSCREEN_HEIGHT / 2 + 4), (dfSCREEN_WIDTH / 2) - (sizeof(cGuide) / 2 - 1) + iCnt, cGuide[iCnt]);
	}

	
	
}



//----------------------------------------
// 스테이지 클리어를 확인한다.
// 
//----------------------------------------
RESULT IsGameClear(void)
{
	FILE* pFile;
	errno_t err;
	int iMaxStage;

	err = fopen_s(&pFile, "MaxStage", "r");
	if (err != NULL)
	{
		system("cls");
		printf_s("스테이지 정보 불러오기 실패 \n");
		return FILE_ERROR;
	}

	fscanf_s(pFile, "%d", &iMaxStage);

	if (g_Stage < iMaxStage)
	{
		return RESULT_FALSE;
	}

	return RESULT_TRUE;
}

//----------------------------------------
// stageInfo 파일을 불러와 메모리에 저장하고,
// 현재 스테이지에 맞는 맵 정보를 불러온다.
// 
//----------------------------------------
RESULT GameLoading(void)
{
	FILE* pFile;
	errno_t err;
	long lFileSize;

	char* pStageInfoMemory;
	int iCnt;
	char cStageName[50];

	err = fopen_s(&pFile, "StageInfo", "r");
	if (err != NULL)
	{
		system("cls");
		printf_s("스테이지 목록 불러오기 실패 \n");
		return FILE_ERROR;
	}
	fseek(pFile, 0, SEEK_END);
	lFileSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	// stageInfo 파일 전체 메모리에 저장
	pStageInfoMemory = (char*)malloc(lFileSize);
	if (pStageInfoMemory == NULL)
	{
		system("cls");
		printf_s("메모리 할당 실패 \n");
		return FILE_ERROR;
	}

	for (iCnt = 0; iCnt < g_Stage; ++iCnt)
	{
		fgets(cStageName, sizeof(cStageName), pFile);
	}


	fclose(pFile);
	free(pStageInfoMemory);

}





//----------------------------------------
// 키보드 입력을 받아서 플레이어를 처리한다.
// 
// 게임중 ESC를 누를 경우 false리턴. (종료 처리)
// 
//----------------------------------------
bool KeyProcess(void)
{
	// 왼쪽 이동 A.
	if (GetAsyncKeyState('A'))
	{
		g_stPlayer.iX -= 1;
	}
	// 오른쪽 이동 D.
	if (GetAsyncKeyState('D'))
	{
		g_stPlayer.iX += 1;
	}
	// 위쪽 이동 W.
	if (GetAsyncKeyState('W') & 0x8001)
	{
		g_stPlayer.iY--;
	}
	// 아래쪽 이동 S.
	if (GetAsyncKeyState('S') & 0x8001)
	{
		g_stPlayer.iY++;
	}

	//-------------------------------------------------------------
	// 플레이어 이동 반경 제한.
	// 게임 화면에서 플레이어가 이동 가능한 구역을 제한한다.
	//-------------------------------------------------------------

	g_stPlayer.iX = max(g_stPlayer.iX, 0);
	g_stPlayer.iX = min(g_stPlayer.iX, 79);
	g_stPlayer.iY = max(g_stPlayer.iY, 0);
	g_stPlayer.iY = min(g_stPlayer.iY, 23);







	// 스페이스바 키. (미사일 키)
	if (GetAsyncKeyState(VK_SPACE))
	{

	}

	// ESC 키. (종료)
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8001)
	{
		// 종료 방법
		return false;
	}

	return true;
}