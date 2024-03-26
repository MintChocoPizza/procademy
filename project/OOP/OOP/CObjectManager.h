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

	//----------------------------------------------------------------------------------------------
	// 1. CSceneBase 프레임 워크를 상속받은 클래스에서 게임 데이터를 로딩한다. 
	// 2. 템플릿을 사용하여 직접 넘겨줄 경우, 클래스의 복사가 일어나서 느리다.  밖에서 new를 하고 주소를 넘겨주면 안되나??
	// 
	//----------------------------------------------------------------------------------------------
	void CreateObject(void* object);
	//void CreateObject(void* Object)
	//{
	//	// 원래는 오류가 생겼었는데, 헤더파일 위치를 바로 잡았더니 사라짐.
	//	ObjectList.push_back(static_cast<CBaseObject*> (Object));
	//}
	void DestroyObject(void);

};


#endif // !__COBJECTMANAGER_H__
