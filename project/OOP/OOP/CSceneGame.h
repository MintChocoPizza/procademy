#ifndef __CSCENEGAME__H__
#define __CSCENEGAME__H__

class CSceneGame : public CSceneBase
{
private:
	CPlayer* player;


public:
	CSceneGame();
	~CSceneGame();

	virtual bool Update(void);
};



#endif // !__CSCENEGAME__H__

