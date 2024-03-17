#ifndef __CPLAYEROBJECT_H__
#define __CPLAYEROBJECT_H__

#include "CBaseObject.h"

class CPlayerObject : public CBaseObject
{
public:
    CPlayerObject(int ObjectType, int Y, int X);
    ~CPlayerObject();

    virtual bool            Update(void);
    virtual bool            Render(void);


protected:
    void KeyBoradInput(void);


};



#endif

