#ifndef __CBASEOBJECT_H__
#define __CBASEOBJECT_H__

class CBaseObject
{
public:
	CBaseObject(int ObjectType, int Y, int X);
	virtual ~CBaseObject();

	virtual bool				Update(void) = 0;
	virtual void				Render(void) = 0;

	int GetObjectType(void);


protected:
	int		_iX;
	int		_iY;
	int		_OBjectType;
	bool	_DestroyCheck;
};


#endif // !__CBASEOBJECT_H__

