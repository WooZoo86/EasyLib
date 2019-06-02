#ifndef EASY_LIB_TEXT_INTERFACE_HEADER_
#define	EASY_LIB_TEXT_INTERFACE_HEADER_

#pragma once

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

/********************************** Text ****************************************/
//Convert
ELIB_API char* __stdcall U2M(const wchar_t* str, UINT cp = CP_ACP);
ELIB_API wchar_t* __stdcall M2U(const char* str, UINT cp = CP_ACP);




#endif//EASY_LIB_TEXT_INTERFACE_HEADER_