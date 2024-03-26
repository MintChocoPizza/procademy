


#include "CSceneBase.h"


#include "CTitleObject.h"


#include "CSceneTitle.h"


CSceneTitle::CSceneTitle()
{
	// 타이틀 Scene에 맞는 기본 객체 생성 및 초기화
	_Object = new CTitleObject("GameFile\\Title", "GameFile\\GameInfo.txt",0);
	
	

}

CSceneTitle::~CSceneTitle()
{
	// 동적 멤버 정리




	delete _Object;
}

bool CSceneTitle::Update(void)
{

	// 실제로 동작하는 
	_Object->Update();
	_Object->Render();


	return false;
}
