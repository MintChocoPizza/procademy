/*
* Procademy - 과제#26
* 파일명: CLogger.cpp
* 파일 버전: 0.1
* 작성자: 이 석원
* 작성 일자: 2024-06-06
* 이전 버전 작성 일자:
* 버전 내용: TCPFighter Server 구현 - 미완
* 이전 버전 내용:
*/

//////////////////////////////////////////////////////////////////////
// 파일명: CLogger.cpp
// 파일 내용: 에러 로깅용 클래스 구현
//////////////////////////////////////////////////////////////////////

#include "pch.h"

// 사용할 표준 라이브러리 Header
#include <cstdio>
#include <ctime>
#include <Windows.h>

// 사용자 정의 Header
#include "CLogger.h"

WCHAR g_logStr[MAX_PATH];

//--------------------------------------------------------------------
// 변수명: m_pScreenManager
// 기능: 싱글톤 인스턴스를 가리킬 멤버 변수 (정적 멤버 변수)
//--------------------------------------------------------------------
myutility::CLogger *myutility::CLogger::m_pLogger = nullptr;

//--------------------------------------------------------------------
// 함수명: GetInstance
// 기능: 싱글톤 인스턴스를 받아올 때 사용
//--------------------------------------------------------------------
myutility::CLogger *myutility::CLogger::GetInstance()
{
	if (nullptr == m_pLogger)
	{
		m_pLogger = new CLogger;
		atexit(Destroy);
	}
	return m_pLogger;
}

//--------------------------------------------------------------------
// 함수명: Destroy
// 기능: 실질적인 인스턴스 소멸
//--------------------------------------------------------------------
void myutility::CLogger::Destroy()
{
	delete m_pLogger;
	m_pLogger = nullptr;
}

// 생성자
myutility::CLogger::CLogger()
	:m_outFlag(static_cast<BYTE>(en_LOG_LEVEL::en_OUT_CONSOLE)), m_logCnt(0)
{
	// 프로그램 시작 시점에서의 시간을 기록하기 위한 부분
	tm currTime;
	__time64_t longTime;
	_time64(&longTime);

	// localtime 함수를 통해 현재 시간을 얻어온다.
	errno_t err = _localtime64_s(&currTime, &longTime);
	// 예외 처리
	if (0 != err)
	{
		wprintf(L"_localtime64_s() Error Occured. Error Code:%d\n", err);
		return;
	}

	// Log 파일명 포맷 만드는 부분
	swprintf_s(m_logFileName, sizeof(m_logFileName), L"LOG_%d%02d%02d_%02d%02d%02d.txt",
		currTime.tm_year + 1900, currTime.tm_mon + 1, currTime.tm_mday,
		currTime.tm_hour, currTime.tm_min, currTime.tm_sec);

	// log 파일 생성
	FILE *pFile = nullptr;
	err = _wfopen_s(&pFile, m_logFileName, L"wt");
	// 예외 처리
	if (0 != err)
	{
		wprintf(L"_wfopen_s() Error Occured. Error Code:%d\n", err);
		return;
	}

	fclose(pFile);
}

void myutility::CLogger::LogSet(const en_LOG_LEVEL &logType)
{
	m_outFlag = static_cast<BYTE>(logType);
}

//--------------------------------------------------------------------
// 함수명: Logging
// 기능: 로그 작성
//--------------------------------------------------------------------
void myutility::CLogger::Logging(const WCHAR *fileName, const int &lineNum, const en_LOG_LEVEL &logLevel, const errno_t &errCode)
{
	// 에러 발생 시점에서의 시간을 기록하기 위한 부분
	tm currTime;
	__time64_t longTime;
	_time64(&longTime);

	// localtime 함수를 통해 현재 시간을 얻어온다.
	errno_t err = _localtime64_s(&currTime, &longTime);
	// 예외 처리
	if (0 != err)
	{
		wprintf(L"_localtime64_s() Error Occured. Error Code:%d\n", err);
		return;
	}

	// Console로 기록하는 플래그가 되어있다면
	if (m_outFlag & static_cast<BYTE>(en_LOG_LEVEL::en_OUT_FILE))
	{
		// 기록할 Log Level
		// 콘솔 창의 경우에는 ERROR 또는 SYSTEM으로 지정한 로그만 따로 LOG LEVEL이 출력되게 한다.
		switch (logLevel)
		{
		case en_LOG_LEVEL::en_LEVEL_ERROR:
			wprintf(L"Log Level: %s\t", L"ERROR");
			break;
		case en_LOG_LEVEL::en_LEVEL_SYSTEM:
			wprintf(L"Log Level: %s\t", L"SYSTEM");
			break;
		default:
			break;
		}

		wprintf(L"WSAGetLastError() Error Code:%d\n\n", errCode);
	}

	
	// File로 기록하는 플래그가 되어있다면
	if (m_outFlag & static_cast<BYTE>(en_LOG_LEVEL::en_OUT_FILE))
	{
		// 생성했던 log 파일 개방
		FILE *pFile = nullptr;
		err = _wfopen_s(&pFile, m_logFileName, L"at");
		// 예외 처리
		if (0 != err)
		{
			wprintf(L"_wfopen_s() Error Occured. Error Code:%d\n", err);
			return;
		}

		// 에러 발생 시간
		fwprintf(pFile, L"Logged Time: %d-%02d-%02d %02d:%02d:%02d\n",
			currTime.tm_year + 1900, currTime.tm_mon + 1, currTime.tm_mday,
			currTime.tm_hour, currTime.tm_min, currTime.tm_sec);
		
		// 로그 내역
		fwprintf(pFile, L"File Name: %s\n", fileName);
		fwprintf(pFile, L"Line No: %d\n", lineNum);
		// 기록할 Log Level
		switch (logLevel)
		{
		case en_LOG_LEVEL::en_LEVEL_DEBUG:
			fwprintf(pFile, L"Log Level: %s\n", L"DEBUG");
			break;
		case en_LOG_LEVEL::en_LEVEL_WARNING:
			fwprintf(pFile, L"Log Level: %s\n", L"WARNING");
			break;
		case en_LOG_LEVEL::en_LEVEL_ERROR:
			fwprintf(pFile, L"Log Level: %s\n", L"ERROR");
			break;
		case en_LOG_LEVEL::en_LEVEL_SYSTEM:
			fwprintf(pFile, L"Log Level: %s\n", L"SYSTEM");
			break;
		default:
			fwprintf(pFile, L"Log Level: %s\n", L"UNDEFINED");
			break;
		}
		fwprintf(pFile, L"WSAGetLastError() Error Code:%d\n\n", errCode);
		fclose(pFile);
	}
}

