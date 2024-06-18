/*
* Procademy - 과제#26
* 파일명: CLogger.h
* 파일 버전: 0.1
* 작성자: 이 석원
* 작성 일자: 2024-06-06
* 이전 버전 작성 일자:
* 버전 내용: TCPFighter Server 구현 - 미완
* 이전 버전 내용:
*/

//////////////////////////////////////////////////////////////////////
// 파일명: CLogger.h
// 파일 내용: 에러 Logging용 클래스 선언
//////////////////////////////////////////////////////////////////////

#pragma once
#define LOGGING
#ifdef LOGGING
	#define LOG_SET(loggingType) myutility::CLogger::GetInstance()->LogSet(loggingType)
	#define LOG(logLevel, err) myutility::CLogger::GetInstance()->Logging(__WFILE__, __LINE__, logLevel, err)
#else
	#define LOG_SET(loggingType) 
	#define LOG(logLevel, err)
#endif

// Logger에서 사용할 상수
constexpr unsigned MAX_FILE_PATH = 260;		// Logging 대상 File (절대경로) Path 길이
constexpr unsigned MAX_FILE_NAME = 32;		// Logging 출력 File명 최대 길이

//--------------------------------------------------------------------
// 네임스페이스 명: mynetworklib
// 용도: 라이브러리 네임스페이스 분류
//--------------------------------------------------------------------
namespace myutility
{

	//--------------------------------------------------------------------
	// 변수명: en_LOG_LEVEL (Scoped Enum)
	// 용도: Logging 시 사용할 상수 유형을 정의
	// Naming Rule: en_OUT_(NAME) -> 출력 방식
	//				en_LEVEL_(NAME) -> Logging 시 에러 수준
	//--------------------------------------------------------------------
	// 
	enum class en_LOG_LEVEL
	{
		// Logging 방식
		en_OUT_CONSOLE		= 0,
		en_OUT_FILE			= 1,

		// Logging Level
		en_LEVEL_DEBUG		= 100,
		en_LEVEL_WARNING	= 101,
		en_LEVEL_ERROR		= 102,
		en_LEVEL_SYSTEM		= 103,
	};

	//--------------------------------------------------------------------
	// 클래스명: CLogger
	// 용도: Logging을 위한 클래스
	//--------------------------------------------------------------------
	class CLogger
	{
	public:
		// 싱글톤 인스턴스를 받아올 때
		static CLogger *GetInstance();
		void LogSet(const en_LOG_LEVEL &logType);
		void Logging(const WCHAR *fileName, const int &lineNum, const en_LOG_LEVEL &logLevel, const errno_t &errCode);
		void Logging(const WCHAR *fileName, const int &lineNum, const en_LOG_LEVEL &logLevel, const WCHAR *errStr);
	private:
		// 싱글톤 클래스
		// GetInstance() 최초 호출 시 동적 할당과 객체 소멸을 예약하는 방식 사용
		// 실제 CNetworkManager 객체의 생성은 GetInstance()를 최초로 호출할 때 생성.
		static CLogger *m_pLogger;
		static void Destroy();

		// 생성자
		CLogger();

		// 소멸자
		~CLogger() {};

		BYTE m_outFlag;
		DWORD m_logCnt;
		WCHAR m_logFileName[MAX_FILE_NAME];	// 로그 파일명
	};
}


// __FILE__을 Wide Character Set으로 만들기 (릭트)
#define WIDE2(x) L##x
#define WIDE1(x) WIDE2(x)
#define __WFILE__ WIDE1(__FILE__)

