#ifndef __MYNEW_H__
#define __MYNEW_H__



struct stALLOCINFO
{
	void* ptr;
	size_t size;
	char fileName[128];
	int line;
	bool arr;
	stALLOCINFO* pPreviousNode;
	stALLOCINFO* pNextNode;
};



class cMYNEW
{
private:
	stALLOCINFO* pHead;
	stALLOCINFO* pTail;

public:
	cMYNEW();
	~cMYNEW();


	///////////////////////////////////////////////////////////////////////////////////////
	// 굳이 클래스 안에 또 new가 있어야 하나???
	// 있으면 뭐하나????
	// 
	//
	///////////////////////////////////////////////////////////////////////////////////////
	//void* operator new(size_t size)
	//{
	//	//메모리 할당
	//	void* ptr = malloc(size);


	//	return ptr;
	//}
	//void operator delete(void* p)
	//{
	//	// 메모리 해제
	//	free(p);
	//}













	///////////////////////////////////////////////////////////////////////////////////////
	// 파일에 데이터 저장 함수
	// 
	///////////////////////////////////////////////////////////////////////////////////////
	void saveFile(const char* log, void* ptr, stALLOCINFO* stData);







	///////////////////////////////////////////////////////////////////////////////////////
	// 데이터 관리 함수들
	// 
	///////////////////////////////////////////////////////////////////////////////////////
	bool setAllocInfo(void* ptr, size_t size, const char* file, int line, bool arr);
	stALLOCINFO* findAndAllocInfo(void* ptr);
	void deleteAllocInfo(stALLOCINFO* ptr);

};




















//------------------------------------------------------------------------
// new 연산자 오버로딩 함수
// 
// 	2번째 인자 file이 const가 아니라면 오류가 발생함
//------------------------------------------------------------------------
void* operator new(size_t size, const char* file, int line);
void* operator new[](size_t size, const char* file, int line);

void operator delete(void* p, char* File, int lIne);
void operator delete[](void* p, char* File, int lIne);




//------------------------------------------------------------------------
// delete 연산자 오버로딩 함수
// 
//------------------------------------------------------------------------
void operator delete(void* ptr);
void operator delete[](void* ptr);


#define  new    new(__FILE__, __LINE__)

#endif // !__MYNEW_H__
