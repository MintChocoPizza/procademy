

#include <unordered_map>
#include <Windows.h>

#include "LOG.h"
#include "Contents.h"
#include "Define.h"
#include "SerializeBuffer.h"
#include "Disconnect.h"
#include "main.h"
#include "Protocol.h"
#include "C_Ring_Buffer.h"
#include "Session.h"
#include "CList.h"
#include "Field.h"
#include "Player.h"


// #define UPDATE_DEBUG

void Update(void)
{
	st_PLAYER* st_p_Player;
	std::unordered_map<DWORD, st_PLAYER*>::iterator iter;
	//DWORD dwCurrentTick;

	///////////////////////////////////////////////////////////////////////////
	//// 50프레임을 맞춘다. 
	//// 그러기 위해서 
	//// 1초 == 1000ms
	//// 
	//// 고로 20ms마다 1번씩 돌아야 한다.
	//// 메인문으로 뺀다.
	///////////////////////////////////////////////////////////////////////////
	//dwCurrentTick = timeGetTime();
	//if (dwCurrentTick - g_Start_Time < 20)
	//{
	//	return;
	//}
	//g_Start_Time += 20;
	



	for (iter = g_CharacterHash.begin(); iter != g_CharacterHash.end();)
	{
		st_p_Player = iter->second;
		iter++;


		if (0 >= st_p_Player->_HP)
		{
			// 사망처리
			_LOG(0, L"Player HP Zero # SessionID:%d", st_p_Player->_SessionID);
			enqueueForDeletion(st_p_Player->_SessionID);
			continue;
		}


		// 일정 시간동안 수신이 없으면 종료 처리
		//if (dwCurrentTick - st_p_Player->_pSession->dwLastRecvTime > dfNETWORK_PACKET_RECV_TIMEOUT)
		//{
		//	enqueueForDeletion(st_p_Player->_SessionID);
		//	continue;
		//}

		//--------------------------------------------------------------------------------------
		// 현재 동작에 따른 처리
		//--------------------------------------------------------------------------------------
		switch (st_p_Player->_dwAction)
		{
			case dfPACKET_MOVE_DIR_LL:
				if (CharacterMoveCheck(st_p_Player->_X - dfSPEED_PLAYER_X, st_p_Player->_Y))
				{
					st_p_Player->_X -= dfSPEED_PLAYER_X;
#ifdef UPDATE_DEBUG
					_LOG(0, L"# SessionID:%d / CurX:%d / CurY:%d", st_p_Player->_SessionID, st_p_Player->_X, st_p_Player->_Y);
#endif // UPDATE_DEBUG
				}
				break;
			case dfPACKET_MOVE_DIR_LU:
				if (CharacterMoveCheck(st_p_Player->_X - dfSPEED_PLAYER_X, st_p_Player->_Y - dfSPEED_PLAYER_Y))
				{
					st_p_Player->_X -= dfSPEED_PLAYER_X;
					st_p_Player->_Y -= dfSPEED_PLAYER_Y;
#ifdef UPDATE_DEBUG
					_LOG(0, L"# SessionID:%d / CurX:%d / CurY:%d", st_p_Player->_SessionID, st_p_Player->_X, st_p_Player->_Y);

#endif // UPDATE_DEBUG
				}
				break;
			case dfPACKET_MOVE_DIR_UU:
				if (CharacterMoveCheck(st_p_Player->_X, st_p_Player->_Y - dfSPEED_PLAYER_Y))
				{
					st_p_Player->_Y -= dfSPEED_PLAYER_Y;
#ifdef UPDATE_DEBUG
					_LOG(0, L"# SessionID:%d / CurX:%d / CurY:%d", st_p_Player->_SessionID, st_p_Player->_X, st_p_Player->_Y);
#endif // UPDATE_DEBUG
				}
				break;
			case dfPACKET_MOVE_DIR_RU:
				if (CharacterMoveCheck(st_p_Player->_X + dfSPEED_PLAYER_X, st_p_Player->_Y - dfSPEED_PLAYER_Y))
				{
					st_p_Player->_X += dfSPEED_PLAYER_X;
					st_p_Player->_Y -= dfSPEED_PLAYER_Y;
#ifdef UPDATE_DEBUG
					_LOG(0, L"# SessionID:%d / CurX:%d / CurY:%d", st_p_Player->_SessionID, st_p_Player->_X, st_p_Player->_Y);

#endif // UPDATE_DEBUG
				}
				break;
			case dfPACKET_MOVE_DIR_RR:
				if (CharacterMoveCheck(st_p_Player->_X + dfSPEED_PLAYER_X, st_p_Player->_Y))
				{
					st_p_Player->_X += dfSPEED_PLAYER_X;
#ifdef UPDATE_DEBUG
					_LOG(0, L"# SessionID:%d / CurX:%d / CurY:%d", st_p_Player->_SessionID, st_p_Player->_X, st_p_Player->_Y);

#endif // UPDATE_DEBUG
				}
				break;
			case dfPACKET_MOVE_DIR_RD:
				if (CharacterMoveCheck(st_p_Player->_X + dfSPEED_PLAYER_X, st_p_Player->_Y + dfSPEED_PLAYER_Y))
				{
					st_p_Player->_X += dfSPEED_PLAYER_X;
					st_p_Player->_Y += dfSPEED_PLAYER_Y;
#ifdef UPDATE_DEBUG
					_LOG(0, L"# SessionID:%d / CurX:%d / CurY:%d", st_p_Player->_SessionID, st_p_Player->_X, st_p_Player->_Y);

#endif // UPDATE_DEBUG
				}
				break;
			case dfPACKET_MOVE_DIR_DD:
				if (CharacterMoveCheck(st_p_Player->_X, st_p_Player->_Y + dfSPEED_PLAYER_Y))
				{
					st_p_Player->_Y += dfSPEED_PLAYER_Y;
#ifdef UPDATE_DEBUG
					_LOG(0, L"# SessionID:%d / CurX:%d / CurY:%d", st_p_Player->_SessionID, st_p_Player->_X, st_p_Player->_Y);

#endif // UPDATE_DEBUG
				}
				break;
			case dfPACKET_MOVE_DIR_LD:
				if (CharacterMoveCheck(st_p_Player->_X - dfSPEED_PLAYER_X, st_p_Player->_Y + dfSPEED_PLAYER_Y))
				{
					st_p_Player->_X -= dfSPEED_PLAYER_X;
					st_p_Player->_Y += dfSPEED_PLAYER_Y;
#ifdef UPDATE_DEBUG
					_LOG(0, L"# SessionID:%d / CurX:%d / CurY:%d", st_p_Player->_SessionID, st_p_Player->_X, st_p_Player->_Y);

#endif // UPDATE_DEBUG
				}
				break;
			case dfPACKET_CS_MOVE_STOP:
				break;

			default:
			{
				// 이상함 유저의 연결을 끊는다.
				_LOG(dfLOG_LEVEL_SYSTEM, L"#Update error!!! Delete - SessionID:%d", st_p_Player->_SessionID);
				enqueueForDeletion(st_p_Player->_SessionID);
				return;
			}
		}

		//--------------------------------------------------------------------------------------
		// 이동 액션인 경우, 섹터를 체크하여 변경해준다. 
		//--------------------------------------------------------------------------------------
		// 이동인 경우 섹터 업데이트를 함.
		if (C_Field::GetInstance()->Sector_UpdateCharacter(st_p_Player))
		{
			// 섹터가 변경된 경우는 클라에게 관련 정보를 쏜다. 
			C_Field::GetInstance()->CharacterSectorUpdatePacket(st_p_Player);
		}


	}





}

bool CharacterMoveCheck(int SectorX, int SectorY)
{
	if (SectorX < dfRANGE_MOVE_LEFT || dfRANGE_MOVE_RIGHT < SectorX || SectorY < dfRANGE_MOVE_TOP || dfRANGE_MOVE_BOTTOM < SectorY)
		return false;

	return true;
}
