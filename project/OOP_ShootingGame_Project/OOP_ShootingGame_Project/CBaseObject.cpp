#include <stdio.h>

#include "CBaseObject.h"

CBaseObject::CBaseObject(int ObjectType, int Y, int X) : _OBjectType(ObjectType), _iY(Y), _iX(X)
{
	printf_s("베이스 오브젝트 생성자 동작 \n");
}

CBaseObject::~CBaseObject()
{

}

int CBaseObject::GetObjectType(void)
{
	return _OBjectType;
}
