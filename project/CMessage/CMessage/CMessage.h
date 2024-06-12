
#ifndef __CMESSAGE_H__
#define __CMESSAGE_H__

class CMessage
{
public:
	enum en_PACKET
	{
		eBUFFER_DEFAULT = 1400			// 패킷의 기본 버퍼 사이즈.
	};

	//////////////////////////////////////////////////////////////////////////
	// 생성자, 파괴자.
	//
	// Return:
	//////////////////////////////////////////////////////////////////////////
	CMessage();
	CMessage(int iBufferSize);

	virtual	~CMessage();


	//////////////////////////////////////////////////////////////////////////
	// 패킷 청소.
	//
	// Parameters: 없음.
	// Return: 없음.
	//////////////////////////////////////////////////////////////////////////
	void	Clear(void) { m_chpRead = m_chpWrite = m_chpBuffer; }


	//////////////////////////////////////////////////////////////////////////
	// 버퍼 사이즈 얻기.
	//
	// Parameters: 없음.
	// Return: (int)패킷 버퍼 사이즈 얻기.
	//////////////////////////////////////////////////////////////////////////
	int	GetBufferSize(void) { return m_iBufferSize; }
	//////////////////////////////////////////////////////////////////////////
	// 현재 사용중인 사이즈 얻기.
	//
	// Parameters: 없음.
	// Return: (int)사용중인 데이타 사이즈.
	//////////////////////////////////////////////////////////////////////////
	int		GetDataSize(void) { return m_iDataSize; }
	//////////////////////////////////////////////////////////////////////////
	// 현재 사용하지 않는 사이즈 얻기.
	//
	// Parameters: 없음.
	// Return: (int)사용하지 않는 데이타 사이즈.
	//////////////////////////////////////////////////////////////////////////
	int		GetFreeSize(void) { return m_iBufferSize - m_iDataSize; }



	//////////////////////////////////////////////////////////////////////////
	// 버퍼 포인터 얻기.
	//
	// Parameters: 없음.
	// Return: (char *)버퍼 포인터.
	//////////////////////////////////////////////////////////////////////////
	char* GetBufferPtr(void) { return m_chpBuffer; }

	//////////////////////////////////////////////////////////////////////////
	// 버퍼 Pos 이동. (음수이동은 안됨)
	// GetBufferPtr 함수를 이용하여 외부에서 강제로 버퍼 내용을 수정할 경우 사용. 
	//
	// Parameters: (int) 이동 사이즈.
	// Return: (int) 이동된 사이즈.
	//////////////////////////////////////////////////////////////////////////
	//int		MoveWritePos(int iSize);
	//int		MoveReadPos(int iSize);
	void	MoveWritePos(int iSize) { m_chpWrite += iSize; }
	void	MoveReadPos(int iSize) { m_chpRead += iSize; }





	/* ============================================================================= */
	// 연산자 오버로딩
	/* ============================================================================= */
	CMessage& operator = (CMessage& clSrcPacket);

	//////////////////////////////////////////////////////////////////////////
	// 넣기.	각 변수 타입마다 모두 만듬.
	//////////////////////////////////////////////////////////////////////////
	CMessage& operator << (unsigned char byValue);
	CMessage& operator << (char chValue);

	CMessage& operator << (short shValue);
	CMessage& operator << (unsigned short wValue);

	CMessage& operator << (int iValue);
	CMessage& operator << (long lValue);
	CMessage& operator << (float fValue);

	CMessage& operator << (__int64 iValue);
	CMessage& operator << (double dValue);


	//////////////////////////////////////////////////////////////////////////
	// 빼기.	각 변수 타입마다 모두 만듬.
	//////////////////////////////////////////////////////////////////////////
	CMessage& operator >> (BYTE& byValue);
	CMessage& operator >> (char& chValue);

	CMessage& operator >> (short& shValue);
	CMessage& operator >> (WORD& wValue);

	CMessage& operator >> (int& iValue);
	CMessage& operator >> (DWORD& dwValue);
	CMessage& operator >> (float& fValue);

	CMessage& operator >> (__int64& iValue);
	CMessage& operator >> (double& dValue);




	//////////////////////////////////////////////////////////////////////////
	// 데이타 얻기.
	//
	// Parameters: (char *)Dest 포인터. (int)Size.
	// Return: (int)복사한 사이즈.
	//////////////////////////////////////////////////////////////////////////
	int		GetData(char* chpDest, int iSize);

	//////////////////////////////////////////////////////////////////////////
	// 데이타 삽입.
	//
	// Parameters: (char *)Src 포인터. (int)SrcSize.
	// Return: (int)복사한 사이즈.
	//////////////////////////////////////////////////////////////////////////
	int		PutData(char* chpSrc, int iSrcSize);




protected:

	//------------------------------------------------------------
	// 큐 처럼 구현되는 직렬화 버퍼에서 Read(Out), Write(In)
	//------------------------------------------------------------
	char* m_chpRead;
	char* m_chpWrite;

	//------------------------------------------------------------
	// 버퍼 사이즈.
	//------------------------------------------------------------
	char* m_chpBuffer;

	//------------------------------------------------------------
	// 버퍼 사이즈.
	//------------------------------------------------------------
	int	m_iBufferSize;

	//------------------------------------------------------------
	// 현재 버퍼에 사용중인 사이즈.
	//------------------------------------------------------------
	int	m_iDataSize;

};



#endif // !__CMESSAGE_H__

