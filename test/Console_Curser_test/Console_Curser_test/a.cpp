#include <stdio.h>
#include <Windows.h>
#include "a.h"

HANDLE hC;

void csM(int y, int x)
{
    hC = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD stCrd;
    stCrd.Y = y;
    stCrd.X = x;

    if (SetConsoleCursorPosition(hC, stCrd) == 0)
        printf_s("커서 이동에 실패했습니다.");
}