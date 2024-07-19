

#include <unordered_map>
#include <Windows.h>


#include "Contents.h"
#include "Define.h"
#include "SerializeBuffer.h"
#include "main.h"
#include "Protocol.h"
#include "C_Ring_Buffer.h"
#include "Field.h"
#include "Session.h"
#include "Player.h"

void Update(void)
{
	st_Player* st_p_Player;
	std::unordered_map<DWORD, st_Player*>::iterator iter;
	DWORD dwCurrentTick;

	/////////////////////////////////////////////////////////////////////////
	// 50프레임을 맞춘다. 
	// 그러기 위해서 
	// 1초 == 1000ms
	// 
	// 고로 20ms마다 1번씩 돌아야 한다.
	/////////////////////////////////////////////////////////////////////////
	dwCurrentTick = timeGetTime();
	if (dwCurrentTick - g_Start_Time < 20)
	{
		return;
	}
	g_Start_Time += 20;
	


	for (iter = g_CharacterHash.begin(); iter != g_CharacterHash.end();)
	{
		st_p_Player = iter->second;
		iter++;

		if (st_p_Player->_Disconnect == true) continue;

		if (0 >= st_p_Player->_HP)
		{
			// 사망처리
			continue;
		}

		// 일정 시간동안 수신이 없으면 종료 처리
		if (dwCurrentTick - st_p_Player->_pSession->dwLastRecvTime > dfNETWORK_PACKET_RECV_TIMEOUT)
		{
			continue;
		}

		//--------------------------------------------------------------------------------------
		// 현재 동작에 따른 처리
		//--------------------------------------------------------------------------------------
		switch (st_p_Player->_dwAction)
		{
			case dfPACKET_MOVE_DIR_LL:
				if (CharacterMoveCheck(st_p_Player->_X - dfSPEED_PLAYER_X, st_p_Player->_Y))
				{
					st_p_Player->_X -= dfSPEED_PLAYER_X;
				}
				break;
			case dfPACKET_MOVE_DIR_LU:
				if (CharacterMoveCheck(st_p_Player->_X - dfSPEED_PLAYER_X, st_p_Player->_Y - dfSPEED_PLAYER_Y))
				{
					st_p_Player->_X -= dfSPEED_PLAYER_X;
					st_p_Player->_Y -= dfSPEED_PLAYER_Y;
				}
				break;
			case dfPACKET_MOVE_DIR_UU:
				if (CharacterMoveCheck(st_p_Player->_X, st_p_Player->_Y - dfSPEED_PLAYER_Y))
				{
					st_p_Player->_Y -= dfSPEED_PLAYER_Y;
				}
			case dfPACKET_MOVE_DIR_RU:
				if (CharacterMoveCheck(st_p_Player->_X + dfSPEED_PLAYER_X, st_p_Player->_Y - dfSPEED_PLAYER_Y))
				{
					st_p_Player->_X += dfSPEED_PLAYER_X;
					st_p_Player->_Y -= dfSPEED_PLAYER_Y;
				}
				break;
			case dfPACKET_MOVE_DIR_RR:
				if (CharacterMoveCheck(st_p_Player->_X + dfSPEED_PLAYER_X, st_p_Player->_Y))
				{
					st_p_Player->_X += dfSPEED_PLAYER_X;
				}
				break;
			case dfPACKET_MOVE_DIR_RD:
				if (CharacterMoveCheck(st_p_Player->_X + dfSPEED_PLAYER_X, st_p_Player->_Y + dfSPEED_PLAYER_Y))
				{
					st_p_Player->_X += dfSPEED_PLAYER_X;
					st_p_Player->_Y += dfSPEED_PLAYER_Y;
				}
				break;
			case dfPACKET_MOVE_DIR_DD:
				if (CharacterMoveCheck(st_p_Player->_X, st_p_Player->_Y + dfSPEED_PLAYER_Y))
				{
					st_p_Player->_Y += dfSPEED_PLAYER_Y;
				}
				break;
			case dfPACKET_MOVE_DIR_LD:
				if (CharacterMoveCheck(st_p_Player->_X - dfSPEED_PLAYER_X, st_p_Player->_Y + dfSPEED_PLAYER_Y))
				{
					st_p_Player->_X -= dfSPEED_PLAYER_X;
					st_p_Player->_Y += dfSPEED_PLAYER_Y;
				}
				break;
		default:
			// 이상함 유저의 연결을 끊는다.
			break;
		}

		//--------------------------------------------------------------------------------------
		// 이동 액션인 경우, 섹터를 체크하여 변경해준다. 
		//--------------------------------------------------------------------------------------
		if (st_p_Player->_dwAction >= dfPACKET_MOVE_DIR_LL && st_p_Player->_dwAction <= dfPACKET_MOVE_DIR_LD)
		{
			// 이동인 경우 섹터 업데이트를 함.
			if (C_Field::GetInstance()->Sector_UpdateCharacter(st_p_Player))
			{
				// 섹터가 변경된 경우는 클라에게 관련 정보를 쏜다. 
				C_Field::GetInstance()->CharacterSectorUpdatePacket(st_p_Player);
			}
		}
	}





}

bool CharacterMoveCheck(int X, int Y)
{
	if (X < dfRANGE_MOVE_LEFT || dfRANGE_MOVE_RIGHT < X || Y < dfRANGE_MOVE_TOP || dfRANGE_MOVE_BOTTOM < Y)
		return false;

	return true;
}
