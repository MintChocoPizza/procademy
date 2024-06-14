
#include <Windows.h>

#include "CMessage.h"

CMessage::CMessage()
{
	m_chpBuffer = new char[eBUFFER_DEFAULT];
	m_iBufferSize = eBUFFER_DEFAULT;
	m_iDataSize = 0;
	m_ReadPos = 0;
	m_WritePos = 0;
}

CMessage::CMessage(int iBufferSize)
{
	m_chpBuffer = new char[iBufferSize];
	m_iBufferSize = eBUFFER_DEFAULT;
	m_iDataSize = 0;
	m_ReadPos = m_WritePos = 0;
}

CMessage::~CMessage()
{
	delete[] m_chpBuffer;
}

CMessage& CMessage::operator=(CMessage& clSrcPacket)
{
	// TODO: 여기에 return 문을 삽입합니다.
	
}

CMessage& CMessage::operator<<(unsigned char byValue)
{
	// TODO: 여기에 return 문을 삽입합니다.

}

CMessage& CMessage::operator<<(char chValue)
{
	// TODO: 여기에 return 문을 삽입합니다.

}

CMessage& CMessage::operator<<(short shValue)
{
	// TODO: 여기에 return 문을 삽입합니다.
}

CMessage& CMessage::operator<<(unsigned short wValue)
{
	// TODO: 여기에 return 문을 삽입합니다.
}

CMessage& CMessage::operator<<(int iValue)
{
	// TODO: 여기에 return 문을 삽입합니다.
}

CMessage& CMessage::operator<<(long lValue)
{
	// TODO: 여기에 return 문을 삽입합니다.
}

CMessage& CMessage::operator<<(float fValue)
{
	// TODO: 여기에 return 문을 삽입합니다.
}

CMessage& CMessage::operator<<(__int64 iValue)
{
	// TODO: 여기에 return 문을 삽입합니다.
}

CMessage& CMessage::operator<<(double dValue)
{
	// TODO: 여기에 return 문을 삽입합니다.
}

CMessage& CMessage::operator>>(BYTE& byValue)
{
	// TODO: 여기에 return 문을 삽입합니다.
}

CMessage& CMessage::operator>>(char& chValue)
{
	// TODO: 여기에 return 문을 삽입합니다.
}

CMessage& CMessage::operator>>(short& shValue)
{
	// TODO: 여기에 return 문을 삽입합니다.
}

CMessage& CMessage::operator>>(WORD& wValue)
{
	// TODO: 여기에 return 문을 삽입합니다.
}

CMessage& CMessage::operator>>(int& iValue)
{
	// TODO: 여기에 return 문을 삽입합니다.
}

CMessage& CMessage::operator>>(DWORD& dwValue)
{
	// TODO: 여기에 return 문을 삽입합니다.
}

CMessage& CMessage::operator>>(float& fValue)
{
	// TODO: 여기에 return 문을 삽입합니다.
}

CMessage& CMessage::operator>>(__int64& iValue)
{
	// TODO: 여기에 return 문을 삽입합니다.
}

CMessage& CMessage::operator>>(double& dValue)
{
	// TODO: 여기에 return 문을 삽입합니다.
}

int CMessage::GetData(char* chpDest, int iSize)
{

	return iSize;
}

int CMessage::PutData(char* chpSrc, int iSrcSize)
{
	return 0;
}
