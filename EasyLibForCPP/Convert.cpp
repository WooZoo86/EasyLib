#include "stdafx.h"
#include "defines.h"
#include "Interface.h"


ELIB_API char* __stdcall U2M(const wchar_t* str, UINT cp)
{
	int nSize = 0;

	nSize = WideCharToMultiByte(cp, 0, str, -1, nullptr, 0, nullptr, nullptr);
	char* szAlloc = new char[nSize + 1];
	SAFE_CHECK_API_RETURN(szAlloc, == , nullptr, nullptr);

	memset(szAlloc, 0, sizeof(char) * (nSize + 1));
	WideCharToMultiByte(cp, 0, str, -1, szAlloc, nSize, nullptr, nullptr);

	return szAlloc;
}

ELIB_API wchar_t* __stdcall M2U(const char* str, UINT cp)
{
	int nSize = 0;

	nSize = MultiByteToWideChar(cp, 0, str, -1, nullptr, 0);
	wchar_t* szAlloc = new wchar_t[nSize + 1];
	SAFE_CHECK_API_RETURN(szAlloc, == , nullptr, nullptr);

	memset(szAlloc, 0, sizeof(wchar_t)*(nSize + 1));
	MultiByteToWideChar(cp, 0, str, -1, szAlloc, nSize);

	return szAlloc;
}