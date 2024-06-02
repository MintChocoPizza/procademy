#include "CSceneManager.h"

C_SceneManager C_SceneManager::_C_SceneManager;

C_SceneManager::C_SceneManager()
{

}

C_SceneManager::~C_SceneManager()
{
}

C_SceneManager* C_SceneManager::GetInstance(void)
{
	return nullptr;
}

bool C_SceneManager::run(void)
{
	return false;
}
