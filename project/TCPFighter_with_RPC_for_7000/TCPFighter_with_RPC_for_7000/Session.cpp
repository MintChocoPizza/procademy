
#include <map>
#include <stdio.h>


#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#pragma comment (lib, "Ws2_32.lib")
#include "C_Ring_Buffer.h"
#include "Session.h"


C_Session C_Session::_C_Session;

C_Session* C_Session::GetInstance(void)
{
    return &_C_Session;
}

void C_Session::netIOProcess(void)
{
	std::map<DWORD, st_SESSION*>::iterator iter;
	std::map<DWORD, st_SESSION*>::iterator iter_FD_ISSSET;
	st_SESSION* st_pSession;
	SOCKET Listen_Socket;
	FD_SET ReadSet;	
	FD_SET WriteSet;
	timeval Time;

	int iCnt;
	int i_Result;

	Listen_Socket = _Listen_Socket;
	iter = _Session.begin();
	//--------------------------------------------------------------------------------------------------------------------
	// _Session 전부를 Select에 등록하면 while문을 종료한다. 
	// 
	//--------------------------------------------------------------------------------------------------------------------
	while (iter != _Session.end())
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
		iCnt = 0;
		for (; iCnt < 64 - 1 && iter != _Session.end(); ++iter, ++iCnt)
		{
			st_pSession = iter->second;

			if (st_pSession->Disconnect) continue;
			
			//------------------------------------------
			// 해당 클라이언트 Read Set 등록 / SendQ 에 데이터가 있다면 Write Set 등록
			//------------------------------------------
			FD_SET(st_pSession->Socket, &ReadSet);
			if (st_pSession->SendQ.GetUseSize() > 0)
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
			wprintf_s(L"select failed with error: %ld \n", WSAGetLastError());
			__debugbreak();
		}


		//------------------------------------------
		// 리턴값이 0 이상이라면 누군가의 데이터가 왔다.
		//------------------------------------------
		while (i_Result > 0)
		{
			if (FD_ISSET(Listen_Socket, &ReadSet))
			{
				--i_Result;
			}

			//------------------------------------------
			// 전체 세션중 어떤 세션이 반응을 보였는지 다시 확인한다. 
			//------------------------------------------
			for (iter_FD_ISSSET = _Session.begin(); iter_FD_ISSSET != _Session.end(); ++iter_FD_ISSSET)
			{
				st_pSession = (*iter_FD_ISSSET).second;

				if (FD_ISSET(st_pSession->Socket, &ReadSet))
				{
					netProc_Recv(st_pSession->dwSessionID);
					--i_Result;
				}

				if (FD_ISSET(st_pSession->Socket, &WriteSet))
				{
					netProc_Send(st_pSession->dwSessionID);
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
	int Client_Addr_Len;

	Client_Addr_Len = sizeof(Clinet_Addr);
	New_Client_Socket = accept(_Listen_Socket, (sockaddr*)&Clinet_Addr, &Client_Addr_Len);
	if (New_Client_Socket == INVALID_SOCKET)
	{
		Error = WSAGetLastError();

		if (Error == WSAEWOULDBLOCK)
		{
			// Seletc로 거르고 들어왔는데 WSAEWOULDBLOCK이 나오는지 모르겠다. 
			wprintf_s(L"accept failed with error: %ld \n", Error);
			__debugbreak();
		}
		else
		{
			wprintf_s(L"accept failed with error: %ld \n", Error);
			WSACleanup();
			__debugbreak();
		}
	}

	
	// 접속자 인원수 제한. 순간적인 피크를 포함하여 약 8000명으로 가정한다. 
	if (_Session.size() > 8000)
	{
		wprintf_s(L"Full Server cannot accept!!! \n");
		closesocket(New_Client_Socket);
		return;
	}


	

}

void C_Session::netProc_Send(DWORD SessionID)
{
}

void C_Session::netProc_Recv(DWORD SessionID)
{
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
		wprintf_s(L"WSAStartup failed with error: %d \n", Ret_WSAStartup);
		__debugbreak();
	}
	printf_s("WSAStartup # \n");

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
		wprintf_s(L"_itoa_s failed with error \n");
		WSACleanup();
		__debugbreak();
	}
	Port[sizeof(Port) - 1] = '\0';
	Ret_getaddrinfo = getaddrinfo(NULL, Port, &hints, &result);
	if (Ret_getaddrinfo != 0)
	{
		wprintf_s(L"getaddrinfo failed with error: %d \n", Ret_getaddrinfo);
		WSACleanup();
		__debugbreak();
	}

	//---------------------------------------------------
	// Create a SOCKET for the server to listen for client connections.
	// 서버가 클라이언트 연결을 수신을 SOCKET을 만든다.
	Temp_Listen_Socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (Temp_Listen_Socket == INVALID_SOCKET)
	{
		wprintf_s(L"socket failed with error : %ld \n", WSAGetLastError());
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
		wprintf_s(L"setsockopt failed with error: %ld \n", WSAGetLastError());
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
		wprintf_s(L"bind failed with error: %d \n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(Temp_Listen_Socket);
		WSACleanup();
		__debugbreak();
	}
	wprintf_s(L"Bind OK # Port: %d \n", dfNETWORK_PORT);

	freeaddrinfo(result);

	//---------------------------------------------------
	// Listen a client socket
	Ret_listen = listen(Temp_Listen_Socket, SOMAXCONN);
	//i_Result = listen(Temp_Listen_Socket, SOMAXCONN_HINT(65535));	// 양수 ~200 -> 기본 200개, 200~ 갯수 적용,    
	if (Ret_listen == SOCKET_ERROR)
	{
		wprintf_s(L"listen failed with error: %d \n", WSAGetLastError());
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
		wprintf_s(L"ioctlsocket failed with error: %ld \n", WSAGetLastError());
		closesocket(Temp_Listen_Socket);
		WSACleanup();
		__debugbreak();
	}

	_Listen_Socket = Temp_Listen_Socket;
}

C_Session::~C_Session(void)
{
	closesocket(_Listen_Socket);
	WSACleanup();
}


