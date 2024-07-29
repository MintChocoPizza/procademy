

#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#pragma comment (lib, "Ws2_32.lib")
#include <queue>
#include <unordered_map>
#include "LOG.h"
#include "Protocol.h"
#include "C_Ring_Buffer.h"
#include "SerializeBuffer.h"
#include "Disconnect.h"
#include "main.h"
#include "Protocol.h"
#include "Session.h"
#include "Define.h"
#include "Player.h"
#include "Field.h"

constexpr int dfMAX_SESSION = 8000;
constexpr int dfNETWORK_PORT = 20000;
//constexpr int dfNETWORK_PORT = 12994;

std::unordered_map< DWORD, st_SESSION*> g_Session_Hash;
WSADATA g_WsaData;
SOCKET g_Listen_Socket;
//DWORD g_SessionID;
std::queue<DWORD> g_SessionID_Q;

st_SESSION* FindSession(DWORD dwSessionID)
{
	return g_Session_Hash.find(dwSessionID)->second;
}

void SetSession(void)
{
	int iCnt;

	for (iCnt = 0; iCnt < dfMAX_SESSION; ++iCnt)
	{
		g_SessionID_Q.push(iCnt);
	}
}

void DeleteSession(DWORD dwSessionID)
{
	st_SESSION* pSession;
	std::unordered_map<DWORD, st_SESSION*>::iterator iter;

	iter = g_Session_Hash.find(dwSessionID);
	pSession = iter->second;

	delete pSession;
	g_Session_Hash.erase(iter);

	g_SessionID_Q.push(dwSessionID);
}

