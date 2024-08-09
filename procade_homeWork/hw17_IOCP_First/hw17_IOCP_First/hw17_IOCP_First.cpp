
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

    LONG RecvCheck;
    LONG SendCheck;

    OVERLAPPED Recv_Overlapped;
    OVERLAPPED Send_Overlapped;
};

DWORD g_SessionID = 0;
HANDLE g_hCp;

bool netStartUp(SOCKET* listen_sock, WSADATA* wsa);
unsigned __stdcall AcceptThread(void* pArg);
unsigned __stdcall WorkerThread(void* pArg);

int main()
{
    int iCnt;

    WSADATA wsa;
    SOCKET listen_sock;


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
    g_hCp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (g_hCp == NULL) return 1;

    //---------------------------------------------------
    // CPU Count
    GetSystemInfo(&si);

    //---------------------------------------------------
    // Create Worker Thread (CPU * 2) + AcceptThread
    iCreateThreadNum = (int)si.dwNumberOfProcessors * 2 + 1;
    for (iCnt = 0; iCnt < iCreateThreadNum - 1; ++iCnt)
    {
        hThread[iCnt] = (HANDLE)_beginthreadex(NULL, 0, WorkerThread, NULL, NULL, NULL);
    }
    hThread[iCnt] = (HANDLE)_beginthreadex(NULL, 0, AcceptThread, NULL, NULL, NULL);
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
    // 모든 스레드 들의 종료를 기다린다. 
    while (1)
    {

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

unsigned __stdcall AcceptThread(void* pArg)
{
    int Client_Addr_Len;
    sockaddr_in Client_Addr;
    SOCKET listen_sock;
    SOCKET New_Client_Socket;
    int Ret_Accept_Error;

    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    WSABUF wsaBuf[2];
    DWORD flags;
    int Ret_WSARecv;
    int Ret_WSARecv_Error;

    listen_sock = *(SOCKET*)pArg;

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
        pSession->RecvQ = new C_RING_BUFFER();
        pSession->SendQ = new C_RING_BUFFER();
        pSession->socket = New_Client_Socket;
        pSession->SessionID++;
        pSession->SendCheck = 0;
        pSession->RecvCheck = 0;

        //---------------------------------------------------
		// link Client_Socket to IOCP
        CreateIoCompletionPort((HANDLE)New_Client_Socket, g_hCp, (ULONG_PTR)&pSession, 0);

        //---------------------------------------------------
		// wsabuf Setting
		// 커널에 copy 하기 때문에 지역으로 설정하고? , 따로 보관하지 않았다. 

        // 링버퍼 락 거는 코드 필요하다. 
        // ? 여기서 처음 만들었는데 다른데서 접근할 일 없다. 
        wsaBuf[0].buf = pSession->RecvQ->GetInBufferPtr();
        wsaBuf[0].len = pSession->RecvQ->DirectEnqueueSize();
        wsaBuf[1].buf = pSession->RecvQ->GetBeginBufferPtr();
        wsaBuf[1].len = pSession->RecvQ->GetFreeSize() - pSession->RecvQ->DirectEnqueueSize();
        if (wsaBuf[1].len > pSession->RecvQ->GetBufferSize())
            __debugbreak();

        //--------------------------------------------------------
        // 비동기 입출력 시작, 해당 소켓을 비동기IO에 등록한다. 
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
                delete pSession->RecvQ;
                delete pSession->SendQ;
                delete pSession;
                closesocket(pSession->socket);
            }
            else if (Ret_WSARecv_Error == 10053)
            {
                delete pSession->RecvQ;
                delete pSession->SendQ;
                delete pSession;
                closesocket(pSession->socket);
            }
            else
            {
                _LOG(0, L"WSARecv Error : %d", Ret_WSARecv_Error);
                __debugbreak();
            }
        }
    }

    return 0;
}

