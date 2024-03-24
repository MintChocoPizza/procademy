
#include <Windows.h>
#include <stdio.h>

#include "ConsoleBuffer.h"
#include "CBaseObject.h"
#include "CParsing_ANSI.h"

#include "CTitleObject.h"


//CTitleObject::CTitleObject()
//{
// 
//}

CTitleObject::CTitleObject(const char* ccpFileName, const char* ccpGameInfo, int ObjectType) : CBaseObject(ObjectType)
{
	// 생성시 파일을 읽는다. 

	errno_t err;
	FILE* pFile;
	long lFileSize;

	//-----------------------------------------------------------------------------------
	// 배경화면 
	// 
	//-----------------------------------------------------------------------------------
	err = fopen_s(&pFile, ccpFileName, "r");
	if (err != 0)
	{
		printf_s("%s file open false \n", ccpFileName);
		throw;
	}

	fseek(pFile, 0, SEEK_END);
	lFileSize = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);

	_Buff = (unsigned char*)malloc(lFileSize);
	if (_Buff == NULL)
	{
		printf_s("CTitieObject malloc false \n");
		throw;
	}
	fread_s(_Buff, lFileSize, 1, lFileSize, pFile);
	fclose(pFile);


	//-----------------------------------------------------------------------------------
	// 게임의 타이틀과 게임의 버전
	// 
	//-----------------------------------------------------------------------------------
	CParsing_ANSI cParsing;
	cParsing.LoadFile(ccpGameInfo);
	cParsing.GetValue("gameName", cGameName);
	cParsing.GetValue("Version", cGameVersion);
}

CTitleObject::~CTitleObject()
{
	free(_Buff);
}

bool CTitleObject::Update(void)
{
	int iCnt;
	size_t szStringLength;

	ConsoleBuffer::GetInstance()->Buffer_Clear();
	ConsoleBuffer::GetInstance()->Sprite_Background(_Buff);


	//-------------------------
	// 버퍼에 타이틀 출력
	// 
	//-------------------------
	szStringLength = strlen(cGameName);
	for (iCnt = 0; iCnt < szStringLength; ++iCnt)
	{
		ConsoleBuffer::GetInstance()->Sprite_Draw(dfSCREEN_HEIGHT / 2 - 2, (dfSCREEN_WIDTH / 2) - ((int)szStringLength - 1) / 2 + iCnt, cGameName[iCnt]);
	}


	//-------------------------
	// 버퍼에 게임 버전 출력
	// 
	//-------------------------
	szStringLength = strlen(cGameVersion);
	for (iCnt = 0; iCnt < szStringLength; ++iCnt)
	{
		ConsoleBuffer::GetInstance()->Sprite_Draw(dfSCREEN_HEIGHT - 2, 1 + iCnt, cGameVersion[iCnt]);
	}



	return false;
}

void CTitleObject::Render(void)
{
	//ConsoleBuffer::GetInstance()->Buffer_Flip();
}
