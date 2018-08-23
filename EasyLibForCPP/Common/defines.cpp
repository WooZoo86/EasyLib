#include "stdafx.h"
#include "defines.h"
#include <cstdarg>
#include <iostream>
#include <tchar.h>

#if (defined(DBGOUT_ANYWAY) || defined(DBGOUT_NORMAL))
void VDebugOutA(const char* from, const char* format, ...)
{
	char buf[DEBUG_OUT_STR_MAX + 1] = { 0 };
	char info[DEBUG_OUT_STR_MAX + 1] = { 0 };
	va_list va = nullptr;

	va_start(va, format);
	_vsnprintf_s(buf, DEBUG_OUT_STR_MAX, format, va);
	_snprintf_s(info, DEBUG_OUT_STR_MAX, DEBUG_OUT_STR_MAX, "%s %s %s\r\n",
		from, DEBUG_PREFIX_STR_A, buf);
	va_end(va);

	DBGOUTA(info);
}

void VDebugOutW(const wchar_t* from, const wchar_t* format, ...)
{
	wchar_t buf[DEBUG_OUT_STR_MAX + 1] = { 0 };
	wchar_t info[DEBUG_OUT_STR_MAX + 1] = { 0 };
	va_list va = nullptr;

	va_start(va, format);
	_vsnwprintf_s(buf, DEBUG_OUT_STR_MAX, format, va);
	_snwprintf_s(info, DEBUG_OUT_STR_MAX, DEBUG_OUT_STR_MAX, L"%s %s %s\r\n",
		from, DEBUG_PREFIX_STR_W, buf);
	va_end(va);

	DBGOUTW(info);
}
#endif//!defined(DBGOUT_ANYWAY) || defined(DBGOUT_NORMAL)

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

	A_DEBUGOUT(szErr);
}

#if (defined(DBGOUT_ANYWAY) || defined(DBGOUT_NORMAL) || defined(DBGOUT_CONST))
#ifdef USE_CONSOLE_OUTPUT
static bool InitConsole()
{
	SAFE_CHECK_API_RETURN(AllocConsole(), != , TRUE, false);

	FILE* file = nullptr;
	_tfreopen_s(&file, _T("CONOUT$"), _T("w+t"), stdout);
	_tfreopen_s(&file, _T("CONIN$"), _T("r+t"), stdin);

	SetConsoleTitle(DEBUG_CONSOLE_TITLE);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FOREGROUND_MAGENTA
		| CONSOLE_BACKGROUND_WHITE);

	return true;
}

static DWORD WINAPI ThreadInitConsole(LPVOID lpParam)
{
	if (!InitConsole())
	{
		return EXIT_FAILURE;
	}

	return 0;
}

void CloseConsole()
{
	FreeConsole();
}

void OpenConsole()
{
	CreateThread(nullptr, NULL, ThreadHookFunc, nullptr, NULL, nullptr);
}

#endif//!USE_CONSOLE_OUTPUT
#endif//!defined(DBGOUT_ANYWAY) || defined(DBGOUT_NORMAL) || defined(DBGOUT_CONST)

