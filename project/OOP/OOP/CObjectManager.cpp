
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

CObjectManager* CObjectManager::GetInstance(void)
{
	return &_CObjectManager;
}

void CObjectManager::CreateObject(void* Object)
{
	ObjectList.push_back(static_cast<CBaseObject*> (Object));
	
}

void CObjectManager::CreateBullet(void* Object)
{
	BulletList.push_back(static_cast<CBaseObject*>(Object));
}







void CObjectManager::ObjectClear(void)
{
	CList<CBaseObject*> ::iterator removeIter;

	for (removeIter = ObjectList.begin(); removeIter != ObjectList.end();)
	{
		CBaseObject* removeObject = *removeIter;

		delete removeObject;

		removeIter = ObjectList.erase(removeIter);
	}
}

void CObjectManager::ObjectUpdate(void)
{
	CList<CBaseObject*>::iterator iter;

	for (iter = ObjectList.begin(); iter != ObjectList.end(); ++iter)
	{
		(*iter)->Update();
	}
}

void CObjectManager::ObjectRender(void)
{
	CList<CBaseObject*>::iterator iter;

	for (iter = ObjectList.begin(); iter != ObjectList.end(); ++iter)
	{
		(*iter)->Render();
	}
}

void CObjectManager::ObjectVisibleCheck(void)
{
	// 소멸자를 virtual로 선언하여 
	// CBaseObject를 소멸시켜도 잘 소멸된다.
	CList<CBaseObject*>::iterator iter;

	for (iter = ObjectList.begin(); iter != ObjectList.end();)
	{
		CBaseObject* Object = *iter;
		if (Object->GetVisible() == false)
		{
			delete Object;

			iter = ObjectList.erase(iter);

		}
		else
		{
			++iter;
		}
	}
}





void CObjectManager::BulletClear(void)
{
	CList<CBaseObject*> ::iterator removeIter;

	for (removeIter = BulletList.begin(); removeIter != BulletList.end();)
	{
		CBaseObject* removeObject = *removeIter;

		delete removeObject;

		removeIter = BulletList.erase(removeIter);
	}
}

void CObjectManager::BulletUpdate(void)
{
	CList<CBaseObject*>::iterator iter;

	for (iter = BulletList.begin(); iter != BulletList.end(); ++iter)
	{
		CBaseObject* pBullet = *iter;
		pBullet->Update();

		
		// 충돌처리
		// Type 0번은 충돌하지 않는 오브젝트이다.
		if (pBullet->GetObjectType() == 0)
			continue;

		CList<CBaseObject*>::iterator target_iter;
		for (target_iter = ObjectList.begin(); target_iter != ObjectList.end(); ++target_iter)
		{
			CBaseObject* pObject = *target_iter;

			// 총알을 생성한 오브젝트와 총알의 타입은 같음
			if (pBullet->GetObjectType() == pObject->GetObjectType())
				continue;

			// 총알과 유닛의 리스트를 분리하였음. 
			// 해당 총알과 충돌하는 것은 무조건 유닛임. 
			if (pBullet->OnCollision(pObject))
			{
				pObject->OnCollision(pBullet);
			}
		}

	}
}

void CObjectManager::BulletRender(void)
{
	CList<CBaseObject*>::iterator iter;

	for (iter = BulletList.begin(); iter != BulletList.end(); ++iter)
	{
		(*iter)->Render();
	}
}

void CObjectManager::BulletVisibleCheck(void)
{
	CList<CBaseObject*>::iterator iter;

	for (iter = BulletList.begin(); iter != BulletList.end();)
	{
		CBaseObject* Object = *iter;
		if (Object->GetVisible() == false)
		{
			delete Object;

			iter = BulletList.erase(iter);

		}
		else
		{
			++iter;
		}
	}
}

