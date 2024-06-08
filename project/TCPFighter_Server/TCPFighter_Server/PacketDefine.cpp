
#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include "C_Ring_Buffer.h"
#include "NetWork.h"
#include "PacketDefine.h"



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
	p_Session->byDirection = ((st_PACKET_CS_MOVE_START*)pPacket)->Direction;
	p_Session->shX = ((st_PACKET_CS_MOVE_START*)pPacket)->X;
	p_Session->shY = ((st_PACKET_CS_MOVE_START*)pPacket)->Y;
	return true;
}
bool netPacketProc_SC_MOVE_START(st_SESSION* p_Session, char* pHeader, char* pPacket)
{
	((st_PACKET_HEADER*)pHeader)->byCode = 0x89;
	((st_PACKET_HEADER*)pHeader)->bySize = sizeof(st_PACKET_SC_MOVE_START);
	((st_PACKET_HEADER*)pHeader)->byType = dfPACKET_SC_MOVE_START;

	((st_PACKET_SC_MOVE_START*)pPacket)->Direction = p_Session->byDirection;
	((st_PACKET_SC_MOVE_START*)pPacket)->ID = p_Session->dwSessionID;
	((st_PACKET_SC_MOVE_START*)pPacket)->X = p_Session->shX;
	((st_PACKET_SC_MOVE_START*)pPacket)->Y = p_Session->shY;
	return true;
}

bool netPacketProc_CS_MOVE_STOP(st_SESSION* p_Session, char* pPacket)
{
	p_Session->byDirection = ((st_PACKET_CS_MOVE_STOP*)pPacket)->Direction;
	p_Session->shX = ((st_PACKET_CS_MOVE_STOP*)pPacket)->X;
	p_Session->shY = ((st_PACKET_CS_MOVE_STOP*)pPacket)->Y;
	return true;
}
bool netPacketProc_SC_MOVE_STOP(st_SESSION* p_Session, char* pHeader, char* pPacket)
{
	((st_PACKET_HEADER*)pHeader)->byCode = 0x89;
	((st_PACKET_HEADER*)pHeader)->bySize = sizeof(st_PACKET_SC_MOVE_STOP);
	((st_PACKET_HEADER*)pHeader)->byType = dfPACKET_SC_MOVE_STOP;

	((st_PACKET_SC_MOVE_STOP*)pPacket)->Direction = p_Session->byDirection;
	((st_PACKET_SC_MOVE_STOP*)pPacket)->ID = p_Session->dwSessionID;
	((st_PACKET_SC_MOVE_STOP*)pPacket)->X = p_Session->shX;
	((st_PACKET_SC_MOVE_STOP*)pPacket)->Y = p_Session->shY;
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

bool netPakcetProc_CS_ATTACK3(st_SESSION* p_Session, char* pPacket)
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
	((st_PACKET_SC_DAMAGE*)pPacket)->DamageID - p_Damage_Session->dwSessionID;
	((st_PACKET_SC_DAMAGE*)pPacket)->DamageHP = p_Damage_Session->chHP - Damage;
	return true;
}
