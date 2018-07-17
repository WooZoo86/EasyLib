#ifndef EASY_LIB_FOR_CPP_INTERFACE_HEADER
#define	EASY_LIB_FOR_CPP_INTERFACE_HEADER

#pragma once

#include <functional>



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
ELIB_API bool __stdcall ProcessDirByRegexEx(const CString dir, const CString regExpress, std::function<bool(const CString)> callback);
ELIB_API bool __stdcall ProcessDirEx(const CString dir, const CString filter, std::function<bool(const CString)> callback);
ELIB_API bool __stdcall ProcessStorageFile(CString file, std::function<bool(IStorage *)>callback);



#endif // !EASY_LIB_FOR_CPP_INTERFACE_HEADER


