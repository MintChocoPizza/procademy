
#include <Windows.h>
#include <unordered_map>
#include "Field.h"
#include "Player.h"

C_Player::C_Player()
{

}

C_Player::C_Player(DWORD SessionID) : _SessionID(SessionID), _Disconnect(false), _HP(100), _dwAction(-1)
{
	// rand() % 최대값 + 최솟값
	// 0 < _Y < 6401
	// 0 < _X < 6401
	_Y = rand() % (dfRANGE_MOVE_BOTTOM - dfRANGE_MOVE_TOP + 1) + dfRANGE_MOVE_TOP;
	_X = rand() % (dfRANGE_MOVE_RIGHT - dfRANGE_MOVE_LEFT + 1) + dfRANGE_MOVE_LEFT;

	_byDirection = (rand() % 8) < 4 ? 0 : 4;
}

C_Player::~C_Player()
{
}