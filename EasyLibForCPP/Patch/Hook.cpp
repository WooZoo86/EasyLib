#include "stdafx.h"
#include "defines.h"
#include "Interface.h"


static bool SafeCopyCode(DWORD addr, const BYTE* code, DWORD len, BYTE* saved)
{
	DWORD dwOldProtect = 0;

	try
	{
		SAFE_CHECK_API_RETURN(VirtualProtect((LPVOID)addr, len, 
			PAGE_EXECUTE_READWRITE, &dwOldProtect), ==, FALSE, false);

		if (saved)
		{
			memcpy((PVOID)saved, (PVOID)addr, len);
		}

		memcpy((PVOID)addr, (PVOID)code, len);
		VirtualProtect((LPVOID)addr, len, dwOldProtect, nullptr);
	}
	catch (std::exception& e)
	{
		A_DEBUGOUTA("exception: addr: %#p,code: %#p, len: %#x,des: %s", addr,
			code, len, e.what());
		return false;
	}

	return true;
}

ELIB_API bool __stdcall InstallHook(DWORD addr, DWORD len, HookProc pfunc, BYTE* saved)
{
	SAFE_ASSERT_RETURN(addr, false);
	SAFE_ASSERT_RETURN(len, false);
	SAFE_ASSERT_RETURN(pfunc, false);

	BYTE* e9_jmp = (BYTE *)malloc(len);
	SAFE_CHECK_API_RETURN(e9_jmp, == , nullptr, false);

	memset(e9_jmp, 0x90, len);	//fill nop
	e9_jmp[0] = 0xe9;
	*(DWORD *)&e9_jmp[1] = (DWORD)pfunc - addr - 5;

	bool result = SafeCopyCode(addr, e9_jmp, len, saved);
	SAFE_FREE_POINTER(e9_jmp);

	return result;
}

ELIB_API bool __stdcall UnInstallHook(DWORD addr, DWORD len, BYTE* saved)
{
	SAFE_ASSERT_RETURN(addr, false);
	SAFE_ASSERT_RETURN(len, false);
	SAFE_ASSERT_RETURN(saved, false);

	if (!SafeCopyCode(addr, saved, len, nullptr))
	{
		return false;
	}

	return true;
}