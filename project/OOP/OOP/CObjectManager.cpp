
#include "CList.h"


//------------------------------------------------------------------------
// 앞으로 같이 선언되어야 하는 헤더 파일은 제일 하단에 붙여서 선언한다.
// 
// CBaseObject.h로 CObjectManager.h 파일에서 사용하여 
// 순서를 맞춰서 같이 선언해줘야 한다. 
//------------------------------------------------------------------------
#include "CBaseObject.h"
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

void CObjectManager::CreateObject(void* Object)
{
	ObjectList.push_back(static_cast<CBaseObject*> (Object));
	
}

void CObjectManager::DestroyObject(void)
{
	CList<CBaseObject*>::iterator iter;

	for (iter = ObjectList.begin(); iter != ObjectList.end(); ++iter)
	{
		CBaseObject* pObject = *iter;

		// pObject.    -> 만약 요소를 파괴해야 한다면 파괴한다. 
	}
}
