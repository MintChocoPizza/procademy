#ifndef __CSCENELOAD_H__
#define __CSCENELOAD_H__
 
class CSceneLoad : public CSceneBase
{

public:
	CSceneLoad();

	CSceneLoad(int iGameStage);

	~CSceneLoad();

	virtual bool Update(void);
};

#endif // !__CSCENELOAD_H__

