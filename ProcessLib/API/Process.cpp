#include "../stdafx.h"
#include <cstdlib>
#include "TlHelp32.h"
#include "../Process.h"
#include "../Common/defines.h"


ELIB_API HMODULE __stdcall GetModuleByTlhelpW(wchar_t* name, DWORD pid)
{
	SAFE_ASSERT_RETURN(name, nullptr);

	HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
	SAFE_CHECK_API_RETURN(hModuleSnap, == , INVALID_HANDLE_VALUE, nullptr);

	MODULEENTRY32W me32 = { 0 };
	me32.dwSize = sizeof(MODULEENTRY32W);
	SAFE_CHECK_API_RETURN(Module32FirstW(hModuleSnap, &me32), == , FALSE, nullptr);

	_wcslwr_s(name, wcslen(name));
	do
	{
		_wcslwr_s(me32.szModule);
		if (!wcscmp(me32.szModule, name))
		{
			CloseHandle(hModuleSnap);
			return me32.hModule;
		}
	} while (Module32NextW(hModuleSnap, &me32));

	CloseHandle(hModuleSnap);
	return nullptr;
}

ELIB_API HMODULE __stdcall GetModuleByTlhelpA(char* name, DWORD pid)
{
	SAFE_ASSERT_RETURN(name, nullptr);

	wchar_t szName[MAX_PATH] = { 0 };
	size_t num = 0;
	SAFE_CHECK_API_RETURN(mbstowcs_s(&num, szName, MAX_PATH, name, MAX_PATH), == , 0, nullptr);

	return GetModuleByTlhelpW(szName, pid);
}