
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

