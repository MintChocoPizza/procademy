
#include <Windows.h>
#include <list>
#include <unordered_map>
#include "main.h"
#include "C_Ring_Buffer.h"
#include "SerializeBuffer.h"
#include "Session.h"
#include "Define.h"
#include "Player.h"
#include "Field.h"

C_Player C_Player::_C_Player;

st_Player::st_Player(DWORD SessionID, st_SESSION* pSession)
{
	_pSession = pSession;
	_SessionID = SessionID;
	_Disconnect = false;
	_HP = 100;
	_dwAction = -1;

	// rand() % 최대값 + 최솟값
	// 0 < _Y < 6401
	// 0 < _X < 6401
	_Y = rand() % (dfRANGE_MOVE_BOTTOM - dfRANGE_MOVE_TOP + 1) + dfRANGE_MOVE_TOP;
	_X = rand() % (dfRANGE_MOVE_RIGHT - dfRANGE_MOVE_LEFT + 1) + dfRANGE_MOVE_LEFT;

	_SectorPos = new st_SECTOR_POS(_Y, _X);

	_SectorPos->iY = _Y / C_Field::GetInstance()->Grid_Y_Size;
	_SectorPos->iX = _X / C_Field::GetInstance()->Grid_X_Size;

	_byDirection = (rand() % 8) < 4 ? 0 : 4;
	_byModeDirection = -1;
}

C_Player* C_Player::GetInstance(void)
{
	return &_C_Player;
}

st_Player* C_Player::CreateNewPlayer(DWORD SessionID, st_SESSION* st_p_New_Session)
{
	st_Player* st_Temp_New_Player = new st_Player(SessionID, st_p_New_Session);

	_CharacterMap.insert({ SessionID, st_Temp_New_Player });

	return st_Temp_New_Player;
}

C_Player::C_Player()
{
}

C_Player::~C_Player()
{
	std::unordered_map<DWORD, st_Player*>::iterator iter;

	for (iter = _CharacterMap.begin(); iter != _CharacterMap.end(); ++iter)
	{
		st_Player *st_Delete_Player;

		st_Delete_Player = iter->second;

		delete st_Delete_Player;
	}
}

