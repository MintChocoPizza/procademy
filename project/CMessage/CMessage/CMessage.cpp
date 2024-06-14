
#include <Windows.h>
#include "CMessage.h"

CMessage::CMessage()
{
	m_chpBeginPos = new char[eBUFFER_DEFAULT];
	m_iBufferSize = eBUFFER_DEFAULT;
	m_iDataSize = 0;
	m_chpReadPos = m_chpBeginPos;
	m_chpWritePos = m_chpBeginPos;
	m_chpEndPos = m_chpBeginPos + eBUFFER_DEFAULT;
	m_iReSizeMaxSize = eBUFFER_ReSize_Max_Size;
}

CMessage::CMessage(int iBufferSize)
{
	m_chpBeginPos = new char[iBufferSize];
	m_iBufferSize = iBufferSize;
	m_iDataSize = 0;
	m_chpReadPos = m_chpWritePos = m_chpBeginPos;
	m_chpEndPos = m_chpBeginPos + iBufferSize;
	m_iReSizeMaxSize = eBUFFER_ReSize_Max_Size;
}

CMessage::~CMessage()
{
	delete[] m_chpBeginPos;
}

CMessage& CMessage::operator=(CMessage& clSrcPacket)
{
	// TODO: 여기에 return 문을 삽입합니다.

	return *this;
}

CMessage& CMessage::operator<<(unsigned char byValue)
{
	// TODO: 여기에 return 문을 삽입합니다.

	return *this;
}

CMessage& CMessage::operator<<(char chValue)
{
	// TODO: 여기에 return 문을 삽입합니다.

	return *this;
}

CMessage& CMessage::operator<<(short shValue)
{
	// TODO: 여기에 return 문을 삽입합니다.
	return *this;
}

CMessage& CMessage::operator<<(unsigned short wValue)
{
	// TODO: 여기에 return 문을 삽입합니다.
	return *this;
}

CMessage& CMessage::operator<<(int iValue)
{
	// TODO: 여기에 return 문을 삽입합니다.
	return *this;
}

CMessage& CMessage::operator<<(long lValue)
{
	// TODO: 여기에 return 문을 삽입합니다.
	return *this;
}

CMessage& CMessage::operator<<(float fValue)
{
	// TODO: 여기에 return 문을 삽입합니다.
	return *this;
}

CMessage& CMessage::operator<<(__int64 iValue)
{
	// TODO: 여기에 return 문을 삽입합니다.
	return *this;
}

CMessage& CMessage::operator<<(double dValue)
{
	// TODO: 여기에 return 문을 삽입합니다.
	return *this;
}

CMessage& CMessage::operator>>(BYTE& byValue)
{
	// TODO: 여기에 return 문을 삽입합니다.
	return *this;
}

CMessage& CMessage::operator>>(char& chValue)
{
	// TODO: 여기에 return 문을 삽입합니다.
	return *this;
}

CMessage& CMessage::operator>>(short& shValue)
{
	// TODO: 여기에 return 문을 삽입합니다.
	return *this;
}

CMessage& CMessage::operator>>(WORD& wValue)
{
	// TODO: 여기에 return 문을 삽입합니다.
	return *this;
}

CMessage& CMessage::operator>>(int& iValue)
{
	// TODO: 여기에 return 문을 삽입합니다.
	return *this;
}

CMessage& CMessage::operator>>(DWORD& dwValue)
{
	// TODO: 여기에 return 문을 삽입합니다.
	return *this;
}

CMessage& CMessage::operator>>(float& fValue)
{
	// TODO: 여기에 return 문을 삽입합니다.
	return *this;
}

CMessage& CMessage::operator>>(__int64& iValue)
{
	// TODO: 여기에 return 문을 삽입합니다.
	return *this;
}

CMessage& CMessage::operator>>(double& dValue)
{
	// TODO: 여기에 return 문을 삽입합니다.

	return *this;
}

