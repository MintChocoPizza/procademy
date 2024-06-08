

#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")

#include "CList.h"
#include "c_Save_Log.h"
#include "C_Ring_Buffer.h"
#include "NetWork.h"
#include "PacketDefine.h"
#include "main.h"

const char* DEFAULT_PORT = "5000";


CList<st_SESSION*> g_Session_List;
CList<st_SESSION*> g_Disconnect_List;
DWORD g_Session_ID;


bool init_Listen_Socket(void* Listen_Socket, void* wsa_Data)
{
	struct addrinfo hints;
	struct addrinfo* result = NULL;
	SOCKET Temp_Listen_Socket = *(SOCKET*)Listen_Socket;
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

	//---------------------------------------------------
	// Initialize Winsock
	Ret_WSAStartup = WSAStartup(MAKEWORD(2, 2), (WSADATA*)wsa_Data);
	if (Ret_WSAStartup != 0)
	{
		c_Save_Log.printfLog(L"WSAStartup failed with error: %d \n", Ret_WSAStartup);
		return false;
	}

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
	Ret_getaddrinfo = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (Ret_getaddrinfo != 0)
	{
		c_Save_Log.printfLog(L"getaddrinfo failed with error: %d \n", Ret_getaddrinfo);
		WSACleanup();
		return false;
	}

	//---------------------------------------------------
	// Create a SOCKET for the server to listen for client connections.
	// 서버가 클라이언트 연결을 수신을 SOCKET을 만든다.
	Temp_Listen_Socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (Temp_Listen_Socket == INVALID_SOCKET)
	{
		c_Save_Log.printfLog(L"socket failed with error : %ld \n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return false;
	}

	//---------------------------------------------------
	// setsockopt_Linger
	Linger_Opt.l_onoff = 1;
	Linger_Opt.l_linger = 0;
	Ret_setsockopt = setsockopt(Temp_Listen_Socket, SOL_SOCKET, SO_LINGER, (char*)&Linger_Opt, sizeof(Linger_Opt));
	if (Ret_setsockopt == SOCKET_ERROR)
	{
		c_Save_Log.printfLog(L"setsockopt failed with error: %ld \n", WSAGetLastError());
		closesocket(Temp_Listen_Socket);
		WSACleanup();
		return false;
	}



	//---------------------------------------------------
	// Setup the TCP listening socket
	// TCP listening socket 을 준비한다.
	Ret_bind = bind(Temp_Listen_Socket, result->ai_addr, (int)result->ai_addrlen);
	if (Ret_bind == SOCKET_ERROR)
	{
		c_Save_Log.printfLog(L"bind failed with error: %d \n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(Temp_Listen_Socket);
		WSACleanup();
		return false;
	}

	freeaddrinfo(result);

	//---------------------------------------------------
	// Listen a client socket
	Ret_listen = listen(Temp_Listen_Socket, SOMAXCONN);
	//i_Result = listen(Temp_Listen_Socket, SOMAXCONN_HINT(65535));	// 양수 ~200 -> 기본 200개, 200~ 갯수 적용,    
	if (Ret_listen == SOCKET_ERROR)
	{
		c_Save_Log.printfLog(L"listen failed with error: %d \n", WSAGetLastError());
		closesocket(Temp_Listen_Socket);
		WSACleanup();
		return false;
	}

	//---------------------------------------------------
	// 넌블로킹 소켓으로 전환
	on = 1;
	Ret_ioctlsocket = ioctlsocket(Temp_Listen_Socket, FIONBIO, &on);
	if (Ret_ioctlsocket == SOCKET_ERROR)
	{
		c_Save_Log.printfLog(L"ioctlsocket failed with error: %ld \n", WSAGetLastError());
		closesocket(Temp_Listen_Socket);
		WSACleanup();
		return false;
	}

	*(SOCKET*)Listen_Socket = Temp_Listen_Socket;

	return true;
}

void netIOProcess(void)
{
	int i_Result;

	CList<st_SESSION*>::iterator iter;

	st_SESSION* pSession;

	FD_SET ReadSet;
	FD_SET WriteSet;

	timeval Time;


	FD_ZERO(&ReadSet);
	FD_ZERO(&WriteSet);

	//------------------------------------------
	// 리슨소켓 넣기
	//------------------------------------------
	FD_SET(g_Listen_Socket, &ReadSet);

	//------------------------------------------
	// 리슨소켓 및 접속중인 모든 클라이언트에 대해 SOCKET 을 체크한다. 
	//------------------------------------------
	for (iter = g_Session_List.begin(); iter != g_Session_List.end(); ++iter)
	{
		pSession = *iter;
		//------------------------------------------
		// 해당 클라이언트 Read Set 등록 / SendQ 에 데이터가 있다면 Write Set 등록
		//------------------------------------------
		FD_SET(pSession->Socket, &ReadSet);
		if (pSession->SendQ.GetUseSize() > 0)
			FD_SET(pSession->Socket, &WriteSet);
	}

	Time.tv_sec = 0;
	Time.tv_usec = 0;

	i_Result = select(0, &ReadSet, &WriteSet, 0, &Time);
	if (i_Result == SOCKET_ERROR)
	{
		c_Save_Log.printfLog(L"select failed with error: %ld \n", WSAGetLastError());
		__debugbreak();
	}
	//------------------------------------------
	// 리턴값이 0 이상이라면 누군가의 데이터가 왔다.
	//------------------------------------------
	while (i_Result > 0)
	{
		if (FD_ISSET(g_Listen_Socket, &ReadSet))
		{
			netProc_Accept();
			--i_Result;
		}

		//------------------------------------------
		// 전체 세션중 어떤 세션이 반응을 보였는지 다시 확인한다. 
		//------------------------------------------
		for (iter = g_Session_List.begin(); iter != g_Session_List.end(); ++iter)
		{
			pSession = *iter;
			if (FD_ISSET(pSession->Socket, &ReadSet))
			{
				netProc_Recv(pSession);
				--i_Result;
			}

			if (FD_ISSET(pSession->Socket, &WriteSet))
			{
				netProc_Send(pSession);
				--i_Result;
			}
		}
	}
}

void netProc_Accept(void)
{
	SOCKET Client_Socket;

	st_SESSION* st_New_Player;
	sockaddr_in Clinet_Addr;
	int Client_Addr_Len = NULL;

	Client_Socket = accept(g_Listen_Socket, (sockaddr*)&Clinet_Addr, &Client_Addr_Len);
	if (Client_Socket == INVALID_SOCKET)
	{
		if (Client_Socket != WSAEWOULDBLOCK)
		{
			c_Save_Log.printfLog(L"ioctlsocket failed with error: %ld \n", WSAGetLastError());
			return;
		}
	}

	st_New_Player = new st_SESSION;
	init_Session(Client_Socket, st_New_Player);
	g_Session_List.push_back(st_New_Player);

	// 캐릭터 생성 메시지 생성
	


	


	// 1. 새로운 플레이어 생성 메시지 보내기.
	// 2. 나를 제외한 모든 플레이어에게 나의 생성 메시지 보내기. 
	// 3. 나에게 다른 플레이어들에 대한 생성 메시지 보내기.



}

void init_Session(SOCKET Client_Socket, st_SESSION* st_New_Player)
{
	st_New_Player->Socket = Client_Socket;
	st_New_Player->chHP = 100;
	st_New_Player->byDirection = dfPACKET_MOVE_DIR_LL;
	st_New_Player->dwSessionID = ++g_Session_ID;
	st_New_Player->dwAction = NULL;
	st_New_Player->shY = rand() % (dfRANGE_MOVE_BOTTOM - dfRANGE_MOVE_TOP + 1) + dfRANGE_MOVE_TOP;
	st_New_Player->shX = rand() % (dfRANGE_MOVE_RIGHT - dfRANGE_MOVE_LEFT + 1) + dfRANGE_MOVE_LEFT;
}

void netProc_Recv(st_SESSION* pSession)
{
}

void netProc_Send(st_SESSION* pSession)
{
}

void netSendUnicast(st_SESSION* pSession, st_PACKET_HEADER *header, char* packet, int Packet_Len)
{
	int Ret_Header;
	int Ret_Packet;

	Ret_Header = pSession->SendQ.Enqueue((char*)header, sizeof(st_PACKET_HEADER));
	if (Ret_Header == 0)
	{
		c_Save_Log.printfLog(L"Header Unicast failed with error: \n");
		g_Disconnect_List.push_back(pSession);
	}

	Ret_Packet = pSession->SendQ.Enqueue(packet, Packet_Len);
	if(Ret_Packet == 0)
	{
		c_Save_Log.printfLog(L"Packet Unicast failed with error: \n");
		g_Disconnect_List.push_back(pSession);
	}
}
void netSendBroadcast(st_SESSION* pSession, st_PACKET_HEADER* header, char* packet, int Packet_Len)
{
	CList<st_SESSION*>::iterator iter;
	st_SESSION* p_Temp_Session;

	for (iter = g_Session_List.begin(); iter != g_Session_List.end(); ++iter)
	{
		p_Temp_Session = *iter;

		if (p_Temp_Session == pSession) continue;

		netSendUnicast(pSession, header, packet, Packet_Len);
	}
}

void Disconnect()
{
	CList<st_SESSION*>::iterator iter;
	st_SESSION* p_Session;
	st_PACKET_SC_DELETE_CHARACTER packet;
	st_PACKET_HEADER header;

	for (iter = g_Disconnect_List.begin(); iter != g_Disconnect_List.end(); ++iter)
	{
		p_Session = *iter;
		netPacketProc_SC_DELETE_CHARACTER(p_Session, (char*)&header, (char*)&packet);
		netSendBroadcast(p_Session, &header, (char*)&packet, sizeof(st_PACKET_SC_DELETE_CHARACTER));
	}
	

	for (iter = g_Disconnect_List.begin(); iter != g_Disconnect_List.end(); ++iter)
	{
		
	}
}
