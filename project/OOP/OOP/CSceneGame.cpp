
#include <Windows.h>
#include "ConsoleBuffer.h"

#include "CBaseObject.h"
#include "CSceneBase.h"
#include "CPlayer.h"
#include "CSceneGame.h"


CSceneGame::CSceneGame() 
{
}

CSceneGame::~CSceneGame()
{
}

bool CSceneGame::Update(void)
{
	ConsoleBuffer::GetInstance()->Buffer_Clear();

	ConsoleBuffer::GetInstance()->Sprite_Center_String(dfSCREEN_HEIGHT / 2, "GAME");

	ConsoleBuffer::GetInstance()->Buffer_Flip();

	return true;
}
