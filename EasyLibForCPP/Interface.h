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








#endif // !EASY_LIB_FOR_CPP_INTERFACE_HEADER_


