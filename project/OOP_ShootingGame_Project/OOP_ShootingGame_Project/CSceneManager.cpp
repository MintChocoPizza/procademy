
#include "CSceneBase.h"

#include "CSceneManager.h"

CScenemanager CScenemanager::_CScenemanager;

CScenemanager::CScenemanager()
{

}

CScenemanager::~CScenemanager()
{

}

CScenemanager* CScenemanager::GetInstance(void)
{
	return &_CScenemanager;
}

void CScenemanager::run()
{
	_pScene->Update();
}

void CScenemanager::LoadScene(void)
{

}
