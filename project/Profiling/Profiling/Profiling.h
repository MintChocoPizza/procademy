#ifndef __PROFILING_H__
#define __PROFILING_H__

#include <tchar.h>

struct stPROFILE_SAMPLE
{
	// 00000000 00000000 00000000 00000000
	// 1번: 사용 여부 
	// 2번: 0: End됨->Begin호출 타이밍, 1: Begin됨 -> End호출 타이밍
	// 
	unsigned long		lFlag;					// 프로파일의 사용 여부. (배열시에만)
	TCHAR				szName[64];				// 프로파일 샘플 이름
	LARGE_INTEGER		lStartTime;				// 프로파일 샘플 실행 시간
	__int64				iTotalTime;				// 전체 사용시간 카운터 Time. (출력시 호출횟수로 나누어 평균 구함)
	__int64				iMin[2];				// 최소 사용시간 카운터 Time. (초단위로 계산하여 저장/ [0] 가장 최소 [1] 다음 최소)
	__int64				iMax[2];				// 최대 사용시간 카운터 Time. (초단위로 계산하여 저장/ [0] 가장 최대 [1] 다음 최대)
	__int64				iCall;					// 누적 호출 횟수.
};




//////////////////////////////////////////////////////////////////////////////////////
// 하나의 함수 Profiling 시작, 끝 함수.
// 
// Parameters: (char *)Profiling 이름
// Return: 없음
//////////////////////////////////////////////////////////////////////////////////////
void ProfileBegin(const TCHAR* szName);
void ProfileEnd(const TCHAR* szName);




//////////////////////////////////////////////////////////////////////////////////////
// Profiling 된 데이터를 Text 파일로 출력한다. 
// 
// Parameters: (char *) 출력될 파일 이름
// Return: 없음
//////////////////////////////////////////////////////////////////////////////////////
void ProfileDataOutText(const TCHAR* szFileName);



//////////////////////////////////////////////////////////////////////////////////////
// 프로파일링 된 데이터를 모두 초기화 한다.
// 
// Parameters: 없음 
// Return: 없음
//////////////////////////////////////////////////////////////////////////////////////
void ProfileReset(void);


///////////////////////////////////////////////////////////////
// 클래스 생성자 소멸자를 사용한 begin, end 자동화
// 
// 클래스 내부에 선언과 정의를 같이 하면 inline으로 선언되어 
// 함수 호출이 없어져 성능에 큰 영향을 미치지 않는다.
// 
///////////////////////////////////////////////////////////////
class Profile
{
public:
	Profile(const TCHAR* tag)
	{
		ProfileBegin(tag);
		_tag = tag;
	}
	~Profile()
	{
		ProfileEnd(_tag);
	}

	const TCHAR* _tag;
};







#ifdef PROFILE
	#define PRO_BEGIN(TagName) ProfileBegin(TagName)
	#define PRO_END(TagName) ProfileEnd(TagName)
#else
	#define PRO_BEGIN(TagName) 
	#define PRO_END(TagName) 
#endif




#endif // !__PROFILING_H__
