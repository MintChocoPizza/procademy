
#ifndef __NETWORK_H__
#define __NETWORK_H__


constexpr int dfRANGE_MOVE_TOP = 50;
constexpr int dfRANGE_MOVE_LEFT = 10;
constexpr int dfRANGE_MOVE_RIGHT = 630;
constexpr int dfRANGE_MOVE_BOTTOM = 470;

struct stPACKET_HEADER;

struct st_SESSION
{
	SOCKET			Socket;				// 현 접속의 TCP 소켓
	DWORD			dwSessionID;		// 접속자의 고유 세션 ID
	C_RING_BUFFER	RecvQ;				// 수신 큐;
	C_RING_BUFFER	SendQ;				// 송신 큐;

	DWORD			dwAction;			// 
	BYTE			byDirection;		//	

	short			shX;
	short			shY;

	char			chHP;
};




bool init_Listen_Socket(void* Listen_Socket, void* wsa_Data);
void netIOProcess(void);
void netProc_Accept(void);
void init_Session(SOCKET Client_Socket, st_SESSION* st_New_Player);

void netProc_Recv(st_SESSION* pSession);
void netProc_Send(st_SESSION* pSession);

void netSendUnicast(st_SESSION* pSession, stPACKET_HEADER *header, char* packet, int Packet_Len);
void netSendBroadcast(st_SESSION* pSession, stPACKET_HEADER *header, char* packet, int Packet_Len);

void Disconnect();

#endif // !__NETWORK_H__
