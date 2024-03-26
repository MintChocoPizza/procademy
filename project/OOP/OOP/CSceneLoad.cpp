
#include "CSceneBase.h"


#include "CSceneLoad.h"




CSceneLoad::CSceneLoad()
{
}

CSceneLoad::CSceneLoad(int iGameStage)
{
	switch (iGameStage)
	{
	case 1:
		
		break;
	case 2:
		break;
	default:
		break;
	}
}

CSceneLoad::~CSceneLoad()
{
}

bool CSceneLoad::Update(void)
{
	return false;
}
