#include "stdafx.h"
#include "TlHelp32.h"
#include "defines.h"
#include "Interface.h"


ELIB_API HMODULE __stdcall GetModuleByTlhelpW(wchar_t* name, DWORD pid)
{
	SAFE_ASSERT_RETURN(name, nullptr);

	if(0 == pid)
	{
		pid = GetCurrentProcessId();
	}

	SAFE_ASSERT_RETURN(pid, nullptr);
		
	HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
	SAFE_CHECK_API_RETURN(hModuleSnap, == , INVALID_HANDLE_VALUE, nullptr);

	MODULEENTRY32W me32 = { 0 };
	me32.dwSize = sizeof(MODULEENTRY32W);
	SAFE_CHECK_API_RETURN(Module32FirstW(hModuleSnap, &me32), == , FALSE, nullptr);

	do
	{
		if (wcsstr(me32.szModule, name))
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

	wchar_t szName[MAX_PATH + 1] = { 0 };
	mbstowcs(szName, name, MAX_PATH);
	return GetModuleByTlhelpW(szName, pid);
}