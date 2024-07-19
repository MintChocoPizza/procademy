
#include <Windows.h>
#include <list>
#include <unordered_map>
#include "Protocol.h"
#include "C_Ring_Buffer.h"
#include "SerializeBuffer.h"
#include "main.h"
#include "Session.h"
#include "Define.h"
#include "Player.h"
#include "Field.h"

std::unordered_map< DWORD, st_Player*>	g_CharacterHash;

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
	_CurSector = new st_SECTOR_POS(_Y, _X);
	_OldSector = new st_SECTOR_POS(_Y, _X);
	

	_byDirection = (rand() % 8) < 4 ? 0 : 4;
	_byModeDirection = -1;
}
st_Player::~st_Player()
{
	delete _SectorPos;
	delete _CurSector;
	delete _OldSector;
}


void ClearCharacterHash(void)
{
	st_Player* st_Delete_Player;
	std::unordered_map<DWORD, st_Player*>::iterator iter;

	for (iter = g_CharacterHash.begin(); iter != g_CharacterHash.end();)
	{
		st_Delete_Player = iter->second;

		delete st_Delete_Player->_CurSector;
		delete st_Delete_Player->_OldSector;
		delete st_Delete_Player->_SectorPos;
		delete st_Delete_Player;

		iter = g_CharacterHash.erase(iter);
	}
}

st_Player* CreateNewPlayer(DWORD SessionID, st_SESSION* st_p_New_Session)
{
	st_Player* st_Temp_New_Player = new st_Player(SessionID, st_p_New_Session);

	g_CharacterHash.insert({ SessionID, st_Temp_New_Player });

	return st_Temp_New_Player;
}


