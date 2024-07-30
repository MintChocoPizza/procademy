
//--------------------------------------------------------------------------------------------------------------------------------------
// 네트워크
#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#pragma comment (lib,"Ws2_32.lib")

#include <process.h>
#include <stdio.h>

#include "LOG.h"
#include "C_Ring_Buffer.h"

constexpr int dfNETWORK_PORT = 12345;
constexpr int dfMAX_CPU = 14;

struct st_SESSION
{
    SOCKET  socket;
    DWORD   SessionID;
    C_RING_BUFFER* RecvQ;
    C_RING_BUFFER* SendQ;

    OVERLAPPED Recv_Overlapped;
    OVERLAPPED Send_Overlapped;
};

DWORD g_SessionID = 0;

unsigned __stdcall AcceptThread(void* pArg);
unsigned __stdcall WorkerThread(void* pArg);

int main()
{
    int iCnt;

    WSADATA wsa;
    SOCKET listen_sock;

    HANDLE hCp;

    SYSTEM_INFO si;

    int iCreateThreadNum;
    HANDLE hThread[dfMAX_CPU * 2];

    sockaddr_in Client_Addr;
    int Client_Addr_Len;
    SOCKET New_Client_Socket;

    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    WSABUF wsaBuf[2];
    DWORD flags;

    //---------------------------------------------------
    // return value
    int Ret_Accept_Error;
    int Ret_WSARecv;
    int Ret_WSARecv_Error;

    //---------------------------------------------------
    // listen socket init && 
    netStartUp(&listen_sock, &wsa);

    //---------------------------------------------------
    // Create IOCP
    hCp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (hCp == NULL) return 1;

    //---------------------------------------------------
    // CPU Count
    GetSystemInfo(&si);

    //---------------------------------------------------
    // Create Worker Thread (CPU * 2)
    iCreateThreadNum = (int)si.dwNumberOfProcessors * 2;
    for (iCnt = 0; iCnt < iCreateThreadNum; ++iCnt)
    {
        hThread[iCnt] = (HANDLE)_beginthreadex(NULL, 0, WorkerThread, NULL, NULL, NULL);
    }
    // Create Thread error check
    for (iCnt = 0; iCnt < iCreateThreadNum; ++iCnt)
    {
        if (hThread[iCnt] == NULL)
        {
            _LOG(0, L"thread - error creating child threads");
            return -1;
        }
        else
        {
            _LOG(0, L"thread %d : Create!!", iCnt);
        }
    }

    //---------------------------------------------------
    // main loop
    while (1)
    {
        //---------------------------------------------------
        // accept()
        Client_Addr_Len = sizeof(Client_Addr);
        New_Client_Socket = accept(listen_sock, (sockaddr*)&Client_Addr, &Client_Addr_Len);
        if (New_Client_Socket == INVALID_SOCKET)
        {
            Ret_Accept_Error = WSAGetLastError();
            _LOG(0, L"accept() - error: %d", Ret_Accept_Error);
            break;
        }

        //---------------------------------------------------
        // printf Client info
        if (getnameinfo((sockaddr*)&Client_Addr, sizeof(Client_Addr), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
        {
            _c_LOG(0, "host: %d - sercive: %d Aeccept", host, service);
        }
        else
        {
            inet_ntop(AF_INET, &Client_Addr.sin_addr, host, NI_MAXHOST);
            _c_LOG(0, "%s : %d Aeccept", host, ntohs(Client_Addr.sin_port));
        }
        
        //---------------------------------------------------
        // Session struct new
        st_SESSION* pSession = new st_SESSION;
        if (pSession == NULL)
        {
            _LOG(0, L"pSession new error");
            return -1;
        }
        ZeroMemory(&(pSession->Recv_Overlapped), sizeof(pSession->Recv_Overlapped));
        ZeroMemory(&(pSession->Send_Overlapped), sizeof(pSession->Send_Overlapped));
        pSession->socket = New_Client_Socket;
        pSession->SessionID++;

        //---------------------------------------------------
        // link Client_Socket to IOCP
        CreateIoCompletionPort((HANDLE)New_Client_Socket, hCp, (ULONG_PTR) & pSession, 0);

        //---------------------------------------------------
        // wsabuf Setting
        // 커널에 copy 하기 때문에 지역으로 설정하고? , 따로 보관하지 않았다. 
        wsaBuf[0].buf = pSession->RecvQ->GetInBufferPtr();
        wsaBuf[0].len = pSession->RecvQ->DirectEnqueueSize();
        wsaBuf[1].buf = pSession->RecvQ->GetBeginBufferPtr();
        wsaBuf[1].len = pSession->RecvQ->GetFreeSize() - pSession->RecvQ->DirectEnqueueSize();
        if (wsaBuf[1].len > pSession->RecvQ->GetBufferSize())
            __debugbreak();


        //---------------------------------------------------
        // 비동기 입출력 시작
        flags = 0;
        Ret_WSARecv = WSARecv(New_Client_Socket, wsaBuf, 2, NULL, &flags, &pSession->Recv_Overlapped, NULL);
        if (Ret_WSARecv == SOCKET_ERROR)
        {
            Ret_WSARecv_Error = WSAGetLastError();

            if (Ret_WSARecv_Error == ERROR_IO_PENDING)
            {
                continue;
            }
            else if (Ret_WSARecv_Error == 10054)
            {
                
            }
            else if (Ret_WSARecv_Error == 10053)
            {

            }
            else
            {
                _LOG(0, L"WSARecv Error : %d", Ret_WSARecv_Error);
                __debugbreak();
            }
        }

    }


    printf("Hello World!\n");
}

bool netStartUp(SOCKET* listen_sock, WSADATA* wsa)
{

    struct addrinfo hints;
    struct addrinfo* result = NULL;
    SOCKET Temp_Listen_Socket;
    char Port[6];
    int optval;
    u_long on;
    linger Linger_Opt;


    //---------------------------------------------------
    //  return Value Copy
    errno_t Ret_itoa_s;
    int Ret_getaddrinfo;
    int Ret_bind;
    int Ret_listen;
    int Ret_ioctlsocket;
    int Ret_setsockopt;

    //---------------------------------------------------
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), wsa) != 0)
        return false;
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
    // setsockopt SendBuf Size 0
    optval = 0;
    setsockopt(Temp_Listen_Socket, SOL_SOCKET, SO_SNDBUF, (char*)&optval, sizeof(optval));
    
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
    // Setup the TCP listening 
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
    //// 넌블로킹 소켓으로 전환
    //on = 1;
    //Ret_ioctlsocket = ioctlsocket(Temp_Listen_Socket, FIONBIO, &on);
    //if (Ret_ioctlsocket == SOCKET_ERROR)
    //{
    //    _LOG(0, L"ioctlsocket failed with error: %ld", WSAGetLastError());
    //    closesocket(Temp_Listen_Socket);
    //    WSACleanup();
    //    __debugbreak();
    //}

    *listen_sock = Temp_Listen_Socket;
}

