#ifndef EASY_LIB_SYSTEM_INTERFACE_HEADER_
#define	EASY_LIB_SYSTEM_INTERFACE_HEADER_

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


/********************************** System ****************************************/
//Privilege
ELIB_API bool __stdcall IsUACEnabled();



#endif//EASY_LIB_SYSTEM_INTERFACE_HEADER_