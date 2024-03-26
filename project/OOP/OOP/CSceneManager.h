#ifndef __CSCENEMANAGER_H__
#define	__CSCENEMANAGER_H__



////////////////////////////////////////////////////////////////////////////
// 씬을 관리할 싱글톤 클래스
// 
// 
////////////////////////////////////////////////////////////////////////////
class CSceneManager
{
private:
	static CSceneManager _CSceneManager;
	CSceneBase* _pScene;

	// Scene 전환에 필요한 정보
	enum SceneType
	{
		TITLE,
		GAME,
		CLEAR,
	};
	bool ChangeScene;
	SceneType type;



private:
	CSceneManager();
	~CSceneManager();

public:
	static CSceneManager* GetInstance(void);
	void run(void);


	//---------------------------------------------------------------------------------------
	// 어디서든 Scene를 전환할 수 있게 public으로 선언하였다.
	// 
	// 만약 LoadScene 함수에서 바로 Scene을 전환한다면,
	// Scene의 동작이 다 끝난 후 호출되면 괜찮지만, 동작이 다 끝나지 않은 상태로 호출될 경우
	// delete this 후 다시 this가 있던 자리의 메모리에 접근하게 된다. 
	// 
	// 그러므로 해당 함수에서는 어떤 함수로의 Scene 전환만 세팅만 한다.
	// 
	//---------------------------------------------------------------------------------------
	void LoadScene(SceneType type);			// 전환 씬을 넣어야 한다.


	
};


#endif // !__CSCENEMANAGER_H__

