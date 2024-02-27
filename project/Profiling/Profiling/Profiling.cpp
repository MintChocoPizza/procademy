#pragma comment (lib, "winmm.lib")

#include <Windows.h>
#include <time.h>
#include <strsafe.h>
#include <profileapi.h>



#include "Profiling.h"

#define FLAG_USED		1
#define FLAG_BEGIN		1<<1	

#define MAX_PROFILE_SIZE 30


stPROFILE_SAMPLE stProfile_Sample[MAX_PROFILE_SIZE];
LARGE_INTEGER Start;
LARGE_INTEGER End;
LARGE_INTEGER Freq;



//////////////////////////////////////////////////////////////////////////////////////
// 하나의 함수 Profiling 시작, 끝 함수.
// 
// Parameters: (char *)Profiling 이름
// Return: 없음
//////////////////////////////////////////////////////////////////////////////////////
void ProfileBegin(const TCHAR* szName)
{
	int iCnt;

	QueryPerformanceFrequency(&Freq);
	QueryPerformanceCounter(&Start);

	// 예외처리 
	// Begin이 호출된 다음에 End가 호출 되지 않은 경우 강제로 뻑내서 종료시킨다. 
	for (iCnt = 0; iCnt < MAX_PROFILE_SIZE; ++iCnt)
	{
		// 일치하지 않는다면 넘어간다. 
		if (_tcscmp(stProfile_Sample[iCnt].szName, szName) != 0)
			continue;
		
		// Begin이 두번 호출되었다면 뻑냄
		if ((stProfile_Sample[iCnt].lFlag & FLAG_BEGIN) == FLAG_BEGIN)
		{
			_tprintf_s(_T("Begin이 2번 호출되었습니다. \n"));
			throw;
		}

		stProfile_Sample[iCnt].lFlag |= FLAG_BEGIN;
		stProfile_Sample[iCnt].lStartTime = Start;
		++stProfile_Sample[iCnt].iCall;
		return;
		
	}


	// 처음 호출되어 다양한 정보를 저장해야함
	for (iCnt = 0; iCnt < MAX_PROFILE_SIZE; ++iCnt)
	{
		// 사용하고 있는 자리는 넘어간다. 
		if (stProfile_Sample[iCnt].lFlag & FLAG_USED)
			continue;

		stProfile_Sample[iCnt].lFlag |= FLAG_BEGIN | FLAG_USED;;
		_tcscpy_s(stProfile_Sample[iCnt].szName, szName);
		stProfile_Sample[iCnt].lStartTime = Start;
		stProfile_Sample[iCnt].iMax[0] = 0;
		stProfile_Sample[iCnt].iMax[1] = 0;
		stProfile_Sample[iCnt].iMin[0] = LLONG_MAX;
		stProfile_Sample[iCnt].iMin[1] = LLONG_MAX;
		++stProfile_Sample[iCnt].iCall;
		break;
	}
}

void ProfileEnd(const TCHAR* szName)
{
	int iCnt; 
	int iCntTwo;
	__int64 deltaTime;

	QueryPerformanceFrequency(&Freq);
	QueryPerformanceCounter(&End);


	for (iCnt = 0; iCnt < MAX_PROFILE_SIZE; ++iCnt)
	{
		// 일치하지 않는다면 넘어간다.
		if (_tcscmp(stProfile_Sample[iCnt].szName, szName) != 0)
			continue;

		// 이름은 일치하지만 2번째 End 호출 타이밍이라면 뻑낸다.
		if ((stProfile_Sample[iCnt].lFlag & FLAG_BEGIN) != FLAG_BEGIN)
		{
			_tprintf_s(_T("End가 2번 호출되었습니다. \n"));
			throw;
		}

		stProfile_Sample[iCnt].lFlag &= ~FLAG_BEGIN;
		deltaTime = ((End.QuadPart - stProfile_Sample[iCnt].lStartTime.QuadPart) / Freq.QuadPart) * 1000000;
		stProfile_Sample[iCnt].iTotalTime += deltaTime;

		if (stProfile_Sample[iCnt].iMin[0] > deltaTime)
			stProfile_Sample[iCnt].iMin[0] = deltaTime;
		else if (stProfile_Sample[iCnt].iMin[1] > deltaTime)
			stProfile_Sample[iCnt].iMin[1] = deltaTime;
		
		if (stProfile_Sample[iCnt].iMax[0] < deltaTime)
			stProfile_Sample[iCnt].iMax[0] = deltaTime;
		else if (stProfile_Sample[iCnt].iMax[1] < deltaTime)
			stProfile_Sample[iCnt].iMax[1] = deltaTime;
	}
}




//////////////////////////////////////////////////////////////////////////////////////
// Profiling 된 데이터를 Text 파일로 출력한다. 
// 
// Parameters: (char *) 출력될 파일 이름
// Return: 없음
//////////////////////////////////////////////////////////////////////////////////////
void ProfileDataOutText(const TCHAR* szFileName)
{
	FILE* pFile;
	errno_t err;
	int iCnt;
	__int64 Average;

	err = _tfopen_s(&pFile, szFileName, _T("w, ccs=UNICODE"));
	if (err != 0)
	{
		_tprintf_s(_T("파일 열기 실패 \n"));
		throw;
	}
	if (pFile == 0)
	{
		_tprintf_s(_T("이런 경우가 있음? \n"));
		throw;
	}

	_ftprintf_s(pFile, _T("%s \n"), _T("---------------------------------------------------------------------------------------------------------------------"));
	_ftprintf_s(pFile, _T("%64s | %13s | %13s | %13s | %13s \n"), _T("Name"), _T("Average"), _T("Min"), _T("Max"), _T("Call"));
	for (iCnt = 0; iCnt < MAX_PROFILE_SIZE; ++iCnt)
	{
		if ((stProfile_Sample[iCnt].lFlag & FLAG_USED) != FLAG_USED)
			continue;

		Average = (stProfile_Sample[iCnt].iTotalTime
			- stProfile_Sample[iCnt].iMax[0]
			- stProfile_Sample[iCnt].iMax[1]
			- stProfile_Sample[iCnt].iMin[0]
			- stProfile_Sample[iCnt].iMin[1]) / (stProfile_Sample[iCnt].iCall - 4);
		_ftprintf_s(pFile, _T("%64s | %13I64d µs | %13I64d µs | %13I64d µs | %13I64d \n"), stProfile_Sample[iCnt].szName, Average, stProfile_Sample[iCnt].iMin[0] , stProfile_Sample[iCnt].iMax[0], stProfile_Sample[iCnt].iCall);

	}
	_ftprintf_s(pFile, _T("%s"), _T("---------------------------------------------------------------------------------------------------------------------"));
	
	fclose(pFile);
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