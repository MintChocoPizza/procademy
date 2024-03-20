#ifndef __CSCENEBASE_H__
#define __CSCENEBASE_H__

class CSceneBase
{
public:
	CSceneBase();
	~CSceneBase();

	virtual void		 Update(void) = 0;
};



#endif // !__CSCENEBASE_H__

