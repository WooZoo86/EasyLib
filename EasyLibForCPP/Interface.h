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
ELIB_API bool __stdcall TraverseDirExA(const char* dir, const char* regExpress, std::function<bool(const char*)> callback);
ELIB_API bool __stdcall TraverseDirExW(const wchar_t* dir, const wchar_t* regExpress, std::function<bool(const wchar_t*)> callback);
ELIB_API bool __stdcall TraverseDirA(const char* dir, const char* filter, std::function<bool(const char*)> callback);
ELIB_API bool __stdcall TraverseDirW(const wchar_t* dir, const wchar_t* filter, std::function<bool(const wchar_t*)> callback);
ELIB_API bool __stdcall WatchDirW(const wchar_t* dir, std::function<bool(const wchar_t*)> callback);
ELIB_API bool __stdcall WatchDirA(const char* dir, std::function<bool(const char*)> callback);


//File
ELIB_API bool __stdcall ProcessStorageFile(const wchar_t* file, std::function<bool(IStorage *)>callback);

/********************************** Text ****************************************/
//Convert
ELIB_API char* __stdcall U2M(const wchar_t* str, UINT cp = CP_ACP);
ELIB_API wchar_t* __stdcall M2U(const char* str, UINT cp = CP_ACP);

/********************************** Process ****************************************/
//Process
ELIB_API HMODULE __stdcall GetModuleByTlhelpW(wchar_t* name, DWORD pid = 0);
ELIB_API HMODULE __stdcall GetModuleByTlhelpA(char* name, DWORD pid = 0);

#ifdef _UNICODE
	#define GetModuleByTlhelp	GetModuleByTlhelpW
#else
	#define GetModuleByTlhelp	GetModuleByTlhelpA
#endif // _UNICODE

/********************************** Patch ****************************************/
//Hook
typedef  void  (__stdcall *HookProc)();
ELIB_API bool __stdcall InstallHook(DWORD addr, DWORD len, HookProc pfunc, BYTE* save = nullptr);
ELIB_API bool __stdcall UnInstallHook(DWORD addr, DWORD len, BYTE* saved);
#endif // !EASY_LIB_FOR_CPP_INTERFACE_HEADER_


