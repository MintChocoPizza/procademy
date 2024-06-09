
#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>

#include <stdio.h>

#include "C_Ring_Buffer.h"
#include "NetWork.h"
#include "PacketDefine.h"
#include "Set_Log.h"

//---------------------------------------------------------------
// 헤더와 패킷이 모두 char* 이다. 
// 
// 큰 의미 없다. 그냥 보기 예쁘게 맞춘것이다.
//---------------------------------------------------------------

bool netPacketProc_SC_CREATE_MY_CHARACTER(st_SESSION* p_Session, char* pHeader, char* pPacket)
{
	((st_PACKET_HEADER*)pHeader)->byCode = 0x89;
	((st_PACKET_HEADER*)pHeader)->bySize = sizeof(st_PACKET_SC_CREATE_MY_CHARACTER);
	((st_PACKET_HEADER*)pHeader)->byType = dfPACKET_SC_CREATE_MY_CHARACTER;

	((st_PACKET_SC_CREATE_MY_CHARACTER*)pPacket)->Direction = p_Session->byDirection;
	((st_PACKET_SC_CREATE_MY_CHARACTER*)pPacket)->HP = p_Session->chHP;
	((st_PACKET_SC_CREATE_MY_CHARACTER*)pPacket)->ID = p_Session->dwSessionID;
	((st_PACKET_SC_CREATE_MY_CHARACTER*)pPacket)->X = p_Session->shX;
	((st_PACKET_SC_CREATE_MY_CHARACTER*)pPacket)->Y = p_Session->shY;

	return true;
}
bool netPacketProc_SC_CREATE_OTHER_CHARACTER(st_SESSION* p_Session, char* pHeader, char* pPacket)
{
	((st_PACKET_HEADER*)pHeader)->byCode = 0x89;
	((st_PACKET_HEADER*)pHeader)->bySize = sizeof(st_PACKET_SC_CREATE_OTHER_CHARACTER);
	((st_PACKET_HEADER*)pHeader)->byType = dfPACKET_SC_CREATE_OTHER_CHARACTER;

	((st_PACKET_SC_CREATE_OTHER_CHARACTER*)pPacket)->Direction = p_Session->byDirection;
	((st_PACKET_SC_CREATE_OTHER_CHARACTER*)pPacket)->HP = p_Session->chHP;
	((st_PACKET_SC_CREATE_OTHER_CHARACTER*)pPacket)->ID = p_Session->dwSessionID;
	((st_PACKET_SC_CREATE_OTHER_CHARACTER*)pPacket)->X = p_Session->shX;
	((st_PACKET_SC_CREATE_OTHER_CHARACTER*)pPacket)->Y = p_Session->shY;

	return true;
}

bool netPacketProc_SC_DELETE_CHARACTER(st_SESSION* p_Session, char* pHeader, char* pPacket)
{
	((st_PACKET_HEADER*)pHeader)->byCode = 0x89;
	((st_PACKET_HEADER*)pHeader)->bySize = sizeof(st_PACKET_SC_DELETE_CHARACTER);
	((st_PACKET_HEADER*)pHeader)->byType = dfPACKET_SC_DELETE_CHARACTER;

	((st_PACKET_SC_DELETE_CHARACTER*)pPacket)->ID = p_Session->dwSessionID;
	return true;
}

