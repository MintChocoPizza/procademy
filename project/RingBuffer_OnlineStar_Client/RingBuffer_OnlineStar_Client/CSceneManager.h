
#ifndef __CSCENEMANAGER_H__
#define __CSCENEMANAGER_H__

class C_SceneManager
{
private:
	static C_SceneManager _C_SceneManager;
	
private:
	C_SceneManager();
	~C_SceneManager();

public:
	static C_SceneManager* GetInstance(void);
	bool run(void);
};


#endif // !__CSCENEMANAGER_H__
