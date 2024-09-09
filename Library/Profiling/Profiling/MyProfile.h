//////////////////////////////////////////////////////////////////
// 
// 사용법: 
// 
// 프로그램 시작시 	ProfileReset();
// 
// 원하는 스코프 안 제일 상단에 cMYPROFILE cTest(_T("프로파일링 이름"));
// 
// 프로그램 끝날 때 	ProfileDataOutText(_T("저장할 파일 이름.txt"));
// 
//////////////////////////////////////////////////////////////////
#ifndef __MYPROFILE_H__
#define __MYPROFILE_H__

#include <Windows.h>
#include <time.h>
#include <tchar.h>

#include <stdio.h>

#define PROFILE

#ifdef PROFILE
	#define PRO_BEGIN(TagName)		ProfileBegin(TagName)
	#define PRO_END(TagName)		ProfileEnd(TagName)
#else
	#define PRO_BEGIN(TagName)
	#define PRO_END(TagName)
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 하나의 함수 Profiling 시작, 끝 함수
// 
// Parameters: (char *)Profiling 이름
// Return: 없음
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ProfileBegin(const TCHAR* szName);
void ProfileEnd(const TCHAR* szName);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Profiling 된 데이터를 Text 파일로 출력한다. 
// 
// Parameters: (char *)출력될 파일 이름
// Return: 없음
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ProfileDataOutText(const TCHAR* szFileName);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Profiling 된 데이터를 모두 초기화 한다. 
// 
// Parameters: 없음
// Return: 없음
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ProfileReset(void);


struct stPROFILE_SAMPLE
{
	 unsigned long	lFlag;						// 프로파일의 사용 여부
	TCHAR			szName[64];					// 프로파일 샘플 이름 
	LARGE_INTEGER	lStartTime;					// 프로파일 샘플 실행 시간
	__int64			iTotalTime;					// 전체 사용시간 카운터 Time. (출력시 호출회수로 나누어 평균 구함)
	__int64			iMin[2];					// 최소 사용시간 카운터 Time. (초단위로 계산하여 저장/ [0] 가장 최소 [1] 다음 최소 [2])
	__int64			iMax[2];					// 최대 사용시간 카운터 Time. (초단위로 계산하여 저장/ [0] 가장 최대 [1] 다음 최대 [2])
	__int64			iCall;;						// 누적 호출 횟수
};

class cMYPROFILE
{
	const TCHAR* _tag;

public:
	cMYPROFILE(const TCHAR* tag)
	{
		PRO_BEGIN(tag);
		_tag = tag;
	}
	~cMYPROFILE()
	{
		PRO_END(_tag);
	}
};




#endif // !__MYPROFILE_H__
