#ifndef __COBJECTMANAGER_H__
#define __COBJECTMANAGER_H__


////////////////////////////////////////////////////////////////////////////
// 오브젝트들을 관리할 싱글톤 클래스
// 
// 
////////////////////////////////////////////////////////////////////////////
class CObjectManager
{
private:
	static CObjectManager _CObjectManager;


	CList<CBaseObject*> ObjectList;

private:
	CObjectManager();
	~CObjectManager();


public:
	static CObjectManager* GetInstance(void);


	void CreateObject(void* object);
	//void CreateObject(void* Object)
	//{
	//	// 원래는 오류가 생겼었는데, 헤더파일 위치를 바로 잡았더니 사라짐.
	//	ObjectList.push_back(static_cast<CBaseObject*> (Object));
	//}
	void DestroyObject(void);

	void ObjectClear(void);

	void ObjectUpdate(void);
	void ObjectRender(void);
};


#endif // !__COBJECTMANAGER_H__
