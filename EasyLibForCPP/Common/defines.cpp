#include "stdafx.h"
#include "defines.h"
#include <cstdarg>
#include <iostream>
#include <tchar.h>


void VDebugOutA(const char* format, ...)
{
	char buf[DEBUG_OUT_STR_MAX + 1] = { 0 };
	char info[DEBUG_OUT_STR_MAX + 1] = { 0 };
	va_list va = nullptr;

	va_start(va, format);
	_vsnprintf_s(buf, DEBUG_OUT_STR_MAX, format, va);
	_snprintf_s(info, DEBUG_OUT_STR_MAX, DEBUG_OUT_STR_MAX, "%s %s\r\n",
		DEBUG_PREFIX_STR_A, buf);
	va_end(va);

	DBGOUTA(info);
}

void VDebugOutW(const wchar_t* format, ...)
{
	wchar_t buf[DEBUG_OUT_STR_MAX + 1] = { 0 };
	wchar_t info[DEBUG_OUT_STR_MAX + 1] = { 0 };
	va_list va = nullptr;

	va_start(va, format);
	_vsnwprintf_s(buf, DEBUG_OUT_STR_MAX, format, va);
	_snwprintf_s(info, DEBUG_OUT_STR_MAX, DEBUG_OUT_STR_MAX, L"%s %s\r\n",
		DEBUG_PREFIX_STR_W, buf);
	va_end(va);

	DBGOUTW(info);
}

void FormatWinMsg()
{
	TCHAR szErr[DEBUG_OUT_STR_MAX + 1] = { 0 };
	LPVOID buf = nullptr; 
	DWORD dwErrId = 0;

	dwErrId = GetLastError();
	if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS 
		| FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_MAX_WIDTH_MASK, 
		nullptr, dwErrId, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
		(LPTSTR)&buf, 0, nullptr)) 
	{
		_sntprintf_s(szErr, DEBUG_OUT_STR_MAX, _T("id = %#X,err = %s."), dwErrId, (LPTSTR)buf);
		LocalFree(buf); 
	} 
	else 
	{ 
		_sntprintf_s(szErr, DEBUG_OUT_STR_MAX, _T("id = %#X,err = unknown Windows error."), dwErrId);
	}

	V_DEBUGOUT(szErr);
}

bool InitConsole()
{
	bool result = true;

#ifdef USE_CONSOLE_OUTPUT
	if (!AllocConsole())
	{
		OutputDebugString(_T("AllocConsole failed!!!"));
		return false;
	}

	FILE* file = nullptr;
	freopen_s(&file, "CONOUT$", "w+t", stdout);
	freopen_s(&file, "CONIN$", "r+t", stdin);

	SetConsoleTitle(DEBUG_CONSOLE_TITLE);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FOREGROUND_MAGENTA
		| CONSOLE_BACKGROUND_WHITE);

	result = true;
#endif//!USE_CONSOLE_OUTPUT

	return result;
}
