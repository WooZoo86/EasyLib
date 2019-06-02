#ifndef EASY_LIB_PROCESS_INTERFACE_HEADER_
#define	EASY_LIB_PROCESS_INTERFACE_HEADER_

#pragma once

#include <Windows.h>

#define	ELIB_EXPORT
#ifdef ELIB_EXPORT
#ifdef __cplusplus
#define	ELIB_API	extern "C" __declspec(dllexport)
#else
#define	ELIB_API	__declspec(dllexport)
#endif
#else
#ifdef __cplusplus
#define	ELIB_API	extern "C" __declspec(dllimport)
#else
#define	ELIB_API	__declspec(dllimport)
#endif
#endif // ELIB_EXPORT

/********************************** Process ****************************************/
//Process
ELIB_API HMODULE __stdcall GetModuleByTlhelpW(wchar_t* name, DWORD pid = 0);
ELIB_API HMODULE __stdcall GetModuleByTlhelpA(char* name, DWORD pid = 0);

#ifdef _UNICODE
#define GetModuleByTlhelp	GetModuleByTlhelpW
#else
#define GetModuleByTlhelp	GetModuleByTlhelpA
#endif // _UNICODE

#endif//!EASY_LIB_PROCESS_INTERFACE_HEADER_