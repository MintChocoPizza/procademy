
#include <Windows.h>
#include "ConsoleBuffer.h"

#include "CBaseObject.h"
#include "CSceneBase.h"
#include "CPlayer.h"
#include "CSceneGame.h"


CSceneGame::CSceneGame() : player(nullptr)
{
	player = new CPlayer(1, 20, 20, 'P');
}

CSceneGame::~CSceneGame()
{
	delete player;
}

bool CSceneGame::Update(void)
{
	ConsoleBuffer::GetInstance()->Buffer_Clear();

	player->Update();
	player->Render();

	return true;
}
