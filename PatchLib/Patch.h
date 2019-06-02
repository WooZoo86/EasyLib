#ifndef EASY_LIB_PATCH_INTERFACE_HEADER_
#define	EASY_LIB_PATCH_INTERFACE_HEADER_

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

/********************************** Patch ****************************************/
//Hook
typedef  void(__stdcall *HookProc)();
ELIB_API bool __stdcall InstallHook(DWORD addr, DWORD len, HookProc pfunc, BYTE* save = nullptr);
ELIB_API bool __stdcall UnInstallHook(DWORD addr, DWORD len, BYTE* saved);



#endif//EASY_LIB_PATCH_INTERFACE_HEADER_
