#ifndef __CSCENELOAD_H__
#define __CSCENELOAD_H__
 
class CSceneLoad : public CSceneBase
{
private:
	char _stageName[128];

public:

	CSceneLoad(int iGameStage);

	~CSceneLoad();

	virtual bool Update(void);

private:
	void LoadStage(char* cStageName);

};

#endif // !__CSCENELOAD_H__

