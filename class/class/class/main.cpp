
#include <iostream>


void* operator new[](size_t size, const char* fileName, int line)
    {
        void* ptr = malloc(size);

        return ptr;
    };


void* operator new(size_t size, const char* fileName, int line)
{
    void* ptr = malloc(size);

    return ptr;
}


    void operator delete(void* pMem, const char* pszFilename, int nLine) {
        //free(pMem);
    }

    void operator delete[](void* pMem, const char* Filename, int Line)
        {
            // free(pMem);
        }



#define new     new(__FILE__, __LINE__)

class Test
{
protected:
	int a;
	int b;
	int c;



public:
	Test()
	{
		a = 10;
		b = 10;
		c = 10;
		std::cout << "Test에서 생성자 " << std::endl;
	}
	~Test()
	{
		a = 0;
		b = 0;
		c = 0;
		std::cout << "Test에서 소멸자" << std::endl;
	}
};




int main()
{
    // 메모리 해제
    int* a = new int;
    int* b = new int[5];

    // 메모리 헤제 X
    // log에 ARRAY로 기록이 남음
    int* c = new int;
    int* d = new int[5];

    // 잘못된 메모리 해제
    int* e = new int;
    int* f = new int[5];

    // 메모리 해제
    Test* aTest = new Test;
    Test* bTest = new Test[5];

    // 메모리 해제 X
    Test* cTest = new Test;
    Test* dTest = new Test[5];

    // 잘못된 메모리 해제
    Test* eTest = new Test;
    Test* fTest = new Test[5];


    // 정상 해제
    delete a;
    delete[] b;

    // 정상 해제
    delete aTest;
    delete[] bTest;


    delete[] e;
    delete f;

    // 무한 메모리 해제 시도함
    // delete[] eTest;
    delete fTest;



    return 0;
}