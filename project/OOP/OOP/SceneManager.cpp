
// 프레임 워크라고 하면 Manager와 Base 파일에 변화가 없어야 할거 같음.
// 하지만 메니저에서 씬의 메모리를 할당하고 해제하기 위해서는 
// 씬의 정보가 필수적으로 필요해보임......

#include <stdio.h>
#include <Windows.h>

#include "CParsing_ANSI.h"

#include "CBaseObject.h"
#include "CTitleObject.h"
#include "CSceneBase.h"
#include "CPlayer.h"
#include "CSceneTitle.h"
#include "CSceneGame.h"
#include "CSceneLoad.h"

#include "CSceneManager.h"


CSceneManager CSceneManager::_CSceneManager;

CSceneManager::CSceneManager() : ChangeScene(false), type(TITLE), gameStage(1)
{
	_pScene = new CSceneTitle;
	maxStage = GetMaxStage();
}

CSceneManager::~CSceneManager()
{
}

int CSceneManager::GetMaxStage(void)
{
	int iMaxStage;
	CParsing_ANSI cParsing;
	cParsing.LoadFile("GameFile\\MaxStage.txt");
	cParsing.GetValue("MaxStage", &iMaxStage);
	return iMaxStage;
}

CSceneManager* CSceneManager::GetInstance(void)
{
	return &_CSceneManager;
}

void CSceneManager::run(void)
{
	_pScene->Update();

	// 씬 전환 요청이 있다면 여기서 실질적인 씬 전환 
	if (ChangeScene)
	{
		delete _pScene;

		switch (type)
		{
		case CSceneManager::LOAD:		
			_pScene = new CSceneLoad(gameStage);
			break;
		case CSceneManager::GAME:
			_pScene = new CSceneGame();
			break;
		case CSceneManager::CLEAR:

			break;
		default:
			break;
		}

		ChangeScene = false;
	}

}

void CSceneManager::LoadScene(SceneType type)
{
	ChangeScene = true;

	this->type = type;
}
