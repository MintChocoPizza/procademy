#include "CBaseObject.h"

CBaseObject::CBaseObject(int ObjectType) : _ObjectType(ObjectType) 
{

}

CBaseObject::~CBaseObject()
{
}

int CBaseObject::GetObjectType(void)
{
	return _ObjectType;
}
