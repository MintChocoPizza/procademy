#include <iostream>

class A
{
	int _a;

public:
	virtual void f1()
	{
		printf("111");
	}
	virtual void f2()
	{
		printf("111");
	}
	void f3()
	{
		printf("111");
	}
};


class B:public A
{
	int _b;

public:
	void f1()
	{
		printf("222");
	}

};

int main()
{
	B *cB = new B[10];

	B* cB2 = new B;


	printf("hello world \n");
}






// 가상함수 핸들러 중에 에러를 사용자 컨텐츠에 알리고 싶음. 
//
// 자기 자신을 참조하는 경우가 나타날 수 있음. 
//

class A
{
public:
	A()
	{
		_pA = this;
		_pA->f1();
	}

	virtual void f1() = 0;
	virtual void f2() = 0;

	A* _pA;
};

class B : public A
{

};