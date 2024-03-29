
#include <Windows.h>

#include "CList.h"
#include "ConsoleBuffer.h"

#include "CBaseObject.h"
#include "CSceneBase.h"
#include "CollisionObject.h"
#include "CPlayer.h"
#include "CObjectManager.h"
#include "CSceneGame.h"


CSceneGame::CSceneGame() 
{

}

CSceneGame::~CSceneGame()
{
	CObjectManager::GetInstance()->ObjectClear();
	CObjectManager::GetInstance()->BulletClear();
}


bool CSceneGame::Update(void)
{
	//---------------------------------------------------------
	// 총알이 먼저 움직이고, 유닛이 움직인다. 
	// 
	//---------------------------------------------------------

	CObjectManager::GetInstance()->BulletUpdate();
	CObjectManager::GetInstance()->ObjectUpdate();

	CObjectManager::GetInstance()->BulletVisibleCheck();
	CObjectManager::GetInstance()->ObjectVisibleCheck();

	CObjectManager::GetInstance()->BulletRender();
	CObjectManager::GetInstance()->ObjectRender();

	return true;
}
