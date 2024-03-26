#ifndef __CSCENETITLE_H__
#define __CSCENETITLE_H__

// #include "CTitleObject.h"

class CSceneTitle :public CSceneBase
{
private: 
	CTitleObject* _TitleObject;

public:
	CSceneTitle();
	~CSceneTitle();

	virtual bool Update(void);

};


#endif // !__CSCENETITLE_H__

