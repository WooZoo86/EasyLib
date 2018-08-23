#include "stdafx.h"
#include "TlHelp32.h"
#include "defines.h"
#include "Interface.h"


ELIB_API HMODULE __stdcall GetModuleByTlhelp(TCHAR* name)
{
	SAFE_ASSERT_POINTER(name, nullptr);

	DWORD dwProcessId = GetCurrentProcessId();
	if (dwProcessId)
	{
		C_DEBUGOUT(_T("GetCurrentProcessId() failed!"));
		return nullptr;
	}
		
	HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);
	SAFE_CHECK_API_RETURN(hModuleSnap, == , INVALID_HANDLE_VALUE, nullptr);

	MODULEENTRY32 me32 = { 0 };
	me32.dwSize = sizeof(MODULEENTRY32);
	BOOL bRet = Module32First(hModuleSnap, &me32);
	if (!bRet)
	{
		C_DEBUGOUT(_T("Module32First() failed!"));
		return nullptr;
	}

	do
	{
		if (_tcsstr(me32.szModule, name))
		{
			CloseHandle(hModuleSnap);
			return me32.hModule;
		}
	} while (Module32Next(hModuleSnap, &me32));

	CloseHandle(hModuleSnap);
	return nullptr;
}