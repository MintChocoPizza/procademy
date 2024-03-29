
// 프레임 워크라고 하면 Manager와 Base 파일에 변화가 없어야 할거 같음.
// 하지만 메니저에서 씬의 메모리를 할당하고 해제하기 위해서는 
// 씬의 정보가 필수적으로 필요해보임......

#include <stdio.h>
#include <Windows.h>


#include "CParsing_ANSI.h"
#include "ConsoleBuffer.h"
#include "FPSManager.h"

#include "CBaseObject.h"
#include "CTitleObject.h"
#include "CollisionObject.h"
#include "CSceneBase.h"
#include "CPlayer.h"
#include "CSceneTitle.h"
#include "CSceneGame.h"
#include "CSceneLoad.h"
#include "CSceneClear.h"
#include "CSceneOver.h"

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

bool  CSceneManager::run(void)
{
	bool ret;
	//--------------------------------
	// 로직 프레임 증가
	//--------------------------------
	CFpsManager::GetInstance()->LogicTimeUpdate();

	ConsoleBuffer::GetInstance()->Buffer_Clear();

	// 로직부
	ret = _pScene->Update();

	// 랜더링
	// 종료되기 직전은 무조건 출력한다.
	if (CFpsManager::GetInstance()->FpsSkip() || !ret)
	{
		ConsoleBuffer::GetInstance()->Buffer_Flip();
	}

	ConsoleBuffer::GetInstance()->print_FPS();


	if (ret == false)
		delete _pScene;


	// 씬 전환 요청이 있다면 여기서 실질적인 씬 전환 
	if (ChangeScene)
	{
		delete _pScene;

		switch (type)
		{
		case CSceneManager::LOAD:		
			_pScene = new CSceneLoad(gameStage, maxStage);
			++gameStage;
			break;
		case CSceneManager::GAME:
			_pScene = new CSceneGame;
			break;
		case CSceneManager::CLEAR:
			_pScene = new CSceneClear;
			break;
		case CSceneManager::OVER:
			_pScene = new CSceneOver;
			break;
		}

		ChangeScene = false;
	}



	return ret;
}

void CSceneManager::LoadScene(SceneType type)
{
	ChangeScene = true;

	this->type = type;
}
