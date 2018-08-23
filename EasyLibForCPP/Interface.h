#ifndef EASY_LIB_FOR_CPP_INTERFACE_HEADER_
#define	EASY_LIB_FOR_CPP_INTERFACE_HEADER_

#pragma once

#include <functional>
#include <tchar.h>
#include <ObjIdl.h>


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


/**********************************File Management**************************************/
//Dir
ELIB_API bool __stdcall TraverseDirEx(const TCHAR* dir, const TCHAR* regExpress, std::function<bool(const TCHAR*)> callback);
ELIB_API bool __stdcall TraverseDir(const TCHAR* dir, const TCHAR* filter, std::function<bool(const TCHAR*)> callback);
ELIB_API bool __stdcall ProcessStorageFile(const TCHAR* file, std::function<bool(IStorage *)>callback);
ELIB_API bool __stdcall WatchDir(const TCHAR* dir, std::function<bool(const TCHAR*)> callback);


/********************************** Text ****************************************/
//Convert
ELIB_API char* __stdcall U2M(const wchar_t* str, UINT cp = CP_ACP);
ELIB_API wchar_t* __stdcall M2U(const char* str, UINT cp = CP_ACP);

/********************************** Process ****************************************/
//Process
ELIB_API HMODULE __stdcall GetModuleByTlhelp(TCHAR* strcharset);

/********************************** Patch ****************************************/
//Hook
ELIB_API bool __stdcall InstallHook(DWORD addr, DWORD len, PROC pfunc, BYTE* save = nullptr);
ELIB_API bool __stdcall UnInstallHook(DWORD addr, DWORD len, BYTE* saved);
#endif // !EASY_LIB_FOR_CPP_INTERFACE_HEADER_


