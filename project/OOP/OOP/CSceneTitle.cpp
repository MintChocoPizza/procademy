




/////////////////////////////////////////////////////////////////////////////////
// CTitleObject.h 파일에서 CBaseObject.h 클래스를 사용하기 때문에 
// 
// 두개를 같이 CBaseObject.h를 위에 선언하여야 한다.
// 
/////////////////////////////////////////////////////////////////////////////////
#include "CBaseObject.h"
#include "CTitleObject.h"
#include "CSceneBase.h"
#include "CSceneTitle.h"


CSceneTitle::CSceneTitle()
{
	// 타이틀 Scene에 맞는 기본 객체 생성 및 초기화
	_TitleObject = new CTitleObject("GameFile\\Title", "GameFile\\GameInfo.txt",0);
	
	

}

CSceneTitle::~CSceneTitle()
{
	// 동적 멤버 정리
	delete _TitleObject;
}

bool CSceneTitle::Update(void)
{

	// 실제로 동작하는 
	_TitleObject->Update();
	_TitleObject->Render();


	return false;
}
