#ifndef __CSCENEMANAGER_H__
#define __CSCENEMANAGER_H__

/////////////////////////////////////////////////////////////////////////
// ¾À ¸Þ´ÏÀú
// 
// 
/////////////////////////////////////////////////////////////////////////

class CScenemanager
{
private:
	static CScenemanager _CScenemanager;

	CSceneBase* _pScene;

private:
	CScenemanager();
	~CScenemanager();

public:
	static CScenemanager* GetInstance(void);
	void run(void);
	void LoadScene(void);
};


#endif // !__CSCENEMANAGER_H__
