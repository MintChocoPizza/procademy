#ifndef  __COBJECTMANAGER_H__
#define __COBJECTMANAGER_H__

#include "CList.h"
#include "CBaseObject.h"
#include <iostream>

class CObjectManager
{
private:
	static CObjectManager _CObjectManager;
	CList<CBaseObject* > ObjectList;
private:
	CObjectManager();
	~CObjectManager();

public:
	static CObjectManager* GetInstance(void);

	//-------------------------------------------------------------------------
	// 인자 타입에 따른 객체 생성 new 후 List push
	// 
	// 특정 객체를 찾아서 삭제 시킨 후 List erase
	// 
	//-------------------------------------------------------------------------
	void CreateObject(void);
	void DestroyObject(void);


	void Update(void);
	void Render(void);
};


#endif // ! __COBJECTMANAGER_H__

