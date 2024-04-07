
#include <iostream>
#include <stdio.h>

using namespace std;

class A
{
public:
	A()
	{
		printf_s("A 생성자 \n");
	}
	virtual ~A()
	{
		printf_s("A 소멸자 \n");
	}

	virtual void ttt(void) = 0;

private:
	int a;
};

class B : public A
{
public:
	B()
	{
		printf_s("B 생성자 \n");
	}
	virtual ~B()
	{
		printf_s("B 소멸자 \n");
	}

	void ttt(void)
	{
		printf_s("함수호출 \n");
	}
private:
	int b;
};

class C
{
public:
	C()
	{
		printf_s("c 생성자 \n");
	}
	~C()
	{
		printf_s("c 소멸자 \n");
	}

private:

};

struct st_A
{
	int a;
	int b;
	int c;
};

void func(st_A t)
{
	cout << t.a << endl;
}

int main()
{

	func(st_A{ 1,2,3 });

	return 0;
}

