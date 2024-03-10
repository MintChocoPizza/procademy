#include <iostream>

#include "MyNew.h"

class TTest
{
protected:
    int a;
    int b;
    int c;
public:
    TTest()
    {
        a = 0;
        b = 0;
        c = 0;
        std::cout << "Test에서 메모리 할당 \n";
    }
    ~TTest()
    {
        std::cout << "Test에서 메모리 해제 \n";
    }
};

void ttt(void)
{
    // 메모리 해제
    int* a = new int;
    int* b = new int[5];

    // 메모리 헤제 X
    int* c = new int;
    int* d = new int[5];

    // 잘못된 메모리 해제
    int *e = new int;
    int *f = new int[5];

    // 메모리 해제
    TTest* aTest = new TTest;
    TTest* bTest = new TTest[5];

    // 메모리 해제 X
    TTest* cTest = new TTest;
    TTest* dTest = new TTest[5];

    // 잘못된 메모리 해제
    TTest* eTest = new TTest;
    TTest* fTest = new TTest[5];


    delete a;
    delete[] b;

    delete aTest;
    delete[] bTest;

    delete[] e;
    delete f;
    
    // 무한 메모리 해제 시도
    //delete[] eTest;
    delete fTest;
    
}