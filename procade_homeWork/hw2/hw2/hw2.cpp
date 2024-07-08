// hw2.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <time.h>

// ---------------- fibo 함수

void fiboLoop(int max)
{
    long long ret = 0;
    long long a = 0;
    long long b = 1;

    for (int i = 2; i <= max; ++i)
    {
        ret = a + b;
        a = b;
        b = ret;
    }
    
    printf_s("반복문을 활용한 피보나치(%d): %lld \n", max, ret);
}

long long fiboRecur(int max)
{
    if (max < 2)
        return max;

    return fiboRecur(max - 1) + fiboRecur(max - 2);
}

long long memo[100];
long long fiboMemo(int max)
{
    long long& ret = memo[max];
    if (ret != -1)
        return ret;
    
    memo[max] = fiboMemo(max - 1) + fiboMemo(max - 2);
    return memo[max];
}

// ---------------- string 함수

int strLen(const char* c)
{
    int len=0;
    while (c[len] != '\0')
        len++;
    return len;
}

int strLen2(const char* c)
{
    int len = 0;
    while (*c != '\0')
    {
        len++;
        c++;
    }
    return len;
}

char* strCpy(char* c1, const char* c2)
{
    while (*c2 != '\0')
    {
        *c1 = *c2;
        c1++;
        c2++;
    }
    *c1 = '\0';

    return c1;
}

int strCmp( char* c1,  char* c2)
{
    int ret = 0;
    while (*c1 != '\0' || *c2 != '\0')
    {
        ret += (*c1) - (*c2);
        c1++;
        c2++;
    }
    return ret;
}

char* strCat(char * c1, const char* c2)
{
    while (*c1 != '\0')
        c1++;
    while (*c2 != '\0')
    {
        *c1 = *c2;
        c1++;
        c2++;
    }
    *c1 = '\0';
    return c1;
}

char* strLwr(char* c)
{
    while (*c != '\0')
    {
        if ('A' <= *c && *c <= 'Z')
            *c += 32;
        // 또는 *c ^= (1<<5);

        c++;
    }
    return c;
}

int main()
{
    int max = 98;
    clock_t st, et;
    memset(memo, -1, sizeof(memo));

    st = clock();
    fiboLoop(max);
    et = clock();
    printf_s("소요시간: %lf\n", (double)(et - st) / CLOCKS_PER_SEC);

    // printf_s("재귀호출을 활용한 피보나치(%d): %lld \n", max, fiboRecur(max));
   
    memo[0] = 0;
    memo[1] = 1;
    st = clock();
    printf_s("메모이제이션을 활용한 피보나치(%d) %lld \n", max, fiboMemo(max));
    et = clock();
    printf_s("소요시간: %lf\n", (double)(et - st) / CLOCKS_PER_SEC);


    printf("\n\n\n");


    // -------------------------- string 함수----------
    // strlen strcpy strcmp strcat strlwr strupr
    char ch1[100] = "abcdefg";
    char ch2[100] = "hijklmn";
    char ch3[100] = "opqrstuvwxyz";
    char ch4[100] = "Hello World";
    char ch5[100];
    char ch6[100] = "hijklmniadlkjasldkf";

    printf_s("문자열 길이: strLen(abcdefg): %d \n", strLen(ch1));
    printf_s("문자열 길이: strLen2(abcdefg): %d \n", strLen2(ch1));
    strCpy(ch1, ch2);
    printf_s("문자열 복사: %s \n", ch1);
    printf_s("문자열 비교: %d, %d, %d \n", strCmp(ch2, ch3), strCmp(ch3, ch3), strCmp(ch2, ch5));
    strCat(ch2, ch3);
    printf_s("문자열 붙이기: %s \n", ch2);
    strLwr(ch4);
    printf_s("문자열 소문자로 변환: %s", ch4);

    
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