int CMessage::GetData(char* chpDest, size_t iSize)
{
	if (m_iDataSize < iSize)
		return 0;

	memcpy(chpDest, m_chpReadPos, iSize);
	m_iDataSize -= iSize;
	m_chpReadPos += iSize;
	return iSize;
}

int CMessage::PutData(char* chpSrc, size_t iSrcSize)
{
	// 해당 함수에서 넣을 수 있는 사이즈를 계산하나 
	// 다른 함수를 호출하여 넣을 수 있는 사이즈를 계산하나 
	// 조삼모사 이다.
	// 매번 use, free를 호출하여 갱신하는것과
	// size - use를 계산하는 것과 큰 차이는 없어보인다. 
	// 오히려 Free 를 매번 계산하는 것보다 이게 더 빠르다. 
	// 저장하는데 this호출이 2번 일어나기 때문....
	
	// this 콜을 지역변수로 고치는거 또한 어셈블리의 수가 같다. 
	if (m_iBufferSize - m_iDataSize < iSrcSize)
		return 0;
	memcpy(m_chpWritePos, chpSrc, iSrcSize);
	m_iDataSize += iSrcSize;
	m_chpWritePos += iSrcSize;
	return iSrcSize;
}

int CMessage::ReSize()
{
	size_t NewBufferSize;
	size_t ReadPosIndex;
	size_t WritePosIndex;
	char* chpBeginPos;

	NewBufferSize = m_iBufferSize * 2;

	if (NewBufferSize >= m_iReSizeMaxSize)
	{

	}

	chpBeginPos = m_chpBeginPos;
	// 리사이즈 하면 할 일 
	// 1. 리사이즈 최대 크기보다 작은지 확인.
	// 2. 리사이즈 후 원래 있던 메모리를 새로운 메모리로 복사
	char* chpNewSize = new char[NewBufferSize];
	// memcpy(chpNewSize, chpBeginPos, NewBufferSize >> 2);
	memcpy(chpNewSize, chpBeginPos, m_iBufferSize);
	// 3. 인덱스 대신 사용하던 Pos들 갱신해야함

	ReadPosIndex = m_chpReadPos - chpBeginPos;
	WritePosIndex = m_chpWritePos - chpBeginPos;


	m_chpReadPos = chpNewSize + ReadPosIndex;
	m_chpWritePos = chpNewSize + WritePosIndex;
	m_chpEndPos = chpNewSize + NewBufferSize;
	m_chpBeginPos = chpNewSize;
	m_iBufferSize = NewBufferSize;

	return 0;
}

int CMessage::ReSize(size_t Size)
{
	size_t NewBufferSize;
	size_t ReadPosIndex;
	size_t WritePosIndex;
	char* chpBeginPos;

	NewBufferSize = m_iBufferSize + Size;

	if (NewBufferSize >= m_iReSizeMaxSize)
	{

	}

	chpBeginPos = m_chpBeginPos;
	// 리사이즈 하면 할 일 
	// 1. 리사이즈 최대 크기보다 작은지 확인.
	// 2. 리사이즈 후 원래 있던 메모리를 새로운 메모리로 복사
	char* chpNewSize = new char[NewBufferSize];
	// memcpy(chpNewSize, chpBeginPos, NewBufferSize - Size);
	memcpy(chpNewSize, chpBeginPos, m_iBufferSize);
	// 3. 인덱스 대신 사용하던 Pos들 갱신해야함

	ReadPosIndex = m_chpReadPos - chpBeginPos;
	WritePosIndex = m_chpWritePos - chpBeginPos;


	m_chpReadPos = chpNewSize + ReadPosIndex;
	m_chpWritePos = chpNewSize + WritePosIndex;
	m_chpEndPos = chpNewSize + NewBufferSize;
	m_chpBeginPos = chpNewSize;
	m_iBufferSize = NewBufferSize;

	return 0;

}


