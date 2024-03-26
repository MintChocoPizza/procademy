

#include "CObjectManager.h"


CObjectManager CObjectManager::_CObjectManager;

CObjectManager::CObjectManager()
{
}

CObjectManager::~CObjectManager()
{
}

CObjectManager* CObjectManager::GetInstance(void)
{
	return &_CObjectManager;
}

//void CObjectManager::CreateObject(void* Object)
//{
//	ObjectList.push_back(static_cast<CBaseObject*> (Object));
//}

void CObjectManager::DestroyObject(void)
{
	CList<CBaseObject*>::iterator iter;

	for (iter = ObjectList.begin(); iter != ObjectList.end(); ++iter)
	{
		CBaseObject* pObject = *iter;

		// pObject.    -> 만약 요소를 파괴해야 한다면 파괴한다. 
	}
}
