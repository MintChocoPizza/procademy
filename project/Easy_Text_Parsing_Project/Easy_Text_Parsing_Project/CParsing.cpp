
#include <tchar.h>
#include <cstring>
#include <stdio.h>
#include <malloc.h>

#include "CParsing.h"

bool CParsing::SkipNoneCommand(void)
{

	return false;
}

bool CParsing::GetNextWord(TCHAR** chppBuffer, int* ipLength)
{
	

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
	if (err != NULL)
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


	fread_s(_readBuffer, lFileSize, 1, lFileSize, _pFile);

	fclose(_pFile);
}

bool CParsing::GetValue(TCHAR* key, int *iValue)
{
	
	// 찾고자 하는 단어가 나올때까지 계속 찾을 것이므로 while 문으로 검사
	while (CParsing::GetNextWord())
	{

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
}

