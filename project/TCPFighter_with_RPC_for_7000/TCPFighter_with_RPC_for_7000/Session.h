
#ifndef __SESSION_H__
#define __SESSION_H__

//-------------------------------------------------------------------------------
// 네트워크 1개만 존재해야함 
//-------------------------------------------------------------------------------
struct st_SESSION
{
	SOCKET				Socket;				// 현 접속의 TCP 소켓 
	
	DWORD				dwSessionID;		// 접속자의 고유 세션 ID
	
	C_RING_BUFFER*		RecvQ;				// 수신 큐;
	C_RING_BUFFER*		SendQ;				// 송신 큐; 

	DWORD				dwLastRecvTime;		// 메시지 수신 체크를 위한 시간 (타임아웃용)

	bool				Disconnect;			// 연결 끊김 체크

	st_SESSION(SOCKET New_Socket, DWORD dw_New_SessionID);
};


class C_Session
{
	enum 
	{
		dfNETWORK_PORT = 20000
	};

public:
	static C_Session* GetInstance(void);
	void netIOProcess(void);					// 이 함수를 메인에서 호출하여 내부에서 Send, Recv를 다 마친다. 
	void netProc_Accept(void);
	void netProc_Send(st_SESSION* pSession);
	void netProc_Recv(st_SESSION* pSession);

	//----------------------------------------------------------------
	// 특정 섹터 1개에 있는 클라이언트들 에게 메시지 보내기,
	// 마지막 매개변수 제외하고
	void SendPacket_SectorOne(int iSectorX, int iSectorY, SerializeBuffer* pPacket, st_SESSION* pExceptSession);

	//----------------------------------------------------------------
	// 특정 1명의 클라이언트 에게 메시지 보내기
	void SendPacket_Unicast(st_SESSION* pSession, SerializeBuffer* pPacket);

	//----------------------------------------------------------------
	// 클라이언트 기준 주변 섹터에 메시지 보내기 (최대 9개 영역)
	void SendPacket_Around(st_SESSION* pSession, SerializeBuffer* pPacket, bool bSendMe = false);

	//----------------------------------------------------------------
	// 진정 브로드 캐스딩 (시스템적인 메시지 외에는 사용하지 않음)
	void SendPacket_Broadcast(st_SESSION* pSession, SerializeBuffer* pPacket);


	void Disconnect(void);

private:
	C_Session(void);
	~C_Session(void);

private:
	static C_Session _C_Session;

	// 생성자에서 초기화 하여 0부터 사용할 때 마다 1씩 증가함. 
	DWORD _SessionID;

public:
	// Key: UserID, Value: Session
	//std::map<DWORD, st_SESSION*> _Session;

	// Key: SessionID, Value: Session
	std::unordered_map< DWORD, st_SESSION*> _Session_Map;

public:
	SOCKET _Listen_Socket;
	WSADATA _WsaData;

};

void ForwardDecl(int DestID, SerializeBuffer* sb);

#endif // !__SESSION_H__
