
#include <stdio.h>
#include <Windows.h>

#include "CSceneBase.h"


#include "CSceneLoad.h"





CSceneLoad::CSceneLoad(int iGameStage) :_stageName("")
{
	sprintf_s(_stageName, "GameFile\\stage%d.txt", iGameStage);
	
	LoadStage(_stageName);

}

CSceneLoad::~CSceneLoad()
{
}

bool CSceneLoad::Update(void)
{
	return false;
}

void CSceneLoad::LoadStage(char* cStageName)
{
	errno_t err;
	FILE* pFile;
	long lFileSize;


	unsigned char* ucpBuff;
	
	err = fopen_s(&pFile, cStageName, "rb");
	if (err != NULL)
	{
		printf_s("%s 파일 열기 실패 \n", cStageName);
		throw;
	}
	
	fseek(pFile, 0, SEEK_END);
	lFileSize = ftell(pFile);
	fseek(pFile, 0, SEEK_CUR);

	ucpBuff = (unsigned char*)malloc(lFileSize);

	if (ucpBuff == 0)
	{
		printf_s("%s 파일에서 메모리 할당 실패 \n", cStageName);
		throw;
	}

	fread_s(ucpBuff, lFileSize, 0, lFileSize, pFile);
	fclose(pFile);


	
	
}
