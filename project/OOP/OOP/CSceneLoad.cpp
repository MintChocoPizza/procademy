
#include <stdio.h>

#include "CSceneBase.h"


#include "CSceneLoad.h"





CSceneLoad::CSceneLoad(int iGameStage) :_stageName("")
{
	sprintf_s(_stageName, "stage%d.txt", iGameStage);
}

CSceneLoad::~CSceneLoad()
{
}

bool CSceneLoad::Update(void)
{
	return false;
}
