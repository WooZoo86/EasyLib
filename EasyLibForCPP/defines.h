#ifndef EASY_LIB_FOR_CPP_DEFINES_HEADER_
#define	EASY_LIB_FOR_CPP_DEFINES_HEADER_

#pragma once

#include <typeinfo>


#ifdef _UNICODE

#define tregex	std::wregex
#define	tmatch	std::wsmatch
#define	tstring std::wstring
#else

#define tregex	std::regex
#define	tmatch	std::smatch
#define	tstring std::string
#endif//_UNICODE


/*base macro*/
#define _STR(s)			#s
#define STR(s)			_STR(s)
#define _CAT(x,y)		x##y
#define CAT(x,y)		_CAT(x,y)

/*debug setting*/
#define	DEBUG_OUT_STR_MAX		0x400
#define DEBUG_TIME_INFO			_T("[" __DATE__ "," __TIME__ "]")
#define DEBUG_POS_INFO(x)		_T("[" _STR(x) "," __FUNCTION__ "," __FILE__ "] ")
#define DEBUG_PREFIX_STR		 DEBUG_TIME_INFO DEBUG_POS_INFO(__LINE__)

//#define USE_CONSOLE
#ifdef USE_CONSOLE
#define	DBGOUTA(info)	printf(info)
#define	DBGOUTW(info)	wprintf(info)
#else
#define DBGOUTA(info)	OutputDebugStringA(info)
#define DBGOUTW(info)	OutputDebugStringW(info)
#endif

#ifdef _UNICODE
#define DBGOUT(info)	DBGOUTW(info)
#else
#define DBGOUT(info)	DBGOUTA(info)
#endif

#define _MYDEBUGOUT(info)		DBGOUT(info _T("\r\n"))
#define MYDEBUGOUT(info)		_MYDEBUGOUT(DEBUG_PREFIX_STR info)


#ifndef _DEBUG
#define MYDEBUGOUT(info)		
#define DBGOUTA(info)	
#define DBGOUTW(info)	
#endif

/*safe assert*/
#define	SAFE_ASSERT_POINTER(p,v)	if(!(p)){DBGOUT(DEBUG_PREFIX_STR);DBGOUTA(typeid((p)).name());_MYDEBUGOUT(_T(STR((p)) " == nullptr"));return v;}

/*safe release*/
#define	SAFE_FREE_POINTER(p)			if((p)){free((p));(p)=nullptr;}
#define	SAFE_RELEASE_POINTER(p)			if((p)){delete (p);(p)=nullptr;}
#define	SAFE_REALSE_REGKEY(p)			if((p)){RegCloseKey((p));(p)=nullptr;}
#define	SAFE_CLOSE_FILE(p)				if((p)){fclose((p));(p)=nullptr;}
#define SAFE_RELEASE_LUASTATE(p)		if((p)){lua_close((p));(p)=nullptr;}
#define SAFE_RELEASE_HANDLE(p)			if((p)){CloseHandle((p));(p)=nullptr;}
#define SAFE_REALASE_MAPVIEW(p)			if((p)){UnmapViewOfFile((p));(p)=nullptr;}
#define SAFE_RELEASE_CRITICALSECTION(p)	if((p)){LeaveCriticalSection((p));DeleteCriticalSection((p));}
#define SAFE_RELEASE_LIB(p)				if((p)){FreeLibrary((p));(p)=nullptr;}
#define SAFE_RELEASE_CLASS(p)			if((p)){(p)->Release();(p)=nullptr;}


/*safe check function return*/
#define	SAFE_CHECK_RETURN(value,ops,cmp)	if((value) ops (cmp)){ \
												LPVOID buf=nullptr;DWORD dwErrId=GetLastError(); \
												TCHAR szErr[MAX_PATH + 1]={0}; \
												if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS \
													| FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_MAX_WIDTH_MASK, \
													nullptr,dwErrId,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), \
													(LPTSTR)&buf,0,nullptr)){ \
													_sntprintf_s(szErr,MAX_PATH,_T("id = %#X,err = %s\r\n"),dwErrId,(LPTSTR)buf); \
													DBGOUT(DEBUG_PREFIX_STR);DBGOUT(szErr); \
													LocalFree(buf);\
												} \
												else \
												{ \
													_sntprintf_s(szErr,MAX_PATH,_T("id = %#X,err = unknown Windows error\r\n"),dwErrId); \
													DBGOUT(DEBUG_PREFIX_STR);DBGOUT(szErr); \
												} \
											}









#endif // !EASY_LIB_FOR_CPP_DEFINES_HEADER_


