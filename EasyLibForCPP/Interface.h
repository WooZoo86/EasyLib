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
ELIB_API bool __stdcall ProcessDirByRegex(const TCHAR* dir, const TCHAR* regExpress, std::function<bool(const TCHAR*)> callback);
ELIB_API bool __stdcall ProcessDir(const TCHAR* dir, const TCHAR* filter, std::function<bool(const TCHAR*)> callback);

#ifdef _AFXDLL
ELIB_API bool __stdcall ProcessDirByRegexEx(const CString dir, const CString regExpress, std::function<bool(const CString)> callback);
ELIB_API bool __stdcall ProcessDirEx(const CString dir, const CString filter, std::function<bool(const CString)> callback);
#endif
ELIB_API bool __stdcall ProcessStorageFile(const TCHAR* file, std::function<bool(IStorage *)>callback);



#endif // !EASY_LIB_FOR_CPP_INTERFACE_HEADER_


