
#ifndef __SESSION_H__
#define __SESSION_H__

//-------------------------------------------------------------------------------
// 네트워크 1개만 존재해야함 
//-------------------------------------------------------------------------------

struct st_SESSION
{
	SOCKET				Socket;			// 현 접속의 TCP 소켓 
	
	DWORD				dwSessionID;	// 접속자의 고유 세션 ID
	
	C_RING_BUFFER		RecvQ;			// 수신 큐;
	C_RING_BUFFER		SendQ;			// 송신 큐; 

	bool				Disconnect;		// 연결 끊김 체크
};


class C_Session
{
	enum 
	{
		dfNETWORK_PORT = 20000
	};

public:
	static C_Session* GetInstance(void);
	void netIOProcess(void);
	void netProc_Accept(void);
	void netProc_Send(DWORD SessionID);
	void netProc_Recv(DWORD SessionID);

	void Disconnect();

private:
	C_Session(void);
	~C_Session(void);

private:
	static C_Session _C_Session;
	DWORD _SessionID;

public:
	std::map<DWORD, st_SESSION*> _Session;

public:
	SOCKET _Listen_Socket;
	WSADATA _WsaData;

};


#endif // !__SESSION_H__
