
#include <tchar.h>
#include <cstring>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

#include "CParsing.h"

bool CParsing::SkipNoneCommand(void)
{
	

	return false;
}

bool CParsing::GetNextWord(unsigned char** ucppBuffer, int* ipLength)
{
	int iLength;
	//---------------------------------------------------------------
	// 주석과 스페이스, 탭 등등을 전부 건너 뛴다.
	// 
	//---------------------------------------------------------------
	while (CParsing::SkipNoneCommand()) 
	{

	};

	


	return false;
}

CParsing::~CParsing()
{
}

void CParsing::LoadFile(TCHAR* fileName)
{
	errno_t err;
	long lFileSize;


	err = _tfopen_s(&_pFile, fileName, _T("r, ccs=UTF-8"));
	if (err != NULL || _pFile == NULL)
	{
		_tprintf_s(_T("%s 파일 열기에 실패했습니다. \n"), fileName);
		throw;
	}

	fseek(_pFile, 0, SEEK_END);
	lFileSize = ftell(_pFile);
	fseek(_pFile, 0, SEEK_SET);

	_readBuffer = (unsigned char*)malloc(lFileSize);
	if (_readBuffer == NULL)
	{
		_tprintf_s(_T("%s 버퍼 확보에 실패했습니다. \n"), fileName);
		throw;
	}

	//-----------------------------------------------------------------------
	// 버퍼의 마지막 주소 + 1을 가르키게 한다. 
	// 
	//-----------------------------------------------------------------------
	_pLastAddressBuffer = _readBuffer + lFileSize;


	fread_s(_readBuffer, lFileSize, 1, lFileSize, _pFile);



	fclose(_pFile);
}

bool CParsing::GetValue(TCHAR* key, int *ipValue)
{
	unsigned char* tcpBuff = _readBuffer;
	TCHAR tcWord[256];
	int iLength;
	
	// 찾고자 하는 단어가 나올때까지 계속 찾을 것이므로 while 문으로 검사
	while (CParsing::GetNextWord(&tcpBuff, &iLength))
	{
		// Word 버퍼에 찾은 단어를 저장한다.
		memset(tcWord, 0, 256);
		memcpy_s(tcWord, 256, tcpBuff, iLength);
		
		// tcWord 문자열이 0인지 검사한다.
		if (tcWord[0] == _T('\0'))
			return false;
		
		// 인자로 입력 받은 단어와 같은지 검사한다.
		if (0 == _tcscmp(key, tcWord))
		{
			// 맞다면 바로 뒤에 = 을 찾는다. 
			if (CParsing::GetNextWord(&tcpBuff, &iLength))
			{
				memset(tcWord, 0, 256);
				memcpy_s(tcWord, 256, tcpBuff, iLength);
				if (0 == _tcscmp(tcWord, _T("=")))
				{
					
					// = 다음의 데이터 부분을 얻는다. 
					if (GetNextWord(&tcpBuff, &iLength))
					{
						memset(tcWord, 0, 256);
						memcpy_s(tcWord, 256, tcpBuff, iLength);

						*ipValue = _ttoi(tcWord);
						return true;
					}
					return false;
				}
				return false;
			}
			return false;

		}
	}

	return false;
}

bool CParsing::GetValue(TCHAR* key, float* fValue)
{
	return false;
}

bool CParsing::GetValue(TCHAR* key, TCHAR* cValue)
{
	return false;
}

void CParsing::MemFree()
{
	free(_readBuffer);
}

