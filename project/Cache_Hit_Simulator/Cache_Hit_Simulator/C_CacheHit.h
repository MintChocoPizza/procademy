
///////////////////////////////////////////////////////////////////////
// 
// 현재 시스템 L1 Data 6 * 32KBytes		8-way
// 
// L1 캐시 하나:	32KBytes	8-way
// 
// 캐시 크기: 32 * 1024 = 32768
// 캐시 라인 하나의 크기: 64Byte;
// 
// 캐시의 인텍스 범위 = 캐시 크기 / 캐시 라인 크기
//						= 32768 / 64
//						= 512
// 
// 8way : 캐시 인덱스 하나에 8개의 주소 중복
//		= 512 / 8
//		= 64
// 
// offset: 캐시 라인 크기 
// 
///////////////////////////////////////////////////////////////////////
#ifndef __C_CACHE_HIT_H__
#define __C_CACHE_HIT_H__

#include <cstdint>
#include <map>
#include <time.h>
#include <stdio.h>

#include "C_Queue.h"

//#define __Printing_var__

#ifdef __Printing_var__
#include <iostream>
#include <stdio.h>
using namespace std;
#endif // __Printing_var__


namespace OreoPizza
{
	struct ST_Cache_Hit
	{
#ifdef _WIN64
		C_Queue<unsigned __int64> Cache_Way;
#else
		C_Queue<unsigned __int32> Cache_Way;
#endif
	};

	struct ST_Log
	{
#ifdef _WIN64
		unsigned __int64 Address
#else
		unsigned __int32 Address;
#endif
		unsigned long Hit;
		unsigned long Miss;
	};


	class C_Cache_Hit
	{
	public:
		// 기본 생성자
		inline C_Cache_Hit();
		inline C_Cache_Hit(unsigned int L1_Data_Cache_Size, unsigned int Cache_Line_Size, unsigned int Cache_Way);
		inline ~C_Cache_Hit();

		void CacheHit(void* address, const char* file, int line);


		unsigned int _L1_Data_Cache_Size;
		unsigned int _Cache_Line_Size;
		unsigned int _Cache_Way;

		unsigned int _Max_Index;

		#ifdef _WIN64
		unsigned __int64 _Index_Offset_masking;
		#else 
		unsigned __int32 _Index_Offset_masking;
		#endif  
		unsigned int _Index_Masking;

		ST_Cache_Hit* _Cach_Hit_Check;
		#ifdef _WIN64
		std::map<unsigned __int64,ST_Log> _Log;
		#else 
		std::map<unsigned __int32, ST_Log> _Log;
		#endif  
	};

	inline C_Cache_Hit::C_Cache_Hit() : _L1_Data_Cache_Size(32768), _Cache_Line_Size(64), _Cache_Way(8), _Max_Index(64), _Index_Masking(0xFC0),
		#ifdef _WIN64
		_Index_Offset_masking(0x0000000000000fff)
		#else
		_Index_Offset_masking(0x00000fff)
		#endif 
	{
		_Cach_Hit_Check = (ST_Cache_Hit*)malloc(sizeof(ST_Cache_Hit) * 64);
		for (int i = 0; i < 64; ++i)
		{
		#ifdef _WIN64
			new(&Cach_Hit_Check[i].Cache_Way) C_Queue<unsigned __int64>(8);
		#else
			new(&_Cach_Hit_Check[i].Cache_Way) C_Queue<unsigned __int32>(8);
		#endif
		}
	}

	inline OreoPizza::C_Cache_Hit::C_Cache_Hit(unsigned int L1_Data_Cache_Size, unsigned int Cache_Line_Size, unsigned int Cache_Way) :
		_L1_Data_Cache_Size(L1_Data_Cache_Size),
		_Cache_Line_Size(Cache_Line_Size),
		_Cache_Way(Cache_Way)
	{
		unsigned int Index_Offset_masking = 0;
		unsigned int Index_Masking;
		unsigned int Next_Bit=0;


		// offset Masking
		Index_Offset_masking |= Cache_Line_Size - 1;


		while ((Index_Offset_masking >>Next_Bit & 1) == 1)
		{
			++Next_Bit;
		}


		Index_Masking = L1_Data_Cache_Size / Cache_Line_Size / Cache_Way;
		// 캐쉬의 Max_Index값 저장
		_Max_Index = Index_Masking;

		// 캐쉬의 Max_Index만큼 메모리 동적 할당
		_Cach_Hit_Check = (ST_Cache_Hit*)malloc(sizeof(ST_Cache_Hit) * Index_Masking);
		// C_Queue 생성사 호출
		for (unsigned int i = 0; i < Index_Masking; ++i)
		{
			#ifdef _WIN64
			new(&Cach_Hit_Check[i].Cache_Way) C_Queue<unsigned __int64>(8);
			#else
			new(&_Cach_Hit_Check[i].Cache_Way) C_Queue<unsigned __int32>(8);
			#endif
		}
			
		Index_Masking = (Index_Masking-1) << Next_Bit;
		// save Index_Masking
		_Index_Masking = Index_Masking;

		// Add Index Masking
		Index_Offset_masking |= Index_Masking;


		#ifdef  __Printing_var__
		std::bitset<sizeof(int) * 8> binary(Index_Offset_masking);
		std::cout << binary << std::endl;
		#endif //  __Printing_var__


		// Store in the final member variable
		_Index_Offset_masking = Index_Offset_masking;
	}

	inline C_Cache_Hit::~C_Cache_Hit()
	{
	}

	inline void C_Cache_Hit::CacheHit(void* address, const char* file, int line)
	{
		#ifdef _WIN64
		unsigned __int64 Address_Value	= (unsigned __int64)address;
		unsigned __int64 Index_Offset	= Address_Value & _Index_Offset_masking;
		unsigned __int64 Index			= Address_Value & _Index_Masking;
		#else
		unsigned __int32 Address_Value	= (unsigned __int32)address;
		unsigned __int32 Index_Offset	= Address_Value & _Index_Offset_masking;
		unsigned __int32 Index			= Address_Value & _Index_Masking;
		#endif 

		FILE* pFile;
		errno_t err;
		struct tm New_Time;
		__time64_t Long_Time;
		char Time_Buff[26];
		

		ST_Cache_Hit *Cach_Hit_Check = &_Cach_Hit_Check[Index];
		

		if (Cach_Hit_Check->Cache_Way.find(&Address_Value))
		{
			// 캐쉬 히트
			
		}
		else
		{
			// 캐쉬 미스
			Cach_Hit_Check->Cache_Way.dequeue();
			Cach_Hit_Check->Cache_Way.enqueue(&Address_Value);
			
			// 캐쉬 미스가 난 파일과 라인을 기록한다. 
			_time64(&Long_Time);
			_localtime64_s(&New_Time, &Long_Time);
			sprintf_s(Time_Buff, "Cache_Miss_%d%d%d_%d%d%d.txt", New_Time.tm_year, New_Time.tm_mon, New_Time.tm_mday, New_Time.tm_hour, New_Time.tm_min, New_Time.tm_sec);
			
			fopen_s(&pFile, Time_Buff, "a");
			// [주소] 파일 이름 : 코드 라인
			fprintf_s(pFile, "Cache Miss [%p] %s : %d \n", address, file, line);
			
		}
	
	}
}

#define CacheHit(address) CacheHit(address, __FILE__, __LINE__)

#endif // !__C_CACHE_HIT_H__

