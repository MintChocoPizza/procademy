// fibonacci.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <algorithm>
#include <time.h>

int arr[2000];

int fibonacci(int num)
{
    if (num == 1)
        return 1;
    if (num < 1)
        return 0;

    return fibonacci(num - 1) + fibonacci(num - 2);
}

int cacheFibo(int num)
{
    if (num < 1)
        return 0;
    int& ret = arr[num];
    if (ret != -1) return ret;

    return cacheFibo(num - 1) + cacheFibo(num - 2);
}

int main()
{
    clock_t t, et;

    int num = 41;

    // 재귀호출을 이용한 피보나치
    t = clock();
    printf_s("재귀호출을 이용한 피보나치(%d): %d \n", num, fibonacci(num));
    et = clock();
    printf_s("걸린 시간: %lf \n\n", (double)(et-t)/CLOCKS_PER_SEC);


    // 반복문을 이용한 피보나치
    t = clock();
    int result = 0;
    int a = 0;
    int b = 1;
    for (int i = 2; i <= num; ++i)
    {
        result = a + b;
        a = b;
        b = result;
    }
    printf_s("반복문을 이용한 피보나치(%d): %d \n", num, result);
    et = clock();
    printf_s("걸린 시간: %lf \n\n", (double)(et - t) / CLOCKS_PER_SEC);

    // 배열(메모이제이션)을 이용한 피보나치
    t = clock();
    memset(arr, -1, sizeof(arr));
    arr[0] = 0;
    arr[1] = 1;
    printf_s("배열(메모이제이션)을 이용한 피보나치(%d) %d \n", num, cacheFibo(num));
    et = clock();
    printf_s("걸린 시간: %lf \n\n", (double)(et - t) / CLOCKS_PER_SEC);

}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
