
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
	// ㄷㅗㅇㅈㅓㄱ ㅁㅔㅁㅂㅓ ㅈㅓㅇㄹㅣ




	delete _Object;
}

bool CSceneTitle::Update(void)
{

	// ㅅㅣㄹㅈㅔㄹㅗ ㄷㅗㅇㅈㅏㄱㅎㅏㄴㅡㄴ 
	_Object->Update();
	_Object->Render();


	return false;
}
