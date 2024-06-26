
#ifndef __C_SAVE_LOG_H__
#define __C_SAVE_LOG_H__

/////////////////////////////////////////////////////////////////////////////////////
// 
// 원하는 기능: 
//	1. 로그를 텍스트 파일(utf-16)로 기록하고 싶음. 
// 
// 사용 방법:
//	1. 처음 프로그램이 시작하면 자동으로 현재 시간의 텍스트 파일을 한 개 만든다. 
//	2. 로그를 출력하고 싶은 자리에 함수(로그 내용)을 작성하면 알아서 모든 내용이
//		텍스트 파일에 자동으로 기입된다. 
// 
// 
// 
// 
/////////////////////////////////////////////////////////////////////////////////////

struct st_Log_INFO
{

};

class C_SAVE_LOG
{
public:
	C_SAVE_LOG();
	~C_SAVE_LOG();

	// 오버로딩: 과적하다. 하나의 함수에 여러가지 기능을 '과적하다'
	void saveLog(const wchar_t* Log_Str);
	void printfLog(const wchar_t* format, ...);

private:
	wchar_t timebuf[26];
	bool isWrite;

};

// extern C_SAVE_LOG c_Save_Log;


#endif // !__C_SAVE_LOG_H__

