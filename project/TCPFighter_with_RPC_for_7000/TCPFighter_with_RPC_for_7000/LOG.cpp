
#include <Windows.h>
#include <stdarg.h>
#include <stdio.h>
#include "LOG.h"



int g_iLogLevel;
wchar_t g_szLogBuff[1024];



void Log(wchar_t * szString, int iLogLevel)
{
	wprintf(L"LogLevel: %d - %s \n",iLogLevel, szString);
}
