#ifndef __CPLAYEROBJECT_H__
#define __CPLAYEROBJECT_H__

#include "CBaseObject.h"

class CPlayerObject : CBaseObject
{
public:
	CPlayerObject(int ObjectType, int Y, int X);

public:
	virtual bool Update(void);
	virtual void Render(void);




public:
	void KeyboardInput(void);
	void DrawPlayer(void);
};



#endif // !__CPLAYEROBJECT_H__
