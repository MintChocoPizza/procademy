
#include <Windows.h>
#include <stdarg.h>
#include <stdio.h>
#include "LOG.h"



int g_iLogLevel = 0;
wchar_t g_szLogBuff[1024];
char g_sz_c_LogBuff[1024];



void Log(wchar_t * szString, int iLogLevel)
{
	wprintf(L"LogLevel: %d - %s \n",iLogLevel, szString);
}

void c_Log(char* szString, int iLogLevel)
{
	printf("LogLevel: %d - %s \n", iLogLevel, szString);
}
