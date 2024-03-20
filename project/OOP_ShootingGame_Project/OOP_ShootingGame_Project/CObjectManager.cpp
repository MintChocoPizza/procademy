#include <iostream>

#include "CList.h"
#include "CBaseObject.h"

#include "CObjectManager.h"

CObjectManager CObjectManager::_CObjectManager;

CObjectManager::CObjectManager()
{

}

CObjectManager::~CObjectManager()
{
}

CObjectManager* CObjectManager::GetInstance()
{
	return &_CObjectManager;
}


void CObjectManager::CreateObject(void)
{

}
