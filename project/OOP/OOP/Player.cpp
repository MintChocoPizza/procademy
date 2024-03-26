
#include "CBaseObject.h"

#include "Player.h"

Player::Player(int ObjectType) : CBaseObject(ObjectType)
{

}

Player::~Player()
{
}

bool Player::Update(void)
{
	return false;
}
