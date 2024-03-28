
#include <Windows.h>

#include "CList.h"
#include "ConsoleBuffer.h"

#include "CBaseObject.h"
#include "CSceneBase.h"
#include "CPlayer.h"
#include "CObjectManager.h"
#include "CSceneGame.h"


CSceneGame::CSceneGame() 
{

}

CSceneGame::~CSceneGame()
{
	CObjectManager::GetInstance()->ObjectClear();
}


bool CSceneGame::Update(void)
{



	CObjectManager::GetInstance()->ObjectUpdate();
	CObjectManager::GetInstance()->ObjectRender();




	return true;
}
