#ifndef __CBASEOBJECT_H__
#define __CBASEOBJECT_H__

class CBaseObject
{
public:
    CBaseObject(int ObjectType, int Y, int X) : _ObjectType(ObjectType), _iY(Y), _iX(X) 
    {

    }

    virtual ~CBaseObject();

    virtual bool            Update(void) = 0;
    virtual bool            Render(void) = 0;


protected:
    int             _iX;
    int             _iY;
    int             _ObjectType;
};

#endif 