bool netPacketProc_CS_MOVE_START(st_SESSION* p_Session, char* pPacket)
{
	st_PACKET_HEADER Header;
	st_PACKET_SC_MOVE_START SendMsg;
	st_PACKET_CS_MOVE_START* pMoveStart = (st_PACKET_CS_MOVE_START*)pPacket;
	
	// 메시지 수신 로그 확인
	//---------------------------------------------------------------------------------------------------
	// 서버의 위치와 받은 패킷의 위치값이 너무 큰 차이가 난다면 끊어버림
	// 본 게임의 좌표 동기화 구조가 단순한 키보드 조작 (클라이언트의 선처리, 서버의 후 반영) 방식으로
	// 클라이언트의 좌표를 그대로 믿는 방식을 택하고 있음.
	// 실제 온라인 게임이라면 클라이언트에서 목적지를 공유하는 방식을 택해야 함.
	// 지금은 간단한 구현을 목적으로 하고 있으므로 오차범위 내에서 클라이언트 좌표를 믿도록 한다. 
	//---------------------------------------------------------------------------------------------------

	if (abs(p_Session->shX - pMoveStart->X) > dfERROR_RANGE ||
		abs(p_Session->shY - pMoveStart->Y) > dfERROR_RANGE)
	{
		PushDisconnectList(p_Session);
#ifdef DEFAULT_LOG
		printf_s("# Error Log: CS_MOVE_START - SessionID: %d : dfERROR_RANGE \n", p_Session->dwSessionID);
#endif // DEFAULT_LOG
		return false;
	}

	//---------------------------------------------------------------------------------------------------
	// 동작을 변경. 지금 구현에선 동작번호가 방향값이다.
	//---------------------------------------------------------------------------------------------------
	p_Session->dwAction = pMoveStart->Direction;

	//---------------------------------------------------------------------------------------------------
	// 방향을 변경. 아니면 기존의 방향을 유지하는 결정을 내린다.
	//---------------------------------------------------------------------------------------------------
	switch (pMoveStart->Direction)
	{
	case dfPACKET_MOVE_DIR_RR:
	case dfPACKET_MOVE_DIR_RU:
	case dfPACKET_MOVE_DIR_RD:
		p_Session->byDirection = dfPACKET_MOVE_DIR_RR;
		break;

	case dfPACKET_MOVE_DIR_LU:
	case dfPACKET_MOVE_DIR_LL:
	case dfPACKET_MOVE_DIR_LD:
		p_Session->byDirection = dfPACKET_MOVE_DIR_LL;
		break;
	default:
		break;
	}

	// 서버의 위치와 실제 클라의 위치가 틀어진경우, 현재 코드에서는 클라의 위치를 믿고 있다. 
	p_Session->shX = pMoveStart->X;
	p_Session->shY = pMoveStart->Y;

	//---------------------------------------------------------------------------------------------------
	// 현재 접속중인 사용자에게 모든 패킷을 뿌린다. 당사자 제외
	//---------------------------------------------------------------------------------------------------
	netPacketProc_SC_MOVE_START((char*)&Header, (char*)&SendMsg, p_Session->byDirection, p_Session->dwSessionID, p_Session->shX, p_Session->shY);
	netSendBroadcast(p_Session, (char*)&Header, (char*)&SendMsg, sizeof(SendMsg));

	return true;
}
bool netPacketProc_SC_MOVE_START(char* pHeader, char* pPacket, char Direction, __int32 ID, short X, short Y)
{
	((st_PACKET_HEADER*)pHeader)->byCode = dfNETWORK_PACKET_CODE;
	((st_PACKET_HEADER*)pHeader)->bySize = sizeof(st_PACKET_CS_MOVE_START);
	((st_PACKET_HEADER*)pHeader)->byType = dfPACKET_CS_MOVE_START;

	((st_PACKET_SC_MOVE_START*)pPacket)->Direction = Direction;
	((st_PACKET_SC_MOVE_START*)pPacket)->ID = ID;
	((st_PACKET_SC_MOVE_START*)pPacket)->X = X;
	((st_PACKET_SC_MOVE_START*)pPacket)->Y = Y;

	return true;
}

bool netPacketProc_CS_MOVE_STOP(st_SESSION* p_Session, char* pPacket)
{
	st_PACKET_HEADER Header;
	st_PACKET_SC_MOVE_STOP SendMsg;
	st_PACKET_CS_MOVE_STOP* pMoveStop = (st_PACKET_CS_MOVE_STOP*)pPacket;

	if (abs(p_Session->shX - pMoveStop->X) > dfERROR_RANGE ||
		abs(p_Session->shY - pMoveStop->Y) > dfERROR_RANGE)
	{
		PushDisconnectList(p_Session);
#ifdef DEFAULT_LOG
		printf_s("# Error Log: CS_MOVE_STOP - SessionID: %d : dfERROR_RANGE \n", p_Session->dwSessionID);
#endif // DEFAULT_LOG
		return false;
	}

	//---------------------------------------------------------------------------------------------------
	// 동작을 변경. 지금 구현에선 동작번호가 방향값이다.
	//---------------------------------------------------------------------------------------------------
	p_Session->dwAction = pMoveStop->Direction;

	//---------------------------------------------------------------------------------------------------
	// 방향을 변경. 아니면 기존의 방향을 유지하는 결정을 내린다.
	// 
	// 하지만 현재 상황에서는 이동하던 유지하던 방향을 그대로 중지하여 변경할 일이 없다.
	// 
	//---------------------------------------------------------------------------------------------------
	switch (pMoveStop->Direction)
	{
	case dfPACKET_MOVE_DIR_RR:
	case dfPACKET_MOVE_DIR_RU:
	case dfPACKET_MOVE_DIR_RD:
		p_Session->byDirection = dfPACKET_MOVE_DIR_RR;
		break;

	case dfPACKET_MOVE_DIR_LU:
	case dfPACKET_MOVE_DIR_LL:
	case dfPACKET_MOVE_DIR_LD:
		p_Session->byDirection = dfPACKET_MOVE_DIR_LL;
		break;
	default:
		break;
	}
	// 서버의 위치와 실제 클라의 위치가 틀어진경우, 현재 코드에서는 클라의 위치를 믿고 있다. 
	p_Session->shX = pMoveStop->X;
	p_Session->shY = pMoveStop->Y;

	//---------------------------------------------------------------------------------------------------
	// 현재 접속중인 사용자에게 모든 패킷을 뿌린다. 당사자 제외
	//---------------------------------------------------------------------------------------------------
	netPacketProc_SC_MOVE_STOP((char*)&Header, (char*)&SendMsg, p_Session->byDirection, p_Session->dwSessionID, p_Session->shX, p_Session->shY);
	netSendBroadcast(p_Session, (char*)&Header, (char*)&SendMsg, sizeof(SendMsg));

	return true;
}
bool netPacketProc_SC_MOVE_STOP(char* pHeader, char* pPacket, char Direction, __int32 ID, short X, short Y)
{
	((st_PACKET_HEADER*)pHeader)->byCode = 0x89;
	((st_PACKET_HEADER*)pHeader)->bySize = sizeof(st_PACKET_SC_MOVE_STOP);
	((st_PACKET_HEADER*)pHeader)->byType = dfPACKET_SC_MOVE_STOP;

	((st_PACKET_SC_MOVE_STOP*)pPacket)->Direction = Direction;
	((st_PACKET_SC_MOVE_STOP*)pPacket)->ID = ID;
	((st_PACKET_SC_MOVE_STOP*)pPacket)->X = X;
	((st_PACKET_SC_MOVE_STOP*)pPacket)->Y = Y;
	return true;
}

