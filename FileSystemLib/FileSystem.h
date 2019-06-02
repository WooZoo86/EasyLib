#ifndef EASY_LIB_FILE_SYSTEM_INTERFACE_HEADER_
#define	EASY_LIB_FILE_SYSTEM_INTERFACE_HEADER_

#pragma once

#include <functional>
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

#ifdef _UNICODE
#define TraverseDirEx	TraverseDirExW
#define TraverseDir		TraverseDirW
#define WatchDir		WatchDirW

#else
#define TraverseDirEx	TraverseDirExA
#define TraverseDir		TraverseDirA
#define WatchDir		WatchDirA

#endif // _UNICODE

//File
ELIB_API bool __stdcall ProcessStorageFile(const wchar_t* file, std::function<bool(IStorage *)>callback);


#endif//EASY_LIB_FILE_SYSTEM_INTERFACE_HEADER_