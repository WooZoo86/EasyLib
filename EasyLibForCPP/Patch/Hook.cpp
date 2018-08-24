#include "stdafx.h"
#include "defines.h"
#include "Interface.h"


static bool SafeCopyCode(DWORD addr, BYTE* code, DWORD len, BYTE* saved)
{
	DWORD dwOldProtect = 0;

	try
	{
		if (!VirtualProtect((LPVOID)addr, len, PAGE_EXECUTE_READWRITE, &dwOldProtect))
		{
			return false;
		}

		if (saved)
		{
			memcpy((PVOID)saved, (PVOID)addr, len);
		}

		memcpy((PVOID)addr, (PVOID)code, len);
		VirtualProtect((LPVOID)addr, len, dwOldProtect, nullptr);
	}
	catch (std::exception& e)
	{
		A_DEBUGOUTA("exception: addr: %#p,code: %#p, len: %#x,id: %s,des: %s", addr,
			code, len, e.code(), e.what());
		return false;
	}

	return true;
}

ELIB_API bool __stdcall InstallHook(DWORD addr, DWORD len, PROC pfunc, BYTE* saved)
{
	SAFE_ASSERT_POINTER(addr, false);
	SAFE_ASSERT_POINTER(len, false);
	SAFE_ASSERT_POINTER(pfunc, false);

	BYTE* e9_jmp = (BYTE *)malloc(len);
	SAFE_CHECK_API_RETURN(e9_jmp, == , nullptr, false);

	memset(e9_jmp, 0x90, len);	//fill nop
	e9_jmp[0] = 0xe9;
	*(DWORD *)&e9_jmp[1] = (DWORD)pfunc - addr - 5;

	if (!SafeCopyCode(addr, e9_jmp, len, saved))
	{
		free(e9_jmp);
		return false;
	}

	free(e9_jmp);
	return true;
}

ELIB_API bool __stdcall UnInstallHook(DWORD addr, DWORD len, BYTE* saved)
{
	SAFE_ASSERT_POINTER(addr, false);
	SAFE_ASSERT_POINTER(len, false);
	SAFE_ASSERT_POINTER(saved, false);

	if (!SafeCopyCode(addr, saved, len, nullptr))
	{
		return false;
	}

	return true;
}