bool netPacketProc_CS_ATTACK1(st_SESSION* p_Session, char* pPacket)
{
	p_Session->byDirection = ((st_PACKET_CS_ATTACK1*)pPacket)->Direction;
	p_Session->shX = ((st_PACKET_CS_ATTACK1*)pPacket)->X;
	p_Session->shY = ((st_PACKET_CS_ATTACK1*)pPacket)->Y;
	return true;
}
bool netPacketProc_SC_ATTACK1(st_SESSION* p_Session, char* pHeader, char* pPacket)
{
	((st_PACKET_HEADER*)pHeader)->byCode = 0x89;
	((st_PACKET_HEADER*)pHeader)->bySize = sizeof(st_PACKET_SC_ATTACK1);
	((st_PACKET_HEADER*)pHeader)->byType = dfPACKET_SC_ATTACK1;

	((st_PACKET_SC_ATTACK1*)pPacket)->Direction = p_Session->byDirection;
	((st_PACKET_SC_ATTACK1*)pPacket)->ID = p_Session->dwSessionID;
	((st_PACKET_SC_ATTACK1*)pPacket)->X = p_Session->shX;
	((st_PACKET_SC_ATTACK1*)pPacket)->Y = p_Session->shY;
	return true;
}

bool netPacketProc_CS_ATTACK2(st_SESSION* p_Session, char* pPacket)
{
	p_Session->byDirection = ((st_PACKET_CS_ATTACK2*)pPacket)->Direction;
	p_Session->shX = ((st_PACKET_CS_ATTACK2*)pPacket)->X;
	p_Session->shY = ((st_PACKET_CS_ATTACK2*)pPacket)->Y;
	return true;
}
bool netPacketProc_SC_ATTACK2(st_SESSION* p_Session, char* pHeader, char* pPacket)
{
	((st_PACKET_HEADER*)pHeader)->byCode = 0x89;
	((st_PACKET_HEADER*)pHeader)->bySize = sizeof(st_PACKET_SC_ATTACK2);
	((st_PACKET_HEADER*)pHeader)->byType = dfPACKET_SC_ATTACK2;

	((st_PACKET_SC_ATTACK2*)pPacket)->Direction = p_Session->byDirection;
	((st_PACKET_SC_ATTACK2*)pPacket)->ID = p_Session->dwSessionID;
	((st_PACKET_SC_ATTACK2*)pPacket)->X = p_Session->shX;
	((st_PACKET_SC_ATTACK2*)pPacket)->Y = p_Session->shY;
	return true;
}

bool netPacketProc_CS_ATTACK3(st_SESSION* p_Session, char* pPacket)
{
	p_Session->byDirection = ((st_PACKET_CS_ATTACK3*)pPacket)->Direction;
	p_Session->shX = ((st_PACKET_CS_ATTACK3*)pPacket)->X;
	p_Session->shY = ((st_PACKET_CS_ATTACK3*)pPacket)->Y;
	return true;
}
bool netPacketProc_SC_ATTACK3(st_SESSION* p_Session, char* pHeader, char* pPacket)
{
	((st_PACKET_HEADER*)pHeader)->byCode = 0x89;
	((st_PACKET_HEADER*)pHeader)->bySize = sizeof(st_PACKET_SC_ATTACK3);
	((st_PACKET_HEADER*)pHeader)->byType = dfPACKET_SC_ATTACK3;

	((st_PACKET_SC_ATTACK3*)pPacket)->Direction = p_Session->byDirection;
	((st_PACKET_SC_ATTACK3*)pPacket)->ID = p_Session->dwSessionID;
	((st_PACKET_SC_ATTACK3*)pPacket)->X = p_Session->shX;
	((st_PACKET_SC_ATTACK3*)pPacket)->Y = p_Session->shY;
	return true;
}

bool netPacketProc_SC_DAMAGE(st_SESSION* p_Session, st_SESSION* p_Damage_Session, char* pPacket, int Damage)
{
	((st_PACKET_SC_DAMAGE*)pPacket)->AttackID = p_Session->dwSessionID;
	((st_PACKET_SC_DAMAGE*)pPacket)->DamageID = p_Damage_Session->dwSessionID;
	((st_PACKET_SC_DAMAGE*)pPacket)->DamageHP = p_Damage_Session->chHP - Damage;
	return true;
}
