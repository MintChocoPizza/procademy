#ifndef __CPLAYEROBJECT_H__
#define __CPLAYEROBJECT_H__

#include "CBaseObject.h"

class CPlayerObject : public CBaseObject
{
public:
    CPlayerObject();

    virtual bool            Update(void);
    virtual bool            Render(void);


protected:
    
};



#endif

