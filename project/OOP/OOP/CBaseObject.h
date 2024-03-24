
#ifndef __CBASEOBJECT_H__
#define __CBASEOBJECT_H__

////////////////////////////////////////////////////////////////////////////
// 오브젝트 들을 관리할 최상의 객체
// 
////////////////////////////////////////////////////////////////////////////
class CBaseObject
{
public:
	CBaseObject(int ObjectType);
	virtual ~CBaseObject();

	virtual bool		Update(void) = 0;
	virtual void		Render(void) = 0;

	int GetObjectType(void);


protected:
	int				_ObjectType;
};


#endif // !__CBASEOBJECT_H__
