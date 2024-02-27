#include <tchar.h>
#include <Windows.h>
#include <strsafe.h>

#include "Profiling.h"

#define FLAG_BEGIN		1<<1	

#define MAX_PROFILE_SIZE 30


stPROFILE_SAMPLE stProfile_Sample[MAX_PROFILE_SIZE];



//////////////////////////////////////////////////////////////////////////////////////
// 하나의 함수 Profiling 시작, 끝 함수.
// 
// Parameters: (char *)Profiling 이름
// Return: 없음
//////////////////////////////////////////////////////////////////////////////////////
void ProfileBegin( TCHAR* szName)
{
	int iCnt;

	// 예외처리 
	// Begin이 호출된 다음에 End가 호출 되지 않은 경우 강제로 뻑내서 종료시킨다. 
	for (iCnt = 0; iCnt < MAX_PROFILE_SIZE; ++iCnt)
	{
		if (_tcscmp(stProfile_Sample[iCnt].szName, szName) != 0)
			continue;

		if(stProfile_Sample[iCnt].lFlag & FLAG_BEGIN)

	}
}

void ProfileEnd( TCHAR* szName)
{

}




//////////////////////////////////////////////////////////////////////////////////////
// Profiling 된 데이터를 Text 파일로 출력한다. 
// 
// Parameters: (char *) 출력될 파일 이름
// Return: 없음
//////////////////////////////////////////////////////////////////////////////////////
void ProfileDataOutText( TCHAR* szFileName)
{

}




//////////////////////////////////////////////////////////////////////////////////////
// 프로파일링 된 데이터를 모두 초기화 한다.
// 
// Parameters: 없음 
// Return: 없음
//////////////////////////////////////////////////////////////////////////////////////
void ProfileReset(void)
{
	int iCnt;

	for (iCnt = 0; iCnt < MAX_PROFILE_SIZE; ++iCnt)
	{
		stProfile_Sample[iCnt].lFlag = 0;
		stProfile_Sample[iCnt].szName[0] = '\0';
		stProfile_Sample[iCnt].lStartTime.QuadPart = 0;
		stProfile_Sample[iCnt].iTotalTime = 0;
		stProfile_Sample[iCnt].iMax[0] = 0;
		stProfile_Sample[iCnt].iMax[1] = 0;
		stProfile_Sample[iCnt].iMin[0] = LLONG_MAX;
		stProfile_Sample[iCnt].iMin[1] = LLONG_MAX;
		stProfile_Sample[iCnt].iCall = 0;
	}
}