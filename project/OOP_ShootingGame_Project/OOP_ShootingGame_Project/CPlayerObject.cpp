#include <Window.h>

#include "CPlayerObject.h"


CPlayerObject::CPlayerObject(int ObjectType, int Y, int X) : CBaseObject(ObjectType, Y, X)
{
    
}

bool CPlayerObject::Update()
{
    return false;
}

bool CPlayerObject::Render()
{
    return false;
}

void KeyBoradInput(void)
{
    if(GetAsyncKeyState('A'))
    {

    }
}