//--------------------------------------------------------------------
// 함수명: Logging
// 기능: 로그 작성
//--------------------------------------------------------------------
void myutility::CLogger::Logging(const WCHAR *fileName, const int &lineNum, const en_LOG_LEVEL &logLevel, const WCHAR *errStr)
{
	// 에러 발생 시점에서의 시간을 기록하기 위한 부분
	tm currTime;
	__time64_t longTime;
	_time64(&longTime);

	// localtime 함수를 통해 현재 시간을 얻어온다.
	errno_t err = _localtime64_s(&currTime, &longTime);
	// 예외 처리
	if (0 != err)
	{
		wprintf(L"_localtime64_s() Error Occured. Error Code:%d\n", err);
		return;
	}

	// Console로 기록하는 플래그가 되어있다면
	if (m_outFlag & static_cast<BYTE>(en_LOG_LEVEL::en_OUT_FILE))
	{
		// 기록할 Log Level
		// 콘솔 창의 경우에는 ERROR 또는 SYSTEM으로 지정한 로그만 따로 LOG LEVEL이 출력되게 한다.
		switch (logLevel)
		{
		case en_LOG_LEVEL::en_LEVEL_ERROR:
			wprintf(L"Log Level: %s\t", L"ERROR");
			break;
		case en_LOG_LEVEL::en_LEVEL_SYSTEM:
			wprintf(L"Log Level: %s\t", L"SYSTEM");
			break;
		default:
			break;
		}
		wprintf(L"%s\n", errStr);
	}


	// File로 기록하는 플래그가 되어있다면
	if (m_outFlag & static_cast<BYTE>(en_LOG_LEVEL::en_OUT_FILE))
	{
		// 생성했던 log 파일 개방
		FILE *pFile = nullptr;
		err = _wfopen_s(&pFile, m_logFileName, L"at");
		// 예외 처리
		if (0 != err)
		{
			wprintf(L"_wfopen_s() Error Occured. Error Code:%d\n", err);
			return;
		}

		// 에러 발생 시간
		fwprintf(pFile, L"Logged Time: %d-%02d-%02d %02d:%02d:%02d\n",
			currTime.tm_year + 1900, currTime.tm_mon + 1, currTime.tm_mday,
			currTime.tm_hour, currTime.tm_min, currTime.tm_sec);

		// 로그 내역
		fwprintf(pFile, L"File Name: %s\n", fileName);
		fwprintf(pFile, L"Line No: %d\n", lineNum);
		// 기록할 Log Level
		switch (logLevel)
		{
		case en_LOG_LEVEL::en_LEVEL_DEBUG:
			fwprintf(pFile, L"Log Level: %s\n", L"DEBUG");
			break;
		case en_LOG_LEVEL::en_LEVEL_WARNING:
			fwprintf(pFile, L"Log Level: %s\n", L"WARNING");
			break;
		case en_LOG_LEVEL::en_LEVEL_ERROR:
			fwprintf(pFile, L"Log Level: %s\n", L"ERROR");
			break;
		case en_LOG_LEVEL::en_LEVEL_SYSTEM:
			fwprintf(pFile, L"Log Level: %s\n", L"SYSTEM");
			break;
		default:
			fwprintf(pFile, L"Log Level: %s\n", L"UNDEFINED");
			break;
		}
		fwprintf(pFile, L"Log Description:%s\n\n", errStr);
		fclose(pFile);
	}
}