unsigned __stdcall WorkerThread(void* pArg)
{
    DWORD dwTransgerred;
    st_SESSION* pSession;
    OVERLAPPED* lpOverlapped;
    WSABUF wsaBuf[2];
    DWORD flags;
    bool bDisconnetSession;

    //---------------------------------------------------------------
    // return Value
    bool Ret_GQCS;
    int Ret_WSASend;
    int Ret_WSARecv;
    int Ret_WSASend_Error;
    int Ret_WSARecv_Error;

    while (1)
    {
        dwTransgerred = NULL;
        pSession = NULL;
        lpOverlapped = NULL;
        bDisconnetSession = false;


        Ret_GQCS = GetQueuedCompletionStatus(g_hCp, &dwTransgerred, (ULONG_PTR*)&pSession, &lpOverlapped, INFINITE);

        //-----------------------------------------------------------------------------------------------------------------------------------
        // 예외처리
        // Ret_GQCS == true : I/O & IOCP 성공
        // Ret_GQCs == false
        // Recv_Overlapped == NULL인 경우: 1. GQCS() 뻑남 == 핸들값 에러, 2. 타임아웃_INFINITE가 아닌경우 타임아웃됨
        //  
        // Recv_Overlapped != NULL인 경우: I/O 가 실패함, 매개변수 3개에 실패 정보를 저장한다. 
        //
        // 1. 0,0,0 인경우 GQCS뻑남 || 스레드 종료
        //
        // 2. send, recv 실패에 대한 예외처리를 하지 않는다. 어짜피 다음 Send, Recv 시도할 때 에러가 발생한다. 
        // 
        // 3. 

        //-----------------------------------------------------------------------------------------------------------------------------------
        // 스레드 종료
        if (dwTransgerred == NULL && pSession == NULL && lpOverlapped == NULL)
        {

        }

        //-----------------------------------------------------------------------------------------------------------------------------------
        // GQCS가 뻑이 난 경우
        if(lpOverlapped == NULL)
        {
            
        }

        //-----------------------------------------------------------------------------------------------------------------------------------
        // 

        //-----------------------------------------------------------------------------------------------------------------------------------
        // 리시브인지 확인
        if (&pSession->Recv_Overlapped == lpOverlapped)
        {
            //-----------------------------------------------------------------------------------------------------------------------------------
            // 여기에 진입했다는 것은 WSARecv 요청이 완료되었다.
            InterlockedExchange(&pSession->RecvCheck, true);

            //-----------------------------------------------------------------------------------------------------------------------------------
            // 0Byte를 수신했다면 Session에 대한 연결을 종료한다. / 바로 연결을 종료하면 안됨.
            if (dwTransgerred == NULL)
            {
                bDisconnectSession = true;
                break;
            }

            pSession->RecvQ->Lock();

            // in 위치 이동후, 온 메시지 출력
            pSession->RecvQ->MoveIn(dwTransgerred);
            memcpy(pSession->RecvQ->GetInBufferPtr(), '\0', 1);
            wprintf(L"socket:%d / str: %s \n", pSession->socket, pSession->RecvQ->GetOutBufferPtr());

            // SendQ에 복사
            pSession->SendQ->Enqueue(pSession->RecvQ->GetOutBufferPtr(), dwTransgerred);

            wsaBuf[0].buf = pSession->SendQ->GetOutBufferPtr();
            wsaBuf[0].len = pSession->SendQ->DirectDequeueSize();
            wsaBuf[1].buf = pSession->SendQ->GetBeginBufferPtr();
            wsaBuf[1].len = pSession->SendQ->GetUseSize() - pSession->SendQ->DirectDequeueSize();

            // 비동기 send 걸기
            flags = 0;
            Ret_WSASend = WSASend(pSession->socket, wsaBuf, 2, NULL, flags, &pSession->Send_Overlapped, NULL);
            if (Ret_WSASend == SOCKET_ERROR)
            {
                Ret_WSASend_Error = WSAGetLastError();

                if (Ret_WSARecv_Error == ERROR_IO_PENDING)
                {
                    InterlockedExchange(pSession->SendCheck, false);
                    continue;
                }
                else if (Ret_WSARecv_Error == 10054)
                {
                    bDisconnectSession = true;
                }
                else if (Ret_WSARecv_Error == 10053)
                {
                   bDisconnectSession = true;
                }
                else
                {
                    _LOG(2, L"WSASend Error : %d", Ret_WSARecv_Error);
                    __debugbreak();
                }
            }
            else
            {
                InterlockedExchange(pSession->SendCheck, false);
            }
            pSession->RecvQ->UnLock();
        }

        //-----------------------------------------------------------------------------------------------------------------------------------
        // 센드인지 확인
        if (&pSession->Send_Overlapped == lpOverlapped)
        {
            
        }


        if (bDisconnectSession == true)
        {
            //-----------------------------------------------------------------------------------------------------------------------------------
            // 여기에 락을 사용하지 않은 이유는 
            // 상대방이 먼저 연결을 끊은 경우: 송수신 시도시 에러 나옴 => 체크 변경 안됨
            // i/o 에러: 송수신 시도시 에러 나옴
            if (pSession->RecvCheck == true && pSession->SendCheck == true)
            {
                delete pSession->SendQ;
                delete pSessiin->RecvQ;
                delete pSession;
            }
        }

    }


    return 0;
}
