
#include "CBaseObject.h"

#include "CPlayer.h"

CPlayer::CPlayer(int ObjectType) : CBaseObject(ObjectType)
{

}

CPlayer::~CPlayer()
{
}

bool CPlayer::Update(void)
{
	return false;
}