void netStartUp(void)
{
	// g_SessionID = 0;

	struct addrinfo hints;
	struct addrinfo* result = NULL;
	SOCKET Temp_Listen_Socket = g_Listen_Socket;
	char Port[6];
	u_long on;
	linger Linger_Opt;


	//---------------------------------------------------
	// return 값 저장 변수 
	int Ret_WSAStartup;
	int Ret_getaddrinfo;
	int Ret_bind;
	int Ret_listen;
	int Ret_ioctlsocket;
	int Ret_setsockopt;
	errno_t Ret_itoa_s;


	Temp_Listen_Socket = g_Listen_Socket;


	//---------------------------------------------------
	// Initialize Winsock
	Ret_WSAStartup = WSAStartup(MAKEWORD(2, 2), &g_WsaData);
	if (Ret_WSAStartup != 0)
	{
		_LOG(0, L"WSAStartup failed with error: %d", Ret_WSAStartup);
		__debugbreak();
	}
	_LOG(0, L"WSAStartup # ");

	//---------------------------------------------------
	// SetUp hints 
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	//---------------------------------------------------
	// Resolve the server address and port
	// 서버 주소 및 포트 확인
	// 아래의 Port 코드는 C6054 : 'Port' 문자열이 0으로 종료되지 않을 수 있습니다. 경고를 해결하기 위하여 작성하였다. 
	Ret_itoa_s = _itoa_s(dfNETWORK_PORT, Port, sizeof(Port), 10);
	if (Ret_itoa_s != NULL)
	{
		_LOG(0, L"_itoa_s failed with error");
		WSACleanup();
		__debugbreak();
	}
	Port[sizeof(Port) - 1] = '\0';
	Ret_getaddrinfo = getaddrinfo(NULL, Port, &hints, &result);
	if (Ret_getaddrinfo != 0)
	{
		_LOG(0, L"getaddrinfo failed with error: %d", Ret_getaddrinfo);
		WSACleanup();
		__debugbreak();
	}

	//---------------------------------------------------
	// Create a SOCKET for the server to listen for client connections.
	// 서버가 클라이언트 연결을 수신을 SOCKET을 만든다.
	Temp_Listen_Socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (Temp_Listen_Socket == INVALID_SOCKET)
	{
		_LOG(0, L"socket failed with error : %ld", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		__debugbreak();
	}

	//---------------------------------------------------
	// setsockopt_Linger
	Linger_Opt.l_onoff = 1;
	Linger_Opt.l_linger = 0;
	Ret_setsockopt = setsockopt(Temp_Listen_Socket, SOL_SOCKET, SO_LINGER, (char*)&Linger_Opt, sizeof(Linger_Opt));
	if (Ret_setsockopt == SOCKET_ERROR)
	{
		_LOG(0, L"setsockopt failed with error: %ld", WSAGetLastError());
		closesocket(Temp_Listen_Socket);
		WSACleanup();
		__debugbreak();
	}

	//---------------------------------------------------
	// Setup the TCP listening socket
	// TCP listening socket 을 준비한다.
	Ret_bind = bind(Temp_Listen_Socket, result->ai_addr, (int)result->ai_addrlen);
	if (Ret_bind == SOCKET_ERROR)
	{
		_LOG(0, L"bind failed with error: %d", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(Temp_Listen_Socket);
		WSACleanup();
		__debugbreak();
	}
	_LOG(0, L"Bind OK # Port: %d", dfNETWORK_PORT);

	freeaddrinfo(result);

	//---------------------------------------------------
	// Listen a client socket
	Ret_listen = listen(Temp_Listen_Socket, SOMAXCONN);
	//i_Result = listen(Temp_Listen_Socket, SOMAXCONN_HINT(65535));	// 양수 ~200 -> 기본 200개, 200~ 갯수 적용,    
	if (Ret_listen == SOCKET_ERROR)
	{
		_LOG(0, L"listen failed with error: %d", WSAGetLastError());
		closesocket(Temp_Listen_Socket);
		WSACleanup();
		__debugbreak();
	}
	_LOG(0, L"Listen OK # ");


	//---------------------------------------------------
	// 넌블로킹 소켓으로 전환
	on = 1;
	Ret_ioctlsocket = ioctlsocket(Temp_Listen_Socket, FIONBIO, &on);
	if (Ret_ioctlsocket == SOCKET_ERROR)
	{
		_LOG(0, L"ioctlsocket failed with error: %ld", WSAGetLastError());
		closesocket(Temp_Listen_Socket);
		WSACleanup();
		__debugbreak();
	}

	g_Listen_Socket = Temp_Listen_Socket;
}

void netCleanUp(void)
{
	closesocket(g_Listen_Socket);
	_LOG(0, L"Close Listen_Socket # \n");

	WSACleanup();
	_LOG(0, L"WSACleanup # \n");

	// _Session_Map 순회를 돌면서 삭제해야 하는데,,,, 삭제 구조가 꼬이기 시작했다.
	// 아마도 할당한 쪽에서 데이터에 대한 삭제를 마무리 하는게 좋아 보인다. 
	// 여기서는 여기 클래스에서 생성된 것들만 정리하고 지워준다. 
	// Socket, RecvQ, SendQ
	std::unordered_map<DWORD, st_SESSION*>::iterator iter;
	st_SESSION* st_Temp_Session;
	for (iter = g_Session_Hash.begin(); iter != g_Session_Hash.end();)
	{
		st_Temp_Session = iter->second;

		closesocket(st_Temp_Session->Socket);
		delete st_Temp_Session->RecvQ;
		delete st_Temp_Session->SendQ;
		delete st_Temp_Session;

		iter = g_Session_Hash.erase(iter);
	}
	_LOG(0, L"CleanUp Session_Hash # \n");


}

void netIOProcess(void)
{
	std::unordered_map<DWORD, st_SESSION*>::iterator	iter;
	std::unordered_map<DWORD, st_SESSION*>::iterator	iter_FD_ISSET;
	st_SESSION* st_pSession;
	SOCKET Listen_Socket;
	FD_SET ReadSet;
	FD_SET WriteSet;
	timeval Time;

	int iCnt;
	int i_Result;

	Listen_Socket = g_Listen_Socket;
	iter = g_Session_Hash.begin();
	//--------------------------------------------------------------------------------------------------------------------
	// _Session 전부를 Select에 등록하면 while문을 종료한다. 
	// 
	//--------------------------------------------------------------------------------------------------------------------
	do
	//while (iter != g_Session_Hash.end())
	{
		FD_ZERO(&ReadSet);
		FD_ZERO(&WriteSet);

		//------------------------------------------
		// Listen_Socket 넣기
		//------------------------------------------
		FD_SET(Listen_Socket, &ReadSet);

		//------------------------------------------
		// 리슨소켓 및 접속중인 모든 클라이언트에 대해 SOCKET 을 체크한다. 
		// 
		// 마지막 _Session이거나 or _Listen_Socket 포함 64개의 소켓을 Select에 등록했다면, 반복문을 종료한다. 
		//------------------------------------------
		for (iCnt = 0; iCnt < 64 - 1 && iter != g_Session_Hash.end(); ++iter, ++iCnt)
		{
			st_pSession = iter->second;

			//------------------------------------------
			// 해당 클라이언트 Read Set 등록 / SendQ 에 데이터가 있다면 Write Set 등록
			//------------------------------------------
			FD_SET(st_pSession->Socket, &ReadSet);
			if (st_pSession->SendQ->GetUseSize() > 0)
				FD_SET(st_pSession->Socket, &WriteSet);
		}

		//------------------------------------------
		// NULL: 한개라도 응답 올 때까지 무한히 기다림
		// 0,0: 응답올때까지 기다리지 않고 바로 끝낸다.
		// 양수: 한개라도 응답하거나, 지정된 시간이 지나면 리턴
		//------------------------------------------
		Time.tv_sec = 0;
		Time.tv_usec = 0;

		i_Result = select(0, &ReadSet, &WriteSet, 0, &Time);

		if (i_Result == SOCKET_ERROR)
		{
			_LOG(0, L"select failed with error: %ld \n", WSAGetLastError());
			__debugbreak();
		}


		//------------------------------------------
		// 리턴값이 0 이상이라면 누군가의 데이터가 왔다.
		//------------------------------------------
		while (i_Result > 0)
		{
			if (FD_ISSET(Listen_Socket, &ReadSet))
			{
				netProc_Accept();
				--i_Result;
			}

			//------------------------------------------
			// 전체 세션중 어떤 세션이 반응을 보였는지 다시 확인한다. 
			//------------------------------------------
			for (iter_FD_ISSET = g_Session_Hash.begin(); iter_FD_ISSET != g_Session_Hash.end(); ++iter_FD_ISSET)
			{
				st_pSession = (*iter_FD_ISSET).second;

				if (FD_ISSET(st_pSession->Socket, &ReadSet))
				{
					netProc_Recv(st_pSession);
					--i_Result;
				}

				if (FD_ISSET(st_pSession->Socket, &WriteSet))
				{
					netProc_Send(st_pSession);
					--i_Result;
				}
			}
		}
	} while (iter != g_Session_Hash.end());

	Disconnect();
}

void netProc_Accept(void)
{
	//---------------------------------------------------------------------------------
	// 새로운 연결에 대하여 accept를 하고,
	// SessionID를 할당하고, 
	// 내 눈에 보이는 시야에 한정하여 캐릭터 생성을 보낸다. 
	//---------------------------------------------------------------------------------

	int Error;

	SOCKET New_Client_Socket;
	sockaddr_in Clinet_Addr;

	st_SECTOR_AROUND st_Sector_Around;

	st_SESSION* st_p_New_Session;
	st_PLAYER* st_p_New_Player;

	DWORD SessionID;
	int Client_Addr_Len;

	char host[NI_MAXHOST];

	Client_Addr_Len = sizeof(Clinet_Addr);
	New_Client_Socket = accept(g_Listen_Socket, (sockaddr*)&Clinet_Addr, &Client_Addr_Len);
	if (New_Client_Socket == INVALID_SOCKET)
	{
		Error = WSAGetLastError();

		if (Error == WSAEWOULDBLOCK)
		{
			// Seletc로 거르고 들어왔는데 WSAEWOULDBLOCK이 나오는지 모르겠다. 
			_LOG(1, L"accept failed with error: %ld \n", Error);
			//__debugbreak();
			// 원래는 에러로 처리하면 안된다. 
			return;
		}
		else
		{
			_LOG(0, L"accept failed with error: %ld \n", Error);
			WSACleanup();
			__debugbreak();
		}
	}


	// 접속자 인원수 제한. 순간적인 피크를 포함하여 약 8000명으로 가정한다. 
	if (g_Session_Hash.size() > dfMAX_SESSION)
	{
		_LOG(2, L"Full Server cannot accept!!! \n");
		closesocket(New_Client_Socket);
		return;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// 1. 새로운 세션 만들기.
	// 2. 새로운 플레이어 만들기. 
	// 3. 나에게 생성 메시지 보내기.
	// 4. 주변에 나에대한 생성 메시지를 보낸다. 
	// 5. 행동중인 플레이어가 있다면 동작을 이어서 보여준다. 
	// 
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// 1.
	SessionID = g_SessionID_Q.front();
	g_SessionID_Q.pop();
	st_p_New_Session = new st_SESSION(New_Client_Socket, SessionID);
	g_Session_Hash.insert({ SessionID, st_p_New_Session });
	// 2.
	st_p_New_Player = CreateNewPlayer(SessionID, st_p_New_Session);
	// 3.

	mpCreateMyCharacter(&g_Packet, SessionID, st_p_New_Player->_byDirection, st_p_New_Player->_X, st_p_New_Player->_Y, st_p_New_Player->_HP);
	SendPacket_Unicast(st_p_New_Session, &g_Packet);
	g_Packet.Clear();
	// 4.
	mpCreateOtherCharacter(&g_Packet, SessionID, st_p_New_Player->_byDirection, st_p_New_Player->_X, st_p_New_Player->_Y, st_p_New_Player->_HP);
	C_Field::GetInstance()->GetSectorAround(st_p_New_Player->_CurSector->iX, st_p_New_Player->_CurSector->iY, &st_Sector_Around);
	C_Field::GetInstance()->SendPacket_Around(st_p_New_Session, &g_Packet, &st_Sector_Around);
	g_Packet.Clear();
	// 5. 

	inet_ntop(AF_INET, &Clinet_Addr.sin_addr, host, NI_MAXHOST);
	_c_LOG(1, "Conncet # IP:%s : Port: %d / SessionID: %d", host, ntohs(Clinet_Addr.sin_port), SessionID);
}

int netProc_Recv(st_SESSION* pSession)
{

	st_PACKET_HEADER st_Header;

	int i_ret_Recv_Size;
	int i_ret_WSAError;
	size_t ret_Q_UseSize;
	size_t ret_Peek;
	size_t ret_Dq_Size;


	i_ret_Recv_Size = recv(pSession->Socket, pSession->RecvQ->GetRearBufferPtr(), (int)pSession->RecvQ->DirectEnqueueSize(), 0);
	if (i_ret_Recv_Size == 0)
	{
		// 아마도 상대방의 연결 종료
		_LOG(1, L"Disconnect recv 0 # SessionID: %d", pSession->dwSessionID);

		// 연결 끊기
		//
		enqueueForDeletion(pSession->dwSessionID);

		return -1;
	}
	else if (i_ret_Recv_Size == SOCKET_ERROR)
	{
		//////////////////////////////////////////////////////////////////
		// 여기서 연결을 끊지 않아도 아래에서 강제로 연결이 끊긴다.
		//////////////////////////////////////////////////////////////////

		i_ret_WSAError = WSAGetLastError();
		if (i_ret_WSAError == WSAEWOULDBLOCK)
		{
			//----------------------------------------------------------------------------------------
			// Selete로 걸러서 Recv 할 수 있는 상황에서 WSAEWOULDBLOCK은 절대로 뜨면 안된다.
			// 하지만 원장님에게 물어본 결과 싱글스레드에서는 맞는 말 일 수 있지만,
			// 멀티스레드인 경우는 발생할 수 있다고 한다. 그러므로 넘어간다.
			//----------------------------------------------------------------------------------------
			_LOG(0, L"WSAEWOULDBLOCK # Nect Frame Recv try %d # SessionID: %d", i_ret_WSAError, pSession->dwSessionID);
			return 0;
		}
		else if (i_ret_WSAError == 10054)
		{
			// 상대방이 강제로 연결을 끊었다. 
			_LOG(1, L"Disconnect Recv %d # SessionID: %d \n", i_ret_WSAError, pSession->dwSessionID);

			// 연결 끊기
			//
			enqueueForDeletion(pSession->dwSessionID);

			return -1;
		}
		else if (i_ret_WSAError == 10053)
		{
			// 소프트웨어로 인해 연결이 중단됨
			// 데이터 전송 시간 제한 또는 프로토콜 오류
			_LOG(1, L"Disconnect Recv %d # SessionID: %d \n", i_ret_WSAError, pSession->dwSessionID);

			// 연결 끊기
			//
			enqueueForDeletion(pSession->dwSessionID);

			return -1;
		}
		else
		{
			// 알 수 없는 이유로 연결이 끊겼다. 
			// 로그를 확인하고 원인을 파악한다음 처리한다. 

			_LOG(1, L"Disconnect Recv %d # SessionID: %d \n", i_ret_WSAError, pSession->dwSessionID);
			__debugbreak();
		}
	}
	pSession->RecvQ->MoveRear(i_ret_Recv_Size);

	//////////////////////////////////////////////////////////////////
	// * 완료 패킷 처리부
	// 
	// 수신 버퍼에서 모든 메시지를 꺼낸 다음에 
	// 반복문을 돌면서 완성된 메시지를 처리한다.
	//////////////////////////////////////////////////////////////////
	while (1)
	{
		//---------------------------------------------------
		// 기저 사례
		//---------------------------------------------------
		
		// 기저 사례1. RecvQ에 최소한의 사이즈가 있는지 확인. 조건: 헤더 사이즈 초과
		ret_Q_UseSize = pSession->RecvQ->GetUseSize();
		if (ret_Q_UseSize < sizeof(st_PACKET_HEADER)) return 0;

		// 헤더를 열어서 페이로드 사이즈를 확인한다. 
		ret_Peek = pSession->RecvQ->Peek((char*)&st_Header, sizeof(st_PACKET_HEADER), true);

		// 기저 사례2. st_Header에서 byCode를 확인한다. 다르면 연결 끊기
		if (st_Header.byCode != (char)dfPACKET_CODE)
		{
			_LOG(1, L"Header code Error # SessionID: %d", pSession->dwSessionID);

			// 연결 끊기
			//
			enqueueForDeletion(pSession->dwSessionID);

			return -1;
		}

		// 기저 사례3. 페이로드가 완성되지 않았다면 반환한다.
		if (pSession->RecvQ->GetUseSize() < sizeof(st_PACKET_HEADER) + st_Header.bySize)
		{
			// 메시지가 완성되지 않았으므로 그냥 반환, 다음 프레임에 동작 
			return 0;
		}

		
		//---------------------------------------------------
		// 메시지 완성됨 -> 꺼내서 처리한다. 
		//---------------------------------------------------
		pSession->RecvQ->MoveFront(sizeof(st_PACKET_HEADER));
		ret_Dq_Size = pSession->RecvQ->Dequeue(g_Packet.GetBufferPtr(), st_Header.bySize);
		
		if (!PacketProc(pSession, st_Header.byType, &g_Packet))
		{
			enqueueForDeletion(pSession->dwSessionID);
			g_Packet.Clear();
			return -1;
		}

		g_Packet.Clear();
		return 0;
	}
}

void netProc_Send(st_SESSION* pSession)
{
	// Send의 경우 한번에 링버퍼에 있는 모든 메시지를 전송하면 된다.

	int Send_Size;
	int i_Error;

	while (1)
	{
		if (pSession->SendQ->GetUseSize() == 0)
			break;

		Send_Size = send(pSession->Socket, pSession->SendQ->GetFrontBufferPtr(), (int)(pSession->SendQ->DirectDequeueSize()), 0);

		if (Send_Size == SOCKET_ERROR)
		{
			i_Error = WSAGetLastError();

			if (i_Error == WSAEWOULDBLOCK)
			{
				// Send 할 때 WSAEWOULDBLOCK이 나온다면, 다음 프레임에 데이터를 보낸다. 
				continue;
			}
			else if (i_Error == 10054)
			{
				_LOG(1, L"Send failed with error: %ld / SessionID:%d \n", i_Error, pSession->dwSessionID);
				enqueueForDeletion(pSession->dwSessionID);
				break;
			}
			else if (i_Error == 10053)
			{
				_LOG(1, L"Send failed with error: %ld / SessionID:%d \n", i_Error, pSession->dwSessionID);
				enqueueForDeletion(pSession->dwSessionID);
				break;
			}
			else
			{
				_LOG(0, L"Send failed with error: %ld / SessionID:%d \n", i_Error, pSession->dwSessionID);
				enqueueForDeletion(pSession->dwSessionID);
				__debugbreak();
			}
		}

		pSession->SendQ->MoveFront(Send_Size);
	}
}

//----------------------------------------------------------------
// 특정 1명의 클라이언트 에게 메시지 보내기
void SendPacket_Unicast(st_SESSION* pSession, SerializeBuffer* pPacket)
{
	size_t ret_EnQ_Size;

	ret_EnQ_Size = pSession->SendQ->Enqueue(pPacket->GetBufferPtr(), pPacket->GetDataSize());

	if (ret_EnQ_Size == 0)
	{
		// SendQ 가득참 => SendBuff 가득참 => 클라이언트 RecvBuff 가득참 == 연결을 끊는다.

		_LOG(1, L"Disconnect Packet Unicast failed with error # SessionID: %d \n", pSession->dwSessionID);
		enqueueForDeletion(pSession->dwSessionID);
		return;
	}
}


bool PacketProc(st_SESSION* pSession, unsigned char byPacketType, SerializeBuffer* pPacket)
{
	switch (byPacketType)
	{
	case dfPACKET_CS_MOVE_START:
		return netPacketProc_Movestart(pSession, pPacket);
		break;
	case dfPACKET_CS_MOVE_STOP:
		return netPacketProc_MoveStop(pSession, pPacket);
		break;
	case dfPACKET_CS_ATTACK1:
		return netPacketProc_Attack1(pSession, pPacket);
		break;
	case dfPACKET_CS_ATTACK2:
		return netPacketProc_Attack2(pSession, pPacket);
		break;
	case dfPACKET_CS_ATTACK3:
		return netPacketProc_Attack3(pSession, pPacket);
		break;
	}

	return true;
}

bool netPacketProc_Movestart(st_SESSION* pSession, SerializeBuffer* pPacket)
{
	char byDirection;
	short shX;
	short shY;
	st_PLAYER* pPlayer;
	st_SECTOR_AROUND st_Sector_Around;

	*pPacket >> byDirection;
	*pPacket >> shX;
	*pPacket >> shY;
	pPacket->Clear();

	_LOG(dfLOG_LEVEL_DEBUG, L"# MOVESTART # SessionID:%d / Direction:%d / X:%d / Y:%d", pSession->dwSessionID, byDirection, shX, shY);

	//---------------------------------------------------------------------------------------------------------------
	// ID로 캐릭터를 검색한다. 
	//---------------------------------------------------------------------------------------------------------------
	pPlayer = FindCharacter(pSession->dwSessionID);
	if (pPlayer == NULL)
	{
		_LOG(dfLOG_LEVEL_ERROR, L"# MOVESTART > SessionID:%d Player Not Found!", pSession->dwSessionID);
		return false;
	}

	//---------------------------------------------------------------------------------------------------------------
	// 서버의 위치와 받은 패킷의 위치값이 너무 큰 차이가 난다면 싱크 패킷을 보내어 좌표 보정.
	// 
	// 본 게임의 좌표 동기화 구조가 단순한 키보드 조작 (클라이언트의 선처리, 서버의 후 반영) 방식으로 
	// 클라이언트의 좌표를 그대로 믿는 방식을 택하고 있음. 
	// 실제 온라인 게임이라면 클라이언트에서 목적지를 공유하는 방식을 택해야 함
	// 지금은 좌표에 대해서는 간단한 구현을 목적으로 하고 있으므로 
	// 서버는 클라이언트의 좌표를 그대로 믿지만, 서버와 너무 큰 차이가 있다면 강제 좌표 동기화 하도록 함
	//---------------------------------------------------------------------------------------------------------------
	if (abs(pPlayer->_X - shX) > dfERROR_RANGE || abs(pPlayer->_Y - shY) > dfERROR_RANGE)
	{
		mpSync(pPacket, pPlayer->_SessionID, pPlayer->_X, pPlayer->_Y);
		C_Field::GetInstance()->GetSectorAround(pPlayer->_CurSector->iX, pPlayer->_CurSector->iY, &st_Sector_Around);
		C_Field::GetInstance()->SendPacket_Around(pSession, pPacket, &st_Sector_Around, true);

		shX = pPlayer->_X;
		shY = pPlayer->_Y;

		pPacket->Clear();

		//---------------------------------------------------------------------------------------------------------------
		// 좌표가 틀어진 이유를 찾아야 한다.
		//---------------------------------------------------------------------------------------------------------------

	}

	//---------------------------------------------------------------------------------------------------------------
	// 동작을 변경. 동작번호와, 방향값이 같다.
	//---------------------------------------------------------------------------------------------------------------
	pPlayer->_dwAction = byDirection;

	//---------------------------------------------------------------------------------------------------------------
	// 단순 방향표시용 byDirection (LL, RR)과 
	// 이동시 8방향 (LL, LU, UU, RU, RR, RD, DD, LD) 용 MoveDirecion 이 있음
	//---------------------------------------------------------------------------------------------------------------
	pPlayer->_byModeDirection = byDirection;

	//---------------------------------------------------------------------------------------------------------------
	// 방향을 변경
	//---------------------------------------------------------------------------------------------------------------
	switch (byDirection)
	{
	case dfPACKET_MOVE_DIR_LL:
	case dfPACKET_MOVE_DIR_LU:
	case dfPACKET_MOVE_DIR_LD:
		pPlayer->_byDirection = dfPACKET_MOVE_DIR_LL;
	case dfPACKET_MOVE_DIR_RU:
	case dfPACKET_MOVE_DIR_RR:
	case dfPACKET_MOVE_DIR_RD:
		pPlayer->_byDirection = dfPACKET_MOVE_DIR_RR;
		break;
	default:
		break;
	}
	pPlayer->_X = shX;
	pPlayer->_Y = shY;

	//---------------------------------------------------------------------------------------------------------------
	// 섹터 처리
	// 정지를 하면서 좌표가 약간 조절된 경우 섹터 업데이트를 함
	// 위에서 좌표를 변경하였으면 섹터도 다시 재설정 해야한다. 
	//---------------------------------------------------------------------------------------------------------------
	if (C_Field::GetInstance()->Sector_UpdateCharacter(pPlayer))
	{
		//---------------------------------------------------------------------------------------------------------------
		// 섹터가 변경된 경우는 클라에게 관련 정보를 쏜다. 
		//---------------------------------------------------------------------------------------------------------------
		C_Field::GetInstance()->CharacterSectorUpdatePacket(pPlayer);

	}

	mpMoveStart(pPacket, pSession->dwSessionID, byDirection, pPlayer->_X, pPlayer->_Y);

	//---------------------------------------------------------------------------------------------------------------
	// 현재 접속주인 사용자에게 모든 패킷을 뿌린다. (섹터 단위 패킷 전송 함수)
	//---------------------------------------------------------------------------------------------------------------
	C_Field::GetInstance()->GetSectorAround(pPlayer->_CurSector->iX, pPlayer->_CurSector->iY, &st_Sector_Around);
	C_Field::GetInstance()->SendPacket_Around(pSession, pPacket, &st_Sector_Around);
	pPacket->Clear();

	return true;
}

bool netPacketProc_MoveStop(st_SESSION* pSession, SerializeBuffer* pPacket)
{
	return false;

}
bool netPacketProc_Attack1(st_SESSION* pSession, SerializeBuffer* pPacket)
{
	return false;
}
bool netPacketProc_Attack2(st_SESSION* pSession, SerializeBuffer* pPacket)
{
	return false;

}
bool netPacketProc_Attack3(st_SESSION* pSession, SerializeBuffer* pPacket)
{
	return false;

}

void mpSync(SerializeBuffer* pPacket, DWORD dwSessionID, short shX, short shY)
{
	st_PACKET_HEADER New_Header;

	New_Header.byCode = (char)dfPACKET_CODE;
	New_Header.bySize = 8;
	New_Header.byType = (char)dfPACKET_SC_SYNC;
	(*pPacket).PutData((char*)&New_Header, sizeof(New_Header));
	(*pPacket) << dwSessionID << shX << shY;
}
void mpMoveStart(SerializeBuffer* pPacket, DWORD dwSessionID, char byDirection, short shX, short shY)
{
	st_PACKET_HEADER New_Header;

	New_Header.byCode = (char)dfPACKET_CODE;
	New_Header.bySize = 9;
	New_Header.byType = (char)dfPACKET_SC_MOVE_START;

	// 사용 후 초기화가 패턴이다.
	//pPacket->Clear();

	(*pPacket).PutData((char*)&New_Header, sizeof(New_Header));
	(*pPacket) << dwSessionID << shX << shY;
}
void mpCreateMyCharacter(SerializeBuffer* pPacket, DWORD dwSessionID, char byDirection, short shX, short shY, char HP)
{
	st_PACKET_HEADER New_Header;

	New_Header.byCode = (char)dfPACKET_CODE;
	New_Header.bySize = 10;
	New_Header.byType = (char)dfPACKET_SC_CREATE_MY_CHARACTER;

	(*pPacket).PutData((char*)&New_Header, sizeof(New_Header));

	(*pPacket) << dwSessionID << byDirection << shX << shY << HP;
}
void mpCreateOtherCharacter(SerializeBuffer* pPacket, DWORD dwSessionID, char byDirection, short shX, short shY, char HP)
{
	st_PACKET_HEADER New_Header;

	New_Header.byCode = (char)dfPACKET_CODE;
	New_Header.bySize = 10;
	New_Header.byType = (char)dfPACKET_SC_CREATE_OTHER_CHARACTER;

	(*pPacket).PutData((char*)&New_Header, sizeof(New_Header));

	(*pPacket) << dwSessionID << byDirection << shX << shY << HP;
}














//C_Session C_Session::_C_Session;

st_SESSION::st_SESSION(SOCKET New_Socket, DWORD dw_New_SessionID)
{
	Socket = New_Socket;
	dwSessionID = dw_New_SessionID;

	RecvQ = new C_RING_BUFFER();
	SendQ = new C_RING_BUFFER();

	dwLastRecvTime = g_Start_Time;

	Disconnect = false;
}

C_Session* C_Session::GetInstance(void)
{
    //return &_C_Session;
	return NULL;
}

void ForwardDecl(int DestID, SerializeBuffer* sb)
{
	st_SESSION* pSession = C_Session::GetInstance()->_Session_Hash.find(DestID)->second;

	C_Session::GetInstance()->SendPacket_Unicast(pSession, sb);
}





void C_Session::netIOProcess(void)
{
	std::unordered_map<DWORD, st_SESSION*>::iterator	iter;
	std::unordered_map<DWORD, st_SESSION*>::iterator	iter_FD_ISSET;
	st_SESSION* st_pSession;
	SOCKET Listen_Socket;
	FD_SET ReadSet;	
	FD_SET WriteSet;
	timeval Time;

	int iCnt;
	int i_Result;

	Listen_Socket = _Listen_Socket;
	iter = _Session_Hash.begin();
	//--------------------------------------------------------------------------------------------------------------------
	// _Session 전부를 Select에 등록하면 while문을 종료한다. 
	// 
	//--------------------------------------------------------------------------------------------------------------------
	while (iter != _Session_Hash.end())
	{
		FD_ZERO(&ReadSet);
		FD_ZERO(&WriteSet);

		//------------------------------------------
		// Listen_Socket 넣기
		//------------------------------------------
		FD_SET(Listen_Socket, &ReadSet);

		//------------------------------------------
		// 리슨소켓 및 접속중인 모든 클라이언트에 대해 SOCKET 을 체크한다. 
		// 
		// 마지막 _Session이거나 or _Listen_Socket 포함 64개의 소켓을 Select에 등록했다면, 반복문을 종료한다. 
		//------------------------------------------
		for (iCnt = 0; iCnt < 64 - 1 && iter != _Session_Hash.end(); ++iter, ++iCnt)
		{
			st_pSession = iter->second;

			if (st_pSession->Disconnect) continue;
			
			//------------------------------------------
			// 해당 클라이언트 Read Set 등록 / SendQ 에 데이터가 있다면 Write Set 등록
			//------------------------------------------
			FD_SET(st_pSession->Socket, &ReadSet);
			if (st_pSession->SendQ->GetUseSize() > 0)
				FD_SET(st_pSession->Socket, &WriteSet);
		}

		//------------------------------------------
		// NULL: 한개라도 응답 올 때까지 무한히 기다림
		// 0,0: 응답올때까지 기다리지 않고 바로 끝낸다.
		// 양수: 한개라도 응답하거나, 지정된 시간이 지나면 리턴
		//------------------------------------------
		Time.tv_sec = 0;
		Time.tv_usec = 0;

		i_Result = select(0, &ReadSet, &WriteSet, 0, &Time);

		if (i_Result == SOCKET_ERROR)
		{
			_LOG(0, L"select failed with error: %ld \n", WSAGetLastError());
			__debugbreak();
		}


		//------------------------------------------
		// 리턴값이 0 이상이라면 누군가의 데이터가 왔다.
		//------------------------------------------
		while (i_Result > 0)
		{
			if (FD_ISSET(Listen_Socket, &ReadSet))
			{
				netProc_Accept();
				--i_Result;
			}

			//------------------------------------------
			// 전체 세션중 어떤 세션이 반응을 보였는지 다시 확인한다. 
			//------------------------------------------
			for (iter_FD_ISSET = _Session_Hash.begin(); iter_FD_ISSET != _Session_Hash.end(); ++iter_FD_ISSET)
			{
				st_pSession = (*iter_FD_ISSET).second;

				if (FD_ISSET(st_pSession->Socket, &ReadSet))
				{
					netProc_Recv(st_pSession);
					--i_Result;
				}

				if (FD_ISSET(st_pSession->Socket, &WriteSet))
				{
					netProc_Send(st_pSession);
					--i_Result;
				}
			}
		}
	}

	
}

void C_Session::netProc_Accept(void)
{
	//---------------------------------------------------------------------------------
	// 새로운 연결에 대하여 accept를 하고,
	// SessionID를 할당하고, 
	// 내 눈에 보이는 시야에 한정하여 캐릭터 생성을 보낸다. 
	//---------------------------------------------------------------------------------

	int Error;

	SOCKET New_Client_Socket;
	sockaddr_in Clinet_Addr;
	
	st_PACKET_HEADER st_New_Header_MY_CHAPACTER;

	st_PACKET_HEADER st_New_Header_OTHER_CHAPACTER;
	SerializeBuffer New_Packet_OTHER_CHAPACTER;
	st_SECTOR_AROUND st_Sector_Around;

	st_SESSION* st_p_New_Session;
	st_PLAYER* st_p_New_Player;
	int Client_Addr_Len;

	Client_Addr_Len = sizeof(Clinet_Addr);
	New_Client_Socket = accept(_Listen_Socket, (sockaddr*)&Clinet_Addr, &Client_Addr_Len);
	if (New_Client_Socket == INVALID_SOCKET)
	{
		Error = WSAGetLastError();

		if (Error == WSAEWOULDBLOCK)
		{
			// Seletc로 거르고 들어왔는데 WSAEWOULDBLOCK이 나오는지 모르겠다. 
			_LOG(1, L"accept failed with error: %ld \n", Error);
			//__debugbreak();
			// 원래는 에러로 처리하면 안된다. 
			return;
		}
		else
		{
			_LOG(0, L"accept failed with error: %ld \n", Error);
			WSACleanup();
			__debugbreak();
		}
	}

	
	// 접속자 인원수 제한. 순간적인 피크를 포함하여 약 8000명으로 가정한다. 
	if (_Session_Hash.size() > 8000)
	{
		_LOG(2, L"Full Server cannot accept!!! \n");
		closesocket(New_Client_Socket);
		return;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// 1. 새로운 세션 만들기.
	// 2. 새로운 플레이어 만들기. 
	// 3. 나에게 생성 메시지 보내기.
	// 4. 주변에 나에대한 생성 메시지를 보낸다. 
	// 5. 행동중인 플레이어가 있다면 동작을 이어서 보여준다. 
	// 
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// 1.
	st_p_New_Session = new st_SESSION(New_Client_Socket, ++_SessionID);
	_Session_Hash.insert({ _SessionID, st_p_New_Session });
	// 2.
	st_p_New_Player = CreateNewPlayer(_SessionID, st_p_New_Session);
	// 3.
	st_New_Header_MY_CHAPACTER.byCode = (char)dfPACKET_CODE;
	st_New_Header_MY_CHAPACTER.bySize = 10;
	st_New_Header_MY_CHAPACTER.byType = dfPACKET_SC_CREATE_MY_CHARACTER;
	g_Packet.PutData((char*)&st_New_Header_MY_CHAPACTER, sizeof(st_PACKET_HEADER));
	g_Packet << _SessionID << st_p_New_Player->_byDirection << st_p_New_Player->_X << st_p_New_Player->_Y << st_p_New_Player->_HP;
	SendPacket_Unicast(st_p_New_Session, &g_Packet);
	g_Packet.Clear();
	// 4.
	st_New_Header_OTHER_CHAPACTER.byCode = (char)dfPACKET_CODE;
	st_New_Header_OTHER_CHAPACTER.bySize = 10;
	st_New_Header_OTHER_CHAPACTER.byType = dfPACKET_SC_CREATE_OTHER_CHARACTER;
	g_Packet.PutData((char*)&st_New_Header_OTHER_CHAPACTER, sizeof(st_PACKET_HEADER));
	g_Packet << _SessionID << st_p_New_Player->_byDirection << st_p_New_Player->_X << st_p_New_Player->_Y << st_p_New_Player->_HP;
	C_Field::GetInstance()->GetSectorAround(st_p_New_Player->_CurSector->iX, st_p_New_Player->_CurSector->iY, &st_Sector_Around);
	SendPacket_Around(st_p_New_Session, &New_Packet_OTHER_CHAPACTER, &st_Sector_Around);
	// 5. 


	
}

void C_Session::netProc_Send(st_SESSION* pSession)
{
	// Send의 경우 한번에 링버퍼에 있는 모든 메시지를 전송하면 된다. 
	// field를 기준으로 전송할 대상을 판단해야 한다. 
	// 걍 Send 호출

	int Send_Size;
	int err;

	while (1)
	{
		if (pSession->SendQ->GetUseSize() == 0)
			break;
		
		Send_Size = send(pSession->Socket, pSession->SendQ->GetFrontBufferPtr(), (int)pSession->SendQ->DirectDequeueSize(), 0);

		if (Send_Size == SOCKET_ERROR)
		{
			err = WSAGetLastError();

			if (err == WSAEWOULDBLOCK)
			{
				// 비동기 Send에서 WSAEWOULDBLOCK 가 나오는 경우: 소켓 송신 버퍼가 가득차서 즉시 보낼 수 없을 때
				// 그렇다면 그냥 다음 프레임에 전송하면 된다. 

				break;
			}
			else if (err == 10054)
			{
				// 현재 연결은 원격 호스트에 의해 강제로 끊겼다.

				// PushDisconnectList(pSession);
				break;
			}
			else if (err == 10053)
			{
				// 소프트웨어로 인해 연결이 중단되었다. 


				// PushDisconnectList(pSession);
				break;
			}

			// 에러로그 찍기
			__debugbreak();
		}

		pSession->SendQ->MoveFront(Send_Size);
	}
}

void C_Session::netProc_Recv(st_SESSION* pSession)
{
	
}

void C_Session::SendPacket_SectorOne(int iSectorX, int iSectorY, SerializeBuffer* pPacket, st_SESSION* pExceptSession)
{
	//std::list<st_PLAYER*> *pTemp_Player_List;
	//std::list<st_PLAYER*>::iterator iter;

	//if(iSectorX< dfRANGE_MOVE_LEFT || dfRANGE_MO)

	//pTemp_Player_List = C_Field::GetInstance()->_Sector[iSectorY][iSectorX];

	//if (pExceptSession == NULL)
	//{
	//	for (iter = pTemp_Player_List->begin(); iter != pTemp_Player_List->end(); ++iter)
	//	{
	//		SendPacket_Unicast((*iter)->_pSession, pPacket);
	//	}
	//}
	//else
	//{
	//	for (iter = pTemp_Player_List->begin(); iter != pTemp_Player_List->end(); ++iter)
	//	{
	//		if (pExceptSession == (*iter)->_pSession)
	//			continue;
	//		SendPacket_Unicast((*iter)->_pSession, pPacket);
	//	}
	//}
}

void C_Session::SendPacket_Unicast(st_SESSION* pSession, SerializeBuffer* pPacket)
{
	// 세션에서 SendQ가 가득 차있다면 연결을 끊는다.

	size_t Ret_Packet;

	Ret_Packet = pSession->SendQ->Enqueue(pPacket->GetBufferPtr(), pPacket->GetDataSize());

	if (Ret_Packet == 0)
	{
		// 연결을 끊는다. 
		return;
	}
}

void C_Session::SendPacket_Around(st_SESSION* pSession, SerializeBuffer* pPacket, st_SECTOR_AROUND* pSector_Around, bool bSendMe)
{
	int iCnt;

	if (bSendMe == false)
	{
		for (iCnt = 0; iCnt < pSector_Around->iCount; ++iCnt)
		{
			SendPacket_SectorOne(pSector_Around->Around[iCnt].iX, pSector_Around->Around[iCnt].iY, pPacket, pSession);
		}
	}
	else
	{
		for (iCnt = 0; iCnt < pSector_Around->iCount; ++iCnt)
		{
			SendPacket_SectorOne(pSector_Around->Around[iCnt].iX, pSector_Around->Around[iCnt].iY, pPacket, NULL);
		}
	}
}

void C_Session::SendPacket_Broadcast(st_SESSION* pSession, SerializeBuffer* pPacket)
{
	return;
}

bool C_Session::PacketProc(st_SESSION* pSession, unsigned char byPacketType, SerializeBuffer* pPacket)
{
	switch (byPacketType)
	{
	case dfPACKET_CS_MOVE_START:
		break;
	case dfPACKET_CS_MOVE_STOP:
		break;
	case dfPACKET_CS_ATTACK1:
		break;
	case dfPACKET_CS_ATTACK2:
		break;
	case dfPACKET_CS_ATTACK3:
		break;
	}

	return true;
}

bool C_Session::netPacketProc_MoveStart(st_SESSION* pSession, SerializeBuffer* pPacket)
{
	char byDirection;
	short X;
	short Y;
	st_PLAYER* pPlayer;
	st_PACKET_HEADER New_Header;
	st_SECTOR_AROUND st_Sector_Around;

	*pPacket >> byDirection;
	*pPacket >> X;
	*pPacket >> Y;
	pPacket->Clear();

	_LOG(dfLOG_LEVEL_DEBUG, L"# MOVESTART # SessionID:%d / Direction:%d / X:%d / Y:%d", pSession->dwSessionID, byDirection, X, Y);

	//---------------------------------------------------------------------------------------------------------------
	// ID로 캐릭터를 검색한다. 
	//---------------------------------------------------------------------------------------------------------------
	pPlayer = g_CharacterHash.find(pSession->dwSessionID)->second;
	if (pPlayer == NULL)
	{
		_LOG(dfLOG_LEVEL_ERROR, L"# MOVESTART > SessionID:%d Player Not Found!", pSession->dwSessionID);
		return false;
	}

	//---------------------------------------------------------------------------------------------------------------
	// 서버의 위치와 받은 패킷의 위치값이 너무 큰 차이가 난다면 싱크 패킷을 보내어 좌표 보정.
	// 
	// 본 게임의 좌표 동기화 구조가 단순한 키보드 조작 (클라이언트의 선처리, 서버의 후 반영) 방식으로 
	// 클라이언트의 좌표를 그대로 믿는 방식을 택하고 있음. 
	// 실제 온라인 게임이라면 클라이언트에서 목적지를 공유하는 방식을 택해야 함
	// 지금은 좌표에 대해서는 간단한 구현을 목적으로 하고 있으므로 
	// 서버는 클라이언트의 좌표를 그대로 믿지만, 서버와 너무 큰 차이가 있다면 강제 좌표 동기화 하도록 함
	//---------------------------------------------------------------------------------------------------------------
	if (abs(pPlayer->_X - X) > dfERROR_RANGE || abs(pPlayer->_Y - Y) > dfERROR_RANGE)
	{
		New_Header.byCode = (char)dfPACKET_CODE;
		New_Header.bySize = 8;
		New_Header.byType = (char)dfPACKET_SC_SYNC;
		(*pPacket).PutData((char*)&New_Header, sizeof(New_Header));
		(*pPacket) << pPlayer->_SessionID << pPlayer->_X << pPlayer->_Y;
		C_Field::GetInstance()->GetSectorAround(pPlayer->_CurSector->iX, pPlayer->_CurSector->iY, &st_Sector_Around);
		SendPacket_Around(pSession, pPacket, &st_Sector_Around, true);
		pPacket->Clear();
	}

	//---------------------------------------------------------------------------------------------------------------
	// 동작을 변경. 동작번호와, 방향값이 같다.
	//---------------------------------------------------------------------------------------------------------------
	pPlayer->_dwAction = byDirection;

	//---------------------------------------------------------------------------------------------------------------
	// 방향을 변경
	//---------------------------------------------------------------------------------------------------------------
	switch (byDirection)
	{
	case dfPACKET_MOVE_DIR_LL:
	case dfPACKET_MOVE_DIR_LU:
	case dfPACKET_MOVE_DIR_LD:
		pPlayer->_byDirection = dfPACKET_MOVE_DIR_LL;
	case dfPACKET_MOVE_DIR_RU:
	case dfPACKET_MOVE_DIR_RR:
	case dfPACKET_MOVE_DIR_RD:
		pPlayer->_byDirection = dfPACKET_MOVE_DIR_RR;
		break;
	default:
		break;
	}
	pPlayer->_X = X;
	pPlayer->_Y = Y;

	//---------------------------------------------------------------------------------------------------------------
	// 섹터 처리
	// 정지를 하면서 좌표가 약간 조절된 경우 섹터 업데이트를 함
	// 위에서 좌표를 변경하였으면 섹터도 다시 재설정 해야한다. 
	//---------------------------------------------------------------------------------------------------------------
	if(C_Field::GetInstance()->Sector_UpdateCharacter(pPlayer))
	{
		//---------------------------------------------------------------------------------------------------------------
		// 섹터가 변경된 경우는 클라에게 관련 정보를 쏜다. 
		//---------------------------------------------------------------------------------------------------------------
		C_Field::GetInstance()->CharacterSectorUpdatePacket(pPlayer);
		
	}
	New_Header.byCode = (char)dfPACKET_CODE;
	New_Header.bySize = 9;
	New_Header.byType = (char)dfPACKET_SC_MOVE_START;
	(*pPacket).PutData((char*)&New_Header, sizeof(New_Header));
	(*pPacket) << pPlayer->_SessionID << byDirection << pPlayer->_X << pPlayer->_Y;
	C_Field::GetInstance()->GetSectorAround(pPlayer->_CurSector->iX, pPlayer->_CurSector->iY, &st_Sector_Around);
	SendPacket_Around(pSession, pPacket, &st_Sector_Around);
	pPacket->Clear();

	return true;
}

bool C_Session::netPacketProc_MoveStop(st_SESSION* pSession, SerializeBuffer* pPacket)
{
	return false;
}

bool C_Session::netPacketProc_Attack1(st_SESSION* pSession, SerializeBuffer* pPacket)
{
	return false;
}

bool C_Session::netPacketProc_Attack2(st_SESSION* pSession, SerializeBuffer* pPacket)
{
	return false;
}

bool C_Session::netPacketProc_Attack3(st_SESSION* pSession, SerializeBuffer* pPacket)
{
	return false;
}

C_Session::C_Session(void) : _SessionID(0)
{
	struct addrinfo hints;
	struct addrinfo* result = NULL;
	SOCKET Temp_Listen_Socket = _Listen_Socket;
	char Port[6];
	u_long on;
	linger Linger_Opt;


	//---------------------------------------------------
	// return 값 저장 변수 
	int Ret_WSAStartup;
	int Ret_getaddrinfo;
	int Ret_bind;
	int Ret_listen;
	int Ret_ioctlsocket;
	int Ret_setsockopt;
	errno_t Ret_itoa_s;


	Temp_Listen_Socket = _Listen_Socket;


	//---------------------------------------------------
	// Initialize Winsock
	Ret_WSAStartup = WSAStartup(MAKEWORD(2, 2), &_WsaData);
	if (Ret_WSAStartup != 0)
	{
		_LOG(0, L"WSAStartup failed with error: %d", Ret_WSAStartup);
		__debugbreak();
	}
	_LOG(0, L"WSAStartup # \n");

	//---------------------------------------------------
	// SetUp hints 
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	//---------------------------------------------------
	// Resolve the server address and port
	// 서버 주소 및 포트 확인
	// 아래의 Port 코드는 C6054 : 'Port' 문자열이 0으로 종료되지 않을 수 있습니다. 경고를 해결하기 위하여 작성하였다. 
	Ret_itoa_s = _itoa_s(dfNETWORK_PORT, Port, sizeof(Port), 10);
	if (Ret_itoa_s != NULL)
	{
		_LOG(0, L"_itoa_s failed with error");
		WSACleanup();
		__debugbreak();
	}
	Port[sizeof(Port) - 1] = '\0';
	Ret_getaddrinfo = getaddrinfo(NULL, Port, &hints, &result);
	if (Ret_getaddrinfo != 0)
	{
		_LOG(0, L"getaddrinfo failed with error: %d", Ret_getaddrinfo);
		WSACleanup();
		__debugbreak();
	}

	//---------------------------------------------------
	// Create a SOCKET for the server to listen for client connections.
	// 서버가 클라이언트 연결을 수신을 SOCKET을 만든다.
	Temp_Listen_Socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (Temp_Listen_Socket == INVALID_SOCKET)
	{
		_LOG(0, L"socket failed with error : %ld", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		__debugbreak();
	}

	//---------------------------------------------------
	// setsockopt_Linger
	Linger_Opt.l_onoff = 1;
	Linger_Opt.l_linger = 0;
	Ret_setsockopt = setsockopt(Temp_Listen_Socket, SOL_SOCKET, SO_LINGER, (char*)&Linger_Opt, sizeof(Linger_Opt));
	if (Ret_setsockopt == SOCKET_ERROR)
	{
		_LOG(0, L"setsockopt failed with error: %ld", WSAGetLastError());
		closesocket(Temp_Listen_Socket);
		WSACleanup();
		__debugbreak();
	}

	//---------------------------------------------------
	// Setup the TCP listening socket
	// TCP listening socket 을 준비한다.
	Ret_bind = bind(Temp_Listen_Socket, result->ai_addr, (int)result->ai_addrlen);
	if (Ret_bind == SOCKET_ERROR)
	{
		_LOG(0, L"bind failed with error: %d", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(Temp_Listen_Socket);
		WSACleanup();
		__debugbreak();
	}
	_LOG(0, L"Bind OK # Port: %d", dfNETWORK_PORT);

	freeaddrinfo(result);

	//---------------------------------------------------
	// Listen a client socket
	Ret_listen = listen(Temp_Listen_Socket, SOMAXCONN);
	//i_Result = listen(Temp_Listen_Socket, SOMAXCONN_HINT(65535));	// 양수 ~200 -> 기본 200개, 200~ 갯수 적용,    
	if (Ret_listen == SOCKET_ERROR)
	{
		_LOG(0, L"listen failed with error: %d", WSAGetLastError());
		closesocket(Temp_Listen_Socket);
		WSACleanup();
		__debugbreak();
	}
	wprintf_s(L"Listen OK # \n");

	//---------------------------------------------------
	// 넌블로킹 소켓으로 전환
	on = 1;
	Ret_ioctlsocket = ioctlsocket(Temp_Listen_Socket, FIONBIO, &on);
	if (Ret_ioctlsocket == SOCKET_ERROR)
	{
		_LOG(0, L"ioctlsocket failed with error: %ld", WSAGetLastError());
		closesocket(Temp_Listen_Socket);
		WSACleanup();
		__debugbreak();
	}

	_Listen_Socket = Temp_Listen_Socket;
}

C_Session::~C_Session(void)
{
	closesocket(_Listen_Socket);
	_LOG(0, L"Close Listen_Socket # \n");

	WSACleanup();
	_LOG(0, L"WSACleanup # \n");

	// _Session_Map 순회를 돌면서 삭제해야 하는데,,,, 삭제 구조가 꼬이기 시작했다.
	// 아마도 할당한 쪽에서 데이터에 대한 삭제를 마무리 하는게 좋아 보인다. 
	// 여기서는 여기 클래스에서 생성된 것들만 정리하고 지워준다. 
	// Socket, RecvQ, SendQ
	std::unordered_map<DWORD, st_SESSION*>::iterator iter;
	st_SESSION* st_Temp_Session;
	for (iter = _Session_Hash.begin(); iter != _Session_Hash.end();)
	{
		st_Temp_Session = iter->second;
		
		closesocket(st_Temp_Session->Socket);
		delete st_Temp_Session->RecvQ;
		delete st_Temp_Session->SendQ;
		delete st_Temp_Session;

		iter = _Session_Hash.erase(iter);
	}
	_LOG(0, L"CleanUp Session_Hash # \n");
